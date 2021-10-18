#include "SingletonTable.h"

SingletonTable::SingletonTable(): table_{{Cell()}},error_{""}, isOn{true}{}

SingletonTable::SingletonTable(std::string filename): error_{""}, isOn{true}{
   
    std::string line;
    std::fstream fin;
    fin.open(filename, std::ios::in);
    std::vector<std::string> rows;
    unsigned maxCols = 0;
    while (getline(fin, line))
    {
        unsigned counter = 0;
        for(auto ch : line){
            if(ch == ';'){
                counter+=1;
                if(counter > maxCols){ 
                    maxCols = counter;
                }
            }
        }
        rows.push_back(line);
    }
    for(auto r : rows){
        std::vector<Cell> actRow;
        std::string actLine = r;
        std::string actCell = "";
        for(unsigned ch = 0; ch < actLine.length(); ch++){
            if(actLine[ch] != ';' && actLine.length() != (ch+1)){
                actCell+=actLine[ch];
            }
            else{
                actRow.push_back(Cell(actCell));
                actCell = "";
            }
        }
        while(actRow.size() < maxCols+1){
            actRow.push_back(Cell());
        }
        table_.push_back(actRow);
    }
    fin.close();
}

SingletonTable::SingletonTable(std::string filename,std::string separator): error_{""}, isOn{true}{
    std::string line;
    std::fstream fin;
    fin.open(filename, std::ios::in);
    std::vector<std::string> rows;
    unsigned maxCols = 0;
    while (getline(fin, line))
    {
        unsigned counter = 0;
        for(auto ch : line){
            if(ch == separator[0]){
                counter+=1;
                if(counter > maxCols){ 
                    maxCols = counter;
                }
            }
        }
        rows.push_back(line);
    }
    for(auto r : rows){
        std::vector<Cell> actRow;
        std::string actLine = r;
        std::string actCell = "";
        for(unsigned ch = 0; ch < actLine.length(); ch++){
            if(actLine[ch] != separator[0] && actLine.length() != (ch+1)){
                actCell+=actLine[ch];
            }
            else{
                actRow.push_back(Cell(actCell));
                actCell = "";
            }
        }
        while(actRow.size() < maxCols+1){
            actRow.push_back(Cell());
        }
        table_.push_back(actRow);
    }
    fin.close();
}
bool SingletonTable::GetIsOn() const{
    return isOn;
}

void SingletonTable::SetIsOn(bool value){
    this->isOn = value;
}

void SingletonTable::PrintTable(){
    std::cout << "\n\n\n\n\n\n\n" << std::endl;
    std::vector<int> columnLengths(table_[0].size(),0);
    for(unsigned r = 0; r < table_.size();r++){
        for(unsigned c = 0; c < table_[r].size();c++){
            if(table_[r][c].GetValue().length() > columnLengths[c])
                columnLengths[c] = table_[r][c].GetValue().length();
        }
    }
    std::cout.setf(std::ios::left);
    std::cout.width(std::to_string(table_.size()).length());
    std::cout << "";
    for(unsigned i = 0; i<table_[0].size();i++){
        std::cout << "|";
        std::cout.width(columnLengths[i]);
        std::cout << (char)('A'+i);
    }
    int lengthOfRows = 0;
    for(unsigned i = 0; i < columnLengths.size(); i++){
        if(columnLengths[i] > 0){
            lengthOfRows += columnLengths[i];
        }else{
            lengthOfRows += 1;
        }
    }
    lengthOfRows += std::to_string(table_.size()).length() + table_[0].size()+1;
    std::cout << "|" << std::endl;
    std::string rowSeparator = "";
    for(unsigned i = 0; i< lengthOfRows; i++){
       rowSeparator += '-';
    }
    std::cout << rowSeparator << std::endl;
    for(unsigned r = 0; r < table_.size();r++){
        std::cout.width(std::to_string(table_.size()).length());
        std::cout << r+1;
        for(unsigned c = 0; c < table_[r].size();c++){
            std::cout << "|";
            if(columnLengths[c]>0){
                std::cout.width(columnLengths[c]);
            }else{
                std::cout.width(1);
            }
            std::cout.setf(std::ios_base::fmtflags(table_[r][c].GetAlign()),std::ios::adjustfield);
            std::cout << table_[r][c].GetValue();
        }
        std::cout<< "|" <<std::endl;
        std::cout << rowSeparator << std::endl;
    }
}

