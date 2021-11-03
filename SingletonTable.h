#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <fstream>
#include <sstream>
#include <float.h>
#include "Cell.h"

typedef std::vector<std::vector<Cell>> Table; /*! definition of Table vector */

/*! \brief SingletonTable class:
 *  for the table
 */
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
    void Swap(const std::string&);                                     
    void Sort(const std::string&);
    void RefreshTable(){  /*! calls the refresh function on the table */
        Cell::Refresh(this->table_,"");
    }                                      
    enum SortType{                                                      
        asc=1,
        desc=0
    };                                                                  
    enum SortBy{                                                        
        sortByCol=1,
        sortByRow=0
    };                                                                 
    std::string& string_toupper(std::string& myst);                     
    std::string& string_toupper(std::string&& myst);

public:
    SingletonTable(SingletonTable&) = delete;                           
    void operator=(const SingletonTable&) = delete;
    static SingletonTable *GetInstance(int,char**);                          
    void ExecuteCommand(const std::string&);                                 
    void PrintTable();                                                       
    std::string PrintError();                                               
    bool GetIsOn() const;                                                    
    static bool is_number(const std::string& );                              
    bool compare_func(const Cell& a,const Cell& b, const SortType& stype);   
    friend class Cell;
};