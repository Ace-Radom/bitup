#include"renalog_core.h"

/**
 * @brief create one log system
 * 
 * @param __log_folder log folder path
 */
rena::renalog::renalog( std::string __log_folder ){
    log_folder_path = __log_folder;
    logfile_path_now.clear();

    if ( _access( log_folder_path.c_str() , 0 ) != 0 ) // check if log folder exists
    {
        create_parent_folder( log_folder_path );
        // doesn't exist, create it
    }

    return;
}

/**
 * @brief open one log file
 * 
 * @param __log_title log title
 */
errno_t rena::renalog::open( std::string __log_title ){

//===================================================
//  log file name format:
//  ${log_title}_%YYYY%MM%DD.%HH.%MM.%SS.log
//===================================================

    std::string logfile_name = __log_title;
    logfile_name.append( "_" );
    
    time_now = time( 0 );
    // get time now

    char tmc[32];
    memset( tmc , '\0' , sizeof( tmc ) );
    strftime( tmc , sizeof( tmc ) , "%Y%m%d.%H.%M.%S",localtime( &time_now ) );
    // change time into char in format

    logfile_name.append( tmc ).append( ".log" );
    // link log file name

    logfile_path_now = log_folder_path + "/" + logfile_name;
    // link log folder path to get the log file path

    struct stat buf;
    if ( stat( logfile_path_now.c_str() , &buf ) == 0 ) // log file already existed
    {
        File.open( logfile_path_now , std::ios::out | std::ios::app );
    }
    else
    {
        File.open( logfile_path_now , std::ios::out );
    }

    if ( !File.is_open() )
    {
        return L_OPENLOGFILEERROR;
    }
    return L_OK;
}

/**
 * @brief close current log file
 */
void rena::renalog::close(){
    logfile_path_now.clear();
    File.close();
    return;
}

/**
 * @brief write infos to log
 * 
 * @param __lt log type
 * @param __from info's creator's sign
 * @param __info info to write
 */
void rena::renalog::log( lt __lt , std::string __from , std::string __info ){
    delete_colon_in_info_from( __from );
    
    switch ( __lt ){
        case lt::INFO:
            File << "[INFO    ";
            break;
        
        case lt::WARNING:
            File << "[WARNING ";
            break;
        
        case lt::ERROR:
            File << "[ERROR   ";
            break;
    }

    time_now = time( 0 );
    // get time now

    char tmc[32];
    memset( tmc , '\0' , sizeof( tmc ) );
    strftime( tmc , sizeof( tmc ) , "%Y.%m.%d %H:%M:%S",localtime( &time_now ) );
    // change time into char in format

    File << tmc << get_microsecondsnow();
    // also add microsecond here
    File << "]" << __from << ": " << __info;
    // write info from

    File << std::endl;
    return;
}

/**
 * @brief add comment into log file
 * 
 * @param __info comment info
 */
void rena::renalog::comment( std::string __info ){
    File << "# " << __info << std::endl;
    return;
}

/**
 * @brief wrap line
 */
void rena::renalog::wrap(){
    File << std::endl;
    return;
}

/**
 * @brief get microsecond now
 * 
 * @return .${microsecond, 6 bit}
 */
std::string rena::renalog::get_microsecondsnow(){
    auto now = std::chrono::system_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds> ( now.time_since_epoch() ) % 1000000;

    std::ostringstream oss;
    oss.fill( '0' );
    oss << '.' << std::setw( 6 ) << us.count();

    return oss.str();    
}

void rena::renalog::create_parent_folder( std::string __path ){
    std::string curfolder = __path;
    // current folder
    
    if ( _access( curfolder.c_str() , 0 ) == 0 )
    {
        return;
    }

    std::list <std::string> folderlist;
    folderlist.push_front( curfolder );
    
    std::string curpfolder = get_parent_folder_from_path( curfolder );
    // current parent folder

    while ( curpfolder != curfolder )
    {
        if ( _access( curpfolder.c_str() , 0 ) == 0 )
        {
            break;
        }

        folderlist.push_front( curpfolder );
        curfolder = curpfolder;
        curpfolder = get_parent_folder_from_path( curfolder );
    }

    for ( auto it : folderlist )
    {
        _mkdir( it.c_str() );
    }
    return;
}

std::string rena::renalog::get_parent_folder_from_path( std::string __path ){
    std::string parentpath = __path;
    size_t pos = parentpath.find_last_of( '/' );
    if ( pos != -1 )
    {
        parentpath.erase( pos );
    }

    return parentpath;
}

void rena::renalog::delete_colon_in_info_from( std::string& __from ){
    for ( int i = 0 ; i < __from.size() ; i++ )
    {
        if ( __from[i] == ':' )
        {
            __from[i] = ' ';
        }
    }
    return;
}