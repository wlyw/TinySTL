#ifndef TINYSTL_ALLOCATOR_H_
#define TINYSTL_ALLOCATOR_H_

namespace TinySTL {
	enum { _ALIGN = 8 };		//С��������ϵ��߽�
	enum { _MAX_BYTES = 128 };	//С�����������
	enum { _NFREELISTS = _MAX_BYTES / _ALIGN };	//free-lists����

	//�ݲ����Ƕ��̣߳���һ����������
	//�ޡ�template�ͱ������,�ڶ�����û����
	template<bool threads,int inst>
	class _default_alloc_template {
	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return (((bytes) + _ALIGN - 1) & ~_ALIGN - 1);
		}
	};
}

#endif // ! TINYSTL_ALLOCATOR_H_