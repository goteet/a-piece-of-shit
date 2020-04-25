#include "stdafx.h"


TEST_CLASS(TestCXXMathVector2Group)
{
public:
	TEST_METHOD(cxx_math_vector_2_memsize)
	{
		CXX_IEQUAL(sizeof(float) * 2, sizeof(float2));
		CXX_IEQUAL(sizeof(double) * 2, sizeof(double2));
		CXX_IEQUAL(sizeof(int) * 2, sizeof(int2));
		CXX_IEQUAL(sizeof(unsigned char) * 2, sizeof(byte2));
	}

	float2 memberByStaticFunc = float2::one();
	float2 memberByList = { 1, 2 };

	TEST_METHOD(cxx_math_vector_2_constructors)
	{
		CXX_FEQUALM(1.0f, memberByStaticFunc[0], "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc[1], "member init by static function");

		CXX_FEQUALM(1.0f, memberByList.x, "member init by init-list");
		CXX_FEQUALM(2.0f, memberByList.y, "member init by init-list");

		float2 local = { 1,2 };
		CXX_FEQUALM(1.0f, local.x, "local init by init-list");
		CXX_FEQUALM(2.0f, local.y, "local init by init-list");

		// float2(float)
		float2 v1(0.0f);
		CXX_FEQUAL(0.0f, v1.x);
		CXX_FEQUAL(0.0f, v1.y);

		// float2(float, float)
		float2 v2(3.0f, 4.0f);
		CXX_FEQUAL(3.0f, v2.x);
		CXX_FEQUAL(4.0f, v2.y);

		// float2(float2)
		auto v3(v2);
		CXX_FEQUAL(3.0f, v3.x);
		CXX_FEQUAL(4.0f, v3.y);

		// float2 = float2
		auto v4 = v2;
		CXX_FEQUAL(3.0f, v4.x);
		CXX_FEQUAL(4.0f, v4.y);
	}

	TEST_METHOD(cxx_math_float2_access)
	{
		float2 v = { 1, 2 };
		CXX_FEQUAL(v.x, memberByList[0]);
		CXX_FEQUAL(v.y, memberByList[1]);
	}

	TEST_METHOD(cxx_math_float2_constants)
	{
		// zero
		const auto zero = float2::zero();
		CXX_FEQUAL(0.0f, zero.x);
		CXX_FEQUAL(0.0f, zero.y);

		// one
		const auto one = float2::one();
		CXX_FEQUAL(1.0f, one.x);
		CXX_FEQUAL(1.0f, one.y);

		// right
		const auto right = float2::unit_x();
		CXX_FEQUAL(1.0f, right.x);
		CXX_FEQUAL(0.0f, right.y);

		// up
		const auto up = float2::unit_y();
		CXX_FEQUAL(0.0f, up.x);
		CXX_FEQUAL(1.0f, up.y);
	}

	TEST_METHOD(cxx_math_float2_negative)
	{
		// operator-
		float2 v1(3.0f, 4.0f);
		auto v2 = -v1;
		CXX_FEQUAL(-3.0f, v2.x);
		CXX_FEQUAL(-4.0f, v2.y);

		auto v3 = -float2::zero();
		CXX_FEQUAL(0.0f, v3.x);
		CXX_FEQUAL(0.0f, v3.y);
	}

	TEST_METHOD(cxx_math_float2_compare)
	{
		float2 v1(1.0f, 2.0f);
		float2 v2(1.0f, 2.0f);
		float2 v3(2.0f, 1.0f);

		// operator==
		CXX_IS_TRUE(v1 == v2);
		CXX_IS_FALSE(v1 == v3);

		// operator!=
		CXX_IS_FALSE(v1 != v2);
		CXX_IS_TRUE(v1 != v3);
	}

	TEST_METHOD(cxx_math_float2_add)
	{
		float2 v1(1.0f, 2.0f);
		float2 v2(2.0f, 3.0f);
		float f = 2.0f;
		
		// operator+(float2)
		auto b = v1 + v2;
		CXX_FEQUAL(3.0f, b.x);
		CXX_FEQUAL(5.0f, b.y);
		
		// operator+(float2)
		auto d = v1;
		d += v2;
		CXX_FEQUAL(3.0f, d.x);
		CXX_FEQUAL(5.0f, d.y);
	}

	TEST_METHOD(cxx_math_float2_minus)
	{
		float2 v1(1.0f, 2.0f);
		float2 v2(2.0f, 3.0f);
		float f = 2.0f;

		// operator-(float2)
		auto b = v1 - v2;
		CXX_FEQUAL(-1.0f, b.x);
		CXX_FEQUAL(-1.0f, b.y);
		
		// operator-(float2)
		auto d = v1;
		d -= v2;
		CXX_FEQUAL(-1.0f, d.x);
		CXX_FEQUAL(-1.0f, d.y);
	}

	TEST_METHOD(cxx_math_float2_multiply)
	{
		float2 v1(1.0f, 2.0f);
		float2 v2(2.0f, 3.0f);
		float f = 2.0f;

		// operator*(float)
		auto a = v1 * f;
		CXX_FEQUAL(2.0f, a.x);
		CXX_FEQUAL(4.0f, a.y);

		// operator*(float2)
		auto b = v1 * v2;
		CXX_FEQUAL(2.0f, b.x);
		CXX_FEQUAL(6.0f, b.y);

		// operator*=(float)
		auto c = v1;
		c *= f;
		CXX_FEQUAL(2.0f, c.x);
		CXX_FEQUAL(4.0f, c.y);

		// operator*(float2)
		auto d = v1;
		d *= v2;
		CXX_FEQUAL(2.0f, d.x);
		CXX_FEQUAL(6.0f, d.y);
	}

	TEST_METHOD(cxx_math_float2_index)
	{
		// operator[]
		float2 v1;
		v1[0] = 1.0f;
		v1[1] = 2.0f;
		CXX_FEQUAL(1.0f, v1.x);
		CXX_FEQUAL(2.0f, v1.y);
	}

	TEST_METHOD(cxx_math_float2_topointer)
	{
		// float*
		float2 v1(1.0f, 2.0f);
		float *fv = v1.v;
		CXX_FEQUAL(1.0f, fv[0]);
		CXX_FEQUAL(2.0f, fv[1]);
	}
	
	// normalize
	// normalized
	TEST_METHOD(cxx_math_float2_normalize)
	{
		float2 v1(1.0f, 0.0f);
		auto a = normalized(v1);
		normalize(v1);
		CXX_FEQUAL(1.0f, v1.x);
		CXX_FEQUAL(0.0f, v1.y);
		CXX_FEQUAL(1.0f, a.x);
		CXX_FEQUAL(0.0f, a.y);

		float2 v2(2.0f, 0.0f);
		auto b = normalized(v2);
		normalize(v2);
		CXX_FEQUAL(1.0f, v2.x);
		CXX_FEQUAL(0.0f, v2.y);
		CXX_FEQUAL(1.0f, b.x);
		CXX_FEQUAL(0.0f, b.y);

		float2 v3(0.0f, 2.0f);
		auto c = normalized(v3);
		normalize(v3);
		CXX_FEQUAL(0.0f, v3.x);
		CXX_FEQUAL(1.0f, v3.y);
		CXX_FEQUAL(0.0f, c.x);
		CXX_FEQUAL(1.0f, c.y);

		float2 v4(3.0f, 4.0f);
		auto d = normalized(v4);
		normalize(v4);
		CXX_FEQUAL(0.6f, v4.x);
		CXX_FEQUAL(0.8f, v4.y);
		CXX_FEQUAL(0.6f, d.x);
		CXX_FEQUAL(0.8f, d.y);
	}

	// inverse
	// inversed
	TEST_METHOD(cxx_math_float2_inverse)
	{
		float2 v1(1.0f, 0.0f);
		auto a = inversed(v1);
		inverse(v1);
		CXX_FEQUAL(1.0f, v1.x);
		CXX_FEQUAL(0.0f, v1.y);
		CXX_FEQUAL(1.0f, a.x);
		CXX_FEQUAL(0.0f, a.y);

		float2 v2(2.0f, 0.0f);
		auto b = inversed(v2);
		inverse(v2);
		CXX_FEQUAL(0.5f, v2.x);
		CXX_FEQUAL(0.0f, v2.y);
		CXX_FEQUAL(0.5f, b.x);
		CXX_FEQUAL(0.0f, b.y);

		float2 v3(0.0f, 2.0f);
		auto c = inversed(v3);
		inverse(v3);
		CXX_FEQUAL(0.0f, v3.x);
		CXX_FEQUAL(0.5f, v3.y);
		CXX_FEQUAL(0.0f, c.x);
		CXX_FEQUAL(0.5f, c.y);

		float2 v4(3.0f, 4.0f);
		auto d = inversed(v4);
		inverse(v4);
		CXX_FEQUAL(0.12f, v4.x);
		CXX_FEQUAL(0.16f, v4.y);
		CXX_FEQUAL(0.12f, d.x);
		CXX_FEQUAL(0.16f, d.y);
	}

	TEST_METHOD(cxx_math_float2_length)
	{
		float2 v1(2.0f, 0.0f);
		float2 v2(3.0f, 4.0f);

		// length
		CXX_FEQUAL(2.0f, length(v1));
		CXX_FEQUAL(5.0f, length(v2));

		// length_sqr
		CXX_FEQUAL(4.0f,  length_sqr(v1));
		CXX_FEQUAL(25.0f, length_sqr(v2));
	}

	TEST_METHOD(cxx_math_float2_dot)
	{
		// dot
		float2 v1(1.0f, 2.0f);
		float2 v2(3.0f, 0.0f);

		CXX_FEQUAL(3.0f, dot(v1, v2));
	}

	TEST_METHOD(cxx_math_float2_cross)
	{
		// cross
		float2 v1(1.0f, 2.0f);
		float2 v2(3.0f, 0.0f);

		CXX_FEQUAL(-6.0f, cross(v1, v2));
	}

	TEST_METHOD(cxx_math_float2_combine)
	{
		float2 v1(1.0f, 2.0f);
		float2 v2(3.0f, 0.0f);

		// min_combine
		auto minV = min_v(v1, v2);
		CXX_FEQUAL(1.0f, minV.x);
		CXX_FEQUAL(0.0f, minV.y);

		// max_combine
		auto maxV = max_v(v1, v2);
		CXX_FEQUAL(3.0f, maxV.x);
		CXX_FEQUAL(2.0f, maxV.y);
	}

	TEST_METHOD(cxx_math_float2_lerp)
	{
		const auto v1 = float2::one();
		const float2 v2(2.0f, 2.0f);

		auto lerpV = lerp(v1, v2, 0.5f);
		CXX_FEQUAL(1.5f, lerpV.x);
		CXX_FEQUAL(1.5f, lerpV.y);

		lerpV = lerp(v1, v2, 0.0f);
		CXX_FEQUAL(1.0f, lerpV.x);
		CXX_FEQUAL(1.0f, lerpV.y);

		lerpV = lerp(v1, v2, 1.0f);
		CXX_FEQUAL(2.0f, lerpV.x);
		CXX_FEQUAL(2.0f, lerpV.y);
	}
};
