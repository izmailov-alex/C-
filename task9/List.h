template <typename T>
class List: public IterableObject{
protected:
    T *mass;
public:
    class iterator;
    class reverse_iterator;
    iterator begin();
    iterator end();
    reverse_iterator rbegin();
    reverse_iterator rend();
    iterator find(T elem);
    List(int n=0);
    List(const List &list);
    bool contains(type *buff);
    List<T>& operator=(const List<T> &list);
    bool operator==(const List<T> &list);
    void reserve(int new_length);
    void resize(int new_length);
    void push_back(const T &new_elem);
    T& at(int i);
    T& operator[](int i);
    T& front();
    T& back();
    T* data();
    friend ostream& operator<<(ostream &out, List <T> const &m);
    ~List();
};