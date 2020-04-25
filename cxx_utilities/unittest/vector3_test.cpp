#include "stdafx.h"

TEST_CLASS(TestCXXMathVector3Group)
{
public:
	TEST_METHOD(cxx_math_float3_generic)
	{
		CXX_IEQUAL(sizeof(float) * 3, sizeof(float3));
	}

	float3 memberByStaticFunc = float3::one();
	float3 memberByList = { 1, 2, 3 };

	TEST_METHOD(cxx_math_float3_initialization)
	{
		CXX_FEQUALM(1.0f, memberByStaticFunc[0], "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc[1], "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc[2], "member init by static function");

		CXX_FEQUALM(1.0f, memberByList.x, "member init by init-list");
		CXX_FEQUALM(2.0f, memberByList.y, "member init by init-list");
		CXX_FEQUALM(3.0f, memberByList.z, "member init by init-list");

		float3 local = { 1,2,3 };
		CXX_FEQUALM(1.0f, local.x, "local init by init-list");
		CXX_FEQUALM(2.0f, local.y, "local init by init-list");
		CXX_FEQUALM(3.0f, local.z, "local init by init-list");

		// float3(float)
		float3 v1(0);
		CXX_FEQUAL(0.0f, v1.x);
		CXX_FEQUAL(0.0f, v1.y);
		CXX_FEQUAL(0.0f, v1.z);

		// float3(float, float, float)
		float3 v2(3.0f, 4.0f, 5.0f);
		CXX_FEQUAL(3.0f, v2.x);
		CXX_FEQUAL(4.0f, v2.y);
		CXX_FEQUAL(5.0f, v2.z);

		// float3(float3)
		auto v3(v2);
		CXX_FEQUAL(3.0f, v3.x);
		CXX_FEQUAL(4.0f, v3.y);
		CXX_FEQUAL(5.0f, v3.z);

		// float3 = float3
		auto v4 = v2;
		CXX_FEQUAL(3.0f, v4.x);
		CXX_FEQUAL(4.0f, v4.y);
		CXX_FEQUAL(5.0f, v4.z);
	}

	TEST_METHOD(cxx_math_float3_access)
	{
		float3 v = { 1, 2, 3 };
		CXX_FEQUAL(v.x, memberByList[0]);
		CXX_FEQUAL(v.y, memberByList[1]);
		CXX_FEQUAL(v.z, memberByList[2]);
	}

	TEST_METHOD(cxx_math_float3_constant)
	{
		// zero
		const auto zero = float3::zero();
		CXX_FEQUAL(0.0f, zero.x);
		CXX_FEQUAL(0.0f, zero.y);
		CXX_FEQUAL(0.0f, zero.z);

		// one
		const auto one = float3::one();
		CXX_FEQUAL(1.0f, one.x);
		CXX_FEQUAL(1.0f, one.y);
		CXX_FEQUAL(1.0f, one.z);


		// right
		const auto right = float3::unit_x();
		CXX_FEQUAL(1.0f, right.x);
		CXX_FEQUAL(0.0f, right.y);
		CXX_FEQUAL(0.0f, right.z);

		// up
		const auto up = float3::unit_y();
		CXX_FEQUAL(0.0f, up.x);
		CXX_FEQUAL(1.0f, up.y);
		CXX_FEQUAL(0.0f, up.z);

		// forward
		const auto forward = float3::unit_z();
		CXX_FEQUAL(0.0f, forward.x);
		CXX_FEQUAL(0.0f, forward.y);
		CXX_FEQUAL(1.0f, forward.z);
	}

	TEST_METHOD(cxx_math_float3_negative)
	{
		// operator-
		float3 v1(3.0f, 4.0f, 5.0f);
		auto v2 = -v1;
		CXX_FEQUAL(-3.0f, v2.x);
		CXX_FEQUAL(-4.0f, v2.y);
		CXX_FEQUAL(-5.0f, v2.z);

		auto v3 = -float3::zero();
		CXX_FEQUAL(0.0f, v3.x);
		CXX_FEQUAL(0.0f, v3.y);
		CXX_FEQUAL(0.0f, v3.z);
	}

	TEST_METHOD(cxx_math_float3_compare)
	{
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(1.0f, 2.0f, 3.0f);
		float3 v3(2.0f, 1.0f, 2.0f);

		// operator==
		CXX_IS_TRUE(v1 == v2);
		CXX_IS_FALSE(v1 == v3);

		// operator!=
		CXX_IS_FALSE(v1 != v2);
		CXX_IS_TRUE(v1 != v3);
	}

	TEST_METHOD(cxx_math_float3_add)
	{
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(2.0f, 3.0f, 4.0f);
		float f = 2.0f;
		
		// operator+(float3)
		auto b = v1 + v2;
		CXX_FEQUAL(3.0f, b.x);
		CXX_FEQUAL(5.0f, b.y);
		CXX_FEQUAL(7.0f, b.z);
		
		// operator+(float3)
		auto d = v1;
		d += v2;
		CXX_FEQUAL(3.0f, d.x);
		CXX_FEQUAL(5.0f, d.y);
		CXX_FEQUAL(7.0f, d.z);
	}

	TEST_METHOD(cxx_math_float3_minus)
	{
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(2.0f, 3.0f, 4.0f);
		float f = 2.0f;
		
		// operator-(float3)
		auto b = v1 - v2;
		CXX_FEQUAL(-1.0f, b.x);
		CXX_FEQUAL(-1.0f, b.y);
		CXX_FEQUAL(-1.0f, b.z);

		// operator-(float3)
		auto d = v1;
		d -= v2;
		CXX_FEQUAL(-1.0f, d.x);
		CXX_FEQUAL(-1.0f, d.y);
		CXX_FEQUAL(-1.0f, d.z);
	}

	TEST_METHOD(cxx_math_float3_multiply)
	{
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(2.0f, 3.0f, 4.0f);
		float f = 2.0f;

		// operator*(float)
		auto a = v1 * f;
		CXX_FEQUAL(2.0f, a.x);
		CXX_FEQUAL(4.0f, a.y);
		CXX_FEQUAL(6.0f, a.z);

		// operator*(float3)
		auto b = v1 * v2;
		CXX_FEQUAL(2.0f, b.x);
		CXX_FEQUAL(6.0f, b.y);
		CXX_FEQUAL(12.0f, b.z);

		// operator*=(float)
		auto c = v1;
		c *= f;
		CXX_FEQUAL(2.0f, c.x);
		CXX_FEQUAL(4.0f, c.y);
		CXX_FEQUAL(6.0f, c.z);

		// operator*(float3)
		auto d = v1;
		d *= v2;
		CXX_FEQUAL(2.0f, d.x);
		CXX_FEQUAL(6.0f, d.y);
		CXX_FEQUAL(12.0f, d.z);
	}

	TEST_METHOD(cxx_math_float3_index)
	{
		// operator[]
		float3 v1;
		v1[0] = 1.0f;
		v1[1] = 2.0f;
		v1[2] = 3.0f;
		CXX_FEQUAL(1.0f, v1.x);
		CXX_FEQUAL(2.0f, v1.y);
		CXX_FEQUAL(3.0f, v1.z);
	}

	TEST_METHOD(cxx_math_float3_topointer)
	{
		// float*
		float3 v1(1.0f, 2.0f, 3.0f);
		float *fv = v1.v;
		CXX_FEQUAL(1.0f, fv[0]);
		CXX_FEQUAL(2.0f, fv[1]);
		CXX_FEQUAL(3.0f, fv[2]);
	}
	
	// normalize
	// normalized
	TEST_METHOD(cxx_math_float3_normalize)
	{
		float3 v1(1.0f, 0.0f, 0.0f);
		auto a = normalized(v1);
		normalize(v1);
		CXX_FEQUAL(1.0f, v1.x);
		CXX_FEQUAL(0.0f, v1.y);
		CXX_FEQUAL(0.0f, v1.z);
		CXX_FEQUAL(1.0f, a.x);
		CXX_FEQUAL(0.0f, a.y);
		CXX_FEQUAL(0.0f, a.z);

		float3 v2(2.0f, 0.0f, 0.0f);
		auto b = normalized(v2);
		normalize(v2);
		CXX_FEQUAL(1.0f, v2.x);
		CXX_FEQUAL(0.0f, v2.y);
		CXX_FEQUAL(0.0f, v2.z);
		CXX_FEQUAL(1.0f, b.x);
		CXX_FEQUAL(0.0f, b.y);
		CXX_FEQUAL(0.0f, b.z);

		float3 v3(0.0f, 2.0f, 0.0f);
		auto c = normalized(v3);
		normalize(v3);
		CXX_FEQUAL(0.0f, v3.x);
		CXX_FEQUAL(1.0f, v3.y);
		CXX_FEQUAL(0.0f, v3.z);
		CXX_FEQUAL(0.0f, c.x);
		CXX_FEQUAL(1.0f, c.y);
		CXX_FEQUAL(0.0f, c.z);

		float3 v4(0.0f, 0.0f, 2.0f);
		auto d = normalized(v4);
		normalize(v4);
		CXX_FEQUAL(0.0f, v4.x);
		CXX_FEQUAL(0.0f, v4.y);
		CXX_FEQUAL(1.0f, v4.z);
		CXX_FEQUAL(0.0f, d.x);
		CXX_FEQUAL(0.0f, d.y);
		CXX_FEQUAL(1.0f, d.z);

		float3 v5(3.0f, 4.0f, 5.0f);
		auto e = normalized(v5);
		normalize(v5);
		CXX_FEQUAL(0.424264f, v5.x);
		CXX_FEQUAL(0.5656854f, v5.y);
		CXX_FEQUAL(0.7071067f, v5.z);
		CXX_FEQUAL(0.424264f,  e.x);
		CXX_FEQUAL(0.5656854f, e.y);
		CXX_FEQUAL(0.7071067f, e.z);
	}

	// inverse
	// inversed
	TEST_METHOD(cxx_math_float3_inverse)
	{
		float3 v1(1.0f, 0.0f, 0.0f);
		auto a = inversed(v1);
		inverse(v1);
		CXX_FEQUAL(1.0f, v1.x);
		CXX_FEQUAL(0.0f, v1.y);
		CXX_FEQUAL(0.0f, v1.z);
		CXX_FEQUAL(1.0f, a.x);
		CXX_FEQUAL(0.0f, a.y);
		CXX_FEQUAL(0.0f, a.z);

		float3 v2(2.0f, 0.0f, 0.0f);
		auto b = inversed(v2);
		inverse(v2);
		CXX_FEQUAL(0.5f, v2.x);
		CXX_FEQUAL(0.0f, v2.y);
		CXX_FEQUAL(0.0f, v2.z);
		CXX_FEQUAL(0.5f, b.x);
		CXX_FEQUAL(0.0f, b.y);
		CXX_FEQUAL(0.0f, b.z);

		float3 v3(0.0f, 2.0f, 0.0f);
		auto c = inversed(v3);
		inverse(v3);
		CXX_FEQUAL(0.0f, v3.x);
		CXX_FEQUAL(0.5f, v3.y);
		CXX_FEQUAL(0.0f, v3.z);
		CXX_FEQUAL(0.0f, c.x);
		CXX_FEQUAL(0.5f, c.y);
		CXX_FEQUAL(0.0f, c.z);

		float3 v4(0.0f, 0.0f, 2.0f);
		auto d = inversed(v4);
		inverse(v4);
		CXX_FEQUAL(0.0f, v4.x);
		CXX_FEQUAL(0.0f, v4.y);
		CXX_FEQUAL(0.5f, v4.z);
		CXX_FEQUAL(0.0f, d.x);
		CXX_FEQUAL(0.0f, d.y);
		CXX_FEQUAL(0.5f, d.z);

		float3 v5(3.0f, 4.0f, 5.0f);
		auto e = inversed(v5);
		inverse(v5);
		CXX_FEQUAL(0.06f, v5.x);
		CXX_FEQUAL(0.08f, v5.y);
		CXX_FEQUAL(0.1f, v5.z);
		CXX_FEQUAL(0.06f, e.x);
		CXX_FEQUAL(0.08f, e.y);
		CXX_FEQUAL(0.1f, e.z);
	}

	TEST_METHOD(cxx_math_float3_length)
	{
		float3 v1(2.0f, 0.0f, 0.0f);
		float3 v2(3.0f, 4.0f, 5.0f);

		// length
		CXX_FEQUAL(2.0f, length(v1));
		CXX_FEQUAL(7.0710678f, length(v2));

		// length_sqr
		CXX_FEQUAL(4.0f, length_sqr(v1));
		CXX_FEQUAL(50.0f, length_sqr(v2));
	}

	TEST_METHOD(cxx_math_float3_dot)
	{
		// dot
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(3.0f, 0.0f, 2.0f);

		CXX_FEQUAL(9.0f, dot(v1, v2));
	}

	TEST_METHOD(cxx_math_float3_cross)
	{
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(3.0f, 0.0f, 2.0f);

		// cross
		auto crossV = cross(v1, v2);

		CXX_FEQUAL(4.0f, crossV.x);
		CXX_FEQUAL(7.0f, crossV.y);
		CXX_FEQUAL(-6.0f, crossV.z);
	}

	TEST_METHOD(cxx_math_float3_combine)
	{
		float3 v1(1.0f, 2.0f, 3.0f);
		float3 v2(3.0f, 0.0f, 2.0f);

		// min_combine
		auto minV = min_v(v1, v2);
		CXX_FEQUAL(1.0f, minV.x);
		CXX_FEQUAL(0.0f, minV.y);
		CXX_FEQUAL(2.0f, minV.z);

		// max_combine
		auto maxV = max_v(v1, v2);
		CXX_FEQUAL(3.0f, maxV.x);
		CXX_FEQUAL(2.0f, maxV.y);
		CXX_FEQUAL(3.0f, maxV.z);
	}

	TEST_METHOD(cxx_math_float3_lerp)
	{
		const auto v1 = float3::one();
		const float3 v2(2.0f, 2.0f, 2.0f);

		auto lerpV = lerp(v1, v2, 0.5f);
		CXX_FEQUAL(1.5f, lerpV.x);
		CXX_FEQUAL(1.5f, lerpV.y);
		CXX_FEQUAL(1.5f, lerpV.z);

		lerpV = lerp(v1, v2, 0.0f);
		CXX_FEQUAL(1.0f, lerpV.x);
		CXX_FEQUAL(1.0f, lerpV.y);
		CXX_FEQUAL(1.0f, lerpV.z);

		lerpV = lerp(v1, v2, 1.0f);
		CXX_FEQUAL(2.0f, lerpV.x);
		CXX_FEQUAL(2.0f, lerpV.y);
		CXX_FEQUAL(2.0f, lerpV.z);
	}
};