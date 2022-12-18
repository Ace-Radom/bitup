#include"dirop.h"

errno_t rena::traversedir( std::string __path , FILE_LIST& __fl ){
    boost::filesystem::path this_dir( __path );
    if ( !boost::filesystem::exists( this_dir ) )
    {
        return D_NOTEXIST;
    }

    boost::filesystem::directory_iterator this_dir_list( __path );
    for ( auto& it : this_dir_list )
    {
        if ( it.status().type() == boost::filesystem::file_type::directory_file )
        {
            traversedir( it.path().string() , __fl );
        }
        else
        {
            __fl.push_back( it.path().string() );
        }
    }

    return D_OK;
}