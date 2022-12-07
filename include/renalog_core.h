#ifndef _RENALOG_CORE_H_
#define _RENALOG_CORE_H_

#include<time.h>
#include<string>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<io.h>
#include<direct.h>
#include<chrono>
#include<sstream>
#include<list>
#include<vector>

#define L_OK 0x00
#define L_OPENLOGFILEERROR -( 0x01 )

namespace rena {

    typedef int errno_t;

    typedef enum {
        INFO,
        WARNING,
        ERROR
    }           lt;

    class renalog{

        // Constructor:
        public:
            renalog( std::string );

        // public interface:
        public:
            errno_t open( std::string );
            void close();

            void write( lt , std::string );
            void wrap();

        // data members:
        protected:
            std::string log_folder_path;
            std::string logfile_path_now;

            time_t time_now;

        // file stream members:
        protected:
            std::fstream File;

        // functional functions:
        private:
            std::string get_microsecondsnow();
            
            void create_parent_folder( std::string );
            std::string get_parent_folder_from_path( std::string );

    }; // class renalog

} // namespace rena

#endif