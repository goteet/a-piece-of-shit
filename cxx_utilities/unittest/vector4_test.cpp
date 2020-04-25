#include "stdafx.h"

TEST_CLASS(TestCXXMathVector4Group)
{
public:
	TEST_METHOD(cxx_math_float4_generic)
	{
		CXX_IEQUAL(sizeof(float) * 4, sizeof(float4));
	}

	float4 memberByList = { 1, 2, 3,4 };

	TEST_METHOD(cxx_math_float4_initialization)
	{
		CXX_FEQUALM(1.0f, memberByList.x, "member init by init-list");
		CXX_FEQUALM(2.0f, memberByList.y, "member init by init-list");
		CXX_FEQUALM(3.0f, memberByList.z, "member init by init-list");
		CXX_FEQUALM(4.0f, memberByList.w, "member init by init-list");

		float4 local = { 1,2,3,4 };
		CXX_FEQUALM(1.0f, local[0], "local init by init-list");
		CXX_FEQUALM(2.0f, local[1], "local init by init-list");
		CXX_FEQUALM(3.0f, local[2], "local init by init-list");
		CXX_FEQUALM(4.0f, local[3], "local init by init-list");

		float4 v4(0);
		CXX_FEQUAL(0.0f, v4.x);
		CXX_FEQUAL(0.0f, v4.y);
		CXX_FEQUAL(0.0f, v4.z);
		CXX_FEQUAL(0.0f, v4.w);
	}

	TEST_METHOD(cxx_math_float4_access)
	{
		float4 v = { 1, 2, 3,4 };
		CXX_FEQUAL(v.x, memberByList[0]);
		CXX_FEQUAL(v.y, memberByList[1]);
		CXX_FEQUAL(v.z, memberByList[2]);
		CXX_FEQUAL(v.w, memberByList[3]);
	}

	TEST_METHOD(cxx_math_float4_compare)
	{
		float4 v1(1.0f, 2.0f, 3.0f, 4.0f);
		float4 v2(1.0f, 2.0f, 3.0f, 4.0f);
		float4 v3(2.0f, 1.0f, 2.0f, 1.0f);

		// operator==
		CXX_IS_TRUE(v1 == v2);
		CXX_IS_FALSE(v1 == v3);

		// operator!=
		CXX_IS_FALSE(v1 != v2);
		CXX_IS_TRUE(v1 != v3);

		int2 a;
		a /= 1;
	}
};