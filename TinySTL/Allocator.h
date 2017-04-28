#ifndef TINYSTL_ALLOCATOR_H_
#define TINYSTL_ALLOCATOR_H_

namespace TinySTL {
	enum { _ALIGN = 8 };		//小型区块的上调边界
	enum { _MAX_BYTES = 128 };	//小型区块的上限
	enum { _NFREELISTS = _MAX_BYTES / _ALIGN };	//free-lists个数

	//暂不考虑多线程，第一个参数无用
	//无“template型别参数”,第二参数没用上
	template<bool threads,int inst>
	class _default_alloc_template {
	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return (((bytes) + _ALIGN - 1) & ~_ALIGN - 1);
		}
	};
}

#endif // ! TINYSTL_ALLOCATOR_H_