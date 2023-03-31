#include <iostream>
#include <cstring>
using namespace std;
class matrix{
    class lines{
        public:
            double *line;
            int change;
            lines(){
                line = NULL;
                change = -1;
            }
            void set(double *str){
                line = str;
            }
            ~lines(){
                line = NULL;
                change = -1;
            }
    };
    double **matr;
    int str;
    int col;
    lines *strings;
public:
    class errors{
        public:
            char s[100] = {0,};
            errors(char *str){
                strcpy(s,str);
                cerr<<s<<endl;
            }
    };
    matrix(int n, int m);
    matrix(char *ch);
    matrix(double ch);
    matrix(double *ch, int m);
    matrix(int n, double *ch);
    static matrix identity(int n);
    static matrix diagonal(double *ch, int n);
    int rows();
    int columns();
    friend ostream& operator<<(ostream& out, const matrix &m);
    int set(int i, int j, double val);
    matrix operator+(const matrix& mt);
    matrix& operator+=(const matrix& mt);
    double operator!();
    int gaus();
    matrix operator~();
    matrix solve(matrix *n);
    const double determinant();
    matrix operator-(const matrix& mt);
    matrix& operator-=(const matrix& mt);
    matrix operator*(const matrix& mt);
    matrix& operator-();
    matrix& operator*=(const matrix& mt);
    int operator==(matrix &m);
    int operator!=(matrix &m);
    matrix operator[](const int i);
    matrix& operator|(const matrix &m);
    matrix& operator/(const matrix &m);
    matrix& operator=(const matrix &m);
    void operator=(double n);
    matrix operator*(const int scal);
    matrix& operator*=(const int scal);
    ~matrix();
};