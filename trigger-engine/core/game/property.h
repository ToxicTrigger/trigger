#pragma once
#include <string>
#include <memory>
#include <any>
#include <optional>
#include <fstream>
#include "component.h"
#include <type_traits>
#include "file.h"

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
        // maybe void*
        Unknown,
        Int, 
        Float,
        Double,
        Bool,
        SizeT,
        HashID,
        String,
		FilePath,
		List,
		VectorStr,
		VectorPath
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

    template <typename T>
    std::string to_string()
    {
        std::stringstream ss;
        ss << value.type().name() << " : " << std::any_cast<T>(value);
        return ss.str();
    }

    std::string to_string()
    {
        std::stringstream ss;
        if(!this->controllable)
        {
            return this->name + " !is privated";
        }

        switch(this->type)
        {
            case data_type::Int:
            ss << "int::" << this->name << " " << std::any_cast<int>(value);
            break;
            case data_type::Bool :
            ss << "bool::" << this->name << " " << std::any_cast<bool>(value);
            break;
            case data_type::Double :
            ss << "double::" << this->name << " " << std::any_cast<double>(value);
            break;
            case data_type::FilePath :
            ss << value.type().name() << "::" << this->name << " " << std::any_cast<trigger::core::file>(value).get_path();
            break;
            case data_type::Float :
            ss << "float::" << this->name << " " << std::any_cast<float>(value);
            break;
            case data_type::HashID :
            ss << "hash_id::" << this->name << " " << std::any_cast<hash_id>(value);
            break;
            case data_type::List :
            ss << value.type().name() << "::" << this->name << " " << "List";
            break;
            case data_type::SizeT :
            ss << "size_t::" << this->name << " " << std::any_cast<size_t>(value);
            break;
            case data_type::String :
            ss << "std::string::" << this->name << " " << std::any_cast<std::string>(value);
            break;
            default:
            ss << value.type().name() << "::" << this->name << " ?";
            break;
        }
        return ss.str();
    }

    template<typename T>
    static std::optional<T> parse(std::string& str)
    {
        std::string token = str.substr(str.find(" ")+1);
        if(token.empty()) 
            return {};              // empty!
        else if(token.at(0) == '!')
            return {};              // privated
        else if(token.at(0) == '?')
            return {};              // unknown type

        if constexpr(std::is_arithmetic<T>::value)
        {
            std::optional<T> num = static_cast<T>(std::stod(token));
            return num;
        }
        else if(std::is_same<T, std::string>::value || std::is_same<T, trigger::core::file>::value )
        {
            std::optional<std::string> str = (token);
            return str;
        }
        return {};
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