std::string SingletonTable::PrintError(){
    std::string tmp = error_;
    SetError("");
    return tmp;
}

void SingletonTable::SplitString(const std::string& s, std::vector<std::string>& v){
	std::string temp = "";
	for(int i=0;i<s.length();++i){
		if(s[i]==' '){
			v.push_back(temp);
			temp = "";
		}
		else{
			temp.push_back(s[i]);
		}
	}
	v.push_back(temp);
}

void SingletonTable::Edit(const std::string &attrs){
    if(attrs != ""){
        std::string cell;
        std::string str;
        int pos = attrs.find(' ');
        if(pos != -1){
            cell = attrs.substr(0, pos);
            str = attrs.substr(pos+1, attrs.length());
            int col = int(std::toupper(cell[0]) - 'A');
            int row;
            std::string tmp = cell.substr(1,cell.length());
            bool isNum = true;
            for(unsigned i = 0;i < tmp.length(); i++){
                if(!isdigit(tmp[i])){
                    isNum = false;
                }
            }
            if(isNum){
                row = stoi(tmp) -1;
            }else{
                row = -1;
            }
            if((table_.size() > row) && (row != -1)){
                if(table_[row].size() > col){
                    table_[row][col].SetValue(str);
                }else{
                    SetError("There are only " + std::to_string(table_[row].size()) + " columns in this table.");
                }
            }else{
                SetError("There are only " + std::to_string(table_.size()) + " rows in this table.");
            }
        }else{
            SetError("Not enough argument!");
        }
    }
}

void SingletonTable::Add(const std::string &attrs){
    int errorCode = 0;
    std::vector<std::string> parts;
    SplitString(attrs,parts);
    if(parts.size() < 2){
        errorCode = 1;
    }else if(parts.size() > 2){
        errorCode = 2;
    }else{
        for(unsigned i = 0; i< parts[0].length();i++)
            if(!isdigit(parts[0][i])) errorCode = 3;

        if(errorCode == 0)
            if((parts[1] != "cols") && (parts[1] != "rows"))
                errorCode = 4;
    }
    switch (errorCode)
    {
    case 1:
        SetError("Missing attributes!");
        break;
    case 2:
        SetError("Too much attributes!");
        break;
    case 3:
        SetError("The counter attribute must be unsigned integer.");
        break;
    case 4:
        SetError("The specifier attribute must be one of the \"cols\" or \" rows \" word.");
        break;
    case 0:
        if(parts[1] == "rows"){
            for(unsigned i = 0; i < stoi(parts[0]);i++)
                table_.push_back(std::vector<Cell>(table_[0].size(),Cell()));
        }else{
            for(unsigned i = 0; i < stoi(parts[0]);i++)
                for(unsigned j = 0; j < table_.size(); j++)
                    table_[j].push_back(Cell());
        }
        break;
    default:
        break;
    }
} 
void SingletonTable::Delete(const std::string &attrs){
    int errorCode = 0;
    int row = -1;
    int col = -1;
    std::vector<std::string> parts;
    SplitString(attrs,parts);
    if(parts.size() > 1){
        errorCode = 1;
    }else if(parts.size() < 1){
        errorCode = 2;
    }else{
        if(parts[0] == "")
            errorCode = 2;
        if(parts[0].length() == 1){
            if(isalpha(parts[0][0])){
                col = int(std::toupper(parts[0][0]) - 'A');
                if(col > table_[0].size()-1 || col < 0)
                    errorCode = 5;
            }else{
                if(isdigit(parts[0][0])){
                    row = stoi(parts[0]);
                    if(row > table_.size() || row < 1)
                        errorCode = 5;
                }else{
                    errorCode = 3;
                }
            }
        }else{
            for(unsigned i = 0; i < parts[0].length(); i++){
                if(!isdigit(parts[0][i]) || (parts[0][0] == '0'))
                    errorCode = 4;
            }
            if(errorCode == 0)
                row = stoi(parts[0]);
            if(row > table_.size()){
                errorCode = 5;
            }
        }
    }
    switch (errorCode)
    {
    case 1:
        SetError("Too much attributes!");
        break;
    case 2:
        SetError("Missing attributes!");
        break;
    case 3:
        SetError("The counter attribute must be a single letter.");
        break;
    case 4:
        SetError("The counter attribute must be unsigned integer.");
        break;
    case 5:
        SetError("The counter defines no position in the table.");
        break;
    case 0:
        if(row != -1){
            if((row == 1) && (table_.size() == 1)){
                for(unsigned i = 0; i < table_[0].size(); i++)
                    table_[0][i].SetValue("");
            }else{
                table_.erase(table_.begin() + row-1);
            }
        }else{
            if((table_[0].size() == 1) && (col == 0)){
                for(unsigned i = 0; i < table_.size(); i++)
                    table_[i][0].SetValue("");
            }else{
                for(unsigned i = 0; i < table_.size(); i++)
                    table_[i].erase(table_[i].begin() + col-1);
            }
        }
        break;
    default:
        break;
    }
}

