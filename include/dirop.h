// dir operation header

#ifndef _DIROP_H_
#define _DIROP_H_

#include<iostream>
#include<string>
#include<vector>

#include<boost/filesystem.hpp>

#define D_OK        0
#define D_NOTEXIST -1

namespace rena{

    typedef std::vector <std::string> FILE_LIST;
    typedef int errno_t;

    typedef enum {
        include,
        ignore
    }               FELABEL; // file extension label (what is this file extension for)

    errno_t traversedir( std::string , FILE_LIST& );

} // namespace rena

#endif