#ifndef TINYSTL_ALLOC_H_
#define TINYSTL_ALLOC_H_

namespace TinySTL {
	
	class alloc {
	public:
		static void *allocate(size_t bytes);
	};
}

#endif // ! TINYSTL_ALLOC_H_
