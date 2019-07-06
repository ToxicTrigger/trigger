#pragma once

namespace trigger::impl
{
template <typename T>
class impl_singletone
{
private:
    static T *_inst;

protected:
    impl_singletone()
    {
    }
    virtual ~impl_singletone()
    {
    }

public:
    static T* get_instance()
    {
        if(_inst == nullptr) _inst = new T;
        return _inst;
    }
    static void destroy()
    {
        if(_inst)
        {
            delete _inst;
            _inst = nullptr;
        }
    }


};
} // namespace trigger::impl
