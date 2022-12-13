#include<iostream>
#include<string>

#include"formatext.h"
#include"commandlist.h"

#define FOREVER true

int main(){
    std::string readin;

    std::cout << "==================================================================" << "\n"
              << "===                         bitup v1.0                         ===" << "\n"
              << "===     Project Source: https://github.com/Ace-Radom/bitup     ===" << "\n"
              << "===   For more infos please visit this project's Github site   ===" << "\n"
              << "===        Still in developing, thanks for your support        ===" << "\n"
              << "==================================================================" << "\n"
              << "\n";
    // print shell begin

#pragma region shell

    while ( FOREVER )
    {
        std::cout << "bitup> ";
        // beginning of each line

        std::getline( std::cin , readin );
        rena::allcharlower( readin );
        // readin command and low all characters

//      std::cout << readin << std::endl; // debug
//      debug line: check the readin command after function allcharlower        

//      std::cout << rena::checkcommandlist( readin ) << std::endl; // debug
//      debug line: check function checkcommandlist

        switch ( rena::checkcommandlist( readin ) ){
            case rena::bcmd::quit: // quit shell
                return 0;
                break;

            case rena::bcmd::clear: // clear shell
                if ( SYS == "___WINDOWS___" )
                {
                    system( "cls" );
                }
                if ( SYS == "___linux___" )
                {
                    system( "clear" );
                }
                // clear command is different in win32 or in linux ("cls" and "clear")
                // in powershell do recongnize "clear" as clear-screen command but isn't recongnized by win32 system-shell

        }
    }

#pragma endregion shell
    
    return 0;

}

#undef FOREVER