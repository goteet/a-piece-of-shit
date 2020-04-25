#pragma once
#include "cxx_base_matrix.h"

namespace cxx
{
	template<typename E>
	struct matrix_t<E, dimension::_2, dimension::_2>
	{
		using k = constants<E>;
		using mindex = matrix_index<dimension::_2, dimension::_2>;
		using vindex_r = vector_index<dimension::_2>;
		using vindex_c = vector_index<dimension::_2>;
		using vector_r = vector_t<E, dimension::_2>;
		using vector_c = vector_t<E, dimension::_2>;

		static constexpr dimension	Row = dimension::_2;
		static constexpr dimension	Col = dimension::_2;
		static constexpr size_t		Cell = Row * Col;

		union
		{
			char		m[Cell * sizeof(E)];
			E			v[Cell];
			E			c[Row][Col];
			vector_r	r[Row];
			struct
			{
				E	_00, _01,
					_10, _11;
			};
		};

		constexpr matrix_t() : c{
			 { k::one, k::zero }
			,{ k::zero, k::one } }
		{ 		}

		constexpr matrix_t(
			E _m00, E _m01,
			E _m10, E _m11) : c{
			 { _m00, _m01 }
			,{ _m10, _m11 } }
		{		}

		constexpr matrix_t(
			const vector_r& row0,
			const vector_r& row1) : c{
			 { row0.x, row0.y }
			,{ row1.x, row1.y } } { }

