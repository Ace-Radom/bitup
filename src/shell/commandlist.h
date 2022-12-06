#ifndef _COMMANDLIST_H_
#define _COMMANDLIST_H_

#include<string>
#include<stdint.h>

#ifdef __WIN32__

    #define SYS "___WINDOWS___"

#elif __linux__

    #define SYS "___linux___"

#elif __APPLE__

    #define SYS "___MAC___"

#endif
// get os.

namespace rena{

    typedef enum {
        help,
        list,
        quit,
        clear,
        not_found
    }               bcmd; // basic command

    typedef struct {
        std::string command;
        bcmd        NUM;
    }               CMDLIST;

    extern CMDLIST cmdlist[];
    extern uint16_t cmdlistlength;
    

    uint16_t checkcommandlist( std::string );

}; // namespace rena

#endif