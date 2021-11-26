#include <iostream>
#include <string>
#include "Application.h"


SingletonTable* InitTable(int counter, char** arguments){
    SingletonTable * Table_;
    if(counter == 2){
        std::ifstream ifile;
        ifile.open(arguments[1]);
        if(ifile && std::string(arguments[1]).substr(std::string(arguments[1]).length() - 4) == ".csv") {
            Table_ = new SingletonTable(std::string(arguments[1]));
        } else {
            Table_ = new SingletonTable();
        }
    }else if(counter == 4){
        std::ifstream ifile;
        ifile.open(arguments[1]);
        if(
            ifile && 
            std::string(arguments[1]).substr(std::string(arguments[1]).length() - 4) == ".csv" &&
            std::string(arguments[2]) == "-sep" &&
            std::string(arguments[3]).length() == 1
            ) {
            Table_ = new SingletonTable(std::string(arguments[1]),std::string(arguments[3]));
        } else {
            Table_ = new SingletonTable();
        }
    }else{
        Table_ = new SingletonTable();
    }
    return Table_;
}

/*! \brief main function. \n
 *  The main function of the spreadsheet.
 */

int main(int argc, char **argv)
{
    SingletonTable* initTable_ = InitTable ( argc, argv ) ;
    if (initTable_==nullptr){
        std::cerr<<"Error initializing table!\n";
        return -1;
    }
    Application* myApp=new Application(initTable_, true);
    

    while(myApp->getIsOn()){
        /*
            get is on-t kivenni
            exitet application-be áttenni
            applicationbe is kell execute command a ST alapján, majd ha ott nem találja akkor átadni
            a ST executenak ha az nem talál benne semmit (elseben a végén)
        */
        std::string command;
        std::string error = myApp->PrintError();
        myApp->Print();
        if(error != ""){
            std::cout << error << std::endl;
        }
        getline(std::cin, command);
        //figyewlni ha activeSheetet váltunk akkor errort melyek tableből printeljük
        myApp->ExecuteCommand(command);
    }
    delete myApp;
    return 0;
}
