#pragma once
#include <type_traits>
#include <stdint.h>

namespace cxx
{
	namespace common_impl
	{
		template<class, class = std::void_t<> >
		struct has_release : std::false_type { };

		template<class T>
		struct has_release<T, std::void_t<decltype(std::declval<T>()->Release())>>
			: std::true_type { };
	}

	/**
	*	Deleting a pointer and set it to nullptr
	*/
	template<class tPointer>
	void safe_delete(tPointer& pointer)
	{
		delete pointer;
		pointer = nullptr;
	}

	/**
	*	Deleting an array pointer and set it to nullptr
	*/
	template <class tPointer>
	void safe_delete_array(tPointer& pointer)
	{
		delete[] pointer;
		pointer = nullptr;
	}

	/**
	*	Releasing a interface pointer and set the pointer to nullptr
	*/
	template<class tInterface, class = std::enable_if_t<common_impl::has_release<tInterface>::value>>
	void safe_release(tInterface& pointer)
	{
		if (pointer != nullptr)
		{
			pointer->Release();
			pointer = nullptr;
		}
	}

	template<class T>
	constexpr void swap(T& l, T& r)
	{
		T t = std::move(r);
		r = std::move(l);
		l = std::move(t);
	}

	template<class T>
	constexpr T abs(T v)
	{
		return v >= T(0) ? v : -v;
	}

	constexpr uint16_t reverse_endian(uint16_t v)
	{
		return ((v & 0xFF) << 8) | ((v & 0xFF00) >> 8);
	}

	constexpr uint32_t reverse_endian(uint32_t v)
	{
		return ((v & 0xFF) << 24)
			| ((v & 0xFF00) << 8)
			| ((v & 0xFF0000) >> 8)
			| ((v & 0xFF000000) >> 24);
	}

	constexpr int16_t  reverse_endian(int16_t  v)
	{
		return reverse_endian(static_cast<uint16_t>(v));
	}

	constexpr int32_t reverse_endian(int32_t v)
	{
		return reverse_endian(static_cast<uint32_t>(v));
	}

	template<class T>
	struct helper_identity { using type = T; };
}