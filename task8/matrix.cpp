#include <iostream>
#include <cstring>
#define EPS 1E-10
using namespace std;
#include "matrix.h"

matrix::matrix(int n, int m){
    int i,j;
    str = n;
    col = m;
    matr = new double*[n];
    strings = new lines[n];
    for(i = 0; i < n; i++){
        matr[i] = new double[m];
        strings[i].set(matr[i]);
    }
    for(i = 0; i < n; i++)
        for(j = 0; j < m; j++)
            matr[i][j] = 0.0;
}

matrix::matrix(char *ch){
    int a = 0, b = 0, i, j;
    char *c = ch;
    double k;
    str = 0; col = 0;
    while(*c){
        if(*c == ',') b++;
        if(*c == '}') {
            if(!col) col = b + 1;
            if(*(c+1) == '}'){
                str = a + 1;
                break;
            }
            a++;
        }
        c++;
    }
    matr = new double*[str];
    strings = new lines[str];
    for(i = 0; i<str; i++){
        matr[i] = new double[col];
        strings[i].set(matr[i]);
    }
    char *ph = strtok(ch, "{,}");
    for(i = 0; i<str; i++){
        for(j = 0; j<col; j++){
            matr[i][j] = strtod(ph, NULL);
            ph = strtok(NULL, "{,}");
        }
    }
}

matrix::matrix(double ch){
    str = 1;
    col = 1;
    matr = new double*[1];
    strings = new lines[1];
    matr[0] = new double[1];
    strings[0].set(matr[0]);
    matr[0][0] = ch;
}

matrix::matrix(double *ch, int m){
    str = 1;
    col = m;
    matr = new double*[1];
    strings = new lines[1];
    matr[0] = new double[m];
    strings[0].set(matr[0]);
    for(int i = 0; i<m; i++) matr[0][i] = ch[i];
}

matrix::matrix(int n, double *ch){
    str = n; col = 1;
    int i,j;
    matr = new double*[n];
    strings = new lines[n];
    for(i = 0; i<n; i++){
        matr[i] = new double[1];
        strings[i].set(matr[i]);
    }
    for(i = 0; i<n; i++) matr[i][0] = ch[i];
}

matrix matrix::identity(int n){
    matrix mt(n,n);
    int i,j;
    for(i = 0; i < n; i++)
            mt.set(i,i,1);
    return mt;
}

matrix matrix::diagonal(double *ch, int n){
    matrix mt(n,n);
    int i;
    for(i = 0; i<n; i++) mt.set(i,i,ch[i]);
    return mt;
}

int matrix::rows(){
    return str;
}

int matrix::columns(){
    return col;
}

