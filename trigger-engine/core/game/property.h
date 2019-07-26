#pragma once
#include <string>
#include <memory>
#include <any>
#include <optional>
#include <fstream>
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
    enum data_type
    {
        Unknown,
        Int, 
        Float,
        Double,
        Bool,
        SizeT,
        HashID,
        String
    };

    std::any value;
    data_type type;
	bool controllable = true;


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

    property(std::any val, data_type type, std::string property_name, std::map<hash_id, property>* author)
    {
        this->type = type;
        this->value = val;
        this->name = property_name;
        this->id = hash_str(name.c_str());
        author->insert(std::pair<hash_id, property>(id, *this));
    }

	property(std::any val, data_type type, bool can_access_editor, std::string property_name, std::map<hash_id, property>* author)
	{
		this->type = type;
		this->value = val;
		this->name = property_name;
		this->id = hash_str(name.c_str());
		this->controllable = can_access_editor;
		author->insert(std::pair<hash_id, property>(id, *this));
	}

    hash_id get_id()
    {
        return id;
    }

    static const hash_id get_id(property property)
    {
        return property.get_id();
    }

    const std::string get_name()
    {
        return this->name;
    }

    const static std::string get_name(property property)
    {
        return property.get_name();
    }

    std::optional<int> get_int()
    {
        if(auto var = std::any_cast<int>(&this->value))
        {
            return *var;
        }
        return {};
    }

    
    static std::optional<int> get_int(property property)
    {
        if(auto var = std::any_cast<int>(&property.value))
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

    
    static std::optional<float> get_float(property property)
    {
        if(auto var = std::any_cast<float>(&property.value))
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

    static std::optional<double> get_double(property property)
    {
        if(auto var = std::any_cast<double>(&property.value))
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

    static std::optional<std::string> get_string(property property)
    {
        if(auto var = std::any_cast<std::string>(&property.value))
        {
            return *var;
        }
        return {};
    }

    template<typename T> T get()
    {
        return *std::any_cast<T>(&this->value);
    }

	template<typename T>
	void set(T val)
	{
		this->value = val;
	}

    template<typename T> static auto get(property property) -> decltype(auto)
    {
        return *std::any_cast<T>(&property.value);
    }

    virtual ~property()
    {
        value.reset();
        name.clear();
    }
};



} // namespace trigger
