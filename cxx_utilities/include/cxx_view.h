#pragma once
#include <cassert>

namespace cxx
{
	//delete一个指针，并将指针置空
	template<class E>
	class view_t
	{
		const E* pointer;
		const size_t count;
	public:

		view_t(const E* ptr, size_t cnt)
			: pointer(ptr)
			, count(cnt) { }
			
		view_t(const view_t& other)
			: pointer(other.pointer)
			, count(other.count) { }

		size_t size() const { return count; }

		const E& operator[](size_t index) const 
		{
			assert(index < count);
			return pointer[index];
		}
	};
	
	template<class T>
	view_t<T> make_view(const char* pointer, size_t count)
	{
		return view_t<T>((const T*)pointer, count );
	}

	template<class T>
	view_t<T> make_view(const T* pointer, size_t count)
	{
		return view_t<T>(pointer, count);
	}
}