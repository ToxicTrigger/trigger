#pragma once
#include <string>
#include <memory>
#include <any>
#include <optional>
#include "component.h"

template <class TO, class FROM>
static TO cast(FROM v)
{
    return static_cast<TO>(static_cast<void *>(v));
}

using hash_id = unsigned int;

#define PRI_A 54059   /* a prime */
#define PRI_B 76963   /* another prime */
#define PRI_C 86969   /* yet another prime */
#define PRI_FIRSTH 37 /* also prime */
static hash_id hash_str(const char *s)
{
	unsigned h = PRI_FIRSTH;
	while (*s)
	{
		h = (h * PRI_A) ^ (s[0] * PRI_B);
		s++;
	}
	return h % PRI_C; // or return h % C;
}

namespace trigger
{
class component;

class property
{
private:
    hash_id id;
    std::string name;

public:
    std::any value;

    property()
    {
        value = 0;
        id = hash_str(name.c_str());
    }

    property(std::any val, std::string property_name, std::map<hash_id, property>* author)
    {
        this->value = val;
        this->name = property_name;
        this->id = hash_str(name.c_str());
        author->insert(std::pair<hash_id, property>(id, *this));
    }

    unsigned int get_id()
    {
        return id;
    }

    const std::string get_name()
    {
        return this->name;
    }

    template<typename T>
    std::optional<const char*> to_string()
    {
        if(auto val = std::any_cast<T>(&value))
        {
            void* ref = static_cast<void*>(new T(*val));
            return static_cast<const char*>(ref);
        }
        else
        {
            return {};
        }
    }

    std::optional<int> get_int()
    {
        if(auto var = std::any_cast<int>(&this->value))
        {
            return *var;
        }
        return {};
    }

    std::optional<float> get_float()
    {
        if(auto var = std::any_cast<float>(&this->value))
        {
            return *var;
        }
        return {};
    }

    std::optional<double> get_double()
    {
        if(auto var = std::any_cast<double>(&this->value))
        {
            return *var;
        }
        return {};
    }

    std::optional<std::string> get_string()
    {
        if(auto var = std::any_cast<std::string>(&this->value))
        {
            return *var;
        }
        return {};
    }

    template<typename T>
    static T parse(const char* data)
    {
        void* sum = static_cast<void*>((void*)data);
        return *static_cast<T*>(sum);
    }

    template<typename T>
    T get()
    {
        return std::any_cast<T>(this->value);
    }

    virtual ~property()
    {
        value.reset();
        name.clear();
    }
};



} // namespace trigger
