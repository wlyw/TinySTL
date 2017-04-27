#include"String.h"
#include"Alloc.h"
#include<vector>
#include<iostream>
using namespace std;
using TinySTL::string;

//template <typename T>
//struct is_void {
//	static const bool value = false;
//};
//
//template <>
//struct is_void<void> {
//	static const bool value = true;
//};
//
//struct A {};
//
//A test() {
//	return A();
//}

int main() {
	/*string a("test");
	string b(a);
	string c = b;
	cout << a << std::endl;
	cout << b << std::endl;
	cout << c << std::endl;*/

	//A a = A();
	//std::cout << is_void<int>::value << std::endl;
	//std::cout << is_void<void>::value << std::endl;
	//cerr << "hello" << endl;

	/*int b[5] = { 0,1,2,3,4 };
	unsigned int i;
	vector<int, TinySTL::allocator<int> > vi(b, b + 5);
	for (i = 0; i < vi.size(); ++i)
		cout << vi[i] << ' ';*/

	return 0;
}