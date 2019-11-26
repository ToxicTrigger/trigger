#ifndef TRIGGER_TOOLS_H
#define TRIGGER_TOOLS_H

#include <string>
#include <memory>
#include <map>
#include "../editor/console.h"

#define T_LOG(x) trigger::tools::console::get_instance()->log(x)
#define T_ERR(x) trigger::tools::console::get_instance()->error(x)
#define T_WARN(x) trigger::tools::console::get_instance()->warning(x)

#define T_METHOD methodName(__FUNCTION__).c_str()
#define T_CLASS className(__FUNCTION__).c_str()

inline std::string methodName(const std::string& prettyFunction)
{
	size_t colons = prettyFunction.find("::");

	return prettyFunction.substr(colons + 2, prettyFunction.size()) + "()";
}


// Free Casting 
template <class TO, class FROM>
static TO cast(FROM v)
{
    return static_cast<TO>(static_cast<void *>(v));
}

using hash_id = int;

#define PRI_A 54059   /* a prime */
#define PRI_B 76963   /* another prime */
#define PRI_C 86969   /* yet another prime */
#define PRI_FIRSTH 37 /* also prime */
// for Make String hashed 
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


inline std::string className(const std::string& prettyFunction)
{
#ifndef _WIN64
	return prettyFunction;
#else
	size_t colons = prettyFunction.rfind("::");
	if (colons == std::string::npos)
		return prettyFunction;
	size_t begin = prettyFunction.substr(0, colons).rfind(" ") + 1;
	size_t end = colons - begin;

	return prettyFunction.substr(begin, end);
#endif
}

#endif