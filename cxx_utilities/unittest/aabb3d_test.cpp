#include "stdafx.h"

TEST_CLASS(TestCXXMathAABB3DTGroup)
{
public:
	TEST_METHOD(cxx_math_aabbmove)
	{
		aabb3d<float> bounding(float3(-2, -2, 1), float3(2, 2, -1));
		CXX_IS_TRUE(bounding.center() == float3::zero());

		move(bounding, { 2, 2, 1 });
		CXX_IS_TRUE(bounding.center() == float3(2, 2, 1));
		CXX_IS_TRUE(bounding.min() == float3(0, 0, 0));
		CXX_IS_TRUE(bounding.max() == float3(4, 4, 2));

		bounding.move_to({ 0, 0, 0 });
		CXX_IS_TRUE(bounding.center() == float3(0, 0, 0));
		CXX_IS_TRUE(bounding.min() == float3(-2, -2, -1));
		CXX_IS_TRUE(bounding.max() == float3(2, 2, 1));
	}

	TEST_METHOD(cxx_math_aabbempty)
	{
		aabb3d<float> bounding(float3(-2, 5, -1), float3(2, -5, -1));
		CXX_IS_TRUE(bounding.is_empty());
	}

	TEST_METHOD(cxx_math_aabbcomparison)
	{
		aabb3d<float> empty1;
		aabb3d<float> empty2(float3(10, 0, -5), float3(11, 0, 3));

		CXX_IS_FALSE(empty1 == empty2);
		CXX_IS_TRUE(empty1 != empty2);
		CXX_IS_FALSE(!empty2.is_valid());
		CXX_IS_TRUE(empty2.is_empty());

		empty2.expand(float3(11, 1, 0));
		empty1.expand(float3::zero());
		CXX_IS_TRUE(empty1 != empty2);
		CXX_IS_TRUE(empty1.is_empty());
		CXX_IS_FALSE(!empty1.is_valid());
		CXX_IS_FALSE(empty2.is_empty());

		empty1.clear();
		empty2.clear();
		CXX_IS_TRUE(empty1 == empty2);
		CXX_IS_TRUE(!empty1.is_valid());
		CXX_IS_TRUE(!empty1.is_valid());
	}

	TEST_METHOD(cxx_math_aabbresize)
	{
		aabb3d<float> a;
		CXX_IS_TRUE(a.center() == float3::zero());
		CXX_IS_TRUE(a.extend() == float3::zero());
		CXX_IS_TRUE(a.min() == float3::zero());
		CXX_IS_TRUE(a.max() == float3::zero());

		a.expand(float3::one());

		CXX_IS_TRUE(a.center() == float3::one());
		CXX_IS_TRUE(a.extend() == float3::zero());
		CXX_IS_TRUE(a.min() == float3::one());
		CXX_IS_TRUE(a.max() == float3::one());

		aabb3d<float> b(float3(10, 0, -6), float3(11, 0, -5));
		CXX_IS_TRUE(b.center() == float3(10.5, 0, -5.5));
		CXX_IS_TRUE(b.extend() == float3(0.5, 0, 0.5));
		CXX_IS_TRUE(b.min() == float3(10, 0, -6));
		CXX_IS_TRUE(b.max() == float3(11, 0, -5));

		b.expand(float3(11, 1, 1));
		CXX_IS_TRUE(b.center() == float3(10.5, 0.5, -2.5));
		CXX_IS_TRUE(b.extend() == float3(0.5, 0.5, 3.5));
		CXX_IS_TRUE(b.min() == float3(10, 0, -6));
		CXX_IS_TRUE(b.max() == float3(11, 1, 1));
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