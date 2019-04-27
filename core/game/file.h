#ifndef FILE_H
#define FILE_H

#include <string>

namespace trigger::core
{
	static std::string replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = 0; //string처음부터 검사
		while ((start_pos = str.find(from, start_pos)) != std::string::npos)  //from을 찾을 수 없을 때까지
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // 중복검사를 피하고 from.length() > to.length()인 경우를 위해서
		}
		return str;
	}

	static std::string get_real_path(std::string path)
	{
		std::string real = path;
		std::string from;
		std::string to;
#ifdef _WIN64
		from = '/';
		to = '\\';
		real = trigger::core::replace(real, from, to);
#else
		from = '\\';
		to = '/';
		real = trigger::core::replace(real, from, to);
#endif
		return real;
	}

    class file
    {
    public:
        enum type
        {
            hlsl,
            obj,
            fbx,
            map,
            pot,
            tps,
            unknown
        };

    private:
        std::string path;
        file::type type;

    public:
        file()
        {
            this->type = type::unknown;
        }

        file(std::string path) : path(path)
        {
            std::string exec = this->path.substr(this->path.rfind(".")+1);
            if(exec.compare("hlsl") == 0)
            {
                this->type = file::type::hlsl;
            }
            else if(exec.compare("obj") == 0)
            {
                this->type = file::type::obj;
            }
            else if(exec.compare("fbx") == 0)
            {
                this->type = file::type::fbx;
            }
            else if(exec.compare("map") == 0)
            {
                this->type = file::type::map;
            }
            else if(exec.compare("pot" )== 0)
            {
                this->type = file::type::pot;
            }
            else if(exec.compare("tps") == 0)
            {
                this->type = file::type::tps;
            }
            else
            {
                this->type = file::type::unknown;
            }
        };

        std::string const get_path()
        {
            return this->path;
        }

        int get_type()
        {
            return this->type;
        };

        bool is_usable()
        {
            if(this->type == type::unknown) return false;
            return true;
        };
    };
}

#endif