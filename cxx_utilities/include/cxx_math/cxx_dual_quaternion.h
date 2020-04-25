#pragma once
#include "cxx_quaternion.h"

namespace cxx
{
	template<typename E>
	struct dual_quaternion
	{
		quaternion<E> real;
		quaternion<E> dual;

		constexpr dual_quaternion()
			: real(constants<E>::one, constants<E>::zero, constants<E>::zero, constants<E>::zero)
			, dual(constants<E>::zero, constants<E>::zero, constants<E>::zero, constants<E>::zero)
		{ }

		/**
		*	only translation
		*/
		constexpr dual_quaternion(const vector_t<E, dimension::_3>& translation)
			: real(1, 0, 0, 0)
			, dual(0, translation * constants<E>::half)
		{ }

		/**
		*	only rotate
		*/
		inline dual_quaternion(const vector_t<E, dimension::_3>& axis, const radian<E>& r)
			: real(axis, r)
			, dual(0, 0, 0, 0)
		{ }

		/**
		*	rotate and translate
		*/
		inline dual_quaternion(const vector_t<E, dimension::_3>& axis, const radian<E>& r, const vector_t<E, dimension::_3>& translation);

		inline dual_quaternion(const quaternion<E>& rotation, const vector_t<E, dimension::_3>& translation);

		/**
		* unless you know what u r doing.
		*/
		constexpr dual_quaternion(const quaternion<E>& r, const quaternion<E>& d)
			: real(r), dual(d) { }

		// static utilities
		static constexpr dual_quaternion identity()
		{
			return dual_quaternion(
				quaternion<E>(constants<E>::one, constants<E>::zero, constants<E>::zero, constants<E>::zero),
				quaternion<E>(constants<E>::zero, constants<E>::zero, constants<E>::zero, constants<E>::zero)
			);
		}
	};

	// dual_quternion oprations
	template<typename E> 
	constexpr dual_quaternion<E> operator== (const dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		return lhs.dual == rhs.dual && lhs.real == rhs.real;
	}

