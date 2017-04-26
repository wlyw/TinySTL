#ifndef TINYSTL_ALLOC_H_
#define TINYSTL_ALLOC_H_

#include<new>
#include<cstddef>
#include<cstdlib>
#include<climits>
#include<iostream>

namespace TinySTL {
	//::operator new	分配内存
	//placement new		定位构造，在指定位置调用对象的构造函数
	//new				::operator new + 调用构造函数
	template<class T>
	inline T* _allocate(ptrdiff_t size, T*) {
		set_new_handler(0);	//卸载目前的内存分配异常处理函数，一旦分配内存失败，则强制性抛出std::bad_alloc异常
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T)));
		if (tmp == 0) {
			cerr << "out of memory" << endl;
			exit(1);
		}
		return tmp;
	}

	template<class T>
	inline void _deallocate(T* buffer) {
		::operator delete(buffer);
	}
	
	template<class T1, class T2>
	inline void _construct(T1* p, const T2& value) {
		new(p) T1(value);	//placement new, p->T1(value), but why T2?
	}

	template<class T>
	inline void _destroy(T* ptr) {
		ptr->~T();
	}

	template<class T>
	class allocator {
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		//todo
		template<class U>
		struct rebind {
			typedef allocator<U> other;
		};

		pointer allocate(size_type n, const void* hint = 0) {
			return _allocate((difference_type)n, (pointer)0);
		}

		void deallocate(pointer p, size_type n) {
			_deallocate(p);
		}

		void construct(pointer p, const T& value) {
			_construct(p, value);
		}

		void detroy(pointer p) { _destroy(p); }

		pointer address(reference x) { return (pointer)&x; }

		const_pointer const_address(const_reference x) {
			return (const_pointer)&x;
		}

		size_type max_size() const {
			return size_type(UINT_MAX / sizeof(T));
		}
	};
}

#endif // ! TINYSTL_ALLOC_H_
