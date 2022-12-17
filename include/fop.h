// file operation header

#ifndef _FOP_H_
#define _FOP_H_

#include<errno.h>
#include<string>
#include<fstream>

#define F_OK           0
#define F_READERROR   -1
#define F_WRITEERROR  -2
#define F_BUFFERERROR -3

namespace rena{

    extern const size_t buffer_size;

    errno_t copyfile( std::string , std::string );
    errno_t copyfolder( std::string , std::string );

} // namespace rena

#endif