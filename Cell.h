#include <vector>
#include <iostream>
#include <float.h>

/*! \brief Cell class:for the table cells.
 * Attributes :  
 * enum FormulaType - enumerators: SUM, AVG, MIN, MAX.
 * int align_ -  Bitmask type to represent stream format flags,default is  std::ios::left, 
 * Adjust the text in the field to left or right.
 * std::string value_ - the value of the cell,  default value: "",.
 * bools for the value is a formula or , and it's evaluated.
 * unsigned ints TLR, TLC, BRR, BRC.
 * declaration of FormulaType.
 * string formula_ - value of the formula,  default value: "",.
 */
class Cell{
private:
    enum FormulaType {
        SUM = 0,
        AVG,
        MIN,
        MAX
    };
    int align_ = std::ios::left;
    std::string value_ = "";
    bool is_formula_ = false;
    bool is_formula_right = false;
    unsigned TLR;
    unsigned TLC;
    unsigned BRR;
    unsigned BRC;
    bool evaluated;
    FormulaType type_;
    std::string formula_ = "";

    /*! \brief CalculateFormula:
     * Calculates the aggregator functions: SUM, AVG, MIN, MAX. 
     * \param vector<std::vector<Cell>>& table
     * \param string& formula
     */
    void CalculateFormula(const std::vector<std::vector<Cell>>&, const std::string&);

public:
    /*! \brief Constructor of Cell class:
     *  Contains data of the cell .
     *  Initialize: align with align - default is left, value with value.
     *  Runs the CalculateFormula function with table vector and with value of the formula string.
     *  \param table  The table vector.
     *  \param value  The content of the cell.
     *  \param align  Tthe align of the cell.
     */
    Cell(const std::vector<std::vector<Cell>>&){}
    Cell(const std::vector<std::vector<Cell>>& table, std::string value, int align = std::ios::left):align_{align}{
        CalculateFormula(table,value);
    }   

    /*! \brief Refresh:
     * Static method of the cell that reruns all formulas in the table.
     * \param vector<std::vector<Cell>>& table 
     * \param string& valCheck
     */
    static void Refresh(const std::vector<std::vector<Cell>>& table,const std::string&);

    /*!  
     *  operator overloading =
     *  \param  Cell other
     */
    Cell& operator=(Cell other){
        align_=other.align_;
        value_=other.value_;
        is_formula_ = other.is_formula_;
        is_formula_right = other.is_formula_right;
        TLR = other.TLR;
        TLC = other.TLC;
        BRR = other.BRR;
        BRC = other.BRC;
        evaluated = true;
        type_ = other.type_;
        formula_ = other.formula_;
        return *this;
    }
    int GetAlign() const{ /*! return value of align */
        return align_;
    }
    std::string GetValue() const{   /*! return the value or the formula of the cell */
        if(this->is_formula_ && this->is_formula_right){
            return formula_;
        }else{
            return value_;
        }
    }
    std::string GetPrint() const{  /*! get the value of print */
        return value_;
    }
    std::string GetFormula() const{  /*! get the formula */
        return formula_;
    }
    void SetAlign(int align){       /*! set the value of align */
        this->align_ = align;
    }
    void SetValue(const std::vector<std::vector<Cell>>& table, std::string value){  /*! set the value of the cell with CalculateFormula */
        CalculateFormula(table,value);
    }

    /*!  
     *  operator overloading < 
     *  \param  Cell& a
     *  \param  Cell& b
     */
    friend bool operator<(const Cell& a,const Cell& b);

    /*!  
     *  operator overloading >
     *  \param  Cell& a
     *  \param  Cell& b
     */
    friend bool operator>(const Cell& a,const Cell& b);
};