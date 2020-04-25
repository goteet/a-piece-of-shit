#pragma once
#include "cxx_base_color.h"

namespace cxx
{
	template<typename E>
	struct color_t<E, dimension::_4>
	{
		static constexpr dimension Dimension = dimension::_4;

		using k = constants<E>;
		using vector = vector_t<E, Dimension>;
		using vindex = typename vector::vindex;
		union
		{
			vector value = vector(k::zero, k::zero, k::zero, k::one);

			struct { E r, g, b, a; };
		};

		constexpr	color_t() : value(k::zero) { }

		constexpr	color_t(E _r, E _g, E _b, E _a) : value(_r, _g, _b, _a) { }

		constexpr explicit	color_t(E _v) : value(_v, _v, _v, k::one) { }

		constexpr explicit	color_t(const vector_t<E, dimension::_4>& _v) : value(_v) { }

		constexpr	color_t(const color_t<E, dimension::_3>& c3, E _a) : value(c3.value, _a) { }

		constexpr	color_t(const color_t<unsigned char, dimension::_4>& c4);

		E&			operator[](vindex index) { return value.v[index]; }

		const E&	operator[](vindex index) const { return value.v[index]; }

		// static utilities
		static color_t random()
		{
			return color_t(
				color_impl::r2f_factor<E> * rand(),
				color_impl::r2f_factor<E> * rand(),
				color_impl::r2f_factor<E> * rand(),
				k::one
			);
		}

		static constexpr color_t from_rgba(colordw rgba)
		{
			return color_t(
				color_impl::u2f_factor<E> * (rgba & 0xFF),
				color_impl::u2f_factor<E> * (rgba >> 8 & 0xFF),
				color_impl::u2f_factor<E> * (rgba >> 16 & 0xFF),
				color_impl::u2f_factor<E> * (rgba >> 24 & 0xFF)
			);
		}

		static constexpr color_t from_bgra(colordw bgra)
		{
			return color_t(
				color_impl::u2f_factor<E> * (bgra >> 16 & 0xFF),
				color_impl::u2f_factor<E> * (bgra >> 8 & 0xFF),
				color_impl::u2f_factor<E> * (bgra & 0xFF),
				color_impl::u2f_factor<E> * (bgra >> 24 & 0xFF)
			);
		}

		static constexpr color_t  black() { return color_t(k::zero, k::zero, k::zero, k::one); }

		static constexpr color_t   gray() { return color_t(k::half, k::half, k::half, k::one); }

		static constexpr color_t  white() { return color_t(k::one, k::one, k::one, k::one); }

		static constexpr color_t    red() { return color_t(k::one, k::zero, k::zero, k::one); }

		static constexpr color_t  green() { return color_t(k::zero, k::one, k::zero, k::one); }

		static constexpr color_t   blue() { return color_t(k::zero, k::zero, k::one, k::one); }

		static constexpr color_t yellow() { return color_t(k::one, k::one, k::zero, k::one); }

		static constexpr color_t   cyan() { return color_t(k::zero, k::one, k::one, k::one); }

		static constexpr color_t purple() { return color_t(k::one, k::zero, k::one, k::one); }

	private:
		template<swizzle T> using swz_unpack = swizzle_impl::swz_unpacker<T, Dimension>;
		template<swizzle T> using swz_vector = color_t<E, swz_unpack<T>::Dimension>;
	public:
		template<swizzle T>
		constexpr std::enable_if_t <swz_unpack<T>::support, swz_vector<T>> _() const { return cxx::_<T>(value); }
	};

	//functions
	template<typename E>
	constexpr void clamp(color_t<E, dimension::_4>& c)
	{
		c.value.x = clamp<E>(c.value.x, constants<E>::zero, constants<E>::one);
		c.value.y = clamp<E>(c.value.y, constants<E>::zero, constants<E>::one);
		c.value.z = clamp<E>(c.value.z, constants<E>::zero, constants<E>::one);
		c.value.w = clamp<E>(c.value.w, constants<E>::zero, constants<E>::one);
	}

	template<typename E>
	constexpr color_t<E, dimension::_4> clamped(const color_t<E, dimension::_4>& c)
	{
		color_t<E, dimension::_4> rst(c);
		clamp(rst);
		return rst;
	}


	template<typename E>
	constexpr E dot(const color_t<E, dimension::_4>& lhs, const color_t<E, dimension::_4>& rhs)
	{
		return dot(lhs.value, rhs.value);
	}

	template<typename E>
	constexpr colordw to_rgba(const color_t<E, dimension::_4>& c)
	{
		unsigned int r = static_cast<unsigned int>(c.value.x * 255);
		unsigned int g = static_cast<unsigned int>(c.value.y * 255);
		unsigned int b = static_cast<unsigned int>(c.value.z * 255);
		unsigned int a = static_cast<unsigned int>(c.value.w * 255);

		return color_impl::rgba(
			static_cast<unsigned char>(clamp(r, 0, 255)),
			static_cast<unsigned char>(clamp(g, 0, 255)),
			static_cast<unsigned char>(clamp(b, 0, 255)),
			static_cast<unsigned char>(clamp(a, 0, 255))
		);
	}

