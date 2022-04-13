#include "Matrix.hpp"
#include <cstdlib>

using namespace std;
using namespace zich;




bool is_number(const std::string& s)
{
    char* end = nullptr;
    strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0';
}

void Matrix::is_size_equal(const Matrix &m1, const Matrix &m2){
    if (m1.rows_size != m2.rows_size || m1.columns_size != m2.columns_size){
        throw invalid_argument("the matrixes are not the same size!");
    }   
}

int Matrix::getColumnsSize() const{
    return this->columns_size;
}

int Matrix::getRowsSize() const{
    return this->rows_size;
}

double Matrix::sum() const{
    double sum_of_elems = 0;
    for(int i = 0; i< this->columns_size * this->rows_size; i++){
        sum_of_elems += this->mat[static_cast<std::vector<int>::size_type>(i)];
    }
    return sum_of_elems;
}

double Matrix::getValueAt(int row, int column) const{
    int pos = row*this->getColumnsSize()+ column;
    return this->mat.at((unsigned long)pos);
}

double Matrix::getValueAt(int pos) const{
    return this->mat[static_cast<std::vector<int>::size_type>(pos)];
}

void Matrix::setValueAt(int row, int column, double num){
    // this->mat[static_cast<std::vector<int>::size_type>(row*int(this->getColumnsSize()) + column)] = num;
    // unsigned long r = (unsigned long) row;
    // unsigned long c = (unsigned long) column;
    // this->mat.at(r*int(this->getColumnsSize()) + c) = num;
    int pos = row*this->getColumnsSize()+ column;
    this->mat.at((unsigned long)pos)  = num;
}

void Matrix::setValueAt(int pos, double num){
    this->mat[static_cast<std::vector<int>::size_type>(pos)] = num;
}

Matrix::Matrix(const std::vector<double> &vec, int rows_size, int columns_size){

    if (vec.size() != rows_size*columns_size){
        throw invalid_argument("vector size doesn't match to rows*columns");
    }

    if (vec.empty()){
        throw invalid_argument("vector size is 0");
    }

    if (rows_size <= 0 || columns_size <= 0){
        throw invalid_argument("size is 0");
    }


    this->mat = vec;
    this->rows_size = rows_size;
    this->columns_size = columns_size;
}

std::vector<double> Matrix::getVec() const{
    std::vector<double> vec = this->mat;
    return vec;
}

//-----------------------------
// Arithmetic Operators
//-----------------------------


Matrix Matrix::operator-(){
    int k = -1;
    std::vector<double> new_mat = this->mat;
    transform(new_mat.begin(), new_mat.end(), new_mat.begin(), [k](double &c){ return c*k; });
    return Matrix(new_mat, this->getRowsSize(), this->getColumnsSize());
}

Matrix Matrix::operator-(const Matrix &m){
    is_size_equal(*this, m);

    std::vector<double> new_mat = this->mat;
    std::vector<double> reduce_mat = m.getVec();
    for (unsigned long i = 0; i < this->getRowsSize()*this->getColumnsSize(); i++)
    {
        new_mat[i] -= reduce_mat[i];
    }

    return Matrix(new_mat, this->getRowsSize(), this->getColumnsSize());
}

Matrix Matrix::operator-=(const Matrix &m){
    is_size_equal(*this, m);
    for (int i = 0; i < this->getRowsSize()*this->getColumnsSize(); i++)
    {
        this->setValueAt(i, this->getValueAt(i) - m.getValueAt(i));
    }
    return *this;
}

Matrix Matrix::operator+(){

    std::vector<double> new_mat = this->mat; //copy
    return Matrix(new_mat, this->getRowsSize(), this->getColumnsSize());
}

Matrix Matrix::operator+(const Matrix &m){
    is_size_equal(*this, m);

    std::vector<double> new_mat = this->mat;
    std::vector<double> plus_mat = m.getVec();
    for (unsigned long i = 0; i < this->getRowsSize()*this->getColumnsSize(); i++)
    {
        new_mat[i] += plus_mat[i];
    }

    return Matrix(new_mat, this->getRowsSize(), this->getColumnsSize());
}
Matrix Matrix::operator+=(const Matrix &m){
    is_size_equal(*this, m);
    for (int i = 0; i < this->getRowsSize()*this->getColumnsSize(); i++)
    {
        this->setValueAt(i, this->getValueAt(i) + m.getValueAt(i));
    }
    return *this;
}

