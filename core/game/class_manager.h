#pragma once;
#include "component.h"
#include "../impl/impl_singleton.h"
template <typename T> T * trigger::impl::impl_singletone<T>::_inst = 0;

namespace trigger
{
    class component;
namespace manager
{
class class_manager : public trigger::impl::impl_singletone<class_manager>
{
private:
    std::map<std::string, trigger::component *> CLASS_ARRAY = std::map<std::string, trigger::component *>();

public:
    class_manager()
    {
    }

    auto get_class_array()
    {
        return this->CLASS_ARRAY;
    }

    //BUG BUG
    auto instancing(std::string type) -> decltype(auto)
    {
        decltype(CLASS_ARRAY[type]) ma = (CLASS_ARRAY[type]);
        return ma;
    };
};
} // namespace manager
} // namespace trigger
