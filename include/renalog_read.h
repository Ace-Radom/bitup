#ifndef _RENALOG_READ_H_
#define _RENALOG_READ_H_

#include<fstream>
#include<direct.h>
#include<iostream>
#include<vector>
#include<stdint.h>
#include<sys/stat.h>
#include<sstream>

#include"renalog_core.h"

#define LR_OK 0x00
#define LR_LOGNOTEXIST -( 0x01 )
#define LR_OPENLOGFILEERROR -( 0x02 )

namespace rena {

    typedef int errno_t;

    typedef struct {
        std::string info;
        std::string info_from;
        lt type;

        uint16_t time_year;
        uint16_t time_month;
        uint16_t time_day;
        uint16_t time_hour;
        uint16_t time_minute;
        uint16_t time_second;
        uint32_t time_afsec; // after second
    }           lognode_t;
    
    class renalogreader{

        // Constructor:
        public:
            renalogreader( std::string );

        // public interface:
        public:
            errno_t open( std::string , std::string );
            errno_t open( std::string , uint16_t , uint16_t , uint16_t , uint16_t , uint16_t , uint16_t );
            void close();

            void parse();
            size_t size();

            lognode_t operator[]( size_t ) const;


        // data members:
        protected:
            std::string log_folder_path;
            std::string logfile_path_now;

            std::vector <lognode_t> loginfo;

            bool is_reader_constructure_successful;


        // file stream member:
        protected:
            std::fstream File;

        // functional functions:
        private:
            std::string uint_to_string( uint64_t );

        

    }; // class renalogreader

} // namespace rena

#endif