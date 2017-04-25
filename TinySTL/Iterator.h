#ifndef TINYSTL_ITERATOR_H_
#define TINYSTL_ITERATOR_H_

namespace TinySTL {

	//用于函数重载时标记迭代器类型,输入、输出、前向、双向、随机访问。
	//继承的主要目的是，传递调用。(i,o)<-f<-b<-r
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//迭代器的5个常用属性
	template<class Category, class T, class Distance = ptrdiff_t,
			  class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category		iterator_category;
		typedef T				value_type;
		typedef Distance		difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};

	//榨汁大法，榨出原型
	//1、不能通过迭代器声明迭代器所指对象的类型（可以通过模板参数类型推导解决）
	//2、返回迭代器所指对象的类型（模板参数类型推导无力解决）
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};

	//榨汁机偏特化
	template<class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag iterator_category;
		typedef T						   value_type;
		typedef ptrdiff_t				   difference_type;
		typedef T*						   pointer;
		typedef T&						   reference;
	};

	//榨汁机偏偏特化
	template<class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag iterator_category;
		typedef T						   value_type;
		typedef ptrdiff_t				   difference_type;
		typedef const T*				   pointer;				//*
		typedef const T&				   reference;			//*
	};

	//自动化榨汁机
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//distance type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//value type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//distance
	template<class InputIterator>
	inline iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag) {
		iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last) {
			++first; ++n;
		}
		return n;
	}

	template<class RandomAccessIterator>
	inline iterator_traits<RandomAccessIterator>::difference_type
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}

	template<class InputIterator>
	inline iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last) {
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}
	//end of distance

	//advance
	template<class InputIterator,class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
		while (n--) ++i;
	}

	template<class BidirectionalIterator,class Distance>
	inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--) ++i;
		else
			while (n++) --i;
	}

	template<class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
		i += n;
	}

	template<class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n) {
		__advance(i, n, iterator_category(i));
	}
	//end of advance
}

#endif // !TINYSTL_ITERATOR_H_
