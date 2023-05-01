#include <vector>
#include <string>
void test1(){
	cout<<"========= test 1 ========="<<endl;
	vector<int> v;
	cout<<"is vector empty? "<<v.empty()<<endl;
	v.reserve(5);
	cout<<"reserve(5)"<<endl;
	cout<<"is vector empty? "<<v.empty()<<endl;
	cout<<"vector size = "<< v.size()<<endl;
	cout<<"vector capacity = "<< v.capacity()<<endl;
	for(int i = 0; i < 10; i++){
		v.push_back(i);
	}
	vector<int> new_v = v;
	vector<int> :: iterator vi = new_v.begin();
	v.resize(7);
	for(int i = 0; i < v.size(); i++){
		cout<<*(vi++);
	}
	cout<<endl<<"vector size = "<< v.size()<<endl;
	cout<<"vector capacity = "<< v.capacity()<<endl;


	cout<<"-------------------------"<<endl;

	List<int> l;
	cout<<"is List empty? "<<l.empty()<<endl;
	l.reserve(5);
	cout<<"reserve(5)"<<endl;
	cout<<"is List empty? "<<l.empty()<<endl;
	cout<<"List size = "<< l.size()<<endl;
	cout<<"List volume = "<< l.volume()<<endl;
	for(int i = 0; i < 10; i++){
		l.push_back(i);
	}
	List<int> new_l = l;
	List<int>::iterator li = new_l.begin();
	l.resize(7);
	for(int i = 0; i < l.size(); i++){
		cout<<*(li++);
	}
	cout<<endl<<"List size = "<< l.size()<<endl;
	cout<<"List volume = "<< l.volume()<<endl;
}

void test2(){
	cout<<"========= test 2 ========="<<endl;
	vector<int> v;
	for(int i = 0; i < 10; i++){
		v.push_back(i);
	}
	cout<<"vector.at(5) = "<<v.at(5)<<endl;
	cout<<"vector[4] = "<<v[4]<<endl;
	cout<<"vector.front() = "<<v.front()<<endl;
	cout<<"vector.back() = "<<v.back()<<endl;
	cout<<"vector.data() = ";
	int* tempv = v.data(); 
	for(int i = 0; i < v.size(); i++){
		cout<<tempv[i];
	} cout<<endl;

	cout<<"-------------------------"<<endl;

	List<int> l;
	for(int i = 0; i < 10; i++){
		l.push_back(i);
	}
	cout<<"List.at(5) = "<<l.at(5)<<endl;
	cout<<"List[4] = "<<l[4]<<endl;
	cout<<"List.front() = "<<l.front()<<endl;
	cout<<"List.back() = "<<l.back()<<endl;
	cout<<"List.data() = ";
	int* templ = l.data(); 
	for(int i = 0; i < l.size(); i++){
		cout<<templ[i];
	} cout<<endl;
}

void test3(){
	cout<<"========= test 3 ========="<<endl;
	string s1;
	for(int i = 0; i < 10; i++){
		s1.push_back(i+'0');
	}
	cout<<"s1.at(5) = "<<s1.at(5)<<endl;
	cout<<"s1[3] = "<<s1[3]<<endl;
	cout<<"s1.size() = "<<s1.size()<<endl;
	cout<<"s1.volume() = "<<s1.capacity()<<endl;
	cout<<"-------------------------------"<<endl;

	String s2;
	for(int i = 0; i < 10; i++){
		s2.push_back(i+'0');
	}
	cout<<"s2.at(5) = "<<s2.at(5)<<endl;
	cout<<"s2[3] = "<<s2[3]<<endl;
	cout<<"s2.size() = "<<s2.size()<<endl;
	cout<<"s2.volume() = "<<s2.volume()<<endl;
}