int matrix::set(int i, int j, double val){
    matr[i][j] = val;
    return 0;
}
matrix matrix::operator+(const matrix& mt){
    int i,j;
    matrix m(str, col);
    try {
        if(str != mt.str || col != mt.col){
            char s[] = {"errors: operator +"};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }

    for(i = 0; i < str; i++)
        for(j = 0; j < col; j++)
            m.matr[i][j] = matr[i][j] + mt.matr[i][j];
    return m;
}
matrix& matrix::operator+=(const matrix& mt){
    int i,j;
    try {
        if(str != mt.str || col != mt.col){
            char s[] = {"errors: operator +"};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }
    for(i = 0; i < str; i++)
        for(j = 0; j < col; j++)
            matr[i][j] += mt.matr[i][j];
    return *this;
}
double matrix::operator!(){
    double res=0;
    int i;
    for(i = 0; i<str; i++) res += matr[i][i];
    return res;
}
int matrix::gaus(){
    int i, j, z;
    double cef;
    matrix strok(1,1);
    for(i = 0; i<this->str-1; i++){
        for(j = i; j < this->str; j++){
            if(this->matr[j][i] != 0){
                strok = (*this)[j];
                break;
            }
            cout<<this->matr[i][j]<<endl;
        }
        z = j;
        if(z - i){
            matrix old = (*this)[i];
            for(j = 0; j<this->str; j++){
                this->matr[i][j] = strok.matr[0][j];
                this->matr[z][j] = old.matr[0][j];
            }
        }
        for(j = i + 1; j < this->str; j++){
            if(this->matr[j][i] == 0) continue;
            cef = this->matr[j][i]/this->matr[i][i];
            for(z = i; z < this->col; z++) this->matr[j][z] = this->matr[j][z] - cef*this->matr[i][z];
        }
    }
    return 0;
}
matrix matrix::operator~(){
    int i,j;
    double cef;
    matrix m(str,str), c(1,1);
    try{
        if(str != col){
            char s[] = {"errors: operator ~"};
            throw s;
        }
    } catch (char *s) {
        errors k(s);
        throw 1;
    }
    c = *this;
    matrix Id(1,1);
    Id = matrix::identity(str);
    c|Id;
    c.gaus();
    for(int j = c.str - 1; j > 0; j--){
        for(int z = j; z > 0; z--){
            if(c.matr[z-1][j] == 0) continue;
            cef = c.matr[z-1][j]/c.matr[j][j];
            for(int i = j; i < c.col; i++) c.matr[z-1][i] = c.matr[z-1][i] - cef*c.matr[j][i];
        }
    }
    for(i = 0; i<c.str; i++){
        cef = c.matr[i][i];
        for(j = 0; j<c.col; j++) c.matr[i][j] = c.matr[i][j] / cef;
    }
    for(i = 0; i<c.str; i++)
        for(j = c.str; j<c.col; j++)
            m.matr[i][j-c.str] = c.matr[i][j];
    return m;
}
matrix matrix::solve(matrix *n){
    int i,j,z;
    double cef;
    matrix c(1,1);
    c = *this;
    c|(*n);
    c.gaus();
    for(int j = c.str - 1; j > 0; j--){
        for(int z = j; z > 0; z--){
            if(c.matr[z-1][j] == 0) continue;
            cef = c.matr[z-1][j]/c.matr[j][j];
            for(int i = j; i < c.col; i++) c.matr[z-1][i] = c.matr[z-1][i] - cef*c.matr[j][i];
        }
    }
    for(i = 0; i<c.str; i++){
        cef = c.matr[i][i];
        for(j = 0; j<c.col; j++) c.matr[i][j] = c.matr[i][j] / cef;
    }
    matrix res(1,c.str);
    for(i = 0; i<c.str; i++) res.matr[0][i] = c.matr[i][c.col-1];
    return res;
}
const double matrix::determinant(){
    double result = 1;
    matrix m(1,1);
    m = *this;
    m.gaus();
    for(int i = 0; i < m.str; i++)
        result *= m.matr[i][i];
    return result;
}
matrix matrix::operator-(const matrix& mt){
    int i,j;
    matrix m(str, col);
    try {
        if(str != mt.str || col != mt.col){
            char s[] = {"errors: operator -"};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }
    for(i = 0; i < str; i++)
        for(j = 0; j < col; j++)
            m.matr[i][j] = matr[i][j] - mt.matr[i][j];
    return m;
}

matrix& matrix::operator-=(const matrix& mt){
    int i,j;
    try {
        if(str != mt.str || col != mt.col){
            char s[] = {"errors: operator -="};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }
    for(i = 0; i < str; i++)
        for(j = 0; j < col; j++)
            matr[i][j] -= mt.matr[i][j];
    return *this;
}
matrix matrix::operator*(const matrix& mt){
    int i,j, sum = 0, z;
    matrix m(str, mt.col);
    try {
        if(col != mt.str){
            char s[] = {"errors: operator *"};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }
    for(i = 0; i < str; i++){
        for(j = 0; j < mt.col; j++){
            for(z = 0; z < col; z++) sum += matr[i][z] * mt.matr[z][i];
            m.matr[i][j] = sum;
            sum = 0;
        }
    }
    return m;
}
matrix& matrix::operator-(){
    for(int i = 0; i < str; i++)
        for(int j = 0; j < col; j++)
            matr[i][j] *= -1;
    return *this;
}
matrix& matrix::operator*=(const matrix& mt){
    int i,j, sum = 0, z;
    matrix m(str, mt.col);
    try {
        if(col != mt.str){
            char s[] = {"errors: operator *="};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }
    for(i = 0; i < str; i++){
        for(j = 0; j < mt.col; j++){
            for(z = 0; z < col; z++) sum += matr[i][z] * mt.matr[z][i];
            m.matr[i][j] = sum;
            sum = 0;
        }
    }
    return *this = m;
}
int matrix::operator==(matrix &m){
    if(str != m.str || col != m.col) return 0;
    for(int i = 0; i < str; i++)
        for(int j = 0; j < col; j++)
            if(abs(matr[i][j] - m.matr[i][j]) > EPS) return 0;
    return 1;
}
int matrix::operator!=(matrix &m){
    if(str != m.str || col != m.col) return 1;
    for(int i = 0; i < str; i++)
        for(int j = 0; j < col; j++)
            if(abs(matr[i][j] - m.matr[i][j]) > EPS) return 1;
    return 0;
}
matrix matrix::operator[](const int i) {
    matrix m(str,col);
    try{
        if(i + 1 > str || str == 1)
            if(i + 1 > col || col == 1){
                char s[] = {"errors: operator []"};
                throw s;
            }
    } catch(char *s){
        errors k(s);
        throw 1;
    }
    if(i + 1 <= str && str != 1){
        matrix k(matr[i], col);
        m = k;
        m.strings[0].set(matr[i]);
    } else if(i + 1 <= col && col != 1){
        double b[str];
        for(int j = 0; j < str; j++){
            b[j] = matr[j][i];
        }
        matrix k(str, b);
        m = k;
        for(int j = 0; j<str; j++){
            m.strings[j].set(this->strings[j].line);
            m.strings[j].change = i;
        }
    } else if(col == 1 && str == 1 && i + 1 <= str){
        m.matr[i][i] = matr[i][i];
        m.strings[0].set(matr[0]);
    }
    return m;
}
void matrix::operator=(double n){
    try{
        if(strings[0].change == -1){
            char s[] = {"errors: operator = (double)"};
            throw s;
        }
    } catch(char *s){
        errors k(s);
        throw 1;
    }
    this->strings[0].line[strings[0].change] = n;
}
matrix& matrix::operator|(const matrix &m){
    int i, j, k;
    matrix mt(str, col+m.col);
    try {
        if(str != m.str){
            char s[] = {"errors: operator |"};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }
    for(i = 0; i<str; i++){
        for(j = 0; j<col; j++) mt.matr[i][j] = matr[i][j];
        for(j = 0; j<m.col; j++) mt.matr[i][j+col] = m.matr[i][j];
    }
    return *this = mt;
}
matrix& matrix::operator/(const matrix &m){
    int i, j, k;
    matrix mt(str + m.str, col);
    try {
        if(col != m.col){
            char s[] = {"errors: operator /"};
            throw s;
        }
    }
    catch(char* s) {
        errors k(s);
        throw 1;
    }
    for(i = 0; i<col; i++){
        for(j = 0; j<str; j++) mt.matr[j][i] = matr[j][i];
        for(j = 0; j<m.str; j++) mt.matr[j+str][i] = m.matr[j][i];
    }
    return *this = mt;
}
matrix& matrix::operator=(const matrix &m){
    int i, j;
    for(i = 0; i < str; i++) delete []matr[i];
    delete []matr;
    delete []strings;
    str = m.str; col = m.col;
    matr = new double*[str];
    strings = new lines[str];
    for(i = 0; i < str; i++){
        matr[i] = new double[col];
        strings[i].set(matr[i]);
    }
    for(i = 0; i < str; i++)
        for(j = 0; j < col; j++)
            matr[i][j] = m.matr[i][j];
    return *this;
}
matrix matrix::operator*(const int scal){
    matrix m(str, col);
    for(int i = 0; i < str; i++)
        for(int j = 0; j < col; j++)
            m.matr[i][j] = matr[i][j] * scal;
    return m;
}
matrix& matrix::operator*=(const int scal){
    for(int i = 0; i < str; i++)
        for(int j = 0; j < col; j++)
            matr[i][j] = matr[i][j] * scal;
    return *this;
}
matrix::~matrix(){
    for(int i = 0; i < str; i++) delete []matr[i];
    delete []matr;
    delete []strings;
}

ostream& operator<<(ostream &out,const matrix &m){
    int i,j;
    for(i = 0; i < m.str; i++){
        for(j = 0; j < m.col; j++){
            out.width(5);
            out.precision(3);
            out<<m.matr[i][j]<<' ';
        }
        out<<endl;
    }
    return out;
}