#pragma once
#include "cxx_vector2.h"
#include "cxx_vector3.h"
#include "cxx_point3.h"

namespace cxx
{
	template<typename E>
	struct vector_t<E, dimension::_4>
	{
		static constexpr dimension Dimension = dimension::_4;

		using k = constants<E>;
		using vindex = vector_index<Dimension>;

		union// accessor
		{
			char	m[Dimension * sizeof(E)];
			E		v[Dimension];
			struct { E x, y, z, w; };
		};

		constexpr	vector_t() : x(k::zero), y(k::zero), z(k::zero), w(k::zero) { }

		constexpr	vector_t(E _x, E _y, E _z, E _w) : x(_x), y(_y), z(_z), w(_w) { }

		constexpr explicit	vector_t(E _v) : vector_t(_v, _v, _v, _v) { }

		constexpr	vector_t(const vector_t<E, dimension::_2>& v2, E _z, E _w) : vector_t(v2.x, v2.y, _z, _w) { }

		constexpr	vector_t(E _x, const vector_t<E, dimension::_2>& v2, E _w) : vector_t(_x, v2.x, v2.y, _w) { }

		constexpr	vector_t(E _x, E _y, const vector_t<E, dimension::_2>& v2) : vector_t(_x, _y, v2.x, v2.y) { }

		constexpr	vector_t(const vector_t<E, dimension::_3>& v3, E _w) : vector_t(v3.x, v3.y, v3.z, _w) { }

		constexpr	vector_t(E _x, const vector_t<E, dimension::_3>& v3) : vector_t(_x, v3.x, v3.y, v3.z) { }

		constexpr	vector_t(const point_t<E, dimension::_3>& p3) : vector_t(p3.x, p3.y, p3.z, k::one) { }

		E&			operator[](vindex idx) { return v[idx]; }

		const E&	operator[](vindex idx) const { return v[idx]; }

		// static utilities
		static constexpr vector_t   zero() { return vector_t(k::zero); }

		static constexpr vector_t    one() { return vector_t(k::one); }

		static constexpr vector_t unit_x() { return vector_t(k::one, k::zero, k::zero, k::one); }

		static constexpr vector_t unit_y() { return vector_t(k::zero, k::one, k::zero, k::one); }

		static constexpr vector_t unit_z() { return vector_t(k::zero, k::zero, k::one, k::one); }

		static constexpr vector_t  dir_x() { return vector_t(k::one, k::zero, k::zero, k::zero); }

		static constexpr vector_t  dir_y() { return vector_t(k::zero, k::one, k::zero, k::zero); }

		static constexpr vector_t  dir_z() { return vector_t(k::zero, k::zero, k::one, k::zero); }

	private:
		template<swizzle T> using swz_unpack = swizzle_impl::swz_unpacker<T, Dimension>;
		template<swizzle T> using swz_vector = vector_t<E, swz_unpack<T>::Dimension>;
	public:
		template<swizzle T>
		constexpr std::enable_if_t <swz_unpack<T>::support, swz_vector<T>> _() const { return cxx::_<T>(*this); }
	};


	// operations
	template<typename E>
	constexpr bool operator==(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return &lhs == &rhs
			|| (is_equal(lhs.x, rhs.x) &&
				is_equal(lhs.y, rhs.y) &&
				is_equal(lhs.z, rhs.z) &&
				is_equal(lhs.w, rhs.w)
				);
	}

