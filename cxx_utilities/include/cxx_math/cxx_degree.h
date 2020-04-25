#pragma once
#include "cxx_radian.h"

namespace cxx
{
	template<typename E>
	struct degree
	{
		E value = constants<E>::zero;

		constexpr			degree() = default;

		constexpr explicit	degree(E v) : value(v) { }

		constexpr			degree(const radian<E>& r) : value(r.value * angle_impl::r2d_factor<E>) { }
	};


	// operations
	template<typename E>
	constexpr bool operator==(const degree<E>& l, const degree<E>& r)
	{
		return &l == &r || is_equal(l.value, r.value);
	}

	template<typename E>
	constexpr bool operator!=(const degree<E>& l, const degree<E>& r)
	{
		return !(l == r);
	}

	template<typename E>
	constexpr bool operator<(const degree<E>& l, const degree<E>& r)
	{
		return l.value < r.value;
	}

	template<typename E>
	constexpr bool operator<=(const degree<E>& l, const degree<E>& r)
	{
		return l.value <= r.value;
	}

	template<typename E>
	constexpr bool operator>(const degree<E>& l, const degree<E>& r)
	{
		return l.value > r.value;
	}

	template<typename E>
	constexpr bool operator>=(const degree<E>& l, const degree<E>& r)
	{
		return l.value >= r.value;
	}

	template<typename E>
	constexpr const degree<E>& operator+(const degree<E>& d)
	{
		return d;
	}

	template<typename E>
	constexpr degree<E> operator-(const degree<E>& d)
	{
		return degree<E>(-d.value);
	}

	template<typename E>
	constexpr degree<E> operator+(const degree<E>& l, const degree<E>& r)
	{
		return degree<E>(l.value + r.value);
	}

	template<typename E>
	constexpr degree<E> operator-(const degree<E>& l, const degree<E>& r)
	{
		return degree<E>(l.value - r.value);
	}

	template<typename E>
	constexpr degree<E> operator*(const degree<E>& l, scaler_t<E> r)
	{
		return degree<E>(l.value * r);
	}

	template<typename E>
	constexpr degree<E> operator*(scaler_t<E> l, const degree<E>& r)
	{
		return r * l;
	}

	template<typename E>
	constexpr degree<E> operator/(const degree<E>& l, scaler_t<E> r)
	{
		return l * (constants<E>::one / r);
	}

	template<typename E>
	constexpr degree<E>& operator+=(degree<E>& l, const degree<E>& r)
	{
		l.value += r.value;
		return l;
	}

	template<typename E>
	constexpr degree<E>& operator-=(degree<E>& l, const degree<E>& r)
	{
		l.value -= r.value;
		return l;
	}

	template<typename E>
	constexpr degree<E>& operator*=(degree<E>& l, scaler_t<E> r)
	{
		l.value *= r;
		return l;
	}

	template<typename E>
	constexpr degree<E>& operator/=(degree<E>& l, scaler_t<E> r)
	{
		return l *= (constants<E>::one / r);
	}

	template<class E>
	constexpr bool operator==(const degree<E>& l, const radian<E>& r)
	{
		return radian<E>(l) == r;
	}

	template<class E>
	constexpr bool operator==(const radian<E>& l, const degree<E>& r)
	{
		return l == radian<E>(r);
	}

	template<class E>
	constexpr bool operator!=(const degree<E>& l, const radian<E>& r)
	{
		return !(l == r);
	}

	template<class E>
	constexpr bool operator!=(const radian<E>& l, const degree<E>& r)
	{
		return !(l == r);
	}

	template<class E>
	constexpr bool operator<(const degree<E>& l, const radian<E>& r)
	{
		return radian<E>(l) < r;
	}

	template<class E>
	constexpr bool operator<(const radian<E>& l, const degree<E>& r)
	{
		return l < radian<E>(r);
	}

	template<class E>
	constexpr bool operator<=(const degree<E>& l, const radian<E>& r)
	{
		return radian<E>(l) <= r;
	}

	template<class E>
	constexpr bool operator<=(const radian<E>& l, const degree<E>& r)
	{
		return l <= radian<E>(r);
	}

	template<class E>
	constexpr bool operator>(const degree<E>& l, const radian<E>& r)
	{
		return radian<E>(l) > r;
	}

	template<class E>
	constexpr bool operator>(const radian<E>& l, const degree<E>& r)
	{
		return l > radian<E>(r);
	}

	template<class E>
	constexpr bool operator>=(const degree<E>& l, const radian<E>& r)
	{
		return radian<E>(l) >= r;
	}

	template<class E>
	constexpr bool operator>=(const radian<E>& l, const degree<E>& r)
	{
		return l >= radian<E>(r);
	}

	template<class E>
	constexpr degree<E>& operator+=(degree<E>& l, const radian<E>& r)
	{
		return l += degree<E>(r);
	}

	template<class E>
	constexpr radian<E>& operator+=(radian<E>& l, const degree<E>& r)
	{
		return l += radian<E>(r);
	}

	template<class E>
	constexpr degree<E>& operator-=(degree<E>& l, const radian<E>& r)
	{
		return l -= degree<E>(r);
	}

	template<class E>
	constexpr radian<E>& operator-=(radian<E>& l, const degree<E>& r)
	{
		return l -= radian<E>(r);
	}

	// functions
	template<typename E>
	inline E sin(const degree<E>& d)
	{
		return sin(radian<E>(d));
	}

	template<typename E>
	inline E cos(const degree<E>& d)
	{
		return cos(radian<E>(d));
	}

	template<typename E>
	inline E tan(const degree<E>& d)
	{
		return tan(radian<E>(d));
	}

	template<typename E>
	inline void normalize(degree<E>& d)
	{
		const E caps = E(360);
		if (d.value < constants<E>::zero)
		{
			d.value += ceil(-d.value / caps) * caps;
		}
		else
		{
			d.value = fmod(d.value, caps);
		}
	}

	template<typename E>
	inline degree<E> normalized(const degree<E>& d)
	{
		degree<E> rst = d;
		normalize(rst);
		return rst;
	}

}

// user-defined literals
constexpr cxx::degree<float>  operator"" _degf(long double				d) { return cxx::degree<float>(static_cast<float>(d)); }
constexpr cxx::degree<float>  operator"" _degf(unsigned long long int	d) { return cxx::degree<float>(static_cast<float>(d)); }
constexpr cxx::degree<double> operator"" _degd(long double				d) { return cxx::degree<double>(static_cast<double>(d)); }
constexpr cxx::degree<double> operator"" _degd(unsigned long long int	d) { return cxx::degree<double>(static_cast<double>(d)); }
