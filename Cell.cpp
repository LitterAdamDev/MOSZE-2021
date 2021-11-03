#include <Cell.h>
#include <SingletonTable.h>

/*! \brief CalculateFormula: \n
 * Calculates the aggregator functions: SUM, AVG, MIN, MAX \n
 * Parameters: const std::vector<std::vector<Cell>>& table, const std::string& formula \n
 */

void Cell::CalculateFormula(const std::vector<std::vector<Cell>>& table,const std::string& formula){
    //BASIC INPUT VALIDATION
    unsigned errorCode = 0;
    if(formula.length() < 11){
        errorCode = 1;
    }else{
        if(formula[0] != '='){
            errorCode = 1;
        }else{
            std::string tmp_type = formula.substr(1,3);
            if(tmp_type == "SUM"){
                this->type_ = FormulaType::SUM;
            }else if(tmp_type == "AVG"){
                this->type_ = FormulaType::AVG;
            }else if(tmp_type == "MIN"){
                this->type_ = FormulaType::MIN;
            }else if(tmp_type == "MAX"){
                this->type_ = FormulaType::MAX;
            }else{
                errorCode = 1;
            }
        }
    }
    if(errorCode != 0){
        // THE INPUT DOES NOT CONTAIN FORMULA
        this->value_ = formula;
        this->is_formula_ = false;
        this->is_formula_right = false;
        this->formula_ = "";
    }else{
        // THE INPUT DOES CONTAIN A FORMULA
        // GET THE COORDINATES OF THE RANGE
        int pos = formula.find(':');
        int posLP = formula.find('(');
        int posRP = formula.find(')');
        this->is_formula_ = true;
        this->formula_ = formula;
        if(pos != -1 && posLP != -1 && posLP != -1){
            std::string firstCell = formula.substr(posLP+1,2);
            std::string secondCell = formula.substr(pos+1,2);
            // VALIDATE COORINATES
            if(
                isalpha(firstCell[0]) &&
                SingletonTable::is_number(firstCell.substr(1,firstCell.length())) &&
                isalpha(secondCell[0]) &&
                SingletonTable::is_number(secondCell.substr(1,secondCell.length()))
            ){
                int topLeftRow;
                int topLeftCol;
                int bottomRightRow;
                int bottomRightCol;
                double result = 0;
                unsigned counter = 0;
                double min = DBL_MAX;
                double max = -DBL_MAX;
                topLeftRow = stoi(firstCell.substr(1,firstCell.length())) < stoi(secondCell.substr(1,secondCell.length()))?
                stoi(firstCell.substr(1,firstCell.length())) : stoi(secondCell.substr(1,secondCell.length()));
                bottomRightRow = stoi(firstCell.substr(1,firstCell.length())) > stoi(secondCell.substr(1,secondCell.length()))?
                stoi(firstCell.substr(1,firstCell.length())) : stoi(secondCell.substr(1,secondCell.length()));
                topLeftCol = int(std::toupper(firstCell[0]) - 'A') < int(std::toupper(secondCell[0]) - 'A')?
                int(std::toupper(firstCell[0]) - 'A') : int(std::toupper(secondCell[0]) - 'A');
                bottomRightCol = int(std::toupper(firstCell[0]) - 'A') > int(std::toupper(secondCell[0]) - 'A')?
                int(std::toupper(firstCell[0]) - 'A') : int(std::toupper(secondCell[0]) - 'A');
                this->is_formula_right = true;
                this->TLR = topLeftRow;
                this->TLC = topLeftCol;
                this->BRR = bottomRightRow;
                this->BRC= bottomRightCol;
                //VALIDATE RANGE
                if(  
                    topLeftRow > table.size() ||
                    bottomRightRow > table.size() || 
                    topLeftCol > (table[0].size()-1) || 
                    bottomRightCol > (table[0].size()-1)
                ){
                    // WRONG RANGE
                    this->value_ = "#NAME?";
                }else{
                    // GOOD RANGE
                    // CALCULATE THE RESULT OF THE FORMULA
                    for(unsigned r = topLeftRow; r <= bottomRightRow; r++){
                        for(unsigned c = topLeftCol; c <= bottomRightCol; c++){
                            std::string tmp = table[r-1][c].GetValue();
                            if(this->type_ == FormulaType::SUM){
                                if(SingletonTable::is_number(tmp)){
                                    result += std::stod(tmp);
                                }
                            }else if(this->type_ == FormulaType::AVG){
                                if(SingletonTable::is_number(tmp)){
                                    result += std::stod(tmp);
                                    counter++;
                                }
                                if(r == bottomRightRow && c == bottomRightCol){
                                    result /= (double)counter;
                                }
                            }else if(this->type_ == FormulaType::MIN){
                                if(SingletonTable::is_number(tmp)){
                                    double val = std::stod(tmp);
                                    if(val < min){
                                        min = val;
                                    }
                                }
                                if(r == bottomRightRow && c == bottomRightCol){
                                    result = min;
                                }
                            }else if(this->type_ = FormulaType::MAX){
                                if(SingletonTable::is_number(tmp)){
                                    double val = std::stod(tmp);
                                    if(val > max){
                                        max = val;
                                    }
                                }
                                if(r == bottomRightRow && c == bottomRightCol){
                                    result = max;
                                }
                            }
                        }
                    }
                    // SET THE VALUE OF THE CELL TO THE RESULT
                    this->value_ = std::to_string(result).substr(0,std::to_string(result).find('.')+4);
                }
            }
        }
    }
    // CALCULATE ALL CELL AGAIN //
    /*
        SET EVALUATED TRUE IF IT IS A VALID FORMULA WITH VALID RANGE 
        SET EVALUATED FALSE IF IT IS NOT
    */
    for(unsigned mr = 0; mr < table.size(); mr++){
        for(unsigned mc = 0; mc < table[0].size(); mc++){
            Cell* acell = const_cast<Cell*>(&table[mr][mc]);
            
            if(table[mr][mc].is_formula_ && table[mr][mc].is_formula_right && table[mr][mc].value_ != "#NAME?"){
                acell->evaluated = false;
            }else{
                acell->evaluated = true;
            }
            if(table[mr][mc].value_ == "#NAME?"){
                if(
                    !(acell->TLR > table.size() ||
                    acell->BRR > table.size()||
                    acell->TLC > (table[0].size() - 1)||
                    acell->BRC > (table[0].size() - 1))
                ){
                    acell->evaluated = false;
                }
            }
        }
    }
    //SOLVE ALL FORMULAS
    /*
        THERE MUST BE AT LEAST ONE SOLVABLE FORMULA TO CONTINUE
        IT THERE IS NONE -> ALL THE LEFTOVER FORMULAS ARE IN CYCLE
    */
    unsigned updated = 1;
    while(updated != 0){
        updated = 0;
        //SET UPDATED 0 EVERY ITERATION
        for(unsigned mr = 0; mr < table.size(); mr++){
            for(unsigned mc = 0; mc < table[0].size(); mc++){
                //ITERATE WHOLE TABLE
                if(table[mr][mc].is_formula_ && table[mr][mc].is_formula_right && !table[mr][mc].evaluated){
                    // CHECK CELL IF IT CONTAINS NOT EVALUATED FORMULA
                    double result = 0;
                    unsigned counter = 0;
                    double min = DBL_MAX;
                    double max = -DBL_MAX;
                    // DOES THE RANGE OF THIS FORMULA CONTAINS OTHER FORMULAS?
                    bool has_AGF = false;
                    for(unsigned r = table[mr][mc].TLR; r <= table[mr][mc].BRR; r++){
                        for(unsigned c = table[mr][mc].TLC; c <= table[mr][mc].BRC; c++){
                            //ITERATE THE RANGE OF THE FORMULA
                            if(!table[r - 1][c].evaluated){
                                //SET TRUE IF THERE IS AT LEAST ONE FURMULA IN THE RANGE WHICH IS NOT EVALUATED
                                has_AGF = true;
                            }else{
                                // USE THE VALUE TO CALCULATE RESULT, BUT WONT USE IT IF LATER WE FIND AN NOT EVALUATED FORMULA IN THE RANGE 
                                std::string tmp = table[r-1][c].GetPrint();
                                if(table[mr][mc].type_ == FormulaType::SUM){
                                    if(SingletonTable::is_number(tmp)){
                                        result += std::stod(tmp);
                                    }
                                }else if(table[mr][mc].type_ == FormulaType::AVG){
                                    if(SingletonTable::is_number(tmp)){
                                        result += std::stod(tmp);
                                        counter++;
                                    }
                                    if(r == table[mr][mc].BRR && c == table[mr][mc].BRC){
                                        result /= (double)counter;
                                    }
                                }else if(table[mr][mc].type_ == FormulaType::MIN){
                                    if(SingletonTable::is_number(tmp)){
                                        double val = std::stod(tmp);
                                        if(val < min){
                                            min = val;
                                        }
                                    }
                                    if(r == table[mr][mc].BRR && c == table[mr][mc].BRC){
                                        result = min;
                                        if(result == DBL_MAX){
                                            result = 0;
                                        }
                                    }
                                }else if(table[mr][mc].type_ == FormulaType::MAX){
                                    if(SingletonTable::is_number(tmp)){
                                        double val = std::stod(tmp);
                                        if(val > max){
                                            max = val;
                                        }
                                    }
                                    if(r == table[mr][mc].BRR && c == table[mr][mc].BRC){
                                        result = max;
                                        if(result == -DBL_MAX)
                                            result = 0;
                                    }
                                }
                                //END OF THE ITERATION, CHECK THE NEXT CELL OF THE RANGE
                            }
                        }
                    }
                    // WE NEED THIS TO OVERWRITE THE VALUE
                    Cell* acell = const_cast<Cell*>(&table[mr][mc]);
                    // IS THIS FORMULA IN A CHAIN?
                    if(!has_AGF){
                        updated++;
                        //WE CAN SET IT EVALUATED EVEN IF IT HAS A BAD RANGE
                        if(
                            acell->TLR > table.size() ||
                            acell->BRR > table.size() ||
                            acell->TLC > table[0].size()-1 ||
                            acell->BRC > table[0].size()-1
                        ){
                            acell->evaluated = false;
                            acell->value_ = "#NAME?";
                        }else{
                            acell->evaluated = true;
                            acell->value_ = std::to_string(result).substr(0,std::to_string(result).find('.')+4);
                        }
                    }
                }
            }
        }
        // DEAL WITH LEFTOVER CYCLES
        if(updated == 0){
            for(unsigned mr = 0; mr < table.size(); mr++){
                for(unsigned mc = 0; mc < table[0].size(); mc++){
                    if(!table[mr][mc].evaluated){
                        Cell* acell = const_cast<Cell*>(&table[mr][mc]);
                        acell->value_ = "#CYCLE!";
                    }
                }
            }
        }
    }
}