Matrix Matrix::operator*(double d) const{
    std::vector<double> new_mat = this->mat;
    transform(new_mat.begin(), new_mat.end(), new_mat.begin(), [d](double &c){ return c*d; });
    return Matrix(new_mat, this->getRowsSize(), this->getColumnsSize());
}

Matrix Matrix::operator*(const Matrix &m) const{
    if (this->getColumnsSize() != m.getRowsSize()){
        throw invalid_argument("sizes not fit for matrix multiply");
    }

    std::vector<double> new_mat;
    int size = this->getRowsSize()*m.getColumnsSize();
    new_mat.assign((size_t) size, 0);

    for (int i = 0; i < this->getRowsSize(); i++)
    {
        for (int j = 0; j < m.getColumnsSize(); j++) {
            double sum = 0.0;
            for (int k = 0; k < this->getColumnsSize(); k++){
                sum = sum + this->getValueAt(i,k) * m.getValueAt(k,j);
            }
            new_mat.at((unsigned long)i * (unsigned long)m.columns_size + (unsigned long)j) = sum;
        }
    }

    return Matrix(new_mat, this->getRowsSize(), m.getColumnsSize());
}

Matrix Matrix::operator*=(double num){
    for (int i = 0; i < this->getRowsSize()*this->getColumnsSize(); i++)
    {
        this->setValueAt(i, this->getValueAt(i) * num);
    }

    return *this;
}

Matrix Matrix::operator*=(const Matrix &m){
    if (this->getColumnsSize() != m.getRowsSize()){
        throw invalid_argument("sizes not fit for matrix multiply");
    }

    std::vector<double> new_mat;
    int size = this->getRowsSize()*m.getColumnsSize();
    new_mat.assign((size_t) size, 0);

    for (int i = 0; i < this->getRowsSize(); i++) {
        for (int j = 0; j < m.getColumnsSize(); j++) {
            double sum = 0.0;
            for (int k = 0; k < this->getColumnsSize(); k++){
                sum = sum + this->getValueAt(i,k) * m.getValueAt(k,j);
            }
            new_mat.at((unsigned long)i * (unsigned long)m.columns_size + (unsigned long)j) = sum;
        }
    }

    this->mat = new_mat;
    this->columns_size = m.columns_size;

    return *this;
}




//-----------------------------
// Friend Comparison Operators
//-----------------------------

bool zich::operator!=(const Matrix &m1, const Matrix &m2){
    Matrix::is_size_equal(m1, m2);
    return m1.sum() != m2.sum();
}
bool zich::operator==(const Matrix &m1, const Matrix &m2){
    Matrix::is_size_equal(m1, m2);
    for (int i = 0; i < m1.mat.size(); i++)
    {
        if (m1.getValueAt(i) != m2.getValueAt(i))
        {
            return false;
        }
        
    }
    return true;
}
bool zich::operator<=(const Matrix &m1, const Matrix &m2){
    Matrix::is_size_equal(m1, m2);
    return m1.sum() <= m2.sum();
}
bool zich::operator>=(const Matrix &m1, const Matrix &m2){
    Matrix::is_size_equal(m1, m2);
    return m1.sum() >= m2.sum();
}

bool zich::operator<(const Matrix &m1, const Matrix &m2){
    Matrix::is_size_equal(m1, m2);
    return m1.sum() < m2.sum();
}
bool zich::operator>(const Matrix &m1, const Matrix &m2){
    Matrix::is_size_equal(m1, m2);
    return m1.sum() > m2.sum();
}

//-----------------------------
// Increment Decrement Operators
//-----------------------------

Matrix Matrix::operator--(){
    int d = -1;
    std::vector<double> update_mat = this->getVec();
    transform(update_mat.begin(), update_mat.end(), update_mat.begin(), [d](double &c){ return c+d; });
    this->mat = update_mat;
    return *this;
}
Matrix Matrix::operator--(int num){
    Matrix mat = Matrix(this->getVec(), this->getRowsSize(), this->getColumnsSize());
    int d = -1;
    std::vector<double> update_mat = this->getVec();
    transform(update_mat.begin(), update_mat.end(), update_mat.begin(), [d](double &c){ return c+d; });
    this->mat = update_mat;
    return mat;
}

Matrix Matrix::operator++(){
    int d = 1;
    std::vector<double> update_mat = this->getVec();
    transform(update_mat.begin(), update_mat.end(), update_mat.begin(), [d](double &c){ return c+d; });
    this->mat = update_mat;
    return *this;
}
Matrix Matrix::operator++(int num){
    Matrix mat = Matrix(this->getVec(), this->getRowsSize(), this->getColumnsSize());
    int d = 1;
    std::vector<double> update_mat = this->getVec();
    transform(update_mat.begin(), update_mat.end(), update_mat.begin(), [d](double &c){ return c+d; });
    this->mat = update_mat;
    return mat;
}

