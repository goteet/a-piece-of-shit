#pragma once
#include "cxx_point3.h"
#include "cxx_base_matrix.h"
#include "cxx_matrix2x2.h"
#include "cxx_matrix2x3.h"
#include "cxx_matrix3x3.h"
namespace cxx
{
	template<typename E>
	struct matrix_t<E, dimension::_4, dimension::_4>
	{
		using k = constants<E>;
		using mindex = matrix_index<dimension::_4, dimension::_4>;

		static constexpr dimension Row = dimension::_4;
		static constexpr dimension Col = dimension::_4;
		static constexpr size_t Cell = Row * Col;

		union
		{
			char	m[Cell * sizeof(E)];
			E		v[Cell];
			E		c[Row][Col];
			vector_t<E, dimension::_4> r[Row];
			struct
			{
				E	_00, _01, _02, _03,
					_10, _11, _12, _13,
					_20, _21, _22, _23,
					_30, _31, _32, _33;
			};
		};

		constexpr matrix_t() : c{
			 { k::one, k::zero, k::zero, k::zero }
			,{ k::zero, k::one, k::zero, k::zero }
			,{ k::zero, k::zero, k::one, k::zero }
			,{ k::zero, k::zero, k::zero, k::one } }
		{ }

		constexpr matrix_t(
			E _m00, E _m01, E _m02, E _m03,
			E _m10, E _m11, E _m12, E _m13,
			E _m20, E _m21, E _m22, E _m23,
			E _m30, E _m31, E _m32, E _m33) : c{
			 { _m00, _m01, _m02, _m03 }
			,{ _m10, _m11, _m12, _m13 }
			,{ _m20, _m21, _m22, _m23 }
			,{ _m30, _m31, _m32, _m33 } }
		{ }

		constexpr matrix_t(
			const vector_t<E, dimension::_4>& row0,
			const vector_t<E, dimension::_4>& row1,
			const vector_t<E, dimension::_4>& row2,
			const vector_t<E, dimension::_4>& row3) : c{
			 { row0.x, row0.y, row0.z, row0.w }
			,{ row1.x, row1.y, row1.z, row1.w }
			,{ row2.x, row2.y, row2.z, row2.w }
			,{ row3.x, row3.y, row3.z, row3.w } }
		{ }

		constexpr matrix_t(const matrix_t& m44) : c{
			 { m44.c[0][0], m44.c[0][1], m44.c[0][2], m44.c[0][3] }
			,{ m44.c[1][0], m44.c[1][1], m44.c[1][2], m44.c[1][3] }
			,{ m44.c[2][0], m44.c[2][1], m44.c[2][2], m44.c[2][3] }
			,{ m44.c[3][0], m44.c[3][1], m44.c[3][2], m44.c[3][3] } }
		{ }

		constexpr matrix_t(const matrix_t<E, dimension::_3, dimension::_3>& m33, const vector_t<E, dimension::_3>& translate) : c{
			 { m33.c[0][0], m33.c[0][1], m33.c[0][2], translate.x }
			,{ m33.c[1][0], m33.c[1][1], m33.c[1][2], translate.y }
			,{ m33.c[2][0], m33.c[2][1], m33.c[2][2], translate.z }
			,{ k::zero, k::zero, k::zero, k::one } }
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

		inline E& 			operator[] (mindex index) { return v[index]; }

		inline const E&		operator[] (mindex index) const { return v[index]; }

