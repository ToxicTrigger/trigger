#ifndef FILE_H
#define FILE_H

#include <string>

namespace trigger::core
{
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