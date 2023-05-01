#include <iostream>
#include <cstring>
using namespace std;
class exeption{};
class String;
//Структура для определение типа данных элементов List
struct type{
    int element_int;
    double element_double;
    const char* element_char;
    String *element_string;
    type(int a = 0, double b = 0, String *c = 0, const char* d = 0):element_int(a),element_double(b),element_string(c),element_char(d){}
    ~type(){};
};
class IterableObject{
protected:
    const static int add = 16;
    int length;
    int capacity;
public:
    IterableObject(int len = 0){
        length = len;
        capacity = len;
    }
    virtual bool contains(type* element) = 0;
    bool empty() const { return(length == 0);}
	int size() const{ return length;}
	int volume() const{ return capacity;}
	virtual ~IterableObject(){};
};