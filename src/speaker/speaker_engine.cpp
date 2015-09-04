#include "speaker_engine.hpp"

#include <base/event.hpp>
#include <util/unique_ptr_tools.hpp>

// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
#include <boost/msm/front/euml/state_grammar.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

namespace {
    using namespace aria::internal;

    // FSM events
    struct stop {};
    struct frame_received
    {
        frame_received(aria::frame && frame) : frame(std::move(frame)) {}
        aria::frame frame;
    };

    // State machine
    struct engine_ : public msm::front::state_machine_def<engine_>
    {
        engine_(std::shared_ptr<postable_event_queue> queue,
                std::shared_ptr<frame_buffer> buffer) :
                queue(queue), buffer(buffer) { }

        std::shared_ptr<postable_event_queue> queue;
        std::shared_ptr<frame_buffer> buffer;

        // States
        struct Stopped_tag {};
        typedef msm::front::euml::func_state<Stopped_tag> Stopped;

        struct Buffering_tag {};
        typedef msm::front::euml::func_state<Buffering_tag> Buffering;

        // Set initial state
        typedef Stopped initial_state;

        // Actions
        struct buffer_frame
        {
            template <class FSM, class SourceState, class TargetState>
            void operator()(const frame_received & event, FSM & fsm, SourceState & , TargetState & )
            {
                // TODO: Figure out a way for us to move the data? Possibly add to buffer outside state machine
                fsm.buffer->push(aria::frame(event.frame));
            }
        };

        struct notify_frame_received
        {
            template <class FSM, class SourceState, class TargetState>
            void operator()(const frame_received & event, FSM & fsm, SourceState & , TargetState & )
            {
                fsm.queue->post(make_unique<frame_received_event>());
            }
        };

        struct notify_stopped
        {
            template <class EVT, class FSM, class SourceState, class TargetState>
            void operator()(const EVT & event, FSM & fsm, SourceState & , TargetState & )
            {
                fsm.queue->post(make_unique<stop_event>());
            }
        };

        struct notify_buffering_started
        {
            template <class EVT, class FSM, class SourceState, class TargetState>
            void operator()(const EVT & event, FSM & fsm, SourceState & , TargetState & )
            {
                fsm.queue->post(make_unique<buffering_started_event>());
            }
        };

        template <typename ...T> using Actions = ActionSequence_<mpl::vector<T...>>;

        // Transition table for state machine
        struct transition_table : mpl::vector<
                //    Start       Event                   Next      Action                     Guard
                //  +-----------+-----------------------+----------+---------------------------+--------------+
                Row < Stopped,    frame_received,        Buffering, Actions
                        <                                            notify_buffering_started
                                ,                                    buffer_frame
                                ,                                    notify_frame_received>,    none >,
                //  +-----------+-----------------------+----------+---------------------------+--------------+
                Row < Buffering,  stop,                  Stopped,   notify_stopped,             none >,
                Row < Buffering,  frame_received,        Buffering, Actions
                        <                                            buffer_frame
                                ,                                    notify_frame_received>,    none >
                //  +-----------+-----------------------+----------+---------------------------+--------------+
        > {};
    };

    // State machine back-end
    typedef msm::back::state_machine<engine_> engine;

}

namespace aria {

    namespace internal {

        struct speaker_engine::fsm_wrapper {
        public:
            // Forward constructor arguments to the FSM
            template <typename... T>
            fsm_wrapper(T... args) : fsm(::engine(args...)) { }

            ::engine fsm;
        };

        speaker_engine::speaker_engine(std::shared_ptr<postable_event_queue> queue,
                                       std::shared_ptr<frame_buffer> buffer)
                :   _wrapper(new fsm_wrapper(queue, buffer))
        {

        }

        speaker_engine::~speaker_engine()
        {
            delete _wrapper;
        }

        void speaker_engine::receive_message(std::unique_ptr<message> &&message)
        {
            switch (message->type())
            {
                case message_type::STOP:
                {
                    _wrapper->fsm.process_event(stop());
                    break;
                }
                case message_type::FRAME:
                {
                    auto msg = static_unique_ptr_cast<frame_message>(std::move(message));
                    auto frame = aria::frame(msg->id(), std::move(msg->take_data()));
                    auto event = frame_received(std::move(frame));
                    _wrapper->fsm.process_event(event);
                    break;
                }
                default:
                    break;
            }
        }

    }

}

