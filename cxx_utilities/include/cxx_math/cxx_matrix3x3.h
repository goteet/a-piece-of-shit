#pragma once
#include "cxx_base_matrix.h"
#include "cxx_matrix2x2.h"
#include "cxx_matrix2x3.h"

namespace cxx
{
	template<typename E>
	struct matrix_t<E, dimension::_3, dimension::_3>
	{
		using k = constants<E>;
		using mindex = matrix_index<dimension::_3, dimension::_3>;

		static constexpr dimension	Row = dimension::_3;
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
					_10, _11, _12,
					_20, _21, _22;
			};
		};

		constexpr matrix_t() : c{
			 { k::one, k::zero, k::zero }
			,{ k::zero, k::one, k::zero }
			,{ k::zero, k::zero, k::one } }
		{ }

		constexpr matrix_t(
			E _m00, E _m01, E _m02,
			E _m10, E _m11, E _m12,
			E _m20, E _m21, E _m22) : c{
			 { _m00, _m01, _m02 }
			,{ _m10, _m11, _m12 }
			,{ _m20, _m21, _m22 } }
		{ }

		constexpr matrix_t(
			const vector_t<E, dimension::_3>& row0,
			const vector_t<E, dimension::_3>& row1,
			const vector_t<E, dimension::_3>& row2) : c{
			 { row0.x, row0.y,row0.z }
			,{ row1.x, row1.y,row1.z }
			,{ row2.x, row2.y,row2.z } }
		{ }

		constexpr matrix_t(const matrix_t& m33) : c{
			 { m33.c[0][0], m33.c[0][1], m33.c[0][2] }
			,{ m33.c[1][0], m33.c[1][1], m33.c[1][2] }
			,{ m33.c[2][0], m33.c[2][1], m33.c[2][2] } }
		{ }

		constexpr matrix_t(const matrix_t<E, dimension::_2, dimension::_2>& m22,
			const vector_t<E, dimension::_2>& translate) : c{
			 { m22.c[0][0], m22.c[0][1], translate.x }
			,{ m22.c[1][0], m22.c[1][1], translate.y }
			,{ k::zero, k::zero, k::one } }
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

		// static utilities
		static constexpr matrix_t identity()
		{
			return matrix_t(
				k::one, k::zero, k::zero,
				k::zero, k::one, k::zero,
				k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t scale(E sx, E sy, E sz)
		{
			return matrix_t(
				sx, k::zero, k::zero,
				k::zero, sy, k::zero,
				k::zero, k::zero, sz
			);
		}

		static constexpr matrix_t scale(E s)
		{
			return scale(s, s, s);
		}

		static constexpr matrix_t scale(const vector_t<E, dimension::_3>& s)
		{
			return scale(s.x, s.y, s.z);
		}

		static constexpr matrix_t flip_x()
		{
			return matrix_t(
				-k::one, k::zero, k::zero,
				k::zero, k::one, k::zero,
				k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t flip_y()
		{
			return matrix_t(
				k::one, k::zero, k::zero,
				k::zero, -k::one, k::zero,
				k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t flip_z()
		{
			return matrix_t(
				k::one, k::zero, k::zero,
				k::zero, k::one, k::zero,
				k::zero, k::zero, -k::one
			);
		}

		static inline matrix_t rotate_x(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				k::one, k::zero, k::zero,
				k::zero, cosr, -sinr,
				k::zero, sinr, cosr
			);
		}

		static inline matrix_t rotate_y(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				cosr, k::zero, -sinr,
				k::zero, k::one, k::zero,
				sinr, k::zero, cosr
			);
		}

		static inline matrix_t rotate_z(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				cosr, -sinr, k::zero,
				sinr, cosr, k::zero,
				k::zero, k::zero, k::one
			);
		}


	};

	// getters
	template<typename E>
	constexpr vector_t<E, dimension::_2> row2(const matrix_t<E, dimension::_3, dimension::_3>& matrix, vector_index<dimension::_3> idx)
	{
		return vector_t<E, dimension::_2>(matrix.c[idx][0], matrix.c[idx][1]);
	}


	template<typename E>
	constexpr vector_t<E, dimension::_3> row(const matrix_t<E, dimension::_3, dimension::_3>& matrix, vector_index<dimension::_3> idx)
	{
		return vector_t<E, dimension::_3>(
			matrix.c[idx][0],
			matrix.c[idx][1],
			matrix.c[idx][2]
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> column2(const matrix_t<E, dimension::_3, dimension::_3>& matrix, vector_index<dimension::_3> idx)
	{
		return vector_t<E, dimension::_2>(matrix.c[0][idx], matrix.c[1][idx]);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_3> column(const matrix_t<E, dimension::_3, dimension::_3>& matrix, vector_index<dimension::_3> idx)
	{
		return vector_t<E, dimension::_3>(
			matrix.c[0][idx],
			matrix.c[1][idx],
			matrix.c[2][idx]
			);
	}

	// functions
	template<typename E>
	constexpr void set_column(matrix_t<E, dimension::_3, dimension::_3>& matrix, vector_index<dimension::_3> index, vector_t<E, dimension::_3> v)
	{
		matrix.c[0][index] = v.x;
		matrix.c[1][index] = v.y;
		matrix.c[2][index] = v.z;
	}

	template<typename E>
	constexpr bool is_orthogonal(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		vector_t<E, dimension::_3> r0 = row(matrix, 0);
		vector_t<E, dimension::_3> r1 = row(matrix, 1);
		vector_t<E, dimension::_3> r2 = row(matrix, 2);
		if (!is_equal(length_sqr(r0), constants<E>::one) ||
			!is_equal(length_sqr(r1), constants<E>::one) ||
			!is_equal(length_sqr(r2), constants<E>::one))
		{
			return false;
		}

		return is_equal(dot(r0, r1), constants<E>::zero)
			&& is_equal(dot(r1, r2), constants<E>::zero)
			&& is_equal(dot(r2, r0), constants<E>::zero);
	}

	template<typename E>
	constexpr void transpose(matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		swap(matrix.c[1][0], matrix.c[0][1]);
		swap(matrix.c[2][0], matrix.c[0][2]);
		swap(matrix.c[2][1], matrix.c[1][2]);
	}

	template<typename E>
	constexpr void inverse(matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		if (is_orthogonal(matrix))
		{
			transpose(matrix);
		}
		else
		{
			E det = determinant(matrix);
			if (!is_equal(det, constants<E>::zero))
			{
				det = constants<E>::one / det;
				matrix_t<E, dimension::_3, dimension::_3> old = matrix;

				//to-do det_impl is calculated above in determinant().
				//try to gcd
				matrix.c[0][0] = +matrix_impl::determinant2x2(old.c[1][1], old.c[1][2], old.c[2][1], old.c[2][2]) * det;
				matrix.c[1][0] = -matrix_impl::determinant2x2(old.c[1][0], old.c[1][2], old.c[2][0], old.c[2][2]) * det;
				matrix.c[2][0] = +matrix_impl::determinant2x2(old.c[1][0], old.c[1][1], old.c[2][0], old.c[2][1]) * det;

				matrix.c[0][1] = -matrix_impl::determinant2x2(old.c[0][1], old.c[0][2], old.c[2][1], old.c[2][2]) * det;
				matrix.c[1][1] = +matrix_impl::determinant2x2(old.c[0][0], old.c[0][2], old.c[2][0], old.c[2][2]) * det;
				matrix.c[2][1] = -matrix_impl::determinant2x2(old.c[0][0], old.c[0][1], old.c[2][0], old.c[2][1]) * det;

				matrix.c[0][2] = +matrix_impl::determinant2x2(old.c[0][1], old.c[0][2], old.c[1][1], old.c[1][2]) * det;
				matrix.c[1][2] = -matrix_impl::determinant2x2(old.c[0][0], old.c[0][2], old.c[1][0], old.c[1][2]) * det;
				matrix.c[2][2] = +matrix_impl::determinant2x2(old.c[0][0], old.c[0][1], old.c[1][0], old.c[1][1]) * det;
			}
			else
			{
				//TODO:
				// can not invert here,
				// what should i do when this occur ?
				matrix = matrix_t<E, dimension::_3, dimension::_3>::identity();
			}
		}
	}

	template<typename E>
	constexpr E determinant(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		return matrix_impl::determinant3x3
		(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2],
			matrix.c[1][0], matrix.c[1][1], matrix.c[2][2],
			matrix.c[2][0], matrix.c[2][1], matrix.c[2][2]
		);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_2, dimension::_2> get_rotation_matrix2x2(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_2, dimension::_2>(
			matrix.c[0][0], matrix.c[0][1],
			matrix.c[1][0], matrix.c[1][1]
			);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_2, dimension::_3> get_rotation_matrix2x3(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_2, dimension::_3>(
			matrix.c[0][0], matrix.c[0][1], constants<E>::zero,
			matrix.c[1][0], matrix.c[1][1], constants<E>::zero
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> get_translation(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		vector_t<E, dimension::_2> r_t0 = column2(matrix, 0);
		vector_t<E, dimension::_2> r_t1 = column2(matrix, 1);
		vector_t<E, dimension::_2> t = column2(matrix, 2);

		return vector_t<E, dimension::_2>(
			-dot(r_t0, t),
			-dot(r_t1, t)
			);
	}

	template<typename E>
	inline vector_t<E, dimension::_3> transform(const matrix_t<E, dimension::_3, dimension::_3>& lhs, const vector_t<E, dimension::_3>& rhs)
	{
		return lhs * rhs;
	}

	template<typename E>
	inline vector_t<E, dimension::_3> transform(const matrix_t<E, dimension::_3, dimension::_3>& lhs, const normalized_vector_t<E, dimension::_3>& rhs)
	{
		return lhs * rhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> transform(const matrix_t<E, dimension::_3, dimension::_3>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		vector_t<E, dimension::_3> rhs3(rhs, constants<E>::zero);
		return vector_t<E, dimension::_2>(
			dot(lhs.r[0], rhs3),
			dot(lhs.r[1], rhs3)
			);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> transform(const matrix_t<E, dimension::_3, dimension::_3>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs * rhs.value;
	}

	template<typename E>
	constexpr point_t<E, dimension::_2> transform(const matrix_t<E, dimension::_3, dimension::_3>& lhs, const point_t<E, dimension::_2>& rhs)
	{
		vector_t<E, dimension::_3> rhs3(rhs, constants<E>::one);
		return point_t<E, dimension::_2>(
			dot(lhs.r[0], rhs3),
			dot(lhs.r[1], rhs3)
			);
	}

	// Converter
	template<typename E>
	constexpr matrix_t<E, dimension::_2, dimension::_2> to_matrix2x2(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_2, dimension::_2>(
			matrix.c[0][0], matrix.c[0][1],
			matrix.c[1][0], matrix.c[1][1]
			);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_2, dimension::_3> to_matrix2x3(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_2, dimension::_3>(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2],
			matrix.c[1][0], matrix.c[1][1], matrix.c[1][2]
			);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_3, dimension::_3> to_matrix3x3(const matrix_t<E, dimension::_2, dimension::_2>& matrix)
	{
		return matrix_t<E, dimension::_3, dimension::_3>(
			matrix.c[0][0], matrix.c[0][1], constants<E>::zero,
			matrix.c[1][0], matrix.c[1][1], constants<E>::zero,
			constants<E>::zero, constants<E>::zero, constants<E>::one
			);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_3, dimension::_3> to_matrix3x3(const matrix_t<E, dimension::_2, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_3, dimension::_3>(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2],
			matrix.c[1][0], matrix.c[1][1], matrix.c[1][2],
			constants<E>::zero, constants<E>::zero, constants<E>::one
			);
	}
}