#include"renalog_read.h"

/**
 * @brief create one log reader
 * 
 * @param __log_folder log folder path
 */
rena::renalogreader::renalogreader( std::string __log_folder ){
    log_folder_path = __log_folder;
    logfile_path_now.clear();
    loginfo.clear();
    is_reader_constructure_successful = true;

    // this is a reader, onlhy for existed log file, no folder creation should be needed

    if ( _access( log_folder_path.c_str() , 0 ) != 0 )
    {
        std::cerr << "[renalog runtime] ERROR -> renalogreader class constructor: log file folder \"" << log_folder_path << "\" doesn't exist" << std::endl;
        is_reader_constructure_successful = false;
        // all functions won't work
    }

    return;
}

/**
 * @brief open one log file
 * 
 * @param __log_title log title
 * @param __time time this log creates
 */
errno_t rena::renalogreader::open( std::string __log_title , std::string __time ){
    std::string logfile_name = __log_title + "_" + __time + ".log";

    logfile_path_now = log_folder_path + "/" + logfile_name;

    struct stat buf;
    if ( stat( logfile_path_now.c_str() , &buf ) != 0 ) // does this log file exist
    {
        std::cerr << "[renalog runtime] ERROR -> renalogreader func. open: log file \"" << logfile_path_now << "\" doesn't exist" << std::endl;
        return LR_LOGNOTEXIST;
    }
    File.open( logfile_path_now , std::ios::in );

    if ( !File.is_open() )
    {
        return LR_OPENLOGFILEERROR;
    }
    return LR_OK;
}

/**
 * @brief open one log file
 * 
 * @param __log_title log title
 * @param __year year
 * @param __month month
 * @param __day day
 * @param __hour hour
 * @param __min minute
 * @param __sec second
 */
errno_t rena::renalogreader::open( std::string __log_title , uint16_t __year , uint16_t __month , uint16_t __day ,
                                                             uint16_t __hour , uint16_t __min , uint16_t __sec ){
    std::string ltime;
    ltime.append( uint_to_string( __year ) );
    __month < 10 ? ltime.append( "0" ).append( uint_to_string ( __month ) )
                 : ltime.append( uint_to_string ( __month ) );
    __day < 10   ? ltime.append( "0" ).append( uint_to_string( __day ) )
                 : ltime.append( uint_to_string( __day ) );
    __hour < 10  ? ltime.append( ".0" ).append( uint_to_string( __hour ) )
                 : ltime.append( "." ).append( uint_to_string( __hour ) );
    __min  < 10  ? ltime.append( ".0" ).append( uint_to_string( __min ) )
                 : ltime.append( "." ).append( uint_to_string( __min ) ); 
    __sec  < 10  ? ltime.append( ".0" ).append( uint_to_string( __sec ) )
                 : ltime.append( "." ).append( uint_to_string( __sec ) );
    return open( __log_title , ltime );
}

/**
 * @brief close this log file
 */
void rena::renalogreader::close(){
    logfile_path_now.clear();
    File.close();
    loginfo.clear();
    return;
}

void rena::renalogreader::parse(){
    std::string rline;

    while ( std::getline( File , rline ) ) // read until NULL
    {
        if ( rline.empty() )
        {
            continue;
        }

        if ( rline[0] != '#' ) // not a comment line
        {
            lognode_t info;
            switch ( rline[1] ) // switch the first letter of info, warning and error
            {
                case 'I':
                    info.type = lt::INFO;
                    break;
                
                case 'W':
                    info.type = lt::WARNING;
                    break;

                case 'E':
                    info.type = lt::ERROR;
                    break;
            }

#define YEAR_LINEBEGIN      9
#define MONTH_LINEBEGIN    14
#define DAY_LINEBEGIN      17
#define HOUR_LINEBEGIN     20
#define MINUTE_LINEBEGIN   23
#define SECOND_LINEBEGIN   26
#define AFSEC_LINEBEGIN    29

#define INFOFROM_LINEBEGIN 36

            info.time_year   = std::stoi( rline.substr( YEAR_LINEBEGIN   , 4 ) );
            info.time_month  = std::stoi( rline.substr( MONTH_LINEBEGIN  , 2 ) );
            info.time_day    = std::stoi( rline.substr( DAY_LINEBEGIN    , 2 ) );
            info.time_hour   = std::stoi( rline.substr( HOUR_LINEBEGIN   , 2 ) );
            info.time_minute = std::stoi( rline.substr( MINUTE_LINEBEGIN , 2 ) );
            info.time_second = std::stoi( rline.substr( SECOND_LINEBEGIN , 2 ) );
            info.time_afsec  = std::stoi( rline.substr( AFSEC_LINEBEGIN  , 6 ) );

            //std::cout << info.time_year << info.time_month << info.time_day << " " << info.time_hour << info.time_minute << info.time_second << " " << info.time_afsec << std::endl;

#undef YEAR_LINEBEGIN
#undef MONTH_LINEBEGIN
#undef DAY_LINEBEGIN
#undef HOUR_LINEBEGIN
#undef MINUTE_LINEBEGIN
#undef SECOND_LINEBEGIN
#undef AFSEC_LINEBEGIN

            info.info_from = rline.substr( INFOFROM_LINEBEGIN , rline.find_first_of( ':' , INFOFROM_LINEBEGIN ) - INFOFROM_LINEBEGIN );
            info.info = rline.substr( rline.find_first_of( ':' , INFOFROM_LINEBEGIN ) + 2 );

            //std::cout << info.info_from << std::endl << info.info << std::endl;

#undef INFOFROM_LINEBEGIN

            loginfo.push_back( info );
        }
    }
    return;
}

/**
 * get nr. n's log's info (n up from 1)
 * 
 * @param __n number
 */
rena::lognode_t rena::renalogreader::operator[]( size_t __n ) const {
    return loginfo[__n-1];
}

/**
 * @brief get log file size (how many log infos)
 */
size_t rena::renalogreader::size(){
    return loginfo.size();
}

std::string rena::renalogreader::uint_to_string( uint64_t __uint ){
    std::stringstream ss;
    ss << __uint;
    return ss.str();
}