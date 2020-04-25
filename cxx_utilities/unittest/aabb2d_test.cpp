#include "stdafx.h"

TEST_CLASS(TestCXXMathAABB2DTGroup)
{
public:
	TEST_METHOD(cxx_math_aabb2d_move)
	{
		aabb2d<float> bounding(float2(-2, -2), float2(2, 2));
		CXX_IS_TRUE(bounding.center() == float2::zero());

		move(bounding, 2, 2);
		CXX_IS_TRUE(bounding.center() == float2(2, 2));
		CXX_IS_TRUE(bounding.min() == float2(0, 0));
		CXX_IS_TRUE(bounding.max() == float2(4, 4));

		bounding.move_to({ 0, 0 });
		CXX_IS_TRUE(bounding.center() == float2(0, 0));
		CXX_IS_TRUE(bounding.min() == float2(-2, -2));
		CXX_IS_TRUE(bounding.max() == float2(2, 2));
	}

	TEST_METHOD(cxx_math_aabb2d_empty)
	{
		aabb2d<float> bounding(float2(-2, 0), float2(2, 0));
		CXX_IS_TRUE(bounding.is_empty());
	}

	TEST_METHOD(cxx_math_aabb2d_comparisontest)
	{
		aabb2d<float> empty1;
		aabb2d<float> empty2(float2(10, 0), float2(11, 0));

		CXX_IS_FALSE(empty1 == empty2);
		CXX_IS_TRUE(empty1 != empty2);
		CXX_IS_FALSE(!empty2.is_valid());
		CXX_IS_TRUE(empty2.is_empty());

		empty2.expand(float2(11, 1));
		empty1.expand(float2::zero());
		CXX_IS_TRUE(empty1 != empty2);
		CXX_IS_TRUE(empty1.is_empty());
		CXX_IS_FALSE(!empty1.is_valid());
		CXX_IS_FALSE(empty2.is_empty());

		empty1.clear();
		empty2.clear();
		CXX_IS_TRUE(empty1 == empty2);
		CXX_IS_TRUE(!empty1.is_valid());
		CXX_IS_TRUE(!empty1.is_valid());

		empty2 * 2;
	}

	TEST_METHOD(cxx_math_aabb2d_resizetest)
	{
		aabb2d<float> a;
		CXX_IS_TRUE(a.center() == float2::zero());
		CXX_IS_TRUE(a.extend() == float2::zero());
		CXX_IS_TRUE(a.min() == float2::zero());
		CXX_IS_TRUE(a.max() == float2::zero());

		a.expand(float2::one());

		CXX_IS_TRUE(a.center() == float2::one());
		CXX_IS_TRUE(a.extend() == float2::zero());
		CXX_IS_TRUE(a.min() == float2::one());
		CXX_IS_TRUE(a.max() == float2::one());

		aabb2d<float> b(float2(10, 0), float2(11, 0));
		CXX_IS_TRUE(b.center() == float2(10.5, 0));
		CXX_IS_TRUE(b.extend() == float2(0.5, 0));
		CXX_IS_TRUE(b.min() == float2(10, 0));
		CXX_IS_TRUE(b.max() == float2(11, 0));

		b.expand(float2(11, 1));
		CXX_IS_TRUE(b.center() == float2(10.5, 0.5));
		CXX_IS_TRUE(b.extend() == float2(0.5, 0.5));
		CXX_IS_TRUE(b.min() == float2(10, 0));
		CXX_IS_TRUE(b.max() == float2(11, 1));
	}

	TEST_METHOD(cxx_math_aabb2d_intersect)
	{
		const aabb2d<float> empty;
		aabb2d<float> b(float2(10, 0), float2(11, 0));

		CXX_IS_FALSE(empty.contains(b));
		CXX_IS_FALSE(empty.contains(float2::zero()));
		CXX_IS_TRUE(empty.hit_test(b) == intersection::none);

		CXX_IS_FALSE(b.contains(empty));
		CXX_IS_FALSE(b.contains(float2::zero()));
		CXX_IS_TRUE(b.contains(float2(10.5, 0)));
		CXX_IS_TRUE(b.hit_test(empty) == intersection::none);

		auto a = b;
		a.expand(float2(11, 1));
		CXX_IS_FALSE(empty.contains(a));
		CXX_IS_TRUE(empty.hit_test(a) == intersection::none);

		CXX_IS_FALSE(a.contains(empty));
		CXX_IS_FALSE(a.contains(float2::zero()));
		CXX_IS_TRUE(a.contains(float2(11, 0.5)));
		CXX_IS_TRUE(a.hit_test(empty) == intersection::none);
		CXX_IS_TRUE(a.hit_test(b) == intersection::hit);
	}
};