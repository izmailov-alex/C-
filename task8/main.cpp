#include <iostream>
#include <cstring>
#define EPS 1E-10
using namespace std;
//#include "matrix.h"
#include "matrix.cpp"
int main(){
    try{
        double v[] = {1,1,1,4.45,5.141};
        char st[] = {"{{2,3},{5,7}}"};
        char s[] = {"{{2,-3,6},{2,-2,-1},{3,-1,1}}"};
        char y[] = {"{{32},{6},{-64}}"};
        matrix ob(st);
        cout<<"ob = \n"<<ob<<endl;
        ob[0][0] = 1;
        cout<<"ob[0][0] = 1"<<endl;
        cout<<"new ob = \n"<<ob<<endl;
        cout<<"~ob = \n"<<~ob<<endl;
        cout<<"det ob = "<<ob.determinant()<<endl;
        matrix kl(s);
        matrix res(y);
        cout<<"kl = \n"<<kl<<endl<<"res = \n"<<res<<endl;
        cout<<"kl.solve = \n"<<kl.solve(&res)<<endl;
        cout<<"kl = \n"<<kl<<endl;
        matrix a(5,5);
        matrix b(2.0);
        matrix c = matrix::identity(3);
        matrix d(4,v);
        matrix k(5,5);
        matrix l(5,5);
        matrix mn(1,2);
        matrix km(1,3);
        k = matrix::identity(3);
        l = matrix::diagonal(v,5);
        cout<<"c = \n"<<c<<endl;
        cout<<"k = \n"<<k<<endl;
        cout<<"kl = \n"<<kl<<endl;
        cout<<"kl + c\n"<<kl+c<<endl;
        cout<<"c*=k \n"<<(c*=k)<<endl;
        cout<<"a = \n"<<a<<endl;
        cout<<"b = \n"<<b<<endl;
        cout<<"-c = \n"<<-c<<endl;
        cout<<"-d = \n"<<-d<<endl;
        cout<<"k = "<<k<<endl;
        cout<<"l*=2\n"<<(l*=2)<<endl;
        cout<<"tr l = "<<(!l)<<endl;
        cout<<"tr -c = "<<(!c)<<endl;
        cout<<"c/k\n"<<(c/k)<<endl;
        cout<<"c!=l --> "<<(c!=l)<<endl;
        c = l;
        cout<<"c!=l --> "<<(c!=l)<<endl;
        cout<<"a!=b --> "<<(a!=b)<<endl;
        cout<<"c = \n"<<c<<endl;
        c.gaus();
    } catch(int i) {return 1;}
    return 0;
}