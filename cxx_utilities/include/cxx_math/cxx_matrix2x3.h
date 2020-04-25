#pragma once
#include "cxx_point2.h"
#include "cxx_base_matrix.h"
#include "cxx_matrix2x2.h"

namespace cxx
{
	template<typename E>
	struct matrix_t<E, dimension::_2, dimension::_3>
	{
		using k = constants<E>;
		using mindex = matrix_index<dimension::_2, dimension::_3>;
		static constexpr dimension	Row = dimension::_2;
		static constexpr dimension	Col = dimension::_3;
		static constexpr size_t		Cell = Row * Col;

		union
		{
			char	m[Cell * sizeof(E)];
			E		v[Cell];
			E		c[Row][Col];
			vector_t<E, dimension::_3> r[Row];
			struct
			{
				E	_00, _01, _02,
					_10, _11, _12;
			};
		};

		constexpr matrix_t() : c{
			 { k::one, k::zero, k::zero }
			,{ k::zero, k::one, k::zero } }
		{	}

		constexpr matrix_t(
			E _m00, E _m01, E _m02,
			E _m10, E _m11, E _m12) : c{
			 { _m00, _m01, _m02 }
			,{ _m10, _m11, _m12 } }
		{	}

		constexpr matrix_t(
			const vector_t<E, dimension::_3>& row0,
			const vector_t<E, dimension::_3>& row1) : c{
			 { row0.x, row0.y,row0.z }
			,{ row1.x, row1.y,row1.z } }
		{ }

		constexpr matrix_t(const matrix_t& m32) : c{
			 { m32.c[0][0], m32.c[0][1], m32.c[0][2] }
			,{ m32.c[1][0], m32.c[1][1], m32.c[1][2] } }
		{ }

		constexpr matrix_t(const matrix_t<E, dimension::_2, dimension::_2>& m22,
			const vector_t<E, dimension::_2>& translate) : c{
			 { m22.c[0][0], m22.c[0][1], translate.x }
			,{ m22.c[1][0], m22.c[1][1], translate.y } }
		{ }

		constexpr matrix_t& operator=(const matrix_t& rhs)
		{
			for (size_t ri = 0; ri < Row; ri++)
			{
				for (size_t ci = 0; ci < Col; ci++)
				{
					c[ri][ci] = rhs.c[ri][ci];
				}
			}
			return *this;
		}

		inline E&			operator[] (mindex idx) { return v[idx]; }

		inline const E&		operator[] (mindex idx) const { return v[idx]; }

		//static utilities
		static constexpr matrix_t identity()
		{
			return matrix_t(
				k::one, k::zero, k::zero,
				k::zero, k::one, k::zero
			);
		}

		static constexpr matrix_t scale(E sx, E sy)
		{
			return matrix_t(
				sx, k::zero, k::zero,
				k::zero, sy, k::zero
			);
		}

		static constexpr matrix_t scale(E s)
		{
			return scale(s, s);
		}

		static constexpr matrix_t scale(const vector_t<E, dimension::_2>& s)
		{
			return scale(s.x, s.y);
		}

		static constexpr matrix_t flip_x()
		{
			return matrix_t(
				-k::one, k::zero, k::zero,
				k::zero, k::one, k::zero
			);
		}

		static constexpr matrix_t flip_y()
		{
			return matrix_t(
				k::one, k::zero, k::zero,
				k::zero, -k::one, k::zero
			);
		}

		static constexpr matrix_t translate(E tx, E ty)

		{
			return matrix_t(
				k::one, k::zero, tx,
				k::zero, k::one, ty
			);
		}

		static constexpr matrix_t translate(const vector_t < E, dimension::_2>& t)
		{
			return translate(t.x, t.y);
		}

		static inline matrix_t rotate(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);
			return matrix_t(
				cosr, -sinr, k::zero,
				sinr, cosr, k::zero
			);
		}

