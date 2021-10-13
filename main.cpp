#include <iostream>
#include <string>
#include "SingletonTable.h"

int main(int argc, char **argv)
{
    SingletonTable* singelton = SingletonTable::GetInstance(argc,argv);
    while(singelton->GetIsOn()){
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