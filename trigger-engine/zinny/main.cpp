#include <iostream>
#include <string>
#include <vector>

#include "src/zinny.h"
#include "src/zinny-tool.h"

int main(int in, char **argv)
{
    std::string cur_exec_name = argv[0];
    std::string first_arg;
    std::vector<std::string> all_args;

    if(check_validate(in))
    {
        first_arg = argv[1];
        all_args.assign(argv + 1, argv + in);

        zinny *z = new zinny(all_args);
        action(all_args, z);
    }
    else
    {
        show_help();
    }
    return 0;
}
