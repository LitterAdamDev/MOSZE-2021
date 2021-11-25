#include "SingletonTable.h"

class Application {
    public:
        //there must be an initializing table object, or make execute command static
        Application(SingletonTable& table, bool ison) : isOn{ison}, activeSheetIndex{-1} {
            tables.clear();
            AddTable(table);
        };
        Application(const Application&) = delete;
        void operator=(const Application&) = delete;
        virtual ~Application();
        
        void AddTable(SingletonTable& tableToAdd){
            tables.push_back(tableToAdd);
            if (activeSheetIndex==-1) activeSheetIndex==tables.size()-1;
        }
        bool getIsOn() {return isOn;}
        
        void ExecuteCommand(const std::string& command);

    private:
        void New(const std::string&);
        void Switch(const std::string&);
        void Delete(const std::string&);     
        void Close(const std::string&);
        void Rename(const std::string&);

        void setIsOn(bool ison) {isOn=ison;}
        std::vector<SingletonTable&> tables;
        int activeSheetIndex;
        bool isOn;

};