//-----------------------------
// Friend Operators
//-----------------------------

Matrix zich::operator*(double d, const Matrix &m){
    std::vector<double> new_mat = m.getVec();
    transform(new_mat.begin(), new_mat.end(), new_mat.begin(), [d](double &c){ return c*d; });
    return Matrix(new_mat, m.getRowsSize(), m.getColumnsSize());
}

    //-----------------------------
    // friend I/O Operators
    //-----------------------------
std::ostream& zich::operator<<(std::ostream& out, const Matrix &m){
    std::string result;
    for (int row = 0; row < m.getRowsSize(); row++)
    {
        result += "[";
        for (int col = 0; col < m.getColumnsSize(); col++)
        {
            result += to_string(int(m.getValueAt(row,col)));
            if (col < m.getColumnsSize()-1){
                result += " ";
            }
        }
        result += "]";
        if (row < m.getRowsSize()-1){
                result += "\n";
        }
    }
    out << result;
    return out;
}
std::istream& zich::operator>>(std::istream& in, Matrix &m){
    
    string s;
    char c = ' ';
    while((!in.eof())){
        in.get(c);
        s+= c;
    }

    if(s.find("] [")  != std::string::npos || s.find(",[")  != std::string::npos || s.find(",\n")  != std::string::npos || s.find("  ")  != std::string::npos || s.find(",,")  != std::string::npos || s.find(" ]")  != std::string::npos || s.find("[ ")  != std::string::npos || s.find("]]")  != std::string::npos || s.find("[[")  != std::string::npos){
        throw invalid_argument("input is not correct, 0");
    }

    string delimiter1 = ", "; // between lines
    string delimiter2 = " "; // between values
    string token;
    string num_str;
    double num = 0;

    // get number of columns:
    if (s.find(delimiter1) != std::string::npos) // there is more than one rows
    {
        token = s.substr(0, s.find(delimiter1));
    }
    else // there is only one row
    {
        token = s;
    }

    size_t columns = (unsigned long)std::count(token.begin(), token.end(), ' ')+(unsigned long)1;
    size_t rows = (unsigned long) std::count(s.begin(), s.end(), ']');

    // initial new vector for the matrix
    std::vector<double> new_mat;
    new_mat.assign(rows*columns, 0);
    size_t pos = 0;

    
    while(!s.empty()){
        // token is the string of one row
        // there is 2 more rows
        if (s.find(delimiter1) != std::string::npos)
        {
            token = s.substr(0, s.find(delimiter1));
            // check input
            if(token.at(0) != '[' && token.at(token.length()-1) != ']'){
                throw invalid_argument("input is not correct, 1");
            }
            token =token.substr(1, token.length()-2); // remove []
        }
        // there is only more rows
        else
        {
            token = s;
            // check input
            if(token.at(0) != '[' && token.at(token.length()-2) != ']' && token.at(token.length()-1) != '\n'){
                throw invalid_argument("input is not correct, 2");
            }
            
            token = token.substr(1, token.find_first_of(']')); // remove []\n
            token = token.substr(0, token.length()-1);
        }

        string t = token;
        int counter = 0;
        // split row to values
        while(!token.empty()){
            if(counter >= columns){
                throw invalid_argument("input is not correct");
            }
            num_str = token.substr(0, token.find(delimiter2));

            if (token.find(delimiter2) != std::string::npos)
            {
                num_str = token.substr(0, token.find(delimiter2));
            }
            else
            {
                num_str = token;
            }
            // check input
            if(!is_number(num_str)){
                throw invalid_argument("input is not correct");
            }
            num = stod(num_str);
            new_mat.at(pos) = num;

            if (token.find(delimiter2) != std::string::npos)
            {
                token.erase(0, token.find(delimiter2) + delimiter2.length());
            }
            else{
                token = "";
            }
            pos++;
            counter++;
            
        }
        // check input
        if(counter < columns || !token.empty()){
            throw invalid_argument("input is not correct");
        }

        if (s.find(delimiter1) != std::string::npos)
        {
            s.erase(0, s.find(delimiter1) + delimiter1.length());
            
        }
        else{
            s = "";
        }
    }

    m.columns_size = columns;
    m.rows_size = rows;
    m.mat = new_mat;
    return in;
}
