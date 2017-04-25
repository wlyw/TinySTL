#ifndef TINYSTL_STRING_H_
#define TINYSTL_STRING_H_

#include<iostream>
#include<memory>

namespace TinySTL {
	class string {
	public:
		string();
		string(const string& str);
		string(string&& str);
		//string(const string& str, size_t pos, size_t len = npos);
		string(const char* s);
		string(const char* s, size_t n);
		~string();

		string& operator= (const string& str);
		string& operator= (string&& str);
		string operator+ (const string& str);

		size_t size() const { return finish - start; }
		void reserve(size_t size);

	private:
		char* start;
		char* finish;
		char* end_of_storage;

	public:
		friend std::ostream& operator<< (std::ostream& os, const string& str);
		friend std::istream& operator>> (std::istream& is, string& str);
		//friend string operator+ (const string& lhs, const string& rhs);
	};	//end of string

	string::string() 
		: start(nullptr)
		, finish(nullptr)
		, end_of_storage(nullptr) {
	}

	string::string(const string& str) {
		reserve(str.size());
		for (int i = 0; i < str.size(); ++i)
			*(start + i) = *(str.start + i);
		finish = start + str.size();
	}

	string::string(string&& str) {
		start = str.start;
		finish = str.finish;
		end_of_storage = str.end_of_storage;
		str.start = str.finish = str.end_of_storage = nullptr;
	}

	string::string(const char* s) {
		size_t len = 0;
		for (const char* itor = s; *itor; ++itor)
			++len;

		reserve(len);
		for (int i = 0; i < len; ++i, ++finish)
			*finish = *(s + i);
	}

	string::string(const char* s, size_t n) {
		reserve(n);
		for (int i = 0; i < n; ++i)
			*(start + i) = *(s + i);
		finish = start + n;
	}

	string::~string() {
		if (start != finish) {
			std::allocator<char> tmp;
			for (char* itor = start; itor != finish; ++itor)
				tmp.destroy(itor);
			tmp.deallocate(start, (size_t)(finish - start));
		}
	}

	string& string::operator= (const string& str) {
		reserve(str.size());
		for (int i = 0; i < str.size(); ++i)
			*(start + i) = *(str.start + i);
		finish = start + str.size();
		return *this;
	}

	string& string::operator= (string&& str) {
		std::allocator<char> tmp;
		for (char* itor = start; itor != finish; ++itor)
			tmp.destroy(itor);
		tmp.deallocate(start, (size_t)(finish - start));

		start = str.start;
		finish = str.finish;
		end_of_storage = str.end_of_storage;
		str.start = str.finish = str.end_of_storage = nullptr;
		return *this;
	}

	string string::operator+ (const string& str) {
		reserve((size_t)(finish - start) + str.size());
		for (int itor = 0; itor < str.size(); ++itor, ++finish)
			*finish = *(str.start + itor);
		str.~string();
		return *this;
	}

	std::ostream& operator<< (std::ostream& os, const string& str) {
		for (const char* itor = str.start; itor != str.finish; ++itor)
			os << *itor;
		return os;
	}

	void string::reserve(size_t size) {
		if (start + size + 1 <= end_of_storage) return;

		const size_t capacity = (size_t)(finish - start);

		std::allocator<char> tmp;
		char* newstart = (char*)tmp.allocate(size + 1);
		for (char* itor = start, *newitor = newstart, *end = finish; itor != end; ++itor, ++newitor)
			*newitor = *itor;
		
		if (capacity != 0) {
			for (char* itor = start; itor != finish; ++itor)
				tmp.destroy(itor);
			tmp.deallocate(start, capacity);
		}
		
		start = newstart;
		finish = newstart + capacity;
		end_of_storage = newstart + size;
	}
}

#endif // !_STRING_H_