void SingletonTable::Insert(const std::string &attrs){
    int errorCode = 0;
    std::string type = "";
    int position = -1;
    int counter = 0;
    std::vector<std::string> parts;
    SplitString(attrs,parts);
    if(parts.size() > 4){
        errorCode = 1;
    }else if(parts.size() < 4){
        errorCode = 2;
    }else{
        if(parts[0].length() == 1){
            if(isdigit(parts[0][0])){
                counter = stoi(parts[0]);
            }else{
                errorCode = 3;
            }
        }else{
            for(unsigned i = 0; i < parts[0].length();i++)
                if(!isdigit(parts[0][i]))
                    errorCode = 3;
            if(errorCode == 0){
                counter = stoi(parts[0]);
            }
        }    
        if((parts[1] != "rows") && (parts[1] != "cols"))
            errorCode = 4;
        
        if((parts[2] != "before") && (parts[2] != "after"))
            errorCode = 5;

        if(parts[3].length() == 1){
            if(isalpha(parts[3][0])){
                type = "cols";
                position = int(std::toupper(parts[3][0]) - 'A');
                if(position < 0 || position > table_[0].size()-1)
                    errorCode = 7; 
            }else{
                if(isdigit(parts[3][0]) && parts[3][0] != '0'){
                    type = "rows";
                    position = stoi(parts[3]);
                    if(position < 1 || position > table_.size())
                        errorCode = 7;
                }else{
                    errorCode = 6;
                }
            }
        }else{
            for(unsigned i = 0; i < parts[3].length(); i++){
                if(!isdigit(parts[3][i]) || (parts[3][0] == '0')){
                    errorCode = 6;
                }
            }
            if(errorCode == 0){
                type = "rows";
                position = stoi(parts[3]);
            }
        }
    }
    switch (errorCode)
    {
    case 1:
        SetError("Too much attributes!");
        break;
    case 2:
        SetError("Missing attributes!");
        break;
    case 3:
        SetError("The counter attribute must be a unsigned integer.");
        break;
    case 4:
        SetError("The specifier attribute must be one of the \"cols\" or \" rows \" word.");
        break;
    case 5:
        SetError("The second specifier attribute must be one of the \"before\" or \" rows \" after.");
        break;
    case 6:
        SetError("The position must be an integer or a single letter.");
        break;
    case 7:
        SetError("The counter defines no position in the table.");
        break;
    case 0:
        if(type == "rows"){
            if(parts[2] == "before")
                for(unsigned i = 0; i < counter; i++)
                    table_.insert(table_.begin()+(position-1),std::vector<Cell>(table_[0].size(),Cell()));
            if(parts[2] == "after"){
                if(position > table_.size()){
                    for(unsigned i = 0; i < counter; i++)
                        table_.push_back(std::vector<Cell>(table_[0].size(),Cell()));
                }else{
                    for(unsigned i = 0; i < counter; i++)
                        table_.insert(table_.begin()+(position),std::vector<Cell>(table_[0].size(),Cell()));
                }
            }
        }else{
            if(parts[2] == "before")
                for(unsigned j = 0; j < counter; j++)
                    for(unsigned i = 0; i < table_.size(); i++)
                        table_[i].insert(table_[i].begin()+(position),Cell());
            
            if(parts[2] == "after")
                for(unsigned j = 0; j < counter; j++)
                        for(unsigned i = 0; i < table_.size(); i++)
                            table_[i].insert(table_[i].begin()+(position+1),Cell());
        }
        break;
    default:
        break;
    }
}