		static inline matrix_t trs_inversed(const vector_t<E, dimension::_2>& t, const radian<E>& r, const vector_t<E, dimension::_2>& s)
		{
			/*
			sx,0,0 * +cos,sin,0 * 1,0,-tx
			0,sy,0   -sin,cos,0   0,1,-ty
			=
			sx,0,0 * +cos,sin,-cos*tx-sin*ty
			0,sy,0	 -sin,cos,+sin*tx-cos*ty
			=
			+sx*cos, sx*sin, -sx*(cos*tx+sin*ty)
			-sy*sin, sy*cos, +sy*(sin*tx-cos*ty)
			*/
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				+s.x*cosr, s.x*sinr, -s.x*(cosr*t.x - sinr * t.y),
				-s.y*sinr, s.y*cosr, +s.y*(sinr*t.x - cosr * t.y)
			);
		}

		static inline matrix_t trs(const vector_t<E, dimension::_2>& t, const radian<E>& r, const vector_t<E, dimension::_2>& s)
		{
			/*
			1,0,tx * cos,-sin,0 * sx,0,0
			0,1,ty   sin, cos,0   0,sy,0
			=
			cos,-sin,tx * sx,0,0
			sin,+cos,ty   0,sy,0
			=
			cos*sx, -sin*sy, tx
			sin*sx, +cos*sy, ty
			*/
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				cosr * s.x, -sinr * s.y, t.x,
				sinr * s.x, +cosr * s.y, t.y
			);
		}

		static inline matrix_t trsp(const vector_t<E, dimension::_2>& t, const radian<E>& r, const vector_t<E, dimension::_2>& s, const vector_t<E, dimension::_2>& p)
		{
			/*
			1,0,tx * cos,-sin,0 * sx,0,0 * 1,0,-px
			0,1,ty   sin, cos,0   0,sy,0   0,1,-py
			=
			cos,-sin,tx * sx,0,-sx*px
			sin,+cos,ty   0,sy,-sy*py
			=
			cos*sx, -sin*sy, -cos*sx*px+sin*sy*px + tx
			sin*sx, +cos*sy, -sin*sx*px-cos*sy*py + ty
			*/
			E cosr = cos(r);
			E sinr = sin(r);
			E spx = -s.x*p.x;
			E spy = -s.y*p.y;

			return matrix_t(
				cosr * s.x, -sinr * s.y, cosr*spx - sinr * spy + t.x,
				sinr * s.x, +cosr * s.y, sinr*spx + cosr * spy + t.y
			);
		}

		static inline matrix_t trps(const vector_t<E, dimension::_2>& t, const radian<E>& r, const vector_t<E, dimension::_2>& p, const vector_t<E, dimension::_2>& s)
		{
			/*
			1,0,tx * cos,-sin,0 * 1,0,-px * sx,0,0
			0,1,ty   sin, cos,0   0,1,-px   0,sy,0
			=
			cos,-sin,ty * sx,0,-px
			sin, cos,ty   0,sy,-px
			=
			cos*sx, -sin*sy, -cos*px+sin*px + tx
			sin*sx, +cos*sy, -sin*px-cos*py + ty
			*/
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				cosr * s.x, -sinr * s.y, -cosr * p.x + sinr * p.y + t.x,
				sinr * s.x, +cosr * s.y, -sinr * p.x - cosr * p.y + t.y
			);
		}
	};

	// getters
	template<typename E>
	constexpr vector_t<E, dimension::_2> row2(const matrix_t<E, dimension::_2, dimension::_3>& matrix, vector_index<dimension::_2> idx)
	{
		return vector_t<E, dimension::_2>(matrix.c[idx][0], matrix.c[idx][1]);
	}

	template<typename E>
	constexpr vector_index<dimension::_3> row(const matrix_t<E, dimension::_2, dimension::_3>& matrix, vector_index<dimension::_2> idx)
	{
		return vector_r(
			matrix.c[idx][0],
			matrix.c[idx][1],
			matrix.c[idx][2]
		);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> column(const matrix_t<E, dimension::_2, dimension::_3>& matrix, vector_index<dimension::_3> idx)
	{
		return vector_t<E, dimension::_2>(matrix.c[0][idx], matrix.c[1][idx]);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_3> column3(const matrix_t<E, dimension::_2, dimension::_3>& matrix, vector_index<dimension::_3> idx)
	{
		return vector_t<E, dimension::_3>(
			matrix.c[0][idx],
			matrix.c[1][idx],
			idx == 2 ? constants<E>::one : constants<E>::zero
			);
	}

	// operations
	template<typename E>
	inline matrix_t<E, dimension::_2, dimension::_3> operator*(const matrix_t<E, dimension::_2, dimension::_3>& lhs, const matrix_t<E, dimension::_2, dimension::_3>& rhs)
	{
		matrix_t<E, dimension::_2, dimension::_3> rst;
		for (size_t ri = 0; ri < matrix_t<E, dimension::_2, dimension::_3>::Row; ri++)
		{
			for (size_t ci = 0; ci < matrix_t<E, dimension::_2, dimension::_3>::Col; ci++)
			{
				rst.c[ri][ci] = dot(lhs.r[ri], column3(rhs, ci));
			}
		}
		return rst;
	}

	template<typename E>
	inline vector_t<E, dimension::_3> operator* (const matrix_t<E, dimension::_2, dimension::_3>& lhs, const vector_t<E, dimension::_3> & rhs)
	{
		return vector_t<E, dimension::_3>(
			dot(lhs.r[0], rhs),
			dot(lhs.r[1], rhs),
			rhs.z
			);
	}

	template<typename E>
	inline vector_t<E, dimension::_3> operator* (const matrix_t<E, dimension::_2, dimension::_3>& lhs, const normalized_vector_t<E, dimension::_3> & rhs)
	{
		return lhs * rhs.value;
	}

	// functions
	template<typename E>
	constexpr void set_column(matrix_t<E, dimension::_2, dimension::_3>& matrix, vector_index<dimension::_3> idx, vector_t<E, dimension::_2> _v)
	{
		matrix.c[0][idx] = _v.x;
		matrix.c[1][idx] = _v.y;
	}

	template<typename E>
	constexpr bool is_orthogonal(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		if (!is_equal(matrix.c[0][2], constants<E>::zero) ||
			!is_equal(matrix.c[1][2], constants<E>::zero))
		{
			return false;
		}

		vector_t<E, dimension::_2> r0(matrix.c[0][0], matrix.c[0][1]);
		vector_t<E, dimension::_2> r1(matrix.c[1][0], matrix.c[1][1]);
		if (!is_equal(length_sqr(r0), constants<E>::one) ||
			!is_equal(length_sqr(r1), constants<E>::one))
		{
			return false;
		}

		return is_equal(dot(r0, r1), constants<E>::zero);
	}

	template<typename E>
	constexpr bool can_invert(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		return  is_orthogonal(matrix)
			|| !is_equal(determinant(matrix), constants<E>::zero);
	}

	/**
	*	it is dangerous unless u know what ur doing.
	*/
	template<typename E>
	constexpr void inverse(matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		if (is_orthogonal(matrix))
		{
			swap(matrix.c[0][1], matrix.c[1][0]);
		}
		else
		{
			E det = determinant(matrix);
			if (!is_equal(det, constants<E>::zero))
			{
				matrix_t<E, dimension::_2, dimension::_3> old = matrix;

				swap(matrix.c[0][0], matrix.c[1][1]);
				matrix.c[0][1] = -matrix.c[0][1];
				matrix.c[1][0] = -matrix.c[1][0];
				matrix.c[0][2] = +matrix_impl::determinant2x2(old.c[0][1], old.c[0][2], old.c[1][1], old.c[1][2]);
				matrix.c[1][2] = -matrix_impl::determinant2x2(old.c[0][0], old.c[0][2], old.c[1][0], old.c[1][2]);
				matrix *= (constants<E>::one / det);
			}
			else
			{
				//TODO:
				// can not invert here,
				// what should i do when this occur ?
				matrix = matrix_t<E, dimension::_2, dimension::_3>::identity();
			}
		}
	}

	template<typename E>
	constexpr E determinant(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		return matrix_impl::determinant2x3
		(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2],
			matrix.c[1][0], matrix.c[1][1], matrix.c[2][2]
		);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_2, dimension::_2> get_rotation_matrix2x2(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_2, dimension::_2>(
			matrix.c[0][0], matrix.c[0][1],
			matrix.c[1][0], matrix.c[1][1]
			);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_2, dimension::_3> get_rotation_matrix2x3(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_2, dimension::_3>(
			matrix.c[0][0], matrix.c[0][1], constants<E>::zero,
			matrix.c[1][0], matrix.c[1][1], constants<E>::zero
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> get_translation(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		vector_t<E, dimension::_2> r_t0 = column(matrix, 0);
		vector_t<E, dimension::_2> r_t1 = column(matrix, 1);
		vector_t<E, dimension::_2> t = column(matrix, 2);

		return vector_t<E, dimension::_2>(
			-dot(r_t0, t),
			-dot(r_t1, t)
			);
	}

	template<typename E>
	inline vector_t<E, dimension::_3> transform(const matrix_t<E, dimension::_2, dimension::_3>& lhs, const vector_t<E, dimension::_3>& rhs)
	{
		return lhs * rhs;
	}

	template<typename E>
	inline vector_t<E, dimension::_3> transform(const matrix_t<E, dimension::_2, dimension::_3>& lhs, const normalized_vector_t<E, dimension::_3>& rhs)
	{
		return lhs * rhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> transform(const matrix_t<E, dimension::_2, dimension::_3>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		vector_t<E, dimension::_3> rhs3(rhs, constants<E>::zero);
		return vector_t<E, dimension::_2>(
			dot(lhs.r[0], rhs3),
			dot(lhs.r[1], rhs3)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> transform(const matrix_t<E, dimension::_2, dimension::_3>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return transform(lhs, rhs.value);
	}

	template<typename E>
	constexpr point_t<E, dimension::_2> transform(const matrix_t<E, dimension::_2, dimension::_3>& lhs, const point_t<E, dimension::_2>& rhs)
	{
		vector_t<E, dimension::_3> rhs3(rhs, constants<E>::one);
		return point_t<E, dimension::_2>(
			dot(lhs.r[0], rhs3),
			dot(lhs.r[1], rhs3)
			);
	}

	//converter
	template<typename E>
	constexpr matrix_t<E, dimension::_2, dimension::_2> to_matrix2x2(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_2, dimension::_2>(
			matrix.c[0][0], matrix.c[0][1],
			matrix.c[1][0], matrix.c[1][1]
			);
	}
}