class String: public List <char> {
public:
    String(int n = 0);
    String(const char *str);
    String operator+(const String& str);
    friend String operator+(const char* word, const String &str);
    String& operator=(const char *str);
    void assign(const char * arg);
    friend ostream& operator<<(ostream &out,const String &arg);
    bool contains(type* buff);
};