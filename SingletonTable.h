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
/*! \brief Constructor of SingletonTable class
 *  Reads the data from a file 
 *  \param string filename 
 *  \param string separator
 *  Initialize: error with "", isOn with true
 */
    SingletonTable();  
    SingletonTable(std::string); 
    SingletonTable(std::string,std::string);
    static SingletonTable* SingletonTable_;
    std::string value_;
    Table table_;
    std::string error_;
    bool isOn;

    /*! \brief Edit:
    * Enter the specified string into the cell with XY coordinates
    * \param string &attrs
    */
    void Edit(const std::string&);  
       
    /*! \brief Add rows/columns:
    *  Adds N new rows / columns of empty cells to the end of the table
    *  \param string &attrs
    */                                   
    void Add(const std::string&);     

    /*! \brief Delete row/column:
    *  Deletes the column X (lettel) or row Y (number) 
    *  \param const string &attrs
    */                                      
    void Delete(const std::string&);   

    /*! \brief Insert row/column:
    *   Insert N new rows/columns of empty cells before/after a row/column
    *  \param const string &attrs
    */                                  
    void Insert(const std::string&);    

    /*! 
    *  Exits the running program
    */                                  
    void Exit();         

    /*! \brief Save in file:
    *  Prints the current table to a file. 
    *  If the specified file exists, overwrites the file.
    *  \param const string &attrs
    */                                              
    void Save(const std::string&);   

    /*! \brief SetError:
    *  adds the command to the error that will be written out.
    *  \param  const string &command
    */                                   
    void SetError(const std::string&);      

    /*! \brief SplitString: Split the strings 
    *  \param string& s
    *  \param vector<std::string>& v
    */                              
    void SplitString(const std::string&, std::vector<std::string>&);    

    /*! sets the isOn with a value
    *  \param bool value
    */   
    void SetIsOn(bool);       

    /*! \brief align left/right:
    * Sets the content of the cell to be displayed left/right
    * \param const string &attrs
    */                                             
    void Align(const std::string&);   

    /*! \brief Clear the cells:
    *  Sets the content of the cells to blank
    *  \param const string &attrs
    */                                      
    void Clear(const std::string&);

    
    /*! \brief Swap:
    *  swaps the contents of two cells.
    *  \param const string &attrs
    */                                             
    void Swap(const std::string&); 

    /*! \brief Sort:
    *  sorts the rows/columns in the table by the specified row/column in ascending / descending order.
    *  \param const string &attrs
    */                                       
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

    /*! \brief toupper:
    *  converts a given character to uppercase. 
    *  Only English alphabet a - z.
    *  \param string& myst
    */                                                             
    std::string& string_toupper(std::string& myst);                     
    std::string& string_toupper(std::string&& myst);

public:
    SingletonTable(SingletonTable&) = delete;                           
    void operator=(const SingletonTable&) = delete;

    /*! \brief GetInstance:
    *  initializes the SingletonTable class object 
    *  \param int counter 
    *  \param char** arguments
    */
    static SingletonTable *GetInstance(int,char**);  

    /*! \brief Execute the commands:
    *  Checks which command is used and execute it.
    *  \param const string &command
    */   
    void ExecuteCommand(const std::string&);       

    void PrintTable();   /*! \brief PrintTable: Prints the table */    

    /*! \brief PrintError:Print the error messages 
    *  \return tmp
    */                                                    
    std::string PrintError();   

    bool GetIsOn() const;     /*! \return the value of isOn  */      

    /*! \brief is_number:
    *  checks a string is convertable to a number.
    *  \param string& s
    */
    static bool is_number(const std::string& );    

    /*! \brief compare function:
    * compares a and b 
    * \param  Cell& a  
    * \param  Cell& b
    * \param  SortType& stype
    * \return the result of a>b or a<b
    */                              
    bool compare_func(const Cell& a,const Cell& b, const SortType& stype);   
    friend class Cell;
};