#include"dirop.h"

errno_t rena::listfileindir( std::string __path , FILE_LIST& __list ){
    DIR* pdir;
    struct dirent* ptr;

    if ( !( pdir = opendir( __path.c_str() ) ) )
    {
        closedir( pdir );
        return D_ERROR;
    }

    while ( ( ptr = readdir( pdir ) ) != 0 )
    {
        if ( strcmp( ptr -> d_name , "." ) != 0 && strcmp( ptr -> d_name , ".." ) != 0 )
        {
            std::cout << __path + "/" + ptr -> d_name << std::endl;
            __list.push_back( __path + "/" + ptr -> d_name );
        }
    }

    closedir( pdir );
    return D_OK;
}

errno_t rena::listfileindir( std::string __path , FELABEL , std::string __fe , FILE_LIST& __list ){}

errno_t rena::traversedir( std::string __path , FILE_LIST& __list , uint32_t __layer ){
    _finddata_t fileInfo;
    std::string current_path = __path + "/*.*";
    int handle = _findfirst( current_path.c_str() , &fileInfo );

    if ( handle == -1 ) // traverse failed
    {
        _findclose( handle );
        return D_ERROR;
    }

    do
    {
        if ( fileInfo.attrib == _A_SUBDIR ) // one sub dir is found
        {
            for ( int i = 0 ; i < __layer ; i++ )
            {
                std::cout << "--";
            }
            std::cout << fileInfo.name << std::endl;
            uint32_t layer_temp = __layer;
            if ( strcmp( fileInfo.name , ".." ) != 0 && strcmp( fileInfo.name , "." ) != 0 )
            {
                traversedir( __path + "/" + fileInfo.name , __list , layer_temp + 1 );
            }
        }
        else
        {
            for ( int i = 0 ; i < __layer ; i++ )
            {
                std::cout << "--";
            }
            std::cout << fileInfo.name << std::endl;
        }
    } while ( !_findnext( handle , &fileInfo ) );

    _findclose( handle );
    return D_OK;
}