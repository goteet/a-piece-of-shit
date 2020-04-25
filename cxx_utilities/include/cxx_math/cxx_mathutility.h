#pragma once
#include <numeric>
#include "../cxx_common.h"
#include "cxx_mathcommon.h"

namespace cxx
{
	// Utility Functions
	template<class E>
	inline E frac(E f)
	{
		return f - static_cast<int>(f);
	}

	template<class E>
	constexpr int floor2int(E f)
	{
		return static_cast<int>(f);
	}

	template<class E>
	constexpr int ceil2int(E f)
	{
		return static_cast<int>(std::ceil(f));
	}

	template<class T>
	constexpr bool is_equal(T lhs, T rhs)
	{
		return lhs == rhs;
	}

	template<>
	constexpr bool is_equal<float>(float lhs, float rhs)
	{
		float diff = lhs - rhs;
		return lhs == rhs ||
			(diff <  constants<float>::epsilon &&
			 diff > -constants<float>::epsilon);
	}

	template<>
	constexpr bool is_equal<double>(double lhs, double rhs)
	{
		double diff = lhs - rhs;
		return lhs == rhs ||
			(diff <  constants<double>::epsilon &&
			 diff > -constants<double>::epsilon);
	}

	template<typename T>
	constexpr T clamp(T value, scaler_t<T> minValue, scaler_t<T> maxValue)
	{
		return value < minValue
			? minValue
			: (value > maxValue
				? maxValue
				: value);
	}

	template<typename T>
	constexpr T clamp(T value)
	{
		return clamp(value, T(0), T(1));
	}

	template<class T>
	constexpr T lerp(T l, scaler_t<T> r, float f)
	{
		return l + (r - l) * f;
	}

	template<class T>
	constexpr T lerp(T l, scaler_t<T> r, double f)
	{
		return l + (r - l) * f;
	}

	template<class tBase>
	constexpr tBase pow_i(tBase base, size_t iexp)
	{
		return iexp == 0 ? tBase(1)
			: (iexp == 1 ? base
				: pow_i(base, iexp - 1) * base);
	}

	template<class T>
	constexpr T min2(T a, scaler_t<T> b)
	{
		return a < b ? a : b;
	}

	template<class T>
	constexpr T max2(T a, scaler_t<T> b)
	{
		return a > b ? a : b;
	}

	template<class T>
	constexpr T min3(T a, scaler_t<T> b, scaler_t<T> c)
	{
		return a < b
			? (a < c ? a : c)
			: (b < c ? b : c);
	}

	template<class T>
	constexpr T max3(T a, scaler_t<T> b, scaler_t<T> c)
	{
		return a > b
			? (a > c ? a : c)
			: (b > c ? b : c); 
	}
}
