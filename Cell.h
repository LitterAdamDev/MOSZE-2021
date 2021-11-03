#include <vector>
#include <iostream>
#include <float.h>

/*! \brief Cell class:
 * for the table cells
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
    void CalculateFormula(const std::vector<std::vector<Cell>>&, const std::string&);
public:
    Cell(const std::vector<std::vector<Cell>>&){}        /*! constructor of Cell class */
    Cell(const std::vector<std::vector<Cell>>& table, std::string value, int align = std::ios::left):align_{align}{
        CalculateFormula(table,value);
    }   
    static void Refresh(const std::vector<std::vector<Cell>>& table,const std::string&);
    /*!  = operator overloading */
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
    friend bool operator<(const Cell& a,const Cell& b);
    friend bool operator>(const Cell& a,const Cell& b);
};