	template<typename E> 
	constexpr dual_quaternion<E> operator!= (const dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename E> 
	constexpr dual_quaternion<E> operator+ (const dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		return dual_quaternion<E>(lhs.real + rhs.real, lhs.dual + rhs.dual);
	}

	template<typename E> 
	constexpr dual_quaternion<E> operator- (const dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		return dual_quaternion<E>(lhs.real - rhs.real, lhs.dual - rhs.dual);
	}

	template<typename E> 
	constexpr dual_quaternion<E> operator* (const dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		quaternion<E> new_r = lhs.real * rhs.real;
		quaternion<E> new_d = lhs.real * rhs.dual + lhs.dual * rhs.real; //noticing the order.
		return dual_quaternion<E>(new_r, new_d);
	}

	template<typename E> 
	constexpr dual_quaternion<E> operator* (const dual_quaternion<E>& lhs, scaler_t<E> rhs)
	{
		quaternion<E> new_r = lhs.real * rhs;
		quaternion<E> new_d = lhs.dual * rhs;
		return dual_quaternion<E>(new_r, new_d);
	}

	template<typename E>
	constexpr dual_quaternion<E> operator*(scaler_t<E> lhs, dual_quaternion<E>& rhs)
	{
		return rhs * lhs;
	}

	template<typename E> 
	constexpr dual_quaternion<E>& operator+=(dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		lhs.real += rhs.real;
		lhs.dual += rhs.dual;
		return lhs;
	}

	template<typename E> 
	constexpr dual_quaternion<E>& operator-=(dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		lhs.real -= rhs.real;
		lhs.dual -= rhs.dual;
		return lhs;
	}

	template<typename E> 
	constexpr dual_quaternion<E>& operator*=(dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		quaternion<E> new_r = lhs.real * rhs.real;
		quaternion<E> new_d = lhs.real * rhs.dual + lhs.dual * rhs.real;
		lhs.real = new_r;
		lhs.dual = new_d;
		return lhs;
	}

	template<typename E> 
	constexpr dual_quaternion<E>& operator*=(dual_quaternion<E>& lhs, scaler_t<E> rhs)
	{
		lhs.real *= rhs;
		lhs.dual *= rhs;
		return lhs;
	}

	// functions
	template<typename E> 
	constexpr E dot(const dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		return dot(lhs.real, rhs.real);
	}

	template<typename E> 
	constexpr E length_sqr(const dual_quaternion<E>& dq)
	{
		return dot(dq, dq);
	}

	template<typename E> 
	inline E length(const dual_quaternion<E>& dq)
	{
		return sqrtf(length_sqr(dq));
	}

	template<typename E> 
	constexpr void conjugate(dual_quaternion<E>& dq)
	{
		conjugate(dq.real);
		//q = -q & q.w =0;
		// => q.conjugate() = -q = q
		// so.... what....
		conjugate(dq.dual);
	}

	template<typename E> 
	inline void normalize(dual_quaternion<E>& dq)
	{
		E l = length_sqr(dq);
		if (!is_equal(l, constants<E>::zero) && !is_equal(l, constants<E>::one))
		{
			dq *= constants<E>::one / sqrtf(l);
		}
	}

	template<typename E> 
	inline void inverse(dual_quaternion<E>& dq)
	{
		normalize(dq);
		conjugate(dq);
	}

	template<typename E> 
	inline void exponent(dual_quaternion<E>& dq, scaler_t<E> e)
	{
		E r_inv = constants<E>::one / length(dq);

		// Change the pitch.
		// Screw parameters
		radian<E> r = get_rotate_radian(dq);

		//-2 * dual.w * invr
		E pitch = dq.dual.w * r_inv;

		vector_t<E, dimension::_3> direction = dq.real.v * r_inv;

		//(dual.v - direction * pitch * real.w * constants<E>::half) * invr
		vector_t<E, dimension::_3> moment = (dq.dual.v + direction * pitch * dq.real.w)
			* r_inv;

		// Exponential power 
		r *= e * constants<E>::half;
		pitch *= e;

		// Convert back to dual-quaternion
		E sinAngle = sin(r); // angle/2
		E cosAngle = cos(r); // angle/2

		/*
			dual = quat(
				-pitch * sinAngle * constants<E>::half ,
				sinAngle * moment + constants<E>::half * pitch * cosAngle * direction
			);
		*/
		dq.real = quaternion<E>(cosAngle, direction * sinAngle);
		dq.dual = quaternion<E>(pitch * sinAngle, sinAngle * moment - pitch * cosAngle * direction);
	}

	template<typename E> 
	constexpr dual_quaternion<E> conjugated(const dual_quaternion<E>& dq)
	{
		dual_quaternion<E> rst(dq);
		conjugate(rst);
		return rst;
	}

	template<typename E>
	inline dual_quaternion<E> normalized(const dual_quaternion<E>& dq)
	{
		dual_quaternion<E> rst(dq);
		normalize(rst);
		return rst;
	}

	template<typename E>
	inline dual_quaternion<E> inversed(const dual_quaternion<E>& dq)
	{
		dual_quaternion<E> rst(dq);
		inverse(rst);
		return rst;
	}

	template<typename E>
	inline dual_quaternion<E> exponented(const dual_quaternion<E>& lhs, scaler_t<E> e)
	{
		dual_quaternion<E> rst(dq);
		exponent(rst, t);
		return rst;
	}

	template<typename E>
	constexpr quaternion<E> get_rotation(const dual_quaternion<E>& q)
	{
		return q.real;
	}

	template<typename E> 
	constexpr vector_t<E, dimension::_3> get_translation(const dual_quaternion<E>& q)
	{
		return (constants<E>::two * q.dual * conjugated(q.real)).v;
	}

	template<typename E> 
	inline radian<E> get_rotate_radian(const dual_quaternion<E>& q)
	{
		E w = frac(q.real.w);
		return radian<E>(2 * acos(w));
	}

	template<typename E> 
	dual_quaternion<E> sc_lerp(const dual_quaternion<E>& from, const dual_quaternion<E>& to, scaler_t<E> t)
	{
		/* this code piece may cause an unexpected result,
		which the translation is oppsite from the right direction.

		// Shortest path
		E dotReal = dot(from.real, to.real);
		dquat to_n(to);
		if (dotReal < 0)
		{
		to_n.real = -to_n.real;
		to_n.dual = -to_n.dual;
		}
		*/

		// ScLERP = Qa * (Qa^-1 * Qb)^t 
		dual_quaternion<E> diff = conjugated(from) * to;
		exponent(diff, t);
		return from * diff;
	}

	template<typename E> 
	inline point_t<E, dimension::_3> transform(const dual_quaternion<E>& dq, const point_t<E, dimension::_3>& point)
	{
		dual_quaternion<E> inv = inversed(dq);
		inv.dual = -inv.dual;

		dual_quaternion<E> pt = dual_quaternion<E>(
			quaternion<E>::identity_pos(),
			quaternion<E>(constants<E>::zero, point));

		return (dq * pt * inv).dual.v;
	}

	/** \brief signed add
	*/
	template<typename E> 
	constexpr dual_quaternion<E> operator&(const dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		if (dot(lhs.real, rhs.real) >= 0)
		{
			return lhs + rhs;
		}
		else
		{
			return lhs - rhs;
		}
	}

	template<typename E>
	constexpr dual_quaternion<E>& operator&=(dual_quaternion<E>& lhs, const dual_quaternion<E>& rhs)
	{
		if (dot(lhs.real, rhs.real) >= 0)
		{
			return (lhs += rhs);
		}
		else
		{
			return (lhs -= rhs);
		}
	}

	// Converter

	template<typename E>
	inline matrix_t<E, dimension::_4, dimension::_4> to_matrix4x4(const dual_quaternion<E>& dq)
	{
		matrix_t<E, dimension::_4, dimension::_4> rst;
		dual_quaternion<E> dq_normalize = normalized(dq);

		// Extract rotational information
		E w = dq_normalize.real.w;
		E x = dq_normalize.real.v.x;
		E y = dq_normalize.real.v.y;
		E z = dq_normalize.real.v.z;

		E w2 = w * w;
		E x2 = x * x;
		E y2 = y * y;
		E z2 = z * z;
		E xy2 = x * y * constants<E>::two;
		E yz2 = y * z * constants<E>::two;
		E xz2 = x * z * constants<E>::two;
		E wx2 = w * x * constants<E>::two;
		E wy2 = w * y * constants<E>::two;
		E wz2 = w * z * constants<E>::two;

		rst.c[0][0] = w2 + x2 - y2 - z2;
		rst.c[1][0] = xy2 + wz2;
		rst.c[2][0] = xz2 - wy2;

		rst.c[0][1] = xy2 - wz2;
		rst.c[1][1] = w2 + y2 - x2 - z2;
		rst.c[2][1] = yz2 + wx2;

		rst.c[0][2] = xz2 + wy2;
		rst.c[1][2] = yz2 - wx2;
		rst.c[2][2] = w2 + z2 - x2 - y2;

		// Extract translation information
		vector_t<E, dimension::_3> translation = get_translation(dq_normalize);
		rst.c[0][3] = translation.x;
		rst.c[1][3] = translation.y;
		rst.c[2][3] = translation.z;

		rst.c[3][0] = constants<E>::zero;
		rst.c[3][1] = constants<E>::zero;
		rst.c[3][2] = constants<E>::zero;
		rst.c[3][3] = constants<E>::one;

		return rst;
	}


	// implements
	template<typename E>
	inline dual_quaternion<E>::dual_quaternion(const quaternion<E>& rotation, const vector_t<E, dimension::_3>& translation)
		: real(rotation)
		, dual(0, translation * constants<E>::half)
	{
		normalize(real);
		dual *= real;
	}

	template<typename E>
	inline dual_quaternion<E>::dual_quaternion(
		const vector_t<E, dimension::_3>& axis,
		const radian<E>& r,
		const vector_t<E, dimension::_3>& translation)
		: real(axis, r)
		, dual(0, translation * constants<E>::half)
	{
		dual *= real;
	}

	using dquat_f = dual_quaternion<float>;
	using dquat_d = dual_quaternion<double>;
}