#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <fstream>
#include <sstream>

struct Cell{
private:
    int align_ = std::ios::left;
    std::string value_ = "";

public:
    Cell(){}
    Cell(std::string value, int align = std::ios::left):align_{align},value_{value}{}
    int GetAlign() const{
        return align_;
    }
    std::string GetValue() const{
        return value_;
    }
    void SetAlign(int align){
        this->align_ = align;
    }
    void SetValue(std::string value){
        this->value_ = value;
    }
};
typedef std::vector<std::vector<Cell>> Table;

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
    bool is_number(const std::string& );
    void Swap(const std::string&);

public:
    SingletonTable(SingletonTable&) = delete;
    void operator=(const SingletonTable&) = delete;
    static SingletonTable *GetInstance(int,char**);
    void ExecuteCommand(const std::string&);
    void PrintTable();
    std::string PrintError();
    bool GetIsOn() const;
};