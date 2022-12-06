#include"formatext.h"

void rena::allcharupper( std::string &__str ){
    for ( int i = 0 ; i < __str.size() ; i++ )
    {
        if ( __str[i] >= 'a' && __str[i] <= 'z' )
        {
            __str[i] -= 32;
        }
    }
    return;
}

void rena::allcharlower( std::string &__str ){
    for ( int i = 0 ; i < __str.size() ; i++ )
    {
        if ( __str[i] >= 'A' && __str[i] <= 'Z' )
        {
            __str[i] += 32;
        }
    }
    return;
}