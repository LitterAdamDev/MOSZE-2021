#include "SingletonTable.h"

class Application {
    public:
        //there must be an initializing table object, or make execute command static
        Application(SingletonTable* table, bool ison) : isOn{ison}, activeSheetIndex{-1}, error_{""} {
            tables.clear();
            AddTable(table);
        };
        Application(Application&) = delete;
        void operator=(const Application&) = delete;
        virtual ~Application();
        
        void AddTable(SingletonTable* tableToAdd){
            tables.push_back(tableToAdd);
            if (activeSheetIndex==-1) activeSheetIndex=tables.size()-1;
        }
        bool getIsOn() const {return isOn;}
        void setIsOn(bool ison){isOn=ison;}
        int getActiveSheetIndex() const {return activeSheetIndex;}
        std::string getError() const {return error_;}
        void setError(std::string err) {error_=err;}
        void Print();
        std::string PrintError();
        void ExecuteCommand(const std::string& command);

    private:
        void New(const std::string&);
        void Switch(const std::string&);
        void Open(const std::string&);
        void Close(const std::string&);
        void Rename(const std::string&);
        void Exit();

        std::vector<SingletonTable*> tables;
        int activeSheetIndex;
        bool isOn;
        std::string error_;
};