/*! \brief Refresh:
 * Static method of the cell that reruns all formulas in the table \n
 * Parameters: const std::vector<std::vector<Cell>>& table, const std::string& valCheck \n
 */
void Cell::Refresh(const std::vector<std::vector<Cell>>& table,const std::string& valCheck){
    for(unsigned mr = 0; mr < table.size(); mr++){
        for(unsigned mc = 0; mc < table[0].size(); mc++){
            Cell* acell = const_cast<Cell*>(&table[mr][mc]);
            
            if(table[mr][mc].is_formula_ && table[mr][mc].is_formula_right && table[mr][mc].value_ != "#NAME?"){
                acell->evaluated = false;
            }else{
                acell->evaluated = true;
            }
            if( 
                acell->is_formula_ && 
                acell->is_formula_right &&
                (acell->TLR > table.size() ||
                acell->BRR > table.size()||
                acell->TLC > (table[0].size() - 1)||
                acell->BRC > (table[0].size() - 1))
            ){
                acell->value_ = "#NAME?";
            }
            if(table[mr][mc].value_ == "#NAME?"){
                if(
                    !(acell->TLR > table.size() ||
                    acell->BRR > table.size()||
                    acell->TLC > (table[0].size() - 1)||
                    acell->BRC > (table[0].size() - 1))
                ){
                    acell->evaluated = false;
                }else{
                    acell->evaluated = true;
                }
            }
        }
    }
    //SOLVE ALL FORMULAS
    /*
        THERE MUST BE AT LEAST ONE SOLVABLE FORMULA TO CONTINUE
        IT THERE IS NONE -> ALL THE LEFTOVER FORMULAS ARE IN CYCLE
    */
    unsigned updated = 1;
    while(updated != 0){
        updated = 0;
        //SET UPDATED 0 EVERY ITERATION
        for(unsigned mr = 0; mr < table.size(); mr++){
            for(unsigned mc = 0; mc < table[0].size(); mc++){
                //ITERATE WHOLE TABLE
                if(table[mr][mc].is_formula_ && table[mr][mc].is_formula_right && !table[mr][mc].evaluated){
                    // CHECK CELL IF IT CONTAINS NOT EVALUATED FORMULA
                    double result = 0;
                    unsigned counter = 0;
                    double min = DBL_MAX;
                    double max = -DBL_MAX;
                    // DOES THE RANGE OF THIS FORMULA CONTAINS OTHER FORMULAS?
                    bool has_AGF = false;
                    for(unsigned r = table[mr][mc].TLR; r <= table[mr][mc].BRR; r++){
                        for(unsigned c = table[mr][mc].TLC; c <= table[mr][mc].BRC; c++){
                            //ITERATE THE RANGE OF THE FORMULA
                            if(!table[r - 1][c].evaluated){
                                //SET TRUE IF THERE IS AT LEAST ONE FURMULA IN THE RANGE WHICH IS NOT EVALUATED
                                has_AGF = true;
                            }else{
                                // USE THE VALUE TO CALCULATE RESULT, BUT WONT USE IT IF LATER WE FIND AN NOT EVALUATED FORMULA IN THE RANGE 
                                std::string tmp = table[r-1][c].GetPrint();
                                if(table[mr][mc].type_ == FormulaType::SUM){
                                    if(SingletonTable::is_number(tmp)){
                                        result += std::stod(tmp);
                                    }
                                }else if(table[mr][mc].type_ == FormulaType::AVG){
                                    if(SingletonTable::is_number(tmp)){
                                        result += std::stod(tmp);
                                        counter++;
                                    }
                                    if(r == table[mr][mc].BRR && c == table[mr][mc].BRC){
                                        result /= (double)counter;
                                    }
                                }else if(table[mr][mc].type_ == FormulaType::MIN){
                                    if(SingletonTable::is_number(tmp)){
                                        double val = std::stod(tmp);
                                        if(val < min){
                                            min = val;
                                        }
                                    }
                                    if(r == table[mr][mc].BRR && c == table[mr][mc].BRC){
                                        result = min;
                                        if(result == DBL_MAX){
                                            result = 0;
                                        }
                                    }
                                }else if(table[mr][mc].type_ == FormulaType::MAX){
                                    if(SingletonTable::is_number(tmp)){
                                        double val = std::stod(tmp);
                                        if(val > max){
                                            max = val;
                                        }
                                    }
                                    if(r == table[mr][mc].BRR && c == table[mr][mc].BRC){
                                        result = max;
                                        if(result == -DBL_MAX)
                                            result = 0;
                                    }
                                }
                                //END OF THE ITERATION, CHECK THE NEXT CELL OF THE RANGE
                            }
                        }
                    }
                    // WE NEED THIS TO OVERWRITE THE VALUE
                    Cell* acell = const_cast<Cell*>(&table[mr][mc]);
                    // IS THIS FORMULA IN A CHAIN?
                    if(!has_AGF){
                        updated++;
                        //WE CAN SET IT EVALUATED EVEN IF IT HAS A BAD RANGE
                        if(
                            acell->TLR > table.size() ||
                            acell->BRR > table.size() ||
                            acell->TLC > table[0].size()-1 ||
                            acell->BRC > table[0].size()-1
                        ){
                            acell->evaluated = false;
                            acell->value_ = "#NAME?";
                        }else{
                            acell->evaluated = true;
                            acell->value_ = std::to_string(result).substr(0,std::to_string(result).find('.')+4);
                        }
                    }
                }
            }
        }
        // DEAL WITH LEFTOVER CYCLES
        if(updated == 0){
            for(unsigned mr = 0; mr < table.size(); mr++){
                for(unsigned mc = 0; mc < table[0].size(); mc++){
                    if(!table[mr][mc].evaluated){
                        Cell* acell = const_cast<Cell*>(&table[mr][mc]);
                        acell->value_ = "#CYCLE!";
                    }
                }
            }
        }
    }
}