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
        Open(param);     
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

void Application::New(const std::string& attrs){
    //new sheet name
    int errorCode = 0;
    SingletonTable* tmpTable;
    std::vector<std::string> parts;
    SingletonTable::SplitString(attrs,parts);
    if(parts.size() < 2){
        errorCode = 1;
    }else if(parts.size() > 2){
        errorCode = 2;
    }else{
        if((SingletonTable::string_toupper(parts[0])) != "SHEET"){
            errorCode = 3;
        }
    }
    switch (errorCode){
    case 1:
        setError("Missing attributes!");
        break;
    case 2:
        setError("Too much attributes!");
        break;
    case 3:
        setError("First attribute of new command must be: \"sheet\" !");
        break;
    case 0:
        tmpTable=new SingletonTable();
        if (tmpTable==nullptr){
            setError("Error while constructing Table!");
            return;
        }
        tmpTable->SetName(parts[1]);
        //ownership goes to tables vector
        tables.push_back(tmpTable);
        activeSheetIndex=tables.size()-1;
    default:
        break;
    }
}
void Application::Switch(const std::string& attrs){
    //switch N
    int errorCode = 0;
    std::vector<std::string> parts;
    SingletonTable::SplitString(attrs,parts);
    int newIndex;
    if(parts.size() < 1){
        errorCode = 1;
    }else if(parts.size() > 1){
        errorCode = 2;
    }else{
        if(!SingletonTable::is_number(parts[0])){
            errorCode = 3;
        }
    }
    if (errorCode==0){
        newIndex=std::stoi(parts[0]);
        if ( ( newIndex < 0 ) || ( newIndex > tables.size() -1 ) ){
            errorCode = 4;
        }
    }
    switch (errorCode){
    case 1:
        setError("Missing attributes!");
        break;
    case 2:
        setError("Too much attributes!");
        break;
    case 3:
        setError("First attribute of switch command must be a positive integer!");
        break;
    case 4:
        setError("Table with that index does not exist!");
        break;
    case 0:
        activeSheetIndex=newIndex;
    default:
        break;
    }
}
void Application::Open(const std::string& attrs){
    //open filename [-sep ,]
    std::vector<std::string> parts;
    SingletonTable::SplitString(attrs,parts);
    std::string separator = ";";
    int errorCode = 0;
    SingletonTable* tmpTable;
    if(parts.size() == 1 || parts.size() == 3){
        if(parts[0].length() >4 && parts[0].substr(parts[0].length()-4) == ".csv"){
            if(parts.size() == 3){
                if(parts[1] == "-sep" && parts[2].length() == 1){
                    separator = parts[2];
                }else{
                    errorCode = -1;
                }
            }
        }else{
            errorCode = -1;
        }
    }else{
        errorCode = -1;
    }
    if(errorCode == 0){
        tmpTable=new SingletonTable(parts[0], separator);
        if (tmpTable==nullptr){
            setError("Error while constructing Table!");
            return;
        }
        tables.push_back(tmpTable);
        activeSheetIndex=tables.size()-1;
    }else{
        setError("Error while opening!");
    }
}

void Application::Close(const std::string&){}
void Application::Rename(const std::string&){}
void Application::Exit(){
    setIsOn(false);
    std::cout << "Folyamat megszakítva..." << std::endl;
}