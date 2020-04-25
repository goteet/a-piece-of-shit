#pragma once
#include "cxx_mathcommon.h"
#include "cxx_radian.h"
#include "cxx_vector.h"

namespace cxx
{
#ifdef _DEBUG
	template<size_t Row, size_t Col>
	using matrix_index = index_t<(size_t)Row * (size_t)Col>;
#else
	template<size_t Row, size_t Col>
	using matrix_index = size_t;
#endif

	template<typename E, dimension Row, dimension Col>
	struct matrix_t;

	template<typename E> using matrix2x2 = matrix_t<E, dimension::_2, dimension::_2>;
	template<typename E> using matrix2x3 = matrix_t<E, dimension::_2, dimension::_3>;
	template<typename E> using matrix3x3 = matrix_t<E, dimension::_3, dimension::_3>;
	template<typename E> using matrix4x4 = matrix_t<E, dimension::_4, dimension::_4>;

	using float2x2 = matrix2x2<float>;
	using float2x3 = matrix2x3<float>;
	using float3x3 = matrix3x3<float>;
	using float4x4 = matrix4x4<float>;

	using double2x2 = matrix2x2<double>;
	using double2x3 = matrix2x3<double>;
	using double3x3 = matrix3x3<double>;
	using double4x4 = matrix4x4<double>;

	namespace matrix_impl
	{
		template<typename T>
		struct is_matrix : std::false_type { };

		template<typename T>
		struct is_square_matrix : std::false_type { };

		template<typename E, dimension Row, dimension Col>
		struct is_matrix<matrix_t<E, Row, Col>> : std::true_type { };

		template<typename E, dimension D>
		struct is_square_matrix<matrix_t<E, D, D>> : std::true_type { };

		template<typename E>
		constexpr E determinant2x2(
			E a00, E a01,
			E a10, E a11)
		{
			return a00 * a11 - a01 * a10;
		}

		template<typename E>
		constexpr E determinant2x3(
			E a00, E a01, E a02,
			E a10, E a11, E a12)
		{
			return a00 * a11 - a01 * a10;
		}

		template<typename E>
		constexpr E determinant3x3(
			E a00, E a01, E a02,
			E a10, E a11, E a12,
			E a20, E a21, E a22)
		{
			return a00 * determinant2x2(a11, a12, a21, a22)
				- a01 * determinant2x2(a10, a12, a20, a22)
				+ a02 * determinant2x2(a10, a11, a20, a21);
		}

		template<typename E>
		constexpr E determinant4x4(
			E a00, E a01, E a02, E a03,
			E a10, E a11, E a12, E a13,
			E a20, E a21, E a22, E a23,
			E a30, E a31, E a32, E a33)
		{
			return a00 * determinant3x3(a11, a12, a13, a21, a22, a23, a31, a32, a33)
				- a01 * determinant3x3(a10, a12, a13, a20, a22, a23, a30, a32, a33)
				+ a02 * determinant3x3(a10, a11, a13, a20, a21, a23, a30, a31, a33)
				- a03 * determinant3x3(a10, a11, a12, a20, a21, a22, a30, a31, a32);
		}
	}

	//  matrix operations
	template<typename E, dimension Row, dimension Col>
	constexpr bool operator== (
		const matrix_t<E, Row, Col>& lhs,
		const matrix_t<E, Row, Col>& rhs);

	template<typename E, dimension Row, dimension Col>
	constexpr bool operator!= (
		const matrix_t<E, Row, Col>& lhs,
		const matrix_t<E, Row, Col>& rhs);

	template<typename E, dimension RowL, dimension Mid, dimension ColR>
	inline matrix_t<E, RowL, ColR> operator* (
		const matrix_t<E, RowL, Mid>& lhs,
		const matrix_t<E, Mid, ColR>& rhs);

	template<typename E, dimension Row, dimension Col>
	constexpr const matrix_t<E, Row, Col> operator* (const matrix_t<E, Row, Col>& lhs, scaler_t<E> rhs);

	template<typename E, dimension Row, dimension Col>
	constexpr matrix_t<E, Row, Col> operator* (scaler_t<E> lhs, const matrix_t<E, Row, Col>& rhs);

	template<typename E, dimension Row, dimension Col>
	inline vector_t<E, Row> operator* (const matrix_t<E, Row, Col>& lhs, const vector_t<E, Col> & rhs);

	template<typename E, dimension Row, dimension Col>
	inline vector_t<E, Col> operator* (const vector_t<E, Row>& lhs, const matrix_t<E, Row, Col>& rhs);

	template<typename E, dimension Row, dimension Col>
	inline matrix_t<E, Row, Col>& operator*=(matrix_t<E, Row, Col>& lhs, const matrix_t<E, Row, Col>& rhs);

	template<typename E, dimension Row, dimension Col>
	inline matrix_t<E, Row, Col>& operator*=(matrix_t<E, Row, Col>& lhs, scaler_t<E> rhs);

	template<typename E, dimension D>
	constexpr bool can_invert(const matrix_t<E, D, D>& matrix);

	template<typename E, dimension D>
	constexpr matrix_t<E, D, D> transposed(const matrix_t<E, D, D>& matrix);

	template<typename E, dimension D>
	constexpr matrix_t<E, D, D> inversed(const matrix_t<E, D, D>& matrix);
}