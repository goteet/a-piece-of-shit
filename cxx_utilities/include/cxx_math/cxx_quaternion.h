#pragma once
#include "cxx_vector.h"
#include "cxx_angle.h"
#include "cxx_euler.h"
#include "cxx_matrix.h"

namespace cxx
{
	template<typename E>
	struct quaternion
	{
		vector_t<E, dimension::_3> v = vector_t<E, dimension::_3>(constants<E>::zero);
		E w = constants<E>::one;

		constexpr quaternion() = default;

		constexpr quaternion(const quaternion& q)
			: w(q.w), v(q.v) { }

		inline quaternion(const vector_t<E, dimension::_3>& axis, const radian<E>& r);

		constexpr quaternion(E _w, vector_t<E, dimension::_3> _v)
			: w(_w), v(_v) { }

		constexpr quaternion(const vector_t<E, dimension::_4>& v4)
			: w(v4.w), v(v4.x, v4.y, v4.z) { }

		constexpr quaternion(E _w, E _x, E _y, E _z)
			: w(_w), v(_x, _y, _z) { }

		constexpr quaternion& operator=(const quaternion& rhs)
		{
			w = rhs.w;
			v = rhs.v;
			return *this;
		}

		// static utilities
		static constexpr quaternion identity_pos()
		{
			return quaternion(
				constants<E>::one,
				constants<E>::zero,
				constants<E>::zero,
				constants<E>::zero);
		}

		static constexpr quaternion identity_neg()
		{
			return quaternion(
				-constants<E>::one,
				constants<E>::zero,
				constants<E>::zero,
				constants<E>::zero
			);
		}
	};

	// oprations
	template<typename E> 
	constexpr bool operator== (const quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		return &lhs == &rhs
			|| (is_equal(lhs.w, rhs.w) && lhs.v == rhs.v);
	}