	template<typename E>
	constexpr colordw to_bgra(const color_t<E, dimension::_4>& c)
	{
		unsigned int r = static_cast<unsigned int>(c.value.x * 255);
		unsigned int g = static_cast<unsigned int>(c.value.y * 255);
		unsigned int b = static_cast<unsigned int>(c.value.z * 255);
		unsigned int a = static_cast<unsigned int>(c.value.w * 255);

		return color_impl::rgba(
			static_cast<unsigned char>(clamp(b, 0, 255)),
			static_cast<unsigned char>(clamp(g, 0, 255)),
			static_cast<unsigned char>(clamp(r, 0, 255)),
			static_cast<unsigned char>(clamp(a, 0, 255))
		);
	}

	template<>
	struct color_t<unsigned char, dimension::_4>
	{
		static constexpr dimension Dimension = dimension::_4;

		using vector = vector_t<unsigned char, Dimension>;
		using vindex = typename vector::vindex;
		union
		{
			vector value = vector(0, 0, 0, 255);
			struct { unsigned char r, g, b, a; };
			colordw value_rgba;
		};

		constexpr color_t() = default;

		constexpr color_t(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : value(_r, _g, _b, _a) { }

		constexpr explicit	color_t(unsigned char _v) : value(_v, _v, _v, 255) { }

		constexpr explicit	color_t(const vector_t<unsigned char, dimension::_4>& v4) : value(v4) { }

		template<typename E>
		constexpr color_t(const color_t<E, dimension::_4>& c4) : value(
			color_impl::to_255(c4.value.x),
			color_impl::to_255(c4.value.y),
			color_impl::to_255(c4.value.z),
			color_impl::to_255(c4.value.w))
		{ }

		unsigned char&			operator[](vindex index) { return value.v[index]; }

		const unsigned char&	operator[](vindex index) const { return value.v[index]; }

		// static utilities
		static color_t random()
		{
			return color_t(
				static_cast<unsigned char>(clamp<int>(static_cast<int>(rand() * color_impl::r2u_factor), 0, 255)),
				static_cast<unsigned char>(clamp<int>(static_cast<int>(rand() * color_impl::r2u_factor), 0, 255)),
				static_cast<unsigned char>(clamp<int>(static_cast<int>(rand() * color_impl::r2u_factor), 0, 255)),
				255
			);
		}

		static constexpr color_t from_rgba(colordw rgba)
		{
			return color_t(
				rgba & 0xFF,
				rgba >> 8 & 0xFF,
				rgba >> 16 & 0xFF,
				rgba >> 24 & 0xFF
			);
		}

		static constexpr color_t from_bgra(colordw bgra)
		{
			return color_t(
				bgra >> 16 & 0xFF,
				bgra >> 8 & 0xFF,
				bgra & 0xFF,
				bgra >> 24 & 0xFF
			);
		}

		static constexpr color_t  black() { return color_t(0, 0, 0, 255); }

		static constexpr color_t   gray() { return color_t(128, 128, 128, 255); }

		static constexpr color_t  white() { return color_t(255, 255, 255, 255); }

		static constexpr color_t    red() { return color_t(255, 0, 0, 255); }

		static constexpr color_t  green() { return color_t(0, 255, 0, 255); }

		static constexpr color_t   blue() { return color_t(0, 0, 255, 255); }

		static constexpr color_t yellow() { return color_t(255, 255, 0, 255); }

		static constexpr color_t   cyan() { return color_t(0, 255, 255, 255); }

		static constexpr color_t purple() { return color_t(255, 0, 255, 255); }

	private:
		template<swizzle T> using swz_unpack = swizzle_impl::swz_unpacker<T, Dimension>;
		template<swizzle T> using swz_vector = color_t<unsigned char, swz_unpack<T>::Dimension>;
	public:
		template<swizzle T>
		constexpr std::enable_if_t <swz_unpack<T>::support, swz_vector<T>> _() const { return cxx::_<T>(value); }
	};

	template<typename E>
	constexpr color_t<E, dimension::_4>::color_t(const color_t<unsigned char, dimension::_4>& c4) : value(
		c4.value.x * color_impl::u2f_factor<E>,
		c4.value.y * color_impl::u2f_factor<E>,
		c4.value.z * color_impl::u2f_factor<E>,
		c4.value.w * color_impl::u2f_factor<E>)
	{ }

	// functions
	constexpr void clamp(color_t<unsigned char, dimension::_4>& c)
	{
		c.value.x = clamp<unsigned char>(c.value.x, 0, 255);
		c.value.y = clamp<unsigned char>(c.value.y, 0, 255);
		c.value.z = clamp<unsigned char>(c.value.z, 0, 255);
		c.value.w = clamp<unsigned char>(c.value.w, 0, 255);
	}

	constexpr colordw to_rgba(const color_t<unsigned char, dimension::_4>& c) 
	{
		return color_impl::rgba(c.value.x, c.value.y, c.value.z, c.value.w);
	}

	constexpr colordw to_bgra(const color_t<unsigned char, dimension::_4>& c)
	{
		return color_impl::bgra(c.value.z, c.value.y, c.value.x, c.value.w);
	}
}
