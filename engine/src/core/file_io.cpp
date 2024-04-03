#include "pch.h"
#include "file_io.h"

namespace ge
{
    std::string loadFile(std::string const &fpath)
    {
        // not good for pmr, due to local var
        std::string result;
        // system call
        std::ifstream ifs(fpath.c_str());
        if (!ifs.is_open())
        {
            return result;
        }
        // file size
        ifs.seekg(0, std::ios::end);
        // reserve memory, performance issue
        result.reserve(ifs.tellg());
        // start from byte 0
        ifs.seekg(0, std::ios::beg);

        result.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return result;
    }
}