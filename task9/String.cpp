
String::String(int n):List(n){}
String::String(const char *str){
    length = strlen(str);
    capacity = strlen(str);
    delete []mass;
    mass = new char[length];
    for(int i = 0; i < length; i++){
        mass[i] = str[i];
    }
}
String String::operator+(const String& str){
    int tmp = length + str.length;
    int i, j;
    String ans(tmp);
    for(i = 0; i < length; i++) ans.mass[i] = mass[i];
    for(j = 0; j<str.length; j++) ans.mass[i+j] = str.mass[j];
    return ans;
}
String operator+(const char* word, const String &str){
    int tmp = str.length + strlen(word);
    int i, j;
    String ans(tmp);
    for(i = 0; i < strlen(word); i++) ans.mass[i] = word[i];
    for(j = 0; j<str.length; j++) ans.mass[i+j] = str.mass[j];
    return ans;
}
String& String::operator=(const char *str){
    delete []mass;
    length = strlen(str);
    capacity = strlen(str);
    mass = new char[length]; 
    for(int i = 0; i < length; i++) mass[i] = str[i];
    return *this;
}
void String::assign(const char * arg){
    length = strlen(arg);
    capacity = strlen(arg);
    delete[]mass;
    mass = new char[length];
    for(int i = 0; i < length; i++) mass[i] = arg[i];
}
ostream& operator<<(ostream &out, const String &arg){
    for(int i = 0; i < arg.length; i++){
        out << arg.mass[i];
    }
    return out;
}
bool String::contains(type* buff){
    int i = 0, j = 0, len_pod = strlen(buff->element_char);
    int flag = 0;
    while(i < length){
        j = 0;
        while(i < length && mass[i] != buff->element_char[j]) i++;
        if(i >= length) break;
        while(j < len_pod && mass[i] == buff->element_char[j]){ i++; j++;}
        if(j == len_pod) return true;
    }
    return false;
}