	template<typename E>
	constexpr bool operator!=(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename E>
	constexpr const vector_t<E, dimension::_4>& operator+(const vector_t<E, dimension::_4>& v)
	{
		return v;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> operator-(const vector_t<E, dimension::_4>& v)
	{
		return vector_t<E, dimension::_4>(-v.x, -v.y, -v.z, -v.w);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> operator+(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return vector_t<E, dimension::_4>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> operator-(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return vector_t<E, dimension::_4>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> operator*(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return vector_t<E, dimension::_4>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> operator*(const vector_t<E, dimension::_4>& lhs, scaler_t<E> rhs)
	{
		return vector_t<E, dimension::_4>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> operator*(scaler_t<E> lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return rhs * lhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> operator/(const vector_t<E, dimension::_4>& lhs, scaler_t<E> rhs)
	{
		return lhs * (constants<E>::one / rhs);
	}

	constexpr vector_t<int, dimension::_4> operator/(const vector_t<int, dimension::_4>& lhs, int rhs)
	{
		if (rhs == 0)
		{
			return vector_t<int, dimension::_4>::zero();
		}
		else
		{
			return vector_t<int, dimension::_4>
				(
					lhs.x / rhs,
					lhs.y / rhs,
					lhs.z / rhs,
					lhs.w / rhs
					);
		}
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4>& operator+=(vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		lhs.w += rhs.w;
		return lhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4>& operator-=(vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		lhs.w -= rhs.w;
		return lhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4>& operator*=(vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		lhs.w *= rhs.w;
		return lhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4>& operator*=(vector_t<E, dimension::_4>& lhs, scaler_t<E> rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		lhs.w *= rhs;
		return lhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4>& operator/=(vector_t<E, dimension::_4>& lhs, scaler_t<E> rhs)
	{
		return lhs *= constants<E>::one / rhs;
	}

	constexpr vector_t<int, dimension::_4>& operator/=(vector_t<int, dimension::_4>& lhs, scaler_t<int> rhs)
	{
		if (rhs != 0)
		{
			lhs.x /= rhs;
			lhs.y /= rhs;
			lhs.z /= rhs;
			lhs.w /= rhs;
		}
		else
		{
			lhs = vector_t<int, dimension::_4>::zero();
		}
		return lhs;
	}

	// functions
	template<typename E>
	constexpr E dot(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	template<typename E>
	constexpr E length_sqr(const vector_t<E, dimension::_4>& _v)
	{
		return dot(_v, _v);
	}

	template<typename E>
	inline E length(const vector_t<E, dimension::_4> & _v)
	{
		return sqrt(length_sqr(_v));
	}

	template<typename E>
	constexpr void inverse(vector_t<E, dimension::_4>& _v)
	{
		E length2 = length_sqr(_v);
		if (!is_equal(length2, constants<E>::zero))
		{
			_v *= constants<E>::one / length2;
		}
		else
		{
			_v = zero();
		}
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> inversed(const vector_t<E, dimension::_4>& _v)
	{
		vector_t<E, dimension::_4> rst(_v);
		inverse(rst);
		return rst;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> abs(const vector_t<E, dimension::_4>& v)
	{
		return vector_t<E, dimension::_4>(
			v.x >= constants<E>::zero ? v.x : -v.x,
			v.y >= constants<E>::zero ? v.y : -v.y,
			v.z >= constants<E>::zero ? v.z : -v.z,
			v.w >= constants<E>::zero ? v.w : -v.w
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> min_v(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return vector_t<E, dimension::_4>(
			min2(lhs.x, rhs.x),
			min2(lhs.y, rhs.y),
			min2(lhs.z, rhs.z),
			min2(lhs.w, rhs.w)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> min_v(const vector_t<E, dimension::_4>& f0, const vector_t<E, dimension::_4>& f1, const vector_t<E, dimension::_4>& f2)
	{
		return vector_t<E, dimension::_4>(
			min3(f0.x, f1.x, f2.x),
			min3(f0.y, f1.y, f2.y),
			min3(f0.z, f1.z, f2.z),
			min3(f0.w, f1.w, f2.w)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> max_v(const vector_t<E, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return vector_t<E, dimension::_4>(
			max2(lhs.x, rhs.x),
			max2(lhs.y, rhs.y),
			max2(lhs.z, rhs.z),
			max2(lhs.w, rhs.w)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_4> max_v(const vector_t<E, dimension::_4>& f0, const vector_t<E, dimension::_4>& f1, const vector_t<E, dimension::_4>& f2)
	{
		return vector_t<E, dimension::_4>
			(
				max3(f0.x, f1.x, f2.x),
				max3(f0.y, f1.y, f2.y),
				max3(f0.z, f1.z, f2.z),
				max3(f0.w, f1.w, f2.w)
				);
	}

	template<typename E>
	constexpr std::pair<vector_t<E, dimension::_4>, vector_t<E, dimension::_4>> minmax_v(
		const vector_t<E, dimension::_4>& lhs,
		const vector_t<E, dimension::_4>& rhs)
	{
		vector_t<E, dimension::_4> min_vector = lhs;
		vector_t<E, dimension::_4> max_vector = rhs;
		vector_impl::minmax_element(min_vector.x, max_vector.x);
		vector_impl::minmax_element(min_vector.y, max_vector.y);
		vector_impl::minmax_element(min_vector.z, max_vector.z);
		vector_impl::minmax_element(min_vector.w, max_vector.w);
		return { min_vector, max_vector };
	}

	template<typename E>
	constexpr std::pair<vector_t<E, dimension::_4>, vector_t<E, dimension::_4>> minmax_v(
		const vector_t<E, dimension::_4>& f0,
		const vector_t<E, dimension::_4>& f1,
		const vector_t<E, dimension::_4>& f2)
	{
		vector_t<E, dimension::_4> min_vector;
		vector_t<E, dimension::_4> max_vector;
		vector_impl::minmax_element(f0.x, f1.x, f2.x, min_vector.x, max_vector.x);
		vector_impl::minmax_element(f0.y, f1.y, f2.y, min_vector.y, max_vector.y);
		vector_impl::minmax_element(f0.z, f1.z, f2.z, min_vector.z, max_vector.z);
		vector_impl::minmax_element(f0.w, f1.w, f2.w, min_vector.w, max_vector.w);
		return { min_vector, max_vector };
	}
}