		constexpr matrix_t(const matrix_t& m22) : c{
			 { m22.c[0][0], m22.c[0][1] }
			,{ m22.c[1][0], m22.c[1][1] } }
		{		}

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
				k::one, k::zero,
				k::zero, k::one
			);
		}

		static constexpr matrix_t scale(E sx, E sy)
		{
			return matrix_t(
				sx, k::zero,
				k::zero, sy
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
				-k::one, k::zero,
				k::zero, k::one
			);
		}

		static constexpr matrix_t flip_y()
		{
			return matrix_t(
				k::one, k::zero,
				k::zero, -k::one
			);
		}

		static inline matrix_t rotate(const radian<E>& r)
		{
			E cosr = cos(r);
			E sinr = sin(r);
			return  matrix_t(
				cosr, -sinr,
				sinr, cosr
			);
		}
	};

	// globals template implements
	template<typename E, dimension Row, dimension Col>
	constexpr bool operator== (const matrix_t<E, Row, Col>& lhs, const matrix_t<E, Row, Col>& rhs)
	{
		for (size_t ri = 0; ri < Row; ri++)
		{
			for (size_t ci = 0; ci < Col; ci++)
			{
				if (!is_equal(lhs.c[ri][ci], rhs.c[ri][ci]))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<typename E, dimension Row, dimension Col>
	constexpr bool operator!= (const matrix_t<E, Row, Col>& lhs, const matrix_t<E, Row, Col>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename E, dimension RowL, dimension Mid, dimension ColR>
	inline matrix_t<E, RowL, ColR> operator* (
		const matrix_t<E, RowL, Mid>& lhs,
		const matrix_t<E, Mid, ColR>& rhs)
	{
		matrix_t<E, RowL, ColR> rst;
		for (size_t ri = 0; ri < RowL; ri++)
		{
			for (size_t ci = 0; ci < ColR; ci++)
			{
				rst.c[ri][ci] = dot(lhs.r[ri], column(rhs, ci));
			}
		}
		return rst;
	}

	template<typename E, dimension Row, dimension Col>
	constexpr const matrix_t<E, Row, Col> operator* (const matrix_t<E, Row, Col>& lhs, scaler_t<E> rhs)
	{
		matrix_t<E, Row, Col>  rst;
		for (size_t ri = 0; ri < Row; ri++)
		{
			for (size_t ci = 0; ci < Col; ci++)
			{
				rst.c[ri][ci] = lhs.c[ri][ci] * rhs;
			}
		}
		return rst;
	}

	template<typename E, dimension Row, dimension Col>
	constexpr matrix_t<E, Row, Col> operator* (scaler_t<E> lhs, const matrix_t<E, Row, Col>& rhs)
	{
		return rhs * lhs;
	}

	template<typename E, dimension Row, dimension Col>
	inline vector_t<E, Row> operator* (const matrix_t<E, Row, Col>& lhs, const vector_t<E, Col> & rhs)
	{
		vector_t<E, Row> rst;
		for (size_t ri = 0; ri < Row; ri++)
		{
			rst.v[ri] = dot(lhs.r[ri], rhs);
		}
		return rst;
	}

	template<typename E, dimension Row, dimension Col>
	inline vector_t<E, Row> operator* (const matrix_t<E, Row, Col>& lhs, const normalized_vector_t<E, Col> & rhs)
	{
		return lhs * rhs.value;
	}

	template<typename E, dimension Row, dimension Col>
	inline vector_t<E, Col> operator* (const vector_t<E, Row>& lhs, const matrix_t<E, Row, Col>& rhs)
	{
		vector_t<E, Col> rst;
		for (size_t ci = 0; ci < Col; ci++)
		{
			rst[ci] = dot(lhs, column(rhs, ci));
		}
		return rst;
	}

	template<typename E, dimension Row, dimension Col>
	inline vector_t<E, Col> operator* (const normalized_vector_t<E, Row>& lhs, const matrix_t<E, Row, Col>& rhs)
	{
		return lhs.value * rhs;
	}

	template<typename E, dimension Row, dimension Col>
	inline matrix_t<E, Row, Col>& operator*=(matrix_t<E, Row, Col>& lhs, const matrix_t<E, Row, Col>& rhs)
	{
		for (size_t ri = 0; ri < Row; ri++)
		{
			for (size_t ci = 0; ci < Col; ci++)
			{
				lhs.c[ri][ci] *= rhs.c[ri][ci];
			}
		}
		return lhs;
	}

	template<typename E, dimension Row, dimension Col>
	inline matrix_t<E, Row, Col>& operator*=(matrix_t<E, Row, Col>& lhs, scaler_t<E> rhs)
	{
		return lhs = lhs * rhs;
	}

	// functions
	template<typename E>
	constexpr vector_t<E, dimension::_2> row(const matrix_t<E, dimension::_2, dimension::_2>& matrix, vector_index<dimension::_2> idx)
	{
		return vector_t<E, dimension::_2>(matrix.c[idx][0], matrix.c[idx][1]);
	}

	template<typename E>
	constexpr vector_t<E, dimension::_2> column(const matrix_t<E, dimension::_2, dimension::_2>& matrix, vector_index<dimension::_2> idx)
	{
		return vector_t<E, dimension::_2>(matrix.c[0][idx], matrix.c[1][idx]);
	}

	template<typename E>
	constexpr void set_column(matrix_t<E, dimension::_2, dimension::_2>& matrix, vector_index<dimension::_2> idx, vector_t<E, dimension::_2> _v)
	{
		matrix.c[0][idx] = _v.x;
		matrix.c[1][idx] = _v.y;
	}

	template<typename E>
	constexpr bool is_orthogonal(const matrix_t<E, dimension::_2, dimension::_2>& matrix)
	{
		vector_t<E, dimension::_2> r0 = row(matrix, 0);
		vector_t<E, dimension::_2> r1 = row(matrix, 1);
		if (!is_equal(length_sqr(r0), constants<E>::one) ||
			!is_equal(length_sqr(r1), constants<E>::one))
		{
			return false;
		}

		return is_equal(dot(r0, r1), constants<E>::zero);
	}

	template<typename E>
	constexpr void transpose(matrix_t<E, dimension::_2, dimension::_2>& matrix)
	{
		swap(matrix.c[1][0], matrix.c[0][1]);
	}

	template<typename E>
	constexpr void inverse(matrix_t<E, dimension::_2, dimension::_2>& matrix)
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
				//calc adjoint matrix 
				swap(matrix.c[0][0], matrix.c[1][1]);
				matrix.c[0][1] = -matrix.c[0][1];
				matrix.c[1][0] = -matrix.c[1][0];
				matrix *= (constants<E>::one / det);
			}
			else
			{
				//TODO:
				// can not invert here,
				// what should i do when this occur ?
				matrix = matrix_t<E, dimension::_2, dimension::_2>::identity();
			}
		}
	}

	template<typename E>
	constexpr E determinant(const matrix_t<E, dimension::_2, dimension::_2>& matrix)
	{
		return matrix_impl::determinant2x2
		(
			matrix.c[0][0], matrix.c[0][1],
			matrix.c[1][0], matrix.c[1][1]
		);
	}

	template<typename E>
	inline vector_t<E, dimension::_2> transform(const matrix_t<E, dimension::_2, dimension::_2>& lhs, const vector_t<E, dimension::_2>& rhs)
	{
		return lhs * rhs;
	}

	template<typename E>
	inline vector_t<E, dimension::_2> transform(const matrix_t<E, dimension::_2, dimension::_2>& lhs, const normalized_vector_t<E, dimension::_2>& rhs)
	{
		return lhs * rhs;
	}

	template<typename E, dimension D>
	constexpr bool can_invert(const matrix_t<E, D, D>& matrix)
	{
		return  is_orthogonal(matrix)
			|| !is_equal(determinant(matrix), constants<E>::zero);
	}

	template<typename E, dimension D>
	constexpr matrix_t<E, D, D> transposed(const matrix_t<E, D, D>& matrix)
	{
		matrix_t<E, D, D> rst(matrix);
		transpose(rst);
		return rst;
	}

	template<typename E, dimension D>
	constexpr matrix_t<E, D, D> inversed(const matrix_t<E, D, D>& matrix)
	{
		matrix_t<E, D, D> rst(matrix);
		inverse(rst);
		return rst;
	}
}