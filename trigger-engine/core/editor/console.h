#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <map>
#include <functional>
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
        bool open = false;
        
        explicit log(type log_type, std::string message)
        {
            this->log_type = log_type;
            this->message = message;
            this->rising_time = std::chrono::system_clock::now();
        }
    };

	struct trigger_func
	{
		std::function<std::string(const std::string&)> func;
		std::string name;
		std::string comment;
	};

    class console : public trigger::impl::impl_singletone<console>
    {
		public:
			bool view_err;
			bool view_warn;
			bool view_log;
			std::string cmd;
			std::map<std::string, trigger::tools::trigger_func > funcs;
			std::string filter = "";

        private:
        std::vector<trigger::tools::log*> logs;
		trigger::tools::log* current_log = nullptr;

        public:
        void log(std::string message)
        {   
			trigger::tools::log* tmp = new trigger::tools::log(trigger::tools::log::type::Log, message);
            logs.insert(logs.begin(), tmp );
        };
        void warning(std::string message)
        {   
			trigger::tools::log* tmp = new trigger::tools::log(trigger::tools::log::type::Warning, message);
			logs.insert(logs.begin(), tmp);
        };
        void error(std::string message)
        {   
			trigger::tools::log* tmp = new trigger::tools::log(trigger::tools::log::type::Error, message);
			logs.insert(logs.begin(), tmp);
        };
        
        void clear_log()
        {
            logs.clear();
        }

		trigger::tools::log* get_current_log()
		{
			return current_log;
		}

		void set_current_log(trigger::tools::log* log)
		{
			this->current_log = log;
		}

		void insert_func(std::string name, std::function<std::string(const std::string&)> func)
		{
			this->funcs.insert({ name, trigger::tools::trigger_func{ func , name, ""}});
		}

		void insert_func(std::string name, std::function<std::string(const std::string&)> func , std::string comment)
		{
			this->funcs.insert({ name, trigger::tools::trigger_func{ func , name, comment} });
		}

        std::vector<trigger::tools::log*> get_logs()
        {
            return logs;
        }

    };
}