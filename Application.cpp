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
    else if(commandType == "exit"){
        Exit();   
    }
    else{
        tables[activeSheetIndex]->ExecuteCommand(command);
        if (error_=="") setError(tables[activeSheetIndex]->PrintError());
    }
}

void Application::Print(){
    if (activeSheetIndex==-1){
        setError("Active table missing!\n");
        setIsOn(false);
        return;
    }
    tables[activeSheetIndex]->PrintTable();
    std::cout<<"\n";
    for (unsigned i=0; i<tables.size(); i++){
        std::cout<< i << ((i==activeSheetIndex) ? "*" : "" ) << ": " << tables[i]->GetName() << " "; 
    }
    std::cout<<std::endl;
}

std::string Application::PrintError(){
    if (activeSheetIndex==-1){
        setIsOn(false);
        return "Active table missing!\n";
    }
    std::string tmp_st=getError();
    setError("");
    return tmp_st;
}

Application::~Application(){
    for (auto it : tables){
        delete it;
    }
}

void Application::New(const std::string&){}
void Application::Switch(const std::string&){}
void Application::Delete(const std::string&){}
void Application::Close(const std::string&){}
void Application::Rename(const std::string&){}
void Application::Exit(){
    setIsOn(false);
    std::cout << "Folyamat megszakítva..." << std::endl;
}