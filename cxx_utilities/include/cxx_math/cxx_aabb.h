#pragma once
#include "cxx_mathcommon.h"
#include "cxx_vector.h"
#include "cxx_point.h"
#include "cxx_matrix.h"

namespace cxx
{
	template<typename E, dimension Dimension>
	struct aabb_t;

	template<typename E> using aabb2d = aabb_t<E, dimension::_2>;
	template<typename E> using aabb3d = aabb_t<E, dimension::_3>;

	using aabb2d_i = aabb2d<int>;
	using aabb3d_i = aabb3d<int>;
	using aabb2d_f = aabb2d<float>;
	using aabb3d_f = aabb3d<float>;
	using aabb2d_d = aabb2d<double>;
	using aabb3d_d = aabb3d<double>;
	using rect = aabb2d_i;
	using cube = aabb3d_i;

	template<typename E, dimension D>
	struct aabb_t
	{
		using vector = vector_t<E, D>;
		using point = point_t<E, D>;

		constexpr aabb_t() = default;

		constexpr aabb_t(const point_t<E, D>& _min, const point_t<E, D>& _max);

		//properties
		constexpr const point& min_value() const { return m_min_value; }

		constexpr const point& max_value() const { return m_max_value; }

		constexpr const point center() const { return (m_min_value + m_max_value) / constants<E>::two; }

		constexpr const vector extend() const { return (m_max_value - m_min_value) / constants<E>::two; }

		constexpr bool is_valid() const { return valid; }

		inline bool is_empty() const
		{
			if (is_valid())
			{
				for (size_t i = 0, n = D; i < n; i++)
				{
					if (is_equal(m_min_value[i], m_max_value[i]))
					{
						return true;
					}
				}
			}
			return false;
		}

		//functions
		constexpr void set(const point& _min, const point& _max)
		{
			m_min_value = min_v(_min, _max);
			m_max_value = max_v(_min, _max);
			valid = true;
		}

		constexpr void expand(const vector& point)
		{
			if (is_valid())
			{
				m_min_value = min_v(m_min_value, point);
				m_max_value = max_v(m_max_value, point);
			}
			else
			{
				m_min_value = point;
				m_max_value = point;
				valid = true;
			}
		}

		constexpr void expand(const aabb_t& other)
		{
			if (other.is_valid())
			{
				if (is_valid())
				{
					m_min_value = min_v(m_min_value, other.m_min_value);
					m_max_value = max_v(m_max_value, other.m_max_value);
				}
				else
				{
					*this = other;
				}
			}
		}

		constexpr void move_to(const point& newCenter)
		{
			if (is_valid())
			{
				vector offset = newCenter - center();
				m_min_value += offset;
				m_max_value += offset;
			}
			else
			{
				m_min_value = newCenter;
				m_max_value = newCenter;
				valid = true;
			}
		}

		constexpr void set_extend(const vector& e)
		{
			if (is_valid())
			{
				vector c = center();
				m_min_value = c - e;
				m_max_value = c + e;
			}
			else
			{
				m_min_value = -e;
				m_max_value = e;
				valid = true;
			}
		}

		constexpr void clear()
		{
			m_min_value = m_max_value = point::zero();
			valid = false;
		}

		inline bool contains(const aabb_t& other) const
		{
			if (!is_valid() || !other.is_valid())
			{
				return false;
			}
			else
			{
				if (is_empty())
				{
					return other.is_empty();
				}
				else
				{
					for (int i = 0; i < D; i++)
					{
						if (m_min_value[i] > other.m_min_value[i] ||
							m_max_value[i] < other.m_max_value[i])
						{
							return false;
						}
					}
					return true;
				}
			}
		}

