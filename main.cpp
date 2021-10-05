#include <iostream>
#include <string>
#include "SingletonTable.h"

int main()
{
    SingletonTable* singelton = SingletonTable::GetInstance();
    while(singelton->GetIsOn()){
        std::cout << std::flush;
        std::string command;
        std::string error = singelton->PrintError();
        singelton->PrintTable();
        if(error != ""){
            std::cout << error << std::endl;
        }
        getline(std::cin, command);
        singelton->ExecuteCommand(command);
    }
    return 0;
}