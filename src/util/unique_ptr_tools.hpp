#pragma once

#include <memory>

namespace aria {

    namespace internal {
        // The following is stolen from
        // http://stackoverflow.com/questions/12580432/why-does-c11-have-make-shared-but-not-make-unique
        template<typename T, typename ...Args>
        std::unique_ptr<T> make_unique( Args&& ...args )
        {
            return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
        }

        // The following two "cast" functions are copied from
        // http://stackoverflow.com/questions/21174593/downcasting-unique-ptrbase-to-unique-ptrderived
        template<typename Derived, typename Base, typename Del>
        std::unique_ptr<Derived, Del>
        static_unique_ptr_cast( std::unique_ptr<Base, Del>&& p )
        {
            auto d = static_cast<Derived *>(p.release());
            return std::unique_ptr<Derived, Del>(d, std::move(p.get_deleter()));
        }

        template<typename Derived, typename Base, typename Del>
        std::unique_ptr<Derived, Del>
        dynamic_unique_ptr_cast( std::unique_ptr<Base, Del>&& p )
        {
            if(Derived *result = dynamic_cast<Derived *>(p.get())) {
                p.release();
                return std::unique_ptr<Derived, Del>(result, std::move(p.get_deleter()));
            }
            return std::unique_ptr<Derived, Del>(nullptr, p.get_deleter());
        }
    }

}