	template<typename E> 
	constexpr bool operator!= (const quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename E> 
	constexpr const quaternion<E>& operator+(const quaternion<E>& q)
	{
		return q;
	}

	template<typename E>
	constexpr quaternion<E> operator-(const quaternion<E>& q)
	{
		return quaternion<E>(-q.w, -q.v);
	}

	template<typename E> 
	constexpr quaternion<E> operator+(const quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		return quaternion<E>(
			lhs.w + rhs.w,
			lhs.v + rhs.v
		);
	}

	template<typename E> 
	constexpr quaternion<E> operator-(const quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		return quaternion<E>(
			lhs.w - rhs.w,
			lhs.v - rhs.v
		);
	}

	template<typename E> 
	constexpr quaternion<E> operator*(const quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		return quaternion<E>(
			lhs.w * rhs.w - dot(lhs.v, rhs.v),
			lhs.w * rhs.v + rhs.w * lhs.v + cross(lhs.v, rhs.v)
		);
	}

	template<typename E> 
	constexpr quaternion<E> operator*(const quaternion<E>& lhs, scaler_t<E> rhs)
	{
		return quaternion<E>(lhs.w * rhs, lhs.v * rhs);
	}

	template<typename E>
	constexpr quaternion<E> operator*(scaler_t<E> lhs, const quaternion<E>& rhs)
	{
		return rhs * lhs;
	}

	template<typename E>
	constexpr quaternion<E>& operator+=(quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		lhs.w += rhs.w;
		lhs.v += rhs.v;
		return lhs;
	}

	template<typename E> 
	constexpr quaternion<E>& operator-=(quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		lhs.w -= rhs.w;
		lhs.v -= rhs.v;
		return lhs;
	}

	template<typename E>
	constexpr quaternion<E>& operator*=(quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		return (lhs = lhs * rhs);
	}

	template<typename E> 
	constexpr quaternion<E>& operator*=(quaternion<E>& lhs, scaler_t<E> rhs)
	{
		lhs.w *= rhs;
		lhs.v *= rhs;
		return lhs;
	}

	// functions
	template<typename E> 
	constexpr E dot(const quaternion<E>& lhs, const quaternion<E>& rhs)
	{
		return lhs.w * rhs.w + dot(lhs.v, rhs.v);
	}

	template<typename E> 
	inline void normalize(quaternion<E>& q)
	{
		E length2 = length_sqr(q);
		if (!is_equal(length2, constants<E>::zero) & !is_equal(length2, constants<E>::one))
		{
			E invLength = constants<E>::one / sqrtf(length2);
			q.v *= invLength;
			q.w *= invLength;
		}
	}

	template<typename E> 
	constexpr void conjugate(quaternion<E>& q)
	{
		q.v = -q.v;
	}

	template<typename E>
	constexpr void invert_w(quaternion<E>& q)
	{
		if (q.w < constants<E>::zero)
		{
			q.w = -q.w;
			q.v = -q.v;
		}
	}

	template<typename E>
	inline void inverse(quaternion<E>& q)
	{
		conjugate(q);
		normalize(q);
	}

	template<typename E> 
	constexpr E length_sqr(quaternion<E>& q)
	{
		return dot(q, q);
	}

	template<typename E> 
	inline E length(quaternion<E>& q)
	{
		return sqrtf(length_sqr(q));
	}

	template<typename E> 
	constexpr quaternion<E> conjugated(const quaternion<E>& q)
	{
		quaternion<E> rst(q);
		conjugate(rst);
		return rst;
	}

	template<typename E>
	inline quaternion<E> normalized(const quaternion<E>& q)
	{
		quaternion<E> rst(q);
		normalize(rst);
		return rst;
	}

	template<typename E> 
	inline quaternion<E> inversed(const quaternion<E>& q)
	{
		quaternion<E> rst(q);
		inverse(rst);
		return rst;
	}

	template<typename E> 
	inline quaternion<E> slerp(const quaternion<E>& s, const quaternion<E>& d, E f)
	{
		/*
		slerp(Qs, Qd, f) = (Qd * Qs^-1)^f * Qs;
		cross(d,s^-1) means the diff from s to d.

		quat rst = cross(d, s.inversed());
		rst.exp(f);
		return cross(rst, s);

		another slerp:
		slerp(Qs,Qd,f)	=
		(sin((1-t)w) / sinw) * Qs + (sin(tw) / sinw)*Qd =
		(sin((1-t)w)*Qs + sin(tw)*Qd) / sinw
		*/

		E cosw = dot(s, d);
		E f_s, f_e;

		//means sin_w = 0.0
		if (is_equal(cosw, constants<E>::one))
		{
			f_s = constants<E>::one - f;
			f_e = f;
		}
		else
		{
			E sinw = sqrtf(constants<E>::one - cosw * cosw);
			E inv_sinw = constants<E>::one / sinw;
			E w = atan2(sinw, cosw);

			f_s = sinf((constants<E>::one - f) * w)*  inv_sinw;
			f_e = sinf(f * w) * inv_sinw;
		}

		return f_s * s + f_e * d;
	}

	template<typename E> 
	inline vector_t<E, dimension::_3> rotate(const quaternion<E>& q, const vector_t<E, dimension::_3>& p)
	{
		return (q * quaternion<E>(constants<E>::zero, p) * inversed(q)).v;
	}

	template<typename E> 
	inline vector_t<E, dimension::_3> transform(const quaternion<E>& q, const vector_t<E, dimension::_3>& p)
	{
		return rotate(q, p);
	}

	// Converter
	template<typename E>
	constexpr vector_t<E, dimension::_4> to_vector4(const quaternion<E>& q)
	{
		return vector_t<E, dimension::_4>(q.v.x, q.v.y, q.v.z, q.w);
	}

	template<typename E> 
	constexpr euler<E> to_eular(const quaternion<E>& q)
	{
		vector_t<E, dimension::_3> v_sqr2 = q.v * q.v * constants<E>::two;
		E xy2 = q.v.x * q.v.y * constants<E>::two;
		E yz2 = q.v.y * q.v.z * constants<E>::two;
		E zx2 = q.v.z * q.v.x * constants<E>::two;
		E wx2 = q.w * q.v.x * constants<E>::two;
		E wy2 = q.w * q.v.y * constants<E>::two;
		E wz2 = q.w * q.v.z * constants<E>::two;

		return euler<E>(
			radian<E>(atan2(yz2 - wx2, constants<E>::one - v_sqr2.x - v_sqr2.y)),
			radian<E>(asin(clamp<E>(-(zx2 + wy2), -constants<E>::one, constants<E>::one))),
			radian<E>(atan2(xy2 - wz2, constants<E>::one - v_sqr2.y - v_sqr2.z))
			);
	}

	template<typename E> 
	constexpr matrix_t<E, dimension::_3, dimension::_3> to_matrix3x3(const quaternion<E>& q)
	{
		matrix_t<E, dimension::_3, dimension::_3> rst;
		vector_t<E, dimension::_3> vsqr2 = q.v * q.v * constants<E>::two;
		E xy2 = q.v.x * q.v.y * constants<E>::two;
		E yz2 = q.v.y * q.v.z * constants<E>::two;
		E zx2 = q.v.z * q.v.x * constants<E>::two;
		E wx2 = q.w * q.v.x * constants<E>::two;
		E wy2 = q.w * q.v.y * constants<E>::two;
		E wz2 = q.w * q.v.z * constants<E>::two;

		rst.c[0][0] = constants<E>::one - vsqr2.y - vsqr2.z;
		rst.c[1][1] = constants<E>::one - vsqr2.z - vsqr2.x;
		rst.c[2][2] = constants<E>::one - vsqr2.x - vsqr2.y;

		rst.c[0][1] = xy2 - wz2;
		rst.c[0][2] = zx2 + wy2;
		rst.c[1][0] = xy2 + wz2;

		rst.c[1][2] = yz2 - wx2;
		rst.c[2][0] = zx2 - wy2;
		rst.c[2][1] = yz2 + wx2;

		return rst;
	}

	template<typename E> 
	constexpr matrix_t<E, dimension::_4, dimension::_4> to_matrix4x4(const quaternion<E>& q)
	{
		matrix_t<E, dimension::_4, dimension::_4> rst;
		vector_t<E, dimension::_3> vsqr2 = q.v * q.v * constants<E>::two;
		E xy2 = q.v.x * q.v.y * constants<E>::two;
		E yz2 = q.v.y * q.v.z * constants<E>::two;
		E zx2 = q.v.z * q.v.x * constants<E>::two;
		E wx2 = q.w * q.v.x * constants<E>::two;
		E wy2 = q.w * q.v.y * constants<E>::two;
		E wz2 = q.w * q.v.z * constants<E>::two;

		rst.c[0][0] = constants<E>::one - vsqr2.y - vsqr2.z;
		rst.c[1][1] = constants<E>::one - vsqr2.z - vsqr2.x;
		rst.c[2][2] = constants<E>::one - vsqr2.x - vsqr2.y;

		rst.c[0][1] = xy2 - wz2;
		rst.c[0][2] = zx2 + wy2;
		rst.c[1][0] = xy2 + wz2;

		rst.c[1][2] = yz2 - wx2;
		rst.c[2][0] = zx2 - wy2;
		rst.c[2][1] = yz2 + wx2;

		return rst;
	}

	// implements
	template<typename E>
	inline quaternion<E>::quaternion(const vector_t<E, dimension::_3>& axis, const radian<E>& r)
	{
		radian<E> halfRadian = r * constants<E>::half;

		E halfCos = cos(halfRadian);
		E halfSin = sin(halfRadian);

		v = axis * halfSin;
		w = halfCos;

		normalize(*this);
	}

	// global converter
	namespace rotation_impl
	{
		enum { WSqr, XSqr, YSqr, ZSqr };
	}

	template<typename E>
	inline quaternion<E> to_quaternion(const euler<E>& e)
	{
		vector_t<E, dimension::_4> v0
		(
			cosf(e.yaw.value * constants<E>::half),
			constants<E>::zero,
			constants<E>::zero,
			sinf(e.yaw.value * constants<E>::half)
		);

		vector_t<E, dimension::_4> v1
		(
			cosf(e.pitch.value * constants<E>::half),
			constants<E>::zero,
			sinf(e.pitch.value * constants<E>::half),
			constants<E>::zero
		);

		vector_t<E, dimension::_4> v2
		(
			cosf(e.roll.value * constants<E>::half),
			sinf(e.roll.value * constants<E>::half),
			constants<E>::zero,
			constants<E>::zero
		);

		return quaternion<E>(v0 * v1 * v2);
	}

	template<typename E>
	inline quaternion<E> to_quaternion(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		E wsqr = matrix.c[0][0] + matrix.c[1][1] + matrix.c[2][2];
		E xsqr = matrix.c[0][0] - matrix.c[1][1] - matrix.c[2][2];
		E ysqr = matrix.c[1][1] - matrix.c[0][0] - matrix.c[2][2];
		E zsqr = matrix.c[2][2] - matrix.c[0][0] - matrix.c[1][1];

		int maxIndex = WSqr;
		E maxSqr = wsqr;
		if (xsqr > maxSqr)
		{
			maxSqr = xsqr;
			maxIndex = XSqr;
		}
		if (ysqr > maxSqr)
		{
			maxSqr = ysqr;
			maxIndex = YSqr;
		}
		if (zsqr > maxSqr)
		{
			maxSqr = zsqr;
			maxIndex = zSqr;
		}

		maxSqr = sqrtf(maxSqr + constants<E>::one) * constants<E>::half;
		E base = constants<E>::quater / maxSqr;
		quaternion<E> rst;
		switch (maxIndex)
		{
		case WSqr:
			rst.w = maxSqr;
			rst.v.x = (matrix.c[2][1] - matrix.c[1][2]) * base;
			rst.v.y = (matrix.c[0][2] - matrix.c[2][0]) * base;
			rst.v.z = (matrix.c[1][0] - matrix.c[0][1]) * base;
			break;
		case XSqr:
			rst.v.x = maxSqr;
			rst.w = (matrix.c[2][1] - matrix.c[1][2]) * base;
			rst.v.y = (matrix.c[1][0] + matrix.c[0][1]) * base;
			rst.v.z = (matrix.c[2][0] + matrix.c[0][2]) * base;
			break;
		case YSqr:
			rst.v.y = maxSqr;
			rst.w = (matrix.c[0][2] - matrix.c[2][0]) * base;
			rst.v.x = (matrix.c[1][0] + matrix.c[0][1]) * base;
			rst.v.z = (matrix.c[2][1] + matrix.c[1][2]) * base;
			break;
		case ZSqr:
			rst.v.z = maxSqr;
			rst.w = (matrix.c[1][0] - matrix.c[0][1]) * base;
			rst.v.x = (matrix.c[2][0] + matrix.c[0][2]) * base;
			rst.v.y = (matrix.c[2][1] + matrix.c[1][2]) * base;
			break;
		}

		return normalized(rst);
	}

	template<typename E>
	inline quaternion<E> to_quaternion(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		E wsqr = matrix.c[0][0] + matrix.c[1][1] + matrix.c[2][2];
		E xsqr = matrix.c[0][0] - matrix.c[1][1] - matrix.c[2][2];
		E ysqr = matrix.c[1][1] - matrix.c[0][0] - matrix.c[2][2];
		E zsqr = matrix.c[2][2] - matrix.c[0][0] - matrix.c[1][1];

		int maxIndex = rotation_impl::WSqr;
		E maxSqr = wsqr;
		if (xsqr > maxSqr)
		{
			maxSqr = xsqr;
			maxIndex = rotation_impl::XSqr;
		}
		if (ysqr > maxSqr)
		{
			maxSqr = ysqr;
			maxIndex = rotation_impl::YSqr;
		}
		if (zsqr > maxSqr)
		{
			maxSqr = zsqr;
			maxIndex = rotation_impl::ZSqr;
		}

		maxSqr = sqrtf(maxSqr + constants<E>::one) * constants<E>::half;
		E base = constants<E>::quater / maxSqr;
		quaternion<E> rst;
		switch (maxIndex)
		{
		case rotation_impl::WSqr:
			rst.w = maxSqr;
			rst.v.x = (matrix.c[2][1] - matrix.c[1][2]) * base;
			rst.v.y = (matrix.c[0][2] - matrix.c[2][0]) * base;
			rst.v.z = (matrix.c[1][0] - matrix.c[0][1]) * base;
			break;
		case rotation_impl::XSqr:
			rst.v.x = maxSqr;
			rst.w = (matrix.c[2][1] - matrix.c[1][2]) * base;
			rst.v.y = (matrix.c[1][0] + matrix.c[0][1]) * base;
			rst.v.z = (matrix.c[2][0] + matrix.c[0][2]) * base;
			break;
		case rotation_impl::YSqr:
			rst.v.y = maxSqr;
			rst.w = (matrix.c[0][2] - matrix.c[2][0]) * base;
			rst.v.x = (matrix.c[1][0] + matrix.c[0][1]) * base;
			rst.v.z = (matrix.c[2][1] + matrix.c[1][2]) * base;
			break;
		case rotation_impl::ZSqr:
			rst.v.z = maxSqr;
			rst.w = (matrix.c[1][0] - matrix.c[0][1]) * base;
			rst.v.x = (matrix.c[2][0] + matrix.c[0][2]) * base;
			rst.v.y = (matrix.c[2][1] + matrix.c[1][2]) * base;
			break;
		}

		return normalized(rst);
	}

	using quat_f = quaternion<float>;
	using quat_d = quaternion<double>;
}