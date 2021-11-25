#include "Application.h"

void Application::ExecuteCommand(const std::string& command){
    std::string commandType;

    if(command.length() > 0)
        commandType = command.substr(0, command.find(' '));
        std::transform(commandType.begin(), commandType.end(), commandType.begin(),[](unsigned char c){ return std::tolower(c); });
       
    std::string param = command.substr(command.find(' ')+1,command.length());
    if (param.length() < 0)
        param = "";
    if(commandType == "new"){
        New(param);
    }else if(commandType == "switch"){
        Switch(param);
    }else if(commandType == "open"){
        Delete(param);     
    }else if(commandType == "close"){
        Close(param);
    }else if(commandType == "rename"){
        Rename(param);   
    }
    else{
        if (activeSheetIndex==-1){
            std::cerr<<"Active table missing!";
            return;
        }
        tables[activeSheetIndex].ExecuteCommand(command);
    }
}

void Application::Print(){
    if (activeSheetIndex==-1){
        std::cerr<<"Active table missing!";
        return;
    }
    tables[activeSheetIndex].PrintTable();
    std::cout<<"\n\n";
    for (unsigned i=0; i<tables.size(); i++){
        std::cout<< i << ((i==activeSheetIndex) ? "*" : "" ) << ": " << tables[i].GetName() << " "; 
    }
    std::cout<<std::endl;
}