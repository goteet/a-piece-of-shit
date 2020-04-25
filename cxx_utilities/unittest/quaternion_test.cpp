#include "stdafx.h"

TEST_CLASS(TestCXXMathTestQuaternionTGroup)
{
public:
	TEST_METHOD(cxx_math_quaternion_intialization)
	{
		quaternion<float> q;
		float3 left = rotate(q, -float3::unit_x());
		CXX_IS_TRUE(left == -float3::unit_x());
	}

	TEST_METHOD(cxx_math_quaternion_multiple)
	{
		quaternion<float> q1 = quaternion<float>(float3::unit_z(), degree<float>(90));
		quaternion<float> q2 = quaternion<float>(float3::unit_y(), degree<float>(90));

		auto r = q1 * q2;
		auto rt = rotate(r, float3(0, 0, 1));
		auto r2 = rotate(q2, float3(0, 0, 1));
		auto r1 = rotate(q1, r2);

		CXX_FEQUAL(1, r2.x);
		CXX_FEQUAL(0, r2.y);
		CXX_FEQUAL(0, r2.z);

		CXX_FEQUAL(0, r1.x);
		CXX_FEQUAL(1, r1.y);
		CXX_FEQUAL(0, r1.z);

		CXX_FEQUAL(r1.x, rt.x);
		CXX_FEQUAL(r1.y, rt.y);
		CXX_FEQUAL(r1.z, rt.z);
	}


	TEST_METHOD(cxx_math_quaternion_matrixconversion)
	{
		quaternion<float> q1 = quaternion<float>(float3::unit_y(), degree<float>(90));
		quaternion<float> q2 = quaternion<float>(float3::unit_z(), degree<float>(90));
		auto r = q1 * q2;

		auto matrixR = to_matrix4x4(r);

		auto rq = rotate(r, float3(0, 0, 1));
		auto rm = transform(matrixR, point3d<float>(0, 0, 1));
		CXX_IS_TRUE(rm == rq);

		auto rinv = to_quaternion(matrixR);
		CXX_IS_TRUE(r == rinv);

		rq = rotate(rinv, float3(0, 0, 1));
		CXX_IS_TRUE(rm == rq);
	}

	TEST_METHOD(cxx_math_quaternion_slerp)
	{
		quaternion<float> r1(float3::unit_x(), degree<float>(90));
		quaternion<float> r2(-float3::unit_x(), degree<float>(90));
		float3 position(0, 0, 1);

		//slerp
		constexpr int step = 50;
		for (int i = 0; i <= step; i++)
		{
			float k = (float)i / step;
			quaternion<float> r = slerp(r1, r2, k);
			position = rotate(r, float3(0, 0, 1));

			auto d = degree<float>(lerp(-90.0f, 90.0f, k));
			auto cosd = cos(d);
			auto sind = sin(d);

			CXX_IS_TRUE(float3(0, sind, cosd) == position);
		}
	}
};