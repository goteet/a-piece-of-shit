#pragma once
#include <cmath>
#include <cassert>
#include <numeric>
#include "../cxx_common.h"

namespace cxx
{
	namespace common_impl
	{
		template <typename E>	struct epsilon_v { constexpr static E value = E(0); };
		template <>	struct epsilon_v<float> { constexpr static float value = 1e-6f; };
		template <>	struct epsilon_v<double> { constexpr static double value = 1e-15; };
	}

	template<typename E>
	struct constants
	{
		constexpr static E zero = E(0);

		constexpr static E one = E(1);

		constexpr static E two = E(2);

		constexpr static E half = E(0.5);

		constexpr static E quater = E(0.25);

		constexpr static E pi = E(3.14159265358979323846);

		constexpr static E pi2 = pi * two;

		constexpr static E pi_half = pi * half;

		constexpr static E epsilon = common_impl::epsilon_v<E>::value;
	};

	template<typename E>
	constexpr E PI = constants<E>::pi;

	template<typename E>
	constexpr E epsilon = constants<E>::epsilon;

	enum dimension : size_t
	{
		_2 = 2, _3 = 3, _4 = 4
	};

	/**
	*	intersection states
	*/
	enum class intersection : size_t
	{
		none,		//no collide
		same,		//two boundings are same
		inside,		//the testing object contains the bounding
		contain,	//the bounding contains the testing object
		hit			//overlapped
	};

	template<size_t kMax> struct index_t
	{
		const size_t index;

		constexpr index_t(size_t i)
			: index(i)
		{
			assert(index < kMax);
		}

		constexpr operator size_t() const { return index; }
	};

	template<typename T>
	using scaler_t = typename helper_identity<T>::type;
}