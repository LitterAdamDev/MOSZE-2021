#include "SingletonTable.h"

/*! \brief Application class: to handle multiple tables.
 * \param vector<SingletonTable*> tables
 * \param int activeSheetIndex - index of the active table
 * \param bool isOn 
 * \param string error_ - to print the errors
 */

class Application {
    public:
        //there must be an initializing table object, or make execute command static
        /*! \brief Constructor of Application class:
         *  Initialize: activeSheetIndex with -1, isOn with ison parameter and error_ with "".
         *  Runs the clear() and AddTable() functions.
         *  \param table  The table vector.
         *  \param ison.
         */
        Application(SingletonTable* table, bool ison) : activeSheetIndex{-1}, isOn{ison},  error_{""} {
            tables.clear();
            AddTable(table);
        };
        Application(Application&) = delete;
        void operator=(const Application&) = delete;
        virtual ~Application();
        
        /*! \brief AddTable:
         *  Adds a new table and push it to the end of tables vector.
         *  \param SingletonTable* tableToAdd
         */
        void AddTable(SingletonTable* tableToAdd){
            tables.push_back(tableToAdd);
            if (activeSheetIndex==-1) activeSheetIndex=tables.size()-1;
        }

        /*! \return the value of isOn  
         */     
        bool getIsOn() const {return isOn;}

        /*! Sets the isOn with a value
         *  \param bool ison
         */  
        void setIsOn(bool ison){isOn=ison;}

        /*! \return activeSheetIndex - the index of the active table
         */  
        int getActiveSheetIndex() const {return activeSheetIndex;}

        /*! \return the error message 
         */ 
        std::string getError() const {return error_;}

        /*! Sets the error message with the err parameter
         *  \param string err
         */  
        void setError(std::string err) {error_=err;}

        /*! Print the table names
         */ 
        void Print();

        /*! \brief PrintError:Print the error messages 
         *  \return tmp_st
         */    
        std::string PrintError();

        /*! \brief Execute the commands:
         *  Checks which command is used and execute it.
         *  \param  string& command
         */   
        void ExecuteCommand(const std::string& command);

        /*! \return the table from tables
         *  \param int index - the index of the table in the tables vector
         */ 
        const SingletonTable& GetTable(int index) const {return *(tables[index]);}

    private:
        /*! \brief New sheet:
         *  Creates a new table with the specified name and sets it active.
         *  \param  string&
         */ 
        void New(const std::string&);

        /*! \brief Switch:
         *  Sets the table with index N to active.
         *  \param  string&
         */ 
        void Switch(const std::string&);

        /*! \brief Open: 
         *  Creates a new table, loads it from the specified file, sets its name to the file name, and sets it active.
         *  \param  string&
         */ 
        void Open(const std::string&);

        /*! \brief Close: 
         *  Closes the table with index N and frees the memory.
         *  \param  string&
         */ 
        void Close(const std::string&);

        /*! \brief Rename: 
         *  Renames the N index table to the specified new name.
         *  \param  string&
         */
        void Rename(const std::string&);

        /*! 
         *  Exits the running program.
         */ 
        void Exit();

        std::vector<SingletonTable*> tables;
        int activeSheetIndex;
        bool isOn;
        std::string error_;
};