		inline bool contains(const point& point) const
		{
			if (is_valid())
			{
				for (int i = 0; i < D; i++)
				{
					if (m_min_value[i] > point[i] || m_max_value[i] < point[i])
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		intersection hit_test(const aabb_t& other) const
		{
			if (*this == other)
			{
				return intersection::same;
			}

			if (is_valid() && other.is_valid())
			{
				int minless = 0;
				int maxless = 0;
				for (int i = 0; i < D; i++)
				{
					if (m_min_value[i] < other.m_min_value[i]) minless++;
					if (m_max_value[i] < other.m_max_value[i]) maxless++;

					if (m_min_value[i] > other.m_max_value[i] ||
						m_max_value[i] < other.m_min_value[i])
					{
						return intersection::none;
					}
				}

				if (minless == D && maxless == 0)
				{
					return intersection::contain;
				}
				else if (minless == 0 && maxless == D)
				{
					return intersection::inside;
				}
				else
				{
					return intersection::hit;
				}
			}
			else
			{
				return intersection::none;
			}
		}

		//move to = set center.
		template<dimension T = D>
		constexpr std::enable_if_t<dimension::_2 == T> move_to(scaler_t<E> x, scaler_t<E> y)
		{
			move_to(vector(x, y));
		}

		template<dimension T = D>
		constexpr std::enable_if_t<dimension::_3 == T> move_to(scaler_t<E> x, scaler_t<E> y, scaler_t<E> z)
		{
			move_to(vector(x, y, z));
		}

		template<dimension T = D>
		constexpr std::enable_if_t<dimension::_2 == T> expand_to(scaler_t<E> x, scaler_t<E> y)
		{
			set_extend(vector(x, y));
		}

		template<dimension T = D>
		constexpr std::enable_if_t<dimension::_3 == T> expand_to(scaler_t<E> x, scaler_t<E> y, scaler_t<E> z)
		{
			set_extend(vector(x, y, z));
		}

	private:
		bool	valid = false;
		point	m_min_value;
		point	m_max_value;

	private:
		template<typename T, dimension DD>
		friend constexpr aabb_t<T, DD> operator*(const aabb_t<T, DD>& bounding, scaler_t<T> scaler);

		template<typename T, dimension DD>
		friend constexpr bool enlarge(aabb_t<T, DD>& bounding, const vector_t<T, DD>& scaler);

		template<typename T, dimension DD>
		friend constexpr bool move(aabb_t<T, DD>& bounding, const vector_t<T, DD>& offset);

		template<typename T, dimension DD>
		friend inline bool is_intersect(const aabb_t<T, DD>& lhs, const aabb_t<T, DD>& rhs);
	};

	//functions
	template<typename E, dimension D>
	constexpr bool operator==(const aabb_t<E, D>& lhs, const aabb_t<E, D>& rhs)
	{
		return (&lhs == &rhs)
			|| (!lhs.is_valid() && !rhs.is_valid())
			|| (lhs.is_valid() && rhs.is_valid() && lhs.min_value() == rhs.min_value() && lhs.max_value() == rhs.max_value());
	}

	template<typename E, dimension D>
	constexpr bool operator!=(const aabb_t<E, D>& lhs, const aabb_t<E, D>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename E, dimension D>
	constexpr bool move(aabb_t<E, D>& bounding, const vector_t<E, D>& offset)
	{
		if (bounding.is_valid())
		{
			bounding.m_min_value += offset;
			bounding.m_max_value += offset;
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename E, dimension D>
	constexpr aabb_t<E, D> operator*(const aabb_t<E, D>& bounding, scaler_t<E> scaler)
	{
		if (bounding.is_valid())
		{
			aabb_t<E, D> newBound;
			vector_t<E, D> e = bounding.extend() * scaler;
			vector_t<E, D> c = bounding.center();
			newBound.m_min_value = c - e;
			newBound.m_max_value = c + e;
			return newBound;
		}
		else
		{
			return bounding;
		}
	}

	template<typename E, dimension D>
	constexpr aabb_t<E, D> operator*(scaler_t<E> scaler, const aabb_t<E, D>& bounding)
	{
		return bounding * scaler;
	}

	template<typename E, dimension D>
	constexpr aabb_t<E, D>& operator*=(aabb_t<E, D>& bounding, scaler_t<E> scaler)
	{
		if (bounding.is_valid())
		{
			vector_t<E, D> e = bounding.extend() * scaler;
			vector_t<E, D> c = bounding.center();
			bounding.m_min_value = c - e;
			bounding.m_max_value = c + e;
		}
		return bounding;
	}
	template<typename E, dimension D>
	constexpr bool enlarge(aabb_t<E, D>& bounding, const vector_t<E, D>& scaler)
	{
		if (bounding.is_valid())
		{
			vector_t<E, D> e = bounding.extend() + scaler;
			vector_t<E, D> c = bounding.center();
			bounding.m_min_value = c - e;
			bounding.m_max_value = c + e;
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename E, dimension D>
	constexpr bool enlarge(aabb_t<E, D>& bounding, scaler_t<E> scaler)
	{
		return enlarge(bounding, vector(scaler));
	}

	template<typename E, dimension D>
	inline bool is_intersect(const aabb_t<E, D>& lhs, const aabb_t<E, D>& rhs)
	{
		if (&lhs == &rhs)
		{
			return false;
		}

		if (lhs.is_valid() && rhs.is_valid())
		{
			for (size_t i = 0; i < D; i++)
			{
				if (lhs.m_min_value[i] > rhs.m_max_value[i] ||
					lhs.m_max_value[i] < rhs.m_min_value[i])
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename E, dimension D>
	inline bool is_single_point(const aabb_t<E, D>& bounding)
	{
		return bounding.is_valid() && bounding.min_value() == bounding.max_value();
	}

	// implements
	template<typename E, dimension D>
	constexpr aabb_t<E, D>::aabb_t(const point_t<E, D>& _min, const point_t<E, D>& _max)
		: valid(true)
		, m_min_value(min_v(_min, _max))
		, m_max_value(max_v(_min, _max))
	{

	}

	// 2d ui staff operations
	template<typename E>
	constexpr bool move(aabb_t<E, dimension::_2>& bounding, scaler_t<E> x, scaler_t<E> y)
	{
		return move(bounding, vector_t<E, dimension::_2>(x, y));
	}

	template<typename E>
	constexpr bool move(aabb_t<E, dimension::_3>& bounding, scaler_t<E> x, scaler_t<E> y, scaler_t<E> z)
	{
		return move(bounding, vector_t<E, dimension::_3>(x, y, z));
	}

	template<typename E>
	constexpr bool enlarge(aabb_t<E, dimension::_2>& bounding, scaler_t<E> x, scaler_t<E> y)
	{
		return enlarge(bounding, vector_t<E, dimension::_2>(x, y));
	}

	template<typename E>
	constexpr bool enlarge(aabb_t<E, dimension::_3>& bounding, scaler_t<E> x, scaler_t<E> y, scaler_t<E> z)
	{
		return enlarge(bounding, vector_t<E, dimension::_3>(x, y, z));
	}

	// Transforms
	template<typename E>
	constexpr aabb2d<E> transform(const matrix_t<E, dimension::_2, dimension::_3>& matrix, const aabb2d<E>& box)
	{
		return aabb2d<E>(
			transform(matrix, box.min_value()),
			transform(matrix, box.max_value())
			);
	}

	template<typename E>
	constexpr aabb2d<E> transform(const matrix_t<E, dimension::_3, dimension::_3>& matrix, const aabb2d<E>& box)
	{
		return aabb2d<E>(
			transform(matrix, box.min_value()),
			transform(matrix, box.max_value())
			);
	}

	template<typename E>
	constexpr aabb3d<E> transform(const matrix_t<E, dimension::_4, dimension::_4>& matrix, const aabb3d<E>& box)
	{
		return aabb3d<E>(
			transform(matrix, box.min_value()),
			transform(matrix, box.max_value())
			);
	}

}