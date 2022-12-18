#include"fop.h"

const size_t rena::buffer_size = 1024 * 1024; // buffer size as 1 Mb

#ifndef BOOST_FILESYSTEM_FILESYSTEM_HPP

/**
 * @brief copy one file
 * 
 * @param __from source file path
 * @param __to target path
 */
errno_t rena::copyfile( std::string __from , std::string __to ){
    std::fstream rFile;
    std::fstream wFile;

    rFile.open( __from , std::ios::in  | std::ios::binary );
    wFile.open( __to   , std::ios::out | std::ios::binary );

    if ( !rFile.is_open() )
    {
        rFile.close();
        wFile.close();
        return F_READERROR;
    }
    if ( !wFile.is_open() )
    {
        rFile.close();
        wFile.close();
        return F_WRITEERROR;
    }

    char* buf = new char[buffer_size];

    if ( buf == NULL )
    {
        rFile.close();
        wFile.close();
        return F_BUFFERERROR;
    }

    while ( !rFile.eof() )
    {
        rFile.read( buf , buffer_size );
        wFile.write( buf , rFile.gcount() );
    }

    delete[] buf;
    rFile.close();
    wFile.close();

    return F_OK;
}

#else

/**
 * @brief copy one file
 * 
 * @param __from source file path
 * @param __to target path
 * 
 * @note require boost::filesystem
 */
errno_t rena::copyfile( std::string __from , std::string __to ){
    boost::filesystem::path from( __from );
    boost::filesystem::path to( __to );
    if ( !boost::filesystem::exists( from ) )
    {
        return F_READERROR;
    }

    boost::filesystem::copy( from , to );
    return F_OK;
}

#endif

