#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <numeric>

typedef std::vector<std::vector<std::string> > Table;

class SingletonTable
{

protected:
    SingletonTable();
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
    void SetError(const std::string&);
    void SplitString(const std::string&, std::vector<std::string>&);
    void SetIsOn(bool);

public:
    SingletonTable(SingletonTable&) = delete;
    void operator=(const SingletonTable&) = delete;
    static SingletonTable *GetInstance();
    void ExecuteCommand(const std::string&);
    void PrintTable();
    std::string PrintError();
    bool GetIsOn() const;
};