void test4(){
	cout<<"========= test 4 ========="<<endl;
	List<char> ll;
	for(int i = 0; i < 10; i++){
		ll.push_back(i+'0');
	}
	List<char>::reverse_iterator lli = ll.rbegin();
	cout<<"List in reverse:"<<endl;
	for(int i = 0; i < 10; i++){
		cout<<*(lli++);
	}
	cout<<endl<<"---------------------------------"<<endl;
	String ss;
	for(int i = 0; i < 10; i++){
		ss.push_back(i+'0');
	}
	String::reverse_iterator ssi = ss.rbegin();
	cout<<"string in reverse:"<<endl;
	for(int i = 0; i < 10; i++){
		cout<<*(ssi++);
	}
	cout<<endl;
}

void test5(){
	cout<<"========= test 5 ========="<<endl;
	String s1 = "123";
	String s2;
	s2.assign("abc");
	String s3 = s1 + s2;
	cout<<"String concatenation:"<<endl;
	cout<<"123 + abc = "<<s1+s2<<endl;
	cout<<"abc + eee = "<<s2+"eee"<<endl;
	cout<<"eee + 123 = "<<"eee"+s1<<endl;
}

void test6(){
	cout<<"========= test 6 ========="<<endl;
	IterableObject * obj = new String("123456789");
	type * strk = new type;
	strk -> element_char = "567";
	type * str2 = new type;
	str2 -> element_char = "65";
	cout<<"does string 123456789:"<<endl;
	cout<<"contains 567? : "<<(obj->contains(strk)? "yes" : "no")<<endl;
	cout<<"contains 65? : "<<(obj->contains(str2)? "yes" : "no")<<endl;
	delete obj;
	delete strk;
	delete str2;
	cout<<"--------------------------"<<endl;
	List<int> * tmp = new List<int>;
	for(int i = 0; i < 10; ++i){
		tmp -> push_back(i);
	}
	IterableObject * a = tmp;
	type * str3 = new type;
	str3 -> element_int = 12;
	cout<<"does List<int> = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9"<<endl;
	cout<<"contains 12? : "<<(a->contains(str3)? "yes" : "no")<<endl;
	str3 -> element_int = 2;
	cout<<"contains 2? : "<<(a->contains(str3)? "yes" : "no")<<endl;
	cout<<"--------------------------"<<endl;
	List<double> * tmp2 = new List<double>;
	for(int i = 0; i < 10; ++i){
		tmp2 -> push_back(((double)i)/10);
	}
	IterableObject * a2 = tmp2;
	str3 -> element_double = 0.1;
	cout<<"does List<double> = 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9"<<endl;
	cout<<"contains 0.9? : "<<(a2->contains(str3)? "yes" : "no")<<endl;
	str3 -> element_double = 1.0;
	cout<<"contains 1.0? : "<<(a2->contains(str3)? "yes" : "no")<<endl;

	cout<<"--------------------------"<<endl;
	List<String> * tmp3 = new List<String>;
	String s1;
	s1 = "1";
	tmp3 -> push_back(s1);
	s1 = "12";
	tmp3 -> push_back(s1);
	s1 = "123";
	tmp3 -> push_back(s1);
	s1 = "1234";
	tmp3 -> push_back(s1);
	s1 = "12345";
	tmp3 -> push_back(s1);
	IterableObject * a3 = tmp3;

	String s2;
	s2 = "35";
	str3 -> element_string = &s2;
	cout<<"does List<string> = 1, 12, 123, 1234, 12345"<<endl;
	cout<<"contains 35? : "<<(a3->contains(str3)? "yes" : "no")<<endl;
	s2 = "12";
	str3 -> element_string = &s2;
	cout<<"contains 12? : "<<(a3->contains(str3)? "yes" : "no")<<endl;
	delete str3;
	delete tmp;
	delete tmp2;
	delete tmp3;
}
void test7(){
	cout<<"========= test 7 ========="<<endl;
	List<int> l;
	for(int i = 0; i < 10; i++){
		l.push_back(i);
	}
	int* templ = l.data(); 
	for(int i = 0; i < l.size(); i++){
		cout<<templ[i];
	} cout<<endl;
	cout<<"Find: "<<*(l.find(10))<<endl;

}