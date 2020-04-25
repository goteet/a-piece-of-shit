#pragma once
#include "cxx_base_vector.h"

namespace cxx
{
	/** \brief Vector2 Definitions
	*/
	template<typename E>
	struct vector_t<E, dimension::_2>
	{
		static constexpr dimension Dimension = dimension::_2;

		using k = constants<E>;
		using vindex = vector_index<Dimension>;

		union //accessors
		{
			char	m[Dimension * sizeof(E)];
			E		v[Dimension];
			struct { E x, y; };
		};

		constexpr			vector_t() : x(k::zero), y(k::zero) { }

		constexpr			vector_t(E _x, E _y) : x(_x), y(_y) { }

		constexpr explicit	vector_t(E _v) : vector_t(_v, _v) { }

		E&					operator[](vindex idx) { return v[idx]; }

		const E&			operator[](vindex idx) const { return v[idx]; }

		constexpr vector_t& operator=(const normalized_vector_t<E, dimension::_2>& rhs) { *this = rhs.value; return *this; }

		// static utilites
		static const vector_t   zero() { return vector_t(k::zero); }

		static const vector_t    one() { return vector_t(k::one); }

		static const vector_t unit_x() { return vector_t(k::one, k::zero); }

		static const vector_t unit_y() { return vector_t(k::zero, k::one); }

	private:
		template<swizzle T> using swz_unpack = swizzle_impl::swz_unpacker<T, Dimension>;
		template<swizzle T> using swz_vector = vector_t<E, swz_unpack<T>::Dimension>;
	public:
		template<swizzle T>
		constexpr std::enable_if_t <swz_unpack<T>::support, swz_vector<T>> _() const { return cxx::_<T>(*this); }
	};


	// operations
	template<typename E>
	constexpr bool operator==(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return &lhs == &rhs || (is_equal(lhs.x, rhs.x) && is_equal(lhs.y, rhs.y));
	}