void SingletonTable::Exit(){
    SetIsOn(false);
    std::cout << "Folyamat megszakítva..." << std::endl;
}
void SingletonTable::Align(const std::string &attrs){
    std::vector<std::string> parts;
    SplitString(attrs,parts);
    if(parts.size() == 2){
        int pos = parts[0].find(':');
        if(pos != -1){
            if(parts[1] == "left" || parts[1] == "right"){
                std::string firstCell = parts[0].substr(0,pos);
                std::string secondCell = parts[0].substr(pos+1,parts[0].length());
                if(
                    isalpha(firstCell[0]) &&
                    is_number(firstCell.substr(1,firstCell.length())) &&
                    isalpha(secondCell[0]) &&
                    is_number(secondCell.substr(1,secondCell.length()))
                ){
                    int topLeftRow;
                    int topLeftCol;
                    int bottomRightRow;
                    int bottomRightCol;
                    if(firstCell == secondCell){
                        int col = int(std::toupper(firstCell[0]) - 'A');
                        int row = stoi(firstCell.substr(1,firstCell.length()));
                        if(parts[1] == "left"){
                            table_[row-1][col].SetAlign(std::ios::left);
                        }else{
                            table_[row-1][col].SetAlign(std::ios::right);
                        }
                    }else{
                        topLeftRow = stoi(firstCell.substr(1,firstCell.length())) < stoi(secondCell.substr(1,secondCell.length()))?
                        stoi(firstCell.substr(1,firstCell.length())) : stoi(secondCell.substr(1,secondCell.length()));
                        bottomRightRow = stoi(firstCell.substr(1,firstCell.length())) > stoi(secondCell.substr(1,secondCell.length()))?
                        stoi(firstCell.substr(1,firstCell.length())) : stoi(secondCell.substr(1,secondCell.length()));
                        topLeftCol = int(std::toupper(firstCell[0]) - 'A') < int(std::toupper(secondCell[0]) - 'A')?
                        int(std::toupper(firstCell[0]) - 'A') : int(std::toupper(secondCell[0]) - 'A');
                        bottomRightCol = int(std::toupper(firstCell[0]) - 'A') > int(std::toupper(secondCell[0]) - 'A')?
                        int(std::toupper(firstCell[0]) - 'A') : int(std::toupper(secondCell[0]) - 'A');
                        for(unsigned r = topLeftRow; r <= bottomRightRow; r++){
                            for(unsigned c = topLeftCol; c <= bottomRightCol; c++){
                                if(parts[1] == "left"){
                                    table_[r-1][c].SetAlign(std::ios::left);
                                }else{
                                    table_[r-1][c].SetAlign(std::ios::right);
                                }
                            }
                        }
                    }
                }else{
                    SetError("Wrong position attributes!");
                }
            }else{
                SetError("The second parameter must be one of the\"right\" or the\"left\" values!");
            }
        }else{
            if(isalpha(parts[0][0]) && is_number(parts[0].substr(1,parts[0].length()))){
                int col = int(std::toupper(parts[0][0]) - 'A');
                int row = stoi(parts[0].substr(1,parts[0].length()));
                if(parts[1] == "left" || parts[1] == "right"){
                    std::cout<< "Row: " << row << ", Col: " << col << ", Align: " << parts[1] << std::endl;
                    if(parts[1] == "left"){
                        table_[row-1][col].SetAlign(std::ios::left);
                    }else{
                        table_[row-1][col].SetAlign(std::ios::right);
                    }
                }else{
                    SetError("The second parameter must be one of the\"right\" or the\"left\" values!");
                }
            }else{
                SetError("Wrong position attribute!");
            }
        }
    }else{
        SetError("Wrong attributes!");
    }
}
void SingletonTable::Clear(const std::string &attrs){
    std::vector<std::string> parts;
    SplitString(attrs,parts);
    if(parts.size() == 1){
        int pos = parts[0].find(':');
        if(pos != -1){
            std::string firstCell = parts[0].substr(0,pos);
            std::string secondCell = parts[0].substr(pos+1,parts[0].length());
            if(
                isalpha(firstCell[0]) &&
                is_number(firstCell.substr(1,firstCell.length())) &&
                isalpha(secondCell[0]) &&
                is_number(secondCell.substr(1,secondCell.length()))
            ){
                int topLeftRow;
                int topLeftCol;
                int bottomRightRow;
                int bottomRightCol;
                if(firstCell == secondCell){
                    int col = int(std::toupper(firstCell[0]) - 'A');
                    int row = stoi(firstCell.substr(1,firstCell.length()));
                    table_[row-1][col].SetValue("");
                }else{
                    topLeftRow = stoi(firstCell.substr(1,firstCell.length())) < stoi(secondCell.substr(1,secondCell.length()))?
                    stoi(firstCell.substr(1,firstCell.length())) : stoi(secondCell.substr(1,secondCell.length()));
                    bottomRightRow = stoi(firstCell.substr(1,firstCell.length())) > stoi(secondCell.substr(1,secondCell.length()))?
                    stoi(firstCell.substr(1,firstCell.length())) : stoi(secondCell.substr(1,secondCell.length()));
                    topLeftCol = int(std::toupper(firstCell[0]) - 'A') < int(std::toupper(secondCell[0]) - 'A')?
                    int(std::toupper(firstCell[0]) - 'A') : int(std::toupper(secondCell[0]) - 'A');
                    bottomRightCol = int(std::toupper(firstCell[0]) - 'A') > int(std::toupper(secondCell[0]) - 'A')?
                    int(std::toupper(firstCell[0]) - 'A') : int(std::toupper(secondCell[0]) - 'A');
                    for(unsigned r = topLeftRow; r <= bottomRightRow; r++){
                        for(unsigned c = topLeftCol; c <= bottomRightCol; c++){
                            table_[r-1][c].SetValue("");
                        }
                    }
                }
            }else{
                SetError("Wrong position attributes!");
            }
        }else{
            SetError("Wrong range parameter format!");
        }
    }else{
        SetError("Wrong attributes!");
    }
}
void SingletonTable::Save(const std::string &attrs){

    std::vector<std::string> parts;
    SplitString(attrs,parts);
    std::string separator = ";";
    int errorCode = 0;
    if(parts.size() == 1 || parts.size() == 3){
        if(parts[0].length() >4 && parts[0].substr(parts[0].length()-4) == ".csv"){
            if(parts.size() == 3){
                if(parts[1] == "-sep" && parts[2].length() == 1){
                    separator = parts[2];
                }else{
                    errorCode = -1;
                }
            }
        }else{
            errorCode = -1;
        }
    }else{
        errorCode = -1;
    }
    if(errorCode == 0){
        std::ofstream out(parts[0]);
        for(unsigned r = 0; r < table_.size(); r++){
            for(unsigned c = 0; c < table_[0].size(); c++){
                out << table_[r][c].GetValue();
                if(c != table_[r].size()-1)
                    out<< separator;
            }
            out << '\n';
        }
    }else{
        SetError("Error at saving!");
    }
}

