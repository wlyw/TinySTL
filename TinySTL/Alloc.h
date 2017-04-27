#ifndef TINYSTL_ALLOC_H_
#define TINYSTL_ALLOC_H_

//�汾һ
//#include<new>
//#include<cstddef>
//#include<cstdlib>
//#include<climits>
//#include<iostream>

//�汾��


namespace TinySTL {
	//�򵥷�װʵ�ְ汾
	////::operator new		�����ڴ�
	////placement new		��λ���죬��ָ��λ�õ��ö���Ĺ��캯��
	////new					::operator new + ���ù��캯��
	//template<class T>
	//inline T* _allocate(ptrdiff_t size, T*) {
	//	set_new_handler(0);	//ж��Ŀǰ���ڴ�����쳣��������һ�������ڴ�ʧ�ܣ���ǿ�����׳�std::bad_alloc�쳣
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

	//SGI˫�㼶�������汾
	//malloc/free(�൱��::operator new/delete)+�ڴ��

	////���STL����װΪsimple_alloc
	//template<class T,class Alloc>
	//class simple_alloc {
	//public:
	//	static T* allocate(size_t n) {
	//		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	//	}

	//};

	template<int inst>				//����������ͬ��class������class���Ͳ�ͬ��<0><1>...
	class _malloc_alloc_template {
	private:
		//process out of memory(oom)
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *, size_t);
		static void(*_malloc_alloc_oom_handler)();		//�ͷ��ڴ�

	public:
		//��ʱ�����������static��ԭ��
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

		//��Ϊû��ʹ��::operator new�����ڴ棬���Բ���ֱ��ʹ��new-handler����
		//��ơ��ڴ治�㴦����ʹ���ߵ����Σ�����SGI STl��Ĭ��_malloc_alloc_oom_handler=0
		//out of memory handler
		static void (*set_malloc_handler(void(*f)()))(){
			//��������
			//using F = int(int*, int);		//��������
			//int (*f(int))(int*, int);		//f2�β�Ϊint������һ��ָ�룬ָ�������Ϊint(int*, int)��������ָ��

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
			(*my_malloc_handler)();		//�����Զ����oom������_malloc_alloc_oom_handler����ͼ�ͷ��ڴ档
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

	//��inst=0��ָ����ģ������
	typedef _malloc_alloc_template<0> malloc_alloc;
}

#endif // ! TINYSTL_ALLOC_H_