		// static utilities
		static constexpr matrix_t identity()
		{
			return matrix_t(
				k::one, k::zero, k::zero, k::zero,
				k::zero, k::one, k::zero, k::zero,
				k::zero, k::zero, k::one, k::zero,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t scale(E sx, E sy, E sz)
		{
			return matrix_t(
				sx, k::zero, k::zero, k::zero,
				k::zero, sy, k::zero, k::zero,
				k::zero, k::zero, sz, k::zero,
				k::zero, k::zero, k::zero, k::one
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
				-k::one, k::zero, k::zero, k::zero,
				k::zero, k::one, k::zero, k::zero,
				k::zero, k::zero, k::one, k::zero,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t flip_y()
		{
			return matrix_t(
				k::one, k::zero, k::zero, k::zero,
				k::zero, -k::one, k::zero, k::zero,
				k::zero, k::zero, k::one, k::zero,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t flip_z()
		{
			return matrix_t(
				k::one, k::zero, k::zero, k::zero,
				k::zero, k::one, k::zero, k::zero,
				k::zero, k::zero, -k::one, k::zero,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t translate(E tx, E ty, E tz)
		{
			return matrix_t(
				k::one, k::zero, k::zero, tx,
				k::zero, k::one, k::zero, ty,
				k::zero, k::zero, k::one, tz,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static constexpr matrix_t translate(const vector_t<E, dimension::_3>& t)
		{
			return translate(t.x, t.y, t.z);
		}

		static inline matrix_t rotate_x(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				k::one, k::zero, k::zero, k::zero,
				k::zero, cosr, -sinr, k::zero,
				k::zero, sinr, cosr, k::zero,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static inline matrix_t rotate_y(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				cosr, k::zero, -sinr, k::zero,
				k::zero, k::one, k::zero, k::zero,
				sinr, k::zero, cosr, k::zero,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static inline matrix_t rotate_z(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);

			return matrix_t(
				cosr, -sinr, k::zero, k::zero,
				sinr, cosr, k::zero, k::zero,
				k::zero, k::zero, k::one, k::zero,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static inline matrix_t look_at(
			const vector_t<E, dimension::_3>& eye,
			const vector_t<E, dimension::_3>& look,
			const vector_t<E, dimension::_3>& up)
		{
			normalized_vector_t<E, dimension::_3> forward = normalized(look - eye);
			normalized_vector_t<E, dimension::_3> real_up = normalized(up - forward * dot(forward, up));
			normalized_vector_t<E, dimension::_3> right = cross(real_up, forward);
			return matrix_t(
				vector_r(right, -dot(eye, right)),
				vector_r(real_up, -dot(eye, real_up)),
				vector_r(forward, -dot(eye, forward)),
				vector_r(k::zero, k::zero, k::zero, k::one)
			);
		}

		static inline matrix_t perspective_lh(const radian<E>& fov, E aspect, E znear, E zfar)
		{
			E near_top = tan(fov * k::half);
			E near_right = near_top * aspect;
			E znear2 = k::two * znear;
			E z_range_inv = zfar / (zfar - znear);
			return matrix_t(
				znear2 / near_right, k::zero, k::zero, k::zero,
				k::zero, znear2 / near_top, k::zero, k::zero,
				k::zero, k::zero, z_range_inv, -znear * z_range_inv,
				k::zero, k::zero, k::one, k::zero
			);
		}

		static inline matrix_t perspective_lh(const radian<E>& fov, E aspect, const vector2<E>& zpair)
		{
			perspective_lh(fov, aspect, zpair.x, zpair.y);
		}

		static inline matrix_t center_ortho_lh(E width, E height, E znear, E zfar)
		{
			E z_range_inv = k::one / (zfar - znear);
			return matrix_t(
				k::two / width, k::zero, k::zero, k::zero,
				k::zero, -k::two / height, k::zero, k::zero,
				k::zero, k::zero, z_range_inv, -znear * z_range_inv,
				k::zero, k::zero, k::zero, k::one
			);
		}
		static inline matrix_t center_ortho_lh(const vector2<E>& size, const vector2<E>& zpair)
		{
			center_ortho_lh(size.x, size.y, zpair.x, zpair.y);
		}

		static inline matrix_t ortho2d_lh(E width, E height, E znear, E zfar)
		{
			E z_range_inv = k::one / (zfar - znear);
			return matrix_t(
				k::two / width, k::zero, k::zero, -k::one,
				k::zero, -k::two / height, k::zero, k::one,
				k::zero, k::zero, z_range_inv, -znear * z_range_inv,
				k::zero, k::zero, k::zero, k::one
			);
		}

		static inline matrix_t ortho2d_lh(const vector2<E>& size, const vector2<E>& zpair)
		{
			ortho2d_lh(size.x, size.y, zpair.x, zpair.y);
		}
	};


	// getters
	template<typename E> 
	constexpr vector_t<E, dimension::_3> row3(const matrix_t<E, dimension::_4, dimension::_4>& matrix, vector_index<dimension::_4> index)
	{
		return vector_t<E, dimension::_3>(
			matrix.c[index][0],
			matrix.c[index][1],
			matrix.c[index][2]
			);
	}

	template<typename E> 
	constexpr vector_t<E, dimension::_4> row(const matrix_t<E, dimension::_4, dimension::_4>& matrix, vector_index<dimension::_4> index)
	{
		return vector_t<E, dimension::_4>(
			matrix.c[index][0],
			matrix.c[index][1],
			matrix.c[index][2],
			matrix.c[index][3]
		);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_3> column3(const matrix_t<E, dimension::_4, dimension::_4>& matrix, vector_index<dimension::_4> index)
	{
		return vector_t<E, dimension::_3>(
			matrix.c[0][index],
			matrix.c[1][index],
			matrix.c[2][index]
			);
	}

	template<typename E> 
	constexpr vector_t<E, dimension::_4> column(const matrix_t<E, dimension::_4, dimension::_4>& matrix, vector_index<dimension::_4> index)
	{
		return vector_c(
			matrix.c[0][index],
			matrix.c[1][index],
			matrix.c[2][index],
			matrix.c[3][index]
		);
	}


	// funtions
	template<typename E>
	constexpr void set_column(matrix_t<E, dimension::_4, dimension::_4>& matrix, vector_index<dimension::_4> index, vector_t<E, dimension::_4> v)
	{
		matrix.c[0][index] = v.x;
		matrix.c[1][index] = v.y;
		matrix.c[2][index] = v.z;
		matrix.c[3][index] = v.w;
	}

	template<typename E>
	constexpr bool is_orthogonal(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		vector_t<E, dimension::_4> r0 = row(matrix, 0);
		vector_t<E, dimension::_4> r1 = row(matrix, 1);
		vector_t<E, dimension::_4> r2 = row(matrix, 2);
		vector_t<E, dimension::_4> r3 = row(matrix, 3);
		if (!is_equal(length_sqr(r0), constants<E>::one) ||
			!is_equal(length_sqr(r1), constants<E>::one) ||
			!is_equal(length_sqr(r2), constants<E>::one) ||
			!is_equal(length_sqr(r3), constants<E>::one))
		{
			return false;
		}

		return is_equal(dot(r0, r1), constants<E>::zero)
			&& is_equal(dot(r0, r2), constants<E>::zero)
			&& is_equal(dot(r0, r3), constants<E>::zero)
			&& is_equal(dot(r1, r2), constants<E>::zero)
			&& is_equal(dot(r1, r3), constants<E>::zero)
			&& is_equal(dot(r2, r3), constants<E>::zero);
	}

	template<typename E> 
	constexpr void transpose(matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		swap(matrix.c[1][0], matrix.c[0][1]);
		swap(matrix.c[2][0], matrix.c[0][2]);
		swap(matrix.c[3][0], matrix.c[0][3]);
		swap(matrix.c[2][1], matrix.c[1][2]);
		swap(matrix.c[3][1], matrix.c[1][3]);
		swap(matrix.c[3][2], matrix.c[2][3]);
	}

	template<typename E> 
	constexpr void inverse(matrix_t<E, dimension::_4, dimension::_4>& matrix)
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
				matrix_t<E, dimension::_4, dimension::_4> old = matrix;

				//to-do det_impl is calculated above in determinant().
				//try to gcd
				matrix.c[0][0] = +determinant3x3(old.c[1][1], old.c[1][2], old.c[1][3], old.c[2][1], old.c[2][2], old.c[2][3], old.c[3][1], old.c[3][2], old.c[3][3]) * det;
				matrix.c[1][0] = -determinant3x3(old.c[1][0], old.c[1][2], old.c[1][3], old.c[2][0], old.c[2][2], old.c[2][3], old.c[3][0], old.c[3][2], old.c[3][3]) * det;
				matrix.c[2][0] = +determinant3x3(old.c[1][0], old.c[1][1], old.c[1][3], old.c[2][0], old.c[2][1], old.c[2][3], old.c[3][0], old.c[3][1], old.c[3][3]) * det;
				matrix.c[3][0] = -determinant3x3(old.c[1][0], old.c[1][1], old.c[1][2], old.c[2][0], old.c[2][1], old.c[2][2], old.c[3][0], old.c[3][1], old.c[3][2]) * det;

				matrix.c[0][1] = -determinant3x3(old.c[0][1], old.c[0][2], old.c[0][3], old.c[2][1], old.c[2][2], old.c[2][3], old.c[3][1], old.c[3][2], old.c[3][3]) * det;
				matrix.c[1][1] = +determinant3x3(old.c[0][0], old.c[0][2], old.c[0][3], old.c[2][0], old.c[2][2], old.c[2][3], old.c[3][0], old.c[3][2], old.c[3][3]) * det;
				matrix.c[2][1] = -determinant3x3(old.c[0][0], old.c[0][1], old.c[0][3], old.c[2][0], old.c[2][1], old.c[2][3], old.c[3][0], old.c[3][1], old.c[3][3]) * det;
				matrix.c[3][1] = +determinant3x3(old.c[0][0], old.c[0][1], old.c[0][2], old.c[2][0], old.c[2][1], old.c[2][2], old.c[3][0], old.c[3][1], old.c[3][2]) * det;

				matrix.c[0][2] = +determinant3x3(old.c[0][1], old.c[0][2], old.c[0][3], old.c[1][1], old.c[1][2], old.c[1][3], old.c[3][1], old.c[3][2], old.c[3][3]) * det;
				matrix.c[1][2] = -determinant3x3(old.c[0][0], old.c[0][2], old.c[0][3], old.c[1][0], old.c[1][2], old.c[1][3], old.c[3][0], old.c[3][2], old.c[3][3]) * det;
				matrix.c[2][2] = +determinant3x3(old.c[0][0], old.c[0][1], old.c[0][3], old.c[1][0], old.c[1][1], old.c[1][3], old.c[3][0], old.c[3][1], old.c[3][3]) * det;
				matrix.c[3][2] = -determinant3x3(old.c[0][0], old.c[0][1], old.c[0][2], old.c[1][0], old.c[1][1], old.c[1][2], old.c[3][0], old.c[3][1], old.c[3][2]) * det;

				matrix.c[0][3] = -determinant3x3(old.c[0][1], old.c[0][2], old.c[0][3], old.c[1][1], old.c[1][2], old.c[1][3], old.c[2][1], old.c[2][2], old.c[2][3]) * det;
				matrix.c[1][3] = +determinant3x3(old.c[0][0], old.c[0][2], old.c[0][3], old.c[1][0], old.c[1][2], old.c[1][3], old.c[2][0], old.c[2][2], old.c[2][3]) * det;
				matrix.c[2][3] = -determinant3x3(old.c[0][0], old.c[0][1], old.c[0][3], old.c[1][0], old.c[1][1], old.c[1][3], old.c[2][0], old.c[2][1], old.c[2][3]) * det;
				matrix.c[3][3] = +determinant3x3(old.c[0][0], old.c[0][1], old.c[0][2], old.c[1][0], old.c[1][1], old.c[1][2], old.c[2][0], old.c[2][1], old.c[2][2]) * det;
			}
			else
			{
				//TODO:
				// can not invert here,
				// what should i do when this occur ?
				matrix = matrix_t<E, dimension::_4, dimension::_4>::identity();
			}
		}
	}

	template<typename E>
	constexpr E determinant(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		return matrix_impl::determinant4x4
		(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2], matrix.c[0][3],
			matrix.c[1][0], matrix.c[1][1], matrix.c[1][2], matrix.c[1][3],
			matrix.c[2][0], matrix.c[2][1], matrix.c[2][2], matrix.c[2][3],
			matrix.c[3][0], matrix.c[3][1], matrix.c[3][2], matrix.c[3][3]
		);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_3, dimension::_3> get_rotation_matrix3x3(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		return matrix_t<E, dimension::_3, dimension::_3>(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2],
			matrix.c[1][0], matrix.c[1][1], matrix.c[1][2],
			matrix.c[2][0], matrix.c[2][1], matrix.c[2][2]
			);
	}

	template<typename E> 
	constexpr matrix_t<E, dimension::_4, dimension::_4> get_rotation_matrix4x4(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		return matrix_t<E, dimension::_4, dimension::_4>(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2], constants<E>::zero,
			matrix.c[1][0], matrix.c[1][1], matrix.c[1][2], constants<E>::zero,
			matrix.c[2][0], matrix.c[2][1], matrix.c[2][2], constants<E>::zero,
			constants<E>::zero, constants<E>::zero, constants<E>::zero, constants<E>::one
		);
	}

	template<typename E> 
	constexpr vector_t<E, dimension::_3> get_translation(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		vector_t<E, dimension::_3> r_t0 = column3(matrix, 0);
		vector_t<E, dimension::_3> r_t1 = column3(matrix, 1);
		vector_t<E, dimension::_3> r_t2 = column3(matrix, 2);
		vector_t<E, dimension::_3> t = column3(matrix, 3);

		return vector_t<E, dimension::_3>(
			-dot(r_t0, t),
			-dot(r_t1, t),
			-dot(r_t2, t)
			);
	}

	template<typename E> 
	inline vector_t<E, dimension::_4> transform(const matrix_t<E, dimension::_4, dimension::_4>& lhs, const vector_t<E, dimension::_4>& rhs)
	{
		return lhs * rhs;
	}

	template<typename E>
	constexpr vector_t<E, dimension::_3> transform(const matrix_t<E, dimension::_4, dimension::_4>& lhs, const vector_t<E, dimension::_3>& rhs)
	{
		vector_t<E, dimension::_4> rhs4(rhs, constants<E>::zero);
		return vector_t<E, dimension::_3>(
			dot(lhs.r[0], rhs4),
			dot(lhs.r[1], rhs4),
			dot(lhs.r[2], rhs4)
			);
	}

	template<typename E> 
	constexpr point_t<E, dimension::_3> transform(const matrix_t<E, dimension::_4, dimension::_4>& lhs, const point_t<E, dimension::_3>& rhs)
	{
		vector_t<E, dimension::_4> rhs4(rhs, constants<E>::one);
		return point_t<E, dimension::_3>(
			dot(lhs.r[0], rhs4),
			dot(lhs.r[1], rhs4),
			dot(lhs.r[2], rhs4)
			);
	}

	// Converter
	template<typename E> 
	constexpr matrix_t<E, dimension::_2, dimension::_2> to_matrix2x2(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		return  matrix_t<E, dimension::_2, dimension::_2>(
			matrix.c[0][0], matrix.c[0][1],
			matrix.c[1][0], matrix.c[1][1]
			);
	}

	template<typename E> 
	constexpr matrix_t<E, dimension::_3, dimension::_3> to_matrix3x3(const matrix_t<E, dimension::_4, dimension::_4>& matrix)
	{
		return matrix_t<E, dimension::_3, dimension::_3>(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2],
			matrix.c[1][0], matrix.c[1][1], matrix.c[1][2],
			matrix.c[2][0], matrix.c[2][1], matrix.c[2][2]
			);
	}

	template<typename E>
	constexpr matrix_t<E, dimension::_4, dimension::_4> to_matrix4x4(const matrix_t<E, dimension::_3, dimension::_3>& matrix)
	{
		return matrix_t<E, dimension::_4, dimension::_4>(
			matrix.c[0][0], matrix.c[0][1], matrix.c[0][2], constants<E>::zero,
			matrix.c[1][0], matrix.c[1][1], matrix.c[1][2], constants<E>::zero,
			matrix.c[2][0], matrix.c[2][1], matrix.c[2][2], constants<E>::zero,
			constants<E>::zero, constants<E>::zero, constants<E>::zero, constants<E>::one
			);
	}
}