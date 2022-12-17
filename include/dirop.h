// dir operation header

#ifndef _DIROP_H_
#define _DIROP_H_

#include<string>
#include<io.h>
#include<errno.h>
#include<stdint.h>
#include<dirent.h>
#include<iostream>
#include<vector>

#define D_OK     0
#define D_ERROR -1

namespace rena{

    typedef std::vector <std::string> FILE_LIST;

    typedef enum {
        include,
        ignore
    }               FELABEL; // file extension label (what is this file extension for)

    errno_t listfileindir( std::string , FILE_LIST& );
    errno_t listfileindir( std::string , FELABEL , std::string , FILE_LIST& );
    errno_t traversedir( std::string , FILE_LIST& , uint32_t );
    errno_t traversedir( std::string , FELABEL , std::string , FILE_LIST& );

} // namespace rena

#endif