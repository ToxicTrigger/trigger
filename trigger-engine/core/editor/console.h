#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include "../impl/impl_singleton.h"
#include "../game/trigger_tools.h"

namespace trigger::tools
{
    struct log
    {
        using time_stamp = std::chrono::time_point<std::chrono::system_clock>;

        enum type
        {
            Log,
            Warning,
            Error
        };  

        type log_type;
        time_stamp rising_time;
        std::string message;
        std::string call_stack; //TODO
        bool open;
        
        explicit log(type log_type, std::string message)
        {
            this->log_type = log_type;
            this->message = message;
            this->rising_time = std::chrono::system_clock::now();
        }
    };

    class console : public trigger::impl::impl_singletone<console>
    {
        private:
        std::vector<trigger::tools::log> logs;

        public:
        void log(std::string message)
        {   
            logs.insert(logs.begin(),trigger::tools::log(trigger::tools::log::type::Log, message) );
        };
        void warning(std::string message)
        {   
            logs.insert(logs.begin(),trigger::tools::log(trigger::tools::log::type::Warning, message) );
        };
        void error(std::string message)
        {   
            logs.insert(logs.begin(),trigger::tools::log(trigger::tools::log::type::Error, message) );
        };
        
        void clear_log()
        {
            logs.clear();
        }

        std::vector<trigger::tools::log> get_logs()
        {
            return logs;
        }

    };
}