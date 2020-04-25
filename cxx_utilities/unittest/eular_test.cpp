#include "stdafx.h"

TEST_CLASS(TestCXXMathTestEulerTGroup)
{
public:
	TEST_METHOD(cxx_math_euler_constrctor)
	{
		euler<float> a;
		CXX_IS_TRUE(radian<float>(0.0f) == a.yaw);
		CXX_IS_TRUE(radian<float>(0.0f) == a.roll);
		CXX_IS_TRUE(radian<float>(0.0f) == a.pitch);
	}

	TEST_METHOD(cxx_math_euler_comparision)
	{
		euler<float> a;
		euler<float> b(float3(constants<float>::pi / 3, constants<float>::pi / 3, constants<float>::pi / 3));

		CXX_IS_FALSE(a == b);
		CXX_IS_TRUE(a != b);

		a.roll = degree<float>(60);
		a.pitch = degree<float>(60);
		a.yaw = degree<float>(60);

		CXX_IS_TRUE(a == b);
		CXX_IS_FALSE(a != b);

		auto v3 = to_vector3(a);
		CXX_FEQUAL(constants<float>::pi / 3, v3.x);
		CXX_FEQUAL(constants<float>::pi / 3, v3.y);
		CXX_FEQUAL(constants<float>::pi / 3, v3.z);

	}
};