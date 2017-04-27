#ifndef TINYSTL_ALLOC_H_
#define TINYSTL_ALLOC_H_

//版本一
//#include<new>
//#include<cstddef>
//#include<cstdlib>
//#include<climits>
//#include<iostream>

//版本二


namespace TinySTL {
	//简单封装实现版本
	////::operator new		分配内存
	////placement new		定位构造，在指定位置调用对象的构造函数
	////new					::operator new + 调用构造函数
	//template<class T>
	//inline T* _allocate(ptrdiff_t size, T*) {
	//	set_new_handler(0);	//卸载目前的内存分配异常处理函数，一旦分配内存失败，则强制性抛出std::bad_alloc异常
	//	T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
	//	if (tmp == 0) {
	//		cerr << "out of memory" << endl;
	//		exit(1);
	//	}
	//	return tmp;
	//}

	//template<class T>
	//inline void _deallocate(T* buffer) {
	//	::operator delete(buffer);
	//}
	//
	//template<class T1, class T2>
	//inline void _construct(T1* p, const T2& value) {
	//	new(p) T1(value);	//placement new, p->T1(value), but why T2?
	//}

	//template<class T>
	//inline void _destroy(T* ptr) {
	//	ptr->~T();
	//}

	//template<class T>
	//class allocator {
	//public:
	//	typedef T			value_type;
	//	typedef T*			pointer;
	//	typedef const T*	const_pointer;
	//	typedef T&			reference;
	//	typedef const T&	const_reference;
	//	typedef size_t		size_type;
	//	typedef ptrdiff_t	difference_type;

	//	//todo
	//	template<class U>
	//	struct rebind {
	//		typedef allocator<U> other;
	//	};

	//	pointer allocate(size_type n, const void* hint = 0) {
	//		return _allocate((difference_type)n, (pointer)0);
	//	}

	//	void deallocate(pointer p, size_type n) {
	//		_deallocate(p);
	//	}

	//	void construct(pointer p, const T& value) {
	//		_construct(p, value);
	//	}

	//	void detroy(pointer p) { _destroy(p); }

	//	pointer address(reference x) { return (pointer)&x; }

	//	const_pointer const_address(const_reference x) {
	//		return (const_pointer)&x;
	//	}

	//	//todo
	//	size_type max_size() const {
	//		return size_type(UINT_MAX / sizeof(T));
	//	}
	//};

	//SGI双层级配置器版本
	//malloc/free(相当于::operator new/delete)+内存池

	////针对STL规格封装为simple_alloc
	//template<class T,class Alloc>
	//class simple_alloc {
	//public:
	//	static T* allocate(size_t n) {
	//		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	//	}

	//};

	template<int inst>				//定义内容相同的class，但是class类型不同，<0><1>...
	class _malloc_alloc_template {
	private:
		//process out of memory(oom)
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *, size_t);
		static void(*_malloc_alloc_oom_handler)();		//释放内存

	public:
		//暂时不清楚这里用static的原因
		static void *allocate(size_t n) {
			void *result = malloc(n);
			if (0 == result) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t /* n */) {
			free(p);
		}

		static void *reallocate(void *p, size_t /* old_sz */, size_t new_sz) {
			void *result = realloc(p, new_sz);
			if (0 == result) result = oom_realloc(p, new_sz);
			return result;
		}

		//因为没有使用::operator new配置内存，所以不能直接使用new-handler机制
		//设计“内存不足处理”是使用者的责任，所以SGI STl中默认_malloc_alloc_oom_handler=0
		//out of memory handler
		static void (*set_malloc_handler(void(*f)()))(){
			//函数类型
			//using F = int(int*, int);		//函数类型
			//int (*f(int))(int*, int);		//f2形参为int，返回一个指针，指针的类型为int(int*, int)，即函数指针

			//using F = void(*)();
			//F old;
			void (*old)() = _malloc_alloc_oom_handler;
			_malloc_alloc_oom_handler = f;
			return (old);
		}
	};

	template<int inst>
	void (*_malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = 0;

	template<int inst>
	void *_malloc_alloc_template<inst>::oom_malloc(size_t n) {
		void (*my_malloc_handler)();
		void *result;

		while (1) {
			my_malloc_handler = _malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) {
				cerr << "bad alloc" << endl;
				exit(-1);
			}
			(*my_malloc_handler)();		//调用自定义的oom处理函数_malloc_alloc_oom_handler，企图释放内存。
			result = malloc(n);
			if (result) return (result);
		}
	}

	template<int inst>
	void *_malloc_alloc_template<inst>::oom_realloc(void *p, size_t n) {
		void(*my_malloc_handler)();
		void *result;

		while (1) {
			my_malloc_handler = _malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) {
				cerr << "bad alloc" << endl;
				exit(-1);
			}
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result) return (result);
		}
	}

	//令inst=0，指定类模板类型
	typedef _malloc_alloc_template<0> malloc_alloc;
}

#endif // ! TINYSTL_ALLOC_H_
