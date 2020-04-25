#pragma once
#include "cxx_base_angle.h"

namespace cxx
{
	template<typename E>
	struct radian
	{
		E value = constants<E>::zero;

		constexpr			radian() = default;

		constexpr explicit	radian(E v) : value(v) { }

		constexpr			radian(const degree<E>& d) : value(d.value * angle_impl::d2r_factor<E>) { }
	};

	// operations	
	template<typename E>
	constexpr bool operator==(const radian<E>& l, const radian<E>& r)
	{
		return &l == &r || is_equal(l.value, r.value);
	}

	template<typename E>
	constexpr bool operator!=(const radian<E>& l, const radian<E>& r)
	{
		return !(l == r);
	}

	template<typename E>
	constexpr const radian<E>& operator+(const radian<E>& r)
	{
		return r;
	}

	template<typename E>
	constexpr radian<E> operator-(const radian<E>& r)
	{
		return radian<E>(-r.value);
	}

	template<typename E>
	constexpr bool operator<(const radian<E>& l, const radian<E>& r)
	{
		return l.value < r.value;
	}

	template<typename E>
	constexpr bool operator<=(const radian<E>& l, const radian<E>& r)
	{
		return l.value <= r.value;
	}

	template<typename E>
	constexpr bool operator>(const radian<E>& l, const radian<E>& r)
	{
		return l.value > r.value;
	}

	template<typename E>
	constexpr bool operator>=(const radian<E>& l, const radian<E>& r)
	{
		return l.value >= r.value;

	}

	template<typename E>
	constexpr radian<E> operator+(const radian<E>& l, const radian<E>& r)
	{
		return radian<E>(l.value + r.value);

	}

	template<typename E>
	constexpr radian<E> operator-(const radian<E>& l, const radian<E>& r)
	{
		return radian<E>(l.value - r.value);
	}

	template<typename E>
	constexpr radian<E> operator*(const radian<E>& l, scaler_t<E> r)
	{
		return radian<E>(l.value * r);

	}
	template<typename E>
	constexpr radian<E> operator*(scaler_t<E> l, const radian<E>& r)
	{
		return r * l;
	}

	template<typename E>
	constexpr radian<E> operator/(const radian<E>& l, scaler_t<E> r)
	{
		return l * (constants<E>::one / r);
	}


	template<typename E>
	constexpr radian<E>& operator+=(radian<E>& l, const radian<E>& r)
	{
		l.value += r.value;
		return l;
	}

	template<typename E>
	constexpr radian<E>& operator-=(radian<E>& l, const radian<E>& r)
	{
		l.value -= r.value;
		return l;
	}

	template<typename E>
	constexpr radian<E>& operator*=(radian<E>& l, scaler_t<E> r)
	{
		l.value *= r;
		return l;
	}

	template<typename E>
	constexpr radian<E>& operator/=(radian<E>& l, scaler_t<E> r)
	{
		return l *= (constants<E>::one / r);
	}

	// functions
	template<typename E>
	inline E sin(const radian<E>& r)
	{
		return ::sin(r.value);
	}

	template<typename E>
	inline E cos(const radian<E>& r)
	{
		return ::cos(r.value);
	}

	template<typename E>
	inline E tan(const radian<E>& r)
	{
		return ::tan(r.value);
	}

	template<typename E>
	inline void normalize(radian<E>& r)
	{
		if (r.value < constants<E>::zero)
		{
			E vtrunc = ceil(-r.value / constants<E>::pi2);
			r.value += vtrunc * constants<E>::pi2;
		}
		else
		{
			r.value = fmod(r.value, constants<E>::pi2);
		}
	}

	template<typename E>
	inline radian<E> normalized(const radian<E>& r)
	{
		radian<E> rst = r;
		normalize(rst);
		return rst;
	}
}

// user-defined literals
constexpr cxx::radian<float>  operator"" _radf(long double				r) { return cxx::radian<float>(static_cast<float>(r)); }
constexpr cxx::radian<float>  operator"" _radf(unsigned long long int	r) { return cxx::radian<float>(static_cast<float>(r)); }
constexpr cxx::radian<double> operator"" _radd(long double				r) { return cxx::radian<double>(static_cast<double>(r)); }
constexpr cxx::radian<double> operator"" _radd(unsigned long long int	r) { return cxx::radian<double>(static_cast<double>(r)); }