	template<typename E>
	constexpr bool operator!=(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename E>
	constexpr const vector_t<E, dimension::_2>& operator+(const vector_t<E, dimension::_2>& _v)
	{
		return _v;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator-(const vector_t<E, dimension::_2>& _v)
	{
		return vector_t<E, dimension::_2>(-_v.x, -_v.y);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator+(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return vector_t<E, dimension::_2>(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator-(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return vector_t<E, dimension::_2>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return vector_t<E, dimension::_2>(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(const vector_t<E, dimension::_2>& lhs, scaler_t<E> rhs)
	{
		return vector_t<E, dimension::_2>(lhs.x * rhs, lhs.y * rhs);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(scaler_t<E> lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return rhs * lhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2>& operator+=(vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}

	template<typename E> constexpr vector_t<E, dimension::_2>& operator-=(vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}

	template<typename E> constexpr vector_t<E, dimension::_2>& operator*=(vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		return lhs;
	}

	template<typename E> constexpr vector_t<E, dimension::_2>& operator*=(vector_t<E, dimension::_2>& lhs, scaler_t<E> rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		return lhs;
	}

	// functions 
	template<typename E>
	constexpr E dot(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	template<typename E>
	constexpr E cross(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	template<typename E>
	void normalize(vector_t<E, dimension::_2>& _v)
	{
		E length2 = length_sqr(_v);
		if (!is_equal(length2, constants<E>::zero))
		{
			if (!is_equal(length2, constants<E>::zero))
			{
				_v *= constants<E>::one / sqrtf(length2);
			}
		}
		else
		{
			_v = vector_t<E, dimension::_2>::zero();
		}
	}

	template<typename E>
	constexpr void inverse(vector_t<E, dimension::_2>& _v)
	{
		E length2 = length_sqr(_v);
		if (!is_equal(length2, constants<E>::zero))
		{
			_v *= constants<E>::one / length2;
		}
		else
		{
			_v = vector_t<E, dimension::_2>::zero();
		}
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> inversed(const vector_t<E, dimension::_2>& _v)
	{
		vector_t<E, dimension::_2> rst(_v);
		inverse(rst);
		return rst;
	}

	template<typename E>
	constexpr E length_sqr(const vector_t<E, dimension::_2>& _v) { return dot(_v, _v); }

	template<typename E>
	E length(const vector_t<E, dimension::_2>& _v) { return sqrt(length_sqr(_v)); }

	template<typename E>
	constexpr vector_t<E, dimension::_2> abs(const vector_t<E, dimension::_2>& _v)
	{
		return vector_t<E, dimension::_2>(
			_v.x >= constants<E>::zero ? _v.x : -_v.x,
			_v.y >= constants<E>::zero ? _v.y : -_v.y);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> min_v(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return vector_t<E, dimension::_2>(
			min2(lhs.x, rhs.x),
			min2(lhs.y, rhs.y)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> min_v(const vector_t<E, dimension::_2>& f0, const vector_t<E, dimension::_2>& f1, const vector_t<E, dimension::_2>& f2)
	{
		return vector_t<E, dimension::_2>(
			min3(f0.x, f1.x, f2.x),
			min3(f0.y, f1.y, f2.y)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> max_v(const vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return vector_t<E, dimension::_2>(
			max2(lhs.x, rhs.x),
			max2(lhs.y, rhs.y)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> max_v(const vector_t<E, dimension::_2>& f0, const vector_t<E, dimension::_2>& f1, const vector_t<E, dimension::_2>& f2)
	{
		return vector_t<E, dimension::_2>(
			max3(f0.x, f1.x, f2.x),
			max3(f0.y, f1.y, f2.y)
			);
	}

	template<typename E>
	constexpr std::pair<vector_t<E, dimension::_2>, vector_t<E, dimension::_2>> minmax_v(
		const vector_t<E, dimension::_2>& lhs,
		const vector_t<E, dimension::_2>& rhs)
	{
		vector_t<E, dimension::_2> min_vector = lhs;
		vector_t<E, dimension::_2> max_vector = rhs;
		vector_impl::minmax_element(min_vector.x, max_vector.x);
		vector_impl::minmax_element(min_vector.y, max_vector.y);
		return { min_vector, max_vector };
	}

	template<typename E>
	constexpr std::pair<vector_t<E, dimension::_2>, vector_t<E, dimension::_2>> minmax_v(
		const vector_t<E, dimension::_2>& f0,
		const vector_t<E, dimension::_2>& f1,
		const vector_t<E, dimension::_2>& f2)
	{
		vector_t<E, dimension::_2> min_vector;
		vector_t<E, dimension::_2> max_vector;
		vector_impl::minmax_element(f0.x, f1.x, f2.x, min_vector.x, max_vector.x);
		vector_impl::minmax_element(f0.y, f1.y, f2.y, min_vector.y, max_vector.y);
		return { min_vector, max_vector };
	}


	template<typename E>
	struct normalized_vector_t<E, dimension::_2>
	{
		static constexpr dimension Dimension = dimension::_2;

		using vector = vector_t<E, Dimension>;

		union
		{
			const char		m[Dimension * sizeof(E)];
			const vector	value;
			const E			v[Dimension];

			vector_impl::const_vector_component_t<E, Dimension, 0> x;
			vector_impl::const_vector_component_t<E, Dimension, 1> y;
		};

		constexpr normalized_vector_t(E _x, E _y) : normalized_vector_t(vector_t<E, dimension::_2>(_x, _y)) { }

		constexpr normalized_vector_t(const vector_t<E, dimension::_2>& _v);

		constexpr normalized_vector_t(const normalized_vector_t& other) : value(other.value) { }

		constexpr operator const vector_t<E, dimension::_2>& () const { return value; }

		normalized_vector_t& operator=(const normalized_vector_t& other) { x.value = other.value; return *this; }

		static const normalized_vector_t unit_x() { return normalized_vector_t(constants<E>::one, constants<E>::zero, vector_impl::hint::is_normalized); }

		static const normalized_vector_t unit_y() { return normalized_vector_t(constants<E>::zero, constants<E>::one, vector_impl::hint::is_normalized); }

	private:
		constexpr normalized_vector_t(E _x, E _y, vector_impl::hint) : value(_x, _y) { }

		constexpr normalized_vector_t(const vector_t<E, dimension::_2>& _v, vector_impl::hint) : value(_v) { }

		template<class T>
		friend inline normalized_vector_t<T, dimension::_2> normalized(const vector_t<T, dimension::_2>& _v);

		template<class T>
		friend constexpr normalized_vector_t<T, dimension::_2> abs(const normalized_vector_t<T, dimension::_2>& _v);

		template<swizzle T> using swz_unpack = swizzle_impl::swz_unpacker<T, Dimension>;
		template<swizzle T> using swz_vector = vector_t<E, swz_unpack<T>::Dimension>;

	public:
		template<swizzle T>
		constexpr std::enable_if_t <swz_unpack<T>::support, swz_vector<T>> _() const { return cxx::_<T>(value); }
	};

	// operations
	template<typename E>
	constexpr bool operator==(const normalized_vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value == rhs.value;
	}

	template<typename E>
	constexpr bool operator==(const normalized_vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value == rhs;
	}

	template<typename E>
	constexpr bool operator==(const vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs == rhs.value;
	}

	template<typename E>
	constexpr bool operator!=(const normalized_vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value != rhs.value;
	}

	template<typename E>
	constexpr bool operator!=(const normalized_vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>&& rhs)
	{
		return lhs.value != rhs;
	}

	template<typename E>
	constexpr bool operator!=(const vector_t<E, dimension::_2>&& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs != rhs.value;
	}

	template<typename E>
	constexpr const normalized_vector_t<E, dimension::_2>& operator+(const normalized_vector_t<E, dimension::_2>& v)
	{
		return v;
	}

	template<typename E>
	constexpr normalized_vector_t<E, dimension::_2> operator-(const normalized_vector_t<E, dimension::_2>& v)
	{
		return normalized_vector_t<E, dimension::_2>(-v.value, vector_impl::hint::is_normalized);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator+(const normalized_vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value + rhs.value;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator+(const normalized_vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value + rhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator+(const vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return rhs + lhs.value;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator-(const normalized_vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value - rhs.value;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator-(const normalized_vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value - rhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator-(const vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return rhs - lhs.value;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(const normalized_vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value * rhs.value;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(const normalized_vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return lhs.value * rhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(const vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs * rhs.value;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(const normalized_vector_t<E, dimension::_2>& lhs, scaler_t<E> rhs)
	{
		return lhs.value * rhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator*(scaler_t<E> lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs * rhs.value;
	}

	// functions
	template<typename E>
	constexpr E dot(const normalized_vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return constants<E>::one;
	}

	template<typename E>
	constexpr E dot(const normalized_vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return dot(lhs.value, rhs);
	}

	template<typename E>
	constexpr E dot(const vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return dot(lhs, rhs.value);
	}

	template<typename E>
	constexpr E cross(const normalized_vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return constants<E>::zero;
	}

	template<typename E>
	constexpr E cross(const normalized_vector_t<E, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return cross(lhs.value, rhs);
	}

	template<typename E>
	constexpr E cross(const vector_t<E, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return cross(lhs, rhs.value);
	}

	template<typename E>
	constexpr E length_sqr(const normalized_vector_t<E, dimension::_2>& _v)
	{
		return constants<E>::one;
	}

	template<typename E>
	E length(const normalized_vector_t<E, dimension::_2>& _v)
	{
		return constants<E>::one;
	}

	template<typename E>
	void normalize(normalized_vector_t<E, dimension::_2>& _v)
	{ }

	template<typename E>
	const normalized_vector_t<E, dimension::_2>& normalized(const normalized_vector_t<E, dimension::_2>& _v)
	{
		return _v;
	}

	template<typename E>
	void prependicular(normalized_vector_t<E, dimension::_2>& _v)
	{
		x = _v.y; y = -_v.x;
	}

	template<typename E>
	normalized_vector_t<E, dimension::_2> prependiculared(normalized_vector_t<E, dimension::_2>& _v)
	{
		normalized_vector_t<E, dimension::_2> rst = _v;
		prependicular(rst);
		return rst;
	}

	template<typename E>
	constexpr normalized_vector_t<E, dimension::_2> abs(const normalized_vector_t<E, dimension::_2>& _v)
	{
		return normalized_vector_t<E, dimension::_2>(abs(_v.value), vector_impl::hint::is_normalized);
	}

	// implements
	template<typename E>
	inline normalized_vector_t<E, dimension::_2> normalized(const vector_t<E, dimension::_2>& _v)
	{
		vector_t<E, dimension::_2> nv(_v);
		normalize(nv);
		return normalized_vector_t<E, dimension::_2>(nv,
			vector_impl::hint::is_normalized);
	}

	template<typename E>
	constexpr normalized_vector_t<E, dimension::_2>::normalized_vector_t(const vector_t<E, dimension::_2>& _v)
		: normalized_vector_t<E, dimension::_2>(normalized(_v), vector_impl::hint::is_normalized)
	{	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> operator/(const vector_t<E, dimension::_2>& lhs, scaler_t<E> rhs)
	{
		return lhs * (constants<E>::one / rhs);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2>& operator/=(vector_t<E, dimension::_2>& lhs, scaler_t<E> rhs)
	{
		return lhs *= constants<E>::one / rhs;
	}

	constexpr vector_t<int, dimension::_2> operator/(const vector_t<int, dimension::_2>& lhs, int rhs)
	{
		if (rhs == 0)
		{
			return vector_t<int, dimension::_2>::zero();
		}
		else
		{
			return vector_t<int, dimension::_2>(lhs.x / rhs, lhs.y / rhs);
		}
	}

	constexpr vector_t<int, dimension::_2>& operator/=(vector_t<int, dimension::_2>& lhs, scaler_t<int> rhs)
	{
		if (rhs != 0)
		{
			lhs.x /= rhs;
			lhs.y /= rhs;
		}
		else
		{
			lhs = vector_t<int, dimension::_2>::zero();
		}
		return lhs;
	}
}