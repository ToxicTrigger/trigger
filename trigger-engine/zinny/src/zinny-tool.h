#ifndef ZINNY_TOOL_H
#define ZINNY_TOOL_H

#include <string>
#include <vector>
#include <iostream>
#include "zinny.h"

static bool const check_validate(int const in)
{
    if( in <= 1 ) return false;
    return true;
}

static void action(std::vector<std::string> const args, zinny *pot)
{
    if(args[0] == "-p")
    {
        pot->packing();
    }
    else if(args[0] == "-v")
    {
        pot->view();
    }
    else if(args[0] == "-u")
    {
        pot->unpack();
    }
    else if(args[0] == "-i")
    {
        pot->init();
    }
    else if(args[0] == "-c")
    {
        pot->check();
    }
}

static void const show_help()
{
    std::cout << "-p {out_name} {files}     \t | Packaged files to out_name" << std::endl;
    std::cout << "-u {file} {path}          \t | un-packaging file to path" << std::endl;
    std::cout << "-v {file}                 \t | show file in-side" << std::endl;
    std::cout << "-i {project_name}         \t | Initialize trigger engines project" << std::endl;
    std::cout << "-c {files}                \t | checking vaildated pot files" << std::endl;
    std::cout << "-a                        \t | checking vaildated trigger project" << std::endl;
}

#endif