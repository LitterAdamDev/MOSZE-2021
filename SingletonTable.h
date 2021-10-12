#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <fstream>
#include <sstream>

typedef std::vector<std::vector<std::string> > Table;

class SingletonTable
{

protected:
    SingletonTable();
    SingletonTable(std::string);
    SingletonTable(std::string,std::string);
    static SingletonTable* SingletonTable_;

    std::string value_;
    Table table_;
    std::string error_;
    bool isOn;
    void Edit(const std::string&);
    void Add(const std::string&);
    void Delete(const std::string&);
    void Insert(const std::string&);
    void Exit();
    void Save(const std::string&);
    void SetError(const std::string&);
    void SplitString(const std::string&, std::vector<std::string>&);
    void SetIsOn(bool);
    void Align(const std::string&);
    void Clear(const std::string&);

public:
    SingletonTable(SingletonTable&) = delete;
    void operator=(const SingletonTable&) = delete;
    static SingletonTable *GetInstance(int,char**);
    void ExecuteCommand(const std::string&);
    void PrintTable();
    std::string PrintError();
    bool GetIsOn() const;
};