void SingletonTable::SetError(const std::string &command){
    if(command == ""){
        error_ = command;
    }else{
        error_ += "\n" + command;
    }
}

void SingletonTable::ExecuteCommand(const std::string& command){
    std::string commandType;

    if(command.length() > 0)
        commandType = command.substr(0, command.find(' '));
        std::transform(commandType.begin(), commandType.end(), commandType.begin(),[](unsigned char c){ return std::tolower(c); });
       
    std::string param = command.substr(command.find(' ')+1,command.length());
    if (param.length() < 0)
        param = "";
    if(commandType == "edit"){
        Edit(param);
    }else if(commandType == "add"){
        Add(param);
    }else if(commandType == "delete"){
        Delete(param);     
    }else if(commandType == "insert"){
        Insert(param);
    }else if(commandType == "swap"){
        Swap(param);   
    }else if(command == "exit"){
        Exit();
    }else if(commandType == "save"){
        Save(param);
    }else if(commandType == "clear"){
        Clear(param);
    }else if(commandType == "align"){
        Align(param);
    }else if(commandType == "sort"){
        Sort(param);
    }else{
        SetError("There is no command with the name \"" + commandType + "\"!");
    }
}

SingletonTable *SingletonTable::SingletonTable_= nullptr;

SingletonTable *SingletonTable::GetInstance(int counter, char** arguments)
{
    if(SingletonTable_==nullptr){
        if(counter == 2){
            std::ifstream ifile;
            ifile.open(arguments[1]);
            if(ifile && std::string(arguments[1]).substr(std::string(arguments[1]).length() - 4) == ".csv") {
                SingletonTable_ = new SingletonTable(std::string(arguments[1]));
            } else {
                SingletonTable_ = new SingletonTable();
            }
        }else if(counter == 4){
            std::ifstream ifile;
            ifile.open(arguments[1]);
            if(
                ifile && 
                std::string(arguments[1]).substr(std::string(arguments[1]).length() - 4) == ".csv" &&
                std::string(arguments[2]) == "-sep" &&
                std::string(arguments[3]).length() == 1
                ) {
                SingletonTable_ = new SingletonTable(std::string(arguments[1]),std::string(arguments[3])); //implementalni
            } else {
                SingletonTable_ = new SingletonTable();
            }
        }else{
            SingletonTable_ = new SingletonTable();
        }
    }
    return SingletonTable_;
}
bool SingletonTable::is_number(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

struct Address{
    unsigned row;
    unsigned col;
};

void SingletonTable::Swap(const std::string &attrs){
    int errorCode = 0;
    Cell tmp;
    Address adrs_arr[2];
    std::vector<std::string> parts;
    SplitString(attrs,parts);
    if(parts.size() < 2){
        errorCode = 1;
    }else if(parts.size() > 2){
        errorCode = 2;
    }else{
        for(unsigned adrs_i=0; adrs_i < parts.size() ; adrs_i++ ){
            char col_chr= std::toupper(parts[adrs_i][0]);
            if ((col_chr > 'Z' ) || (col_chr < 'A') ){
                errorCode=3;
                break;
            }
            for(unsigned i = 1; i< parts[adrs_i].length();i++){
                if(!isdigit(parts[adrs_i][i])){
                    errorCode = 3;
                    break;
                }
            }   
        }
    }
        if(errorCode == 0){
            char col_chr;
            std::string row_st;
            for (unsigned int adrs_i=0; adrs_i<parts.size(); adrs_i++){
                col_chr=(char) std::toupper(parts[adrs_i][0]);
                adrs_arr[adrs_i].col=(unsigned) (col_chr - 'A') +1 ;
                if (adrs_arr[adrs_i].col >table_[adrs_arr[adrs_i].col].size()){
                    errorCode=4;
                    break;
                }
                row_st="";
                for (unsigned i=1; i<=parts[adrs_i].length()-1;i++){
                    row_st += parts[adrs_i][i];
                }
                adrs_arr[adrs_i].row=(unsigned) std::stoi(row_st);
                if ((adrs_arr[adrs_i].row > table_.size()) || adrs_arr[adrs_i].row<=0){
                    errorCode=4;
                    break;
                }
            }
        }
    switch (errorCode){
    case 1:
        SetError("Missing attributes!");
        break;
    case 2:
        SetError("Too much attributes!");
        break;
    case 3:
        SetError("Wrong address format! Use adresses like: \"A1\" !");
        break;
    case 4:
        SetError("Cell is out of range!");
        break;
    case 0:
        tmp.SetValue(table_[adrs_arr[0].row -1][adrs_arr[0].col -1].GetValue());
        table_[adrs_arr[0].row -1][adrs_arr[0].col -1].SetValue(table_[adrs_arr[1].row -1][adrs_arr[1].col -1].GetValue());
        table_[adrs_arr[1].row -1][adrs_arr[1].col -1].SetValue(tmp.GetValue());
        break;
    default:
        break;
    }
} 

void SingletonTable::Sort(const std::string &attrs){
    int errorCode = 0;
    std::vector<std::string> parts;
    SplitString(attrs,parts);
    SortType stype;
    SortBy sby;
    unsigned col_row_num;
    Cell tmp;
    std::vector<Cell> col_vec;
    //Check argument count
    if(parts.size() < 2){
        errorCode = 1;
    }else if(parts.size() > 3){
        errorCode = 2;
    }else{
        //make all parts' string uppercase
        for (unsigned i=0; i<parts.size(); i++){
            string_toupper(parts[i]);
        }
        //check first argument
        if (parts[0] != "BY") errorCode=3;
        if (errorCode==0){
            //check second argument
            if ((parts[1].length()==1) && ((parts[1][0] >= 'A') && (parts[1][0] <= 'Z') )){
                sby=sortByCol;
                col_row_num=parts[1][0] - 'A' + 1;
            }
            else if (is_number(parts[1])) {
                sby=sortByRow;
                col_row_num=std::stoi(parts[1]);
            }
            else {
                errorCode=4;
            }
        }
        //check that second argument's col/row exists
        if (errorCode==0){
            if(sby==sortByRow){
                if ((col_row_num > table_.size())  || (col_row_num<=0))
                    errorCode=5;
            }
            else {
                //only if each row vector is the same size
                if (col_row_num > table_[0].size()){
                    errorCode=5;
                }
            }
        }
        //check third argument, if missing use ASC sorting later
        if(errorCode == 0){
            //parts.szie() can only be 2 or 3 at this point
            if (parts.size()==2){
                stype=asc;
            }
            else if (parts[2]=="ASC"){
                stype=asc;
            }
            else if (parts[2]=="DESC"){
                stype=desc;
            }
            else {
                errorCode=6;
            }
        }

    switch (errorCode){
    case 1:
        SetError("Missing attributes!");
        break;
    case 2:
        SetError("Too much attributes!");
        break;
    case 3:
        SetError("First argument must be \"BY\" !");
        break;
    case 4:
        SetError("Invalid row/col format!");
        break;
    case 5:
        SetError("Given column or row does'nt exist!");
        break;
    case 6:
        SetError("Invalid sort type arguement, use \"asc\" or \"desc\" or leave blank (asc) !");
        break;
    case 0:
        if (sby==sortByRow){
            for (unsigned i=0; i<table_[col_row_num-1].size();i++){
                for (unsigned index=0; index<=table_[col_row_num-1].size()-2; index++){
                    if (compare_func(table_[col_row_num-1][index], table_[col_row_num-1][index+1],stype)){
                        tmp=table_[col_row_num-1][index];
                        table_[col_row_num-1][index]=table_[col_row_num-1][index+1];
                        table_[col_row_num-1][index+1]=tmp;
                    }
                }
            }
        }
        else{
            for (auto& row_vec : table_){
                col_vec.push_back(row_vec[col_row_num-1]);
            }
            for (unsigned i=0; i<col_vec.size();i++){
                for (unsigned index=0; index<=col_vec.size()-2; index++){
                    if (compare_func(col_vec[index], col_vec[index+1],stype)){
                        tmp=col_vec[index];
                        col_vec[index]=col_vec[index+1];
                        col_vec[index+1]=tmp;
                    }
                }
            }
            for (unsigned i=0; i<col_vec.size();i++){
                //kell assignment operator a cell classban
                table_[i][col_row_num-1]=col_vec[i];
            }
        }
        break;
    default:
        break;
    }
} 
}

bool operator<(const Cell& a,const Cell& b){
    if(SingletonTable::is_number(a.GetValue()) && SingletonTable::is_number(b.GetValue())){
        return std::stod(a.GetValue()) < std::stod(b.GetValue());
    }
    else if (!SingletonTable::is_number(a.GetValue()) && !SingletonTable::is_number(b.GetValue())){
        return a.GetValue() < b.GetValue();
    }
    else if (SingletonTable::is_number(a.GetValue())){
        return false;
    }
    else{
        return true;
    }
}
bool operator>(const Cell& a,const Cell& b){
    if(SingletonTable::is_number(a.GetValue()) && SingletonTable::is_number(b.GetValue())){
        return std::stod(a.GetValue()) > std::stod(b.GetValue());
    }
    else if (!SingletonTable::is_number(a.GetValue()) && !SingletonTable::is_number(b.GetValue())){
        return a.GetValue() > b.GetValue();
    }
    else if (SingletonTable::is_number(a.GetValue())){
        return true;
    }
    else{
        return false;
    }
}

std::string& SingletonTable::string_toupper(std::string& myst){
    //Only English alphabet a - z
    for (unsigned i=0; i<myst.length(); i++){
        for (char& ch : myst){
            if ( ( ch >= 'a' ) && ( ch <= 'z' ) ) {
                ch=std::toupper(ch);
            }
        }
    }
    return myst;
}
std::string& SingletonTable::string_toupper(std::string&& myst){
    //Only English alphabet a - z
    for (unsigned i=0; i<myst.length(); i++){
        for (char& ch : myst){
            if ( ( ch >= 'a' ) && ( ch <= 'z' ) ) {
                ch=std::toupper(ch);
            }
        }
    }
    return myst;
}

bool SingletonTable::compare_func(const Cell& a,const Cell& b, const SortType& stype){
    if (stype==asc) {
        return a>b;
    }
    return a<b;
}