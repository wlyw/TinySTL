#include<new>
#include<cstddef>
#include<cstdlib>
#include<climits>
#include<iostream>
#include"String.h"
using std::cout;
using TinySTL::string;

template <typename T>
struct is_void {
	static const bool value = false;
};

template <>
struct is_void<void> {
	static const bool value = true;
};

struct A {};

A test() {
	return A();
}

int main() {
	/*string a("test");
	string b(a);
	string c = b;
	cout << a << std::endl;
	cout << b << std::endl;
	cout << c << std::endl;*/

	A a = A();
	std::cout << is_void<int>::value << std::endl;
	std::cout << is_void<void>::value << std::endl;
	//cerr << "hello" << endl;
	return 0;
}