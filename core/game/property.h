#pragma once
#include <string>
#include <memory>

template <class TO, class FROM>
static TO cast(FROM v)
{
    return static_cast<TO>(static_cast<void *>(v));
}

#define PRI_A 54059   /* a prime */
#define PRI_B 76963   /* another prime */
#define PRI_C 86969   /* yet another prime */
#define PRI_FIRSTH 37 /* also prime */
static unsigned hash_str(const char *s)
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
template <typename T>
class property
{

private:
    unsigned int id;
    std::string name;

public:
    T value;

    property()
    {
        value = 0;
        id = hash_str(name.c_str());
    }

    property(T val, std::string property_name)
    {
        this->value = val;
        this->name = property_name;
        this->id = hash_str(name.c_str());
    }

    unsigned int get_id()
    {
        return id;
    }

    const std::string get_name()
    {
        return this->name;
    }

    std::string to_string()
    {
        auto val = cast<char*>(value);
        return std::string(val);
    }

    std::shared_ptr<T> parse(const char* data)
    {
        auto re = (void*)(data);
        auto sum = std::make_shared<T>(cast<T>(re));
        return sum;
    }

    std::shared_ptr<T> parse(std::string data)
    {
        auto re = (void*)(data.c_str());
        auto sum = std::make_shared<T>(cast<T>(re));
        return sum;
    }

    virtual ~property()
    {
        delete value;
        name.clear();
    }
};

} // namespace trigger
