template<class T>
class List<T>::iterator {
protected:
public:
    T* curr;
    iterator(T* p): curr(p){}
    iterator& operator++(){
        curr++;
        return *this;
    }
    iterator operator++(int){
        iterator tmp(*this);
        curr++;
        return tmp;
    }
    iterator& operator--(){
        curr--;
        return *this;
    }
    iterator operator--(int){
        iterator tmp(*this);
        curr--;
        return tmp;
    }
    T& operator*(){
        return *curr;
    }
    bool operator==(const iterator &el){
        return *curr == *el.curr;
    }
    bool operator!=(const iterator &el){
        return *curr != *el.curr;
    }
};
template<class T>
class List<T>::reverse_iterator {
protected:
    T* curr;
public:
    reverse_iterator(T* p): curr(p){}
    reverse_iterator& operator++(){
        curr--;
        return *this;
    }
    reverse_iterator operator++(int){
        reverse_iterator tmp(*this);
        curr--;
        return tmp;
    }
    reverse_iterator& operator--(){
        curr++;
        return *this;
    }
    reverse_iterator operator--(int){
        iterator tmp(*this);
        curr--;
        return tmp;
    }
    T& operator*(){
        return *curr;
    }
    bool operator==(const reverse_iterator &el){
        return *curr == *el.curr;
    }
    bool operator!=(const reverse_iterator &el){
        return *curr != *el.curr;
    }
};
template<class T>
List<T>::List(int n){
    mass = new T[n];
    for(int i = 0; i < n; i++) mass[i] = T();
    length = n;
}
template<class T>
List<T>::List(const List &list){
    length = list.length;
    capacity = list.length;
    mass = new T[length];
    for(int i = 0; i < length; i++) mass[i] = list.mass[i];
}
template<class T>
typename List<T>::iterator List<T>::find(T elem){
    iterator ans(&mass[0]);
    for(int i = 0; i < length; i++){
        if(*(ans.curr) == elem) break;
        ans++;
    }
    ans--;
    return ans;
}
template<class T>
bool List<T>::contains(type *buff){
    int i;
    if constexpr(is_same<T, int>::value){
        for(i = 0; i < length; i++) if(mass[i] == buff->element_int) return true;
        return false;
    } else if constexpr(is_same<T, double>::value){
        for(i = 0; i < length; i++) if(mass[i] == buff->element_double) return true;
        return false;
    } else if constexpr(is_same<String, T>::value){
        for(i = 0; i < length; i++) if(mass[i] == *(buff->element_string)) return true;
        return false;
    } else{
        cout<<"ERROR"<<endl;
        throw exeption();
    }
    return true;
}
template<class T>
List<T>& List<T>::operator=(const List<T> &list){
    if(&list == this) return *this;
    length = list.length;
    if(list.length <= capacity){
        for(int i = 0; i < length; i++) mass[i] = list.mass[i];
        return *this;
    }
    capacity = length;
    delete []mass;
    mass = new T[length];
    for(int i = 0; i < length; i++) mass[i] = list.mass[i];
    return *this;
}
template<class T>
bool List<T>::operator==(const List<T> &list){
    if(length != list.length) return false;
    for(int i = 0; i < length; i++) if(mass[i] != list.mass[i]) return false;
    return true;
}
template<class T>
void List<T>::reserve(int new_length){
    if(new_length <= capacity) return;
    T *temp = new T[new_length];
    for(int i = 0; i < length; i++) temp[i] = mass[i];
    delete []mass;
    mass = temp;
    capacity = new_length;
}
template<class T>
void List<T>::resize(int new_length){
    if(new_length <= capacity) length = new_length;
    else reserve(new_length);
}
template<class T>
void List<T>::push_back(const T &new_elem){
    if(length >= capacity) reserve(capacity + add);
    mass[length] = new_elem;
    length++;
}
template<class T>
T& List<T>::at(int i){
    if(i < 0 || length <= i) {cout<<"ERROR"<<endl; throw exeption();}
    return mass[i];
}
template<class T>
T& List<T>::operator[](int i){
    return at(i);
}
template<class T>
T& List<T>::front(){
    return mass[0];
}
template<class T>
T& List<T>::back(){
    return mass[length - 1];
}
template<class T>
T* List<T>::data(){
    return mass;
}
template<class T>
ostream& operator<<(ostream &out, List <T> const &m){
    int i;
    for(i = 0; i < m.length; i++) out<<m.mass[i]<<' ';
    out<<endl;
    return out;
}
template<class T>
typename List<T>::iterator List<T>::begin(){
    return List<T>::iterator(&mass[0]);
}
template<class T>
typename List<T>::iterator List<T>::end(){
    return List<T>::iterator(&mass[length]);
}
template<class T>
typename List<T>::reverse_iterator List<T>::rbegin(){
    return List<T>::reverse_iterator(&mass[length - 1]);
}
template<class T>
typename List<T>::reverse_iterator List<T>::rend(){
    return List<T>::reverse_iterator(&mass[-1]);
}
template<class T>
List<T>::~List(){
    delete []mass;
}