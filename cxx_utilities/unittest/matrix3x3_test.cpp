#include "stdafx.h"

TEST_CLASS(TestCXXMathTestMatrix3x3TGroup)
{
public:
	TEST_METHOD(cxx_math_matrix3x3_generic)
	{
		CXX_IEQUAL(sizeof(float) * 3 * 3, sizeof(float3x3));
	}

	float3x3 memberByStaticFunc = float3x3::identity();
	float3x3 memberByList = { 1,2,3,4,5,6,7,8,9 };

	TEST_METHOD(cxx_math_matrix3x3_intialization)
	{
		CXX_FEQUALM(1.0f, memberByStaticFunc._00, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._01, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._02, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._10, "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc._11, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._12, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._20, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._21, "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc._22, "member init by static function");

		CXX_FEQUALM(1.0f, memberByList._00, "member init by init-list");
		CXX_FEQUALM(2.0f, memberByList._01, "member init by init-list");
		CXX_FEQUALM(3.0f, memberByList._02, "member init by init-list");
		CXX_FEQUALM(4.0f, memberByList._10, "member init by init-list");
		CXX_FEQUALM(5.0f, memberByList._11, "member init by init-list");
		CXX_FEQUALM(6.0f, memberByList._12, "member init by init-list");
		CXX_FEQUALM(7.0f, memberByList._20, "member init by init-list");
		CXX_FEQUALM(8.0f, memberByList._21, "member init by init-list");
		CXX_FEQUALM(9.0f, memberByList._22, "member init by init-list");

		float3x3 local = { 1,2,3,4,5,6,7,8,9 };
		CXX_FEQUALM(1.0f, local._00, "local init by init-list");
		CXX_FEQUALM(2.0f, local._01, "local init by init-list");
		CXX_FEQUALM(3.0f, local._02, "local init by init-list");
		CXX_FEQUALM(4.0f, local._10, "local init by init-list");
		CXX_FEQUALM(5.0f, local._11, "local init by init-list");
		CXX_FEQUALM(6.0f, local._12, "local init by init-list");
		CXX_FEQUALM(7.0f, local._20, "local init by init-list");
		CXX_FEQUALM(8.0f, local._21, "local init by init-list");
		CXX_FEQUALM(9.0f, local._22, "local init by init-list");

		float3x3 m322(float2x2(1, 2, 3, 4), float2(0));
		CXX_FEQUAL(1.0f, m322._00);
		CXX_FEQUAL(2.0f, m322._01);
		CXX_FEQUAL(0.0f, m322._02);
		CXX_FEQUAL(3.0f, m322._10);
		CXX_FEQUAL(4.0f, m322._11);
		CXX_FEQUAL(0.0f, m322._12);
		CXX_FEQUAL(0.0f, m322._20);
		CXX_FEQUAL(0.0f, m322._21);
		CXX_FEQUAL(1.0f, m322._22);

		float3x3 m332 = to_matrix3x3(float2x3(1, 2, 3, 4, 5, 6));
		CXX_FEQUAL(1.0f, m332._00);
		CXX_FEQUAL(2.0f, m332._01);
		CXX_FEQUAL(3.0f, m332._02);
		CXX_FEQUAL(4.0f, m332._10);
		CXX_FEQUAL(5.0f, m332._11);
		CXX_FEQUAL(6.0f, m332._12);
		CXX_FEQUAL(0.0f, m332._20);
		CXX_FEQUAL(0.0f, m332._21);
		CXX_FEQUAL(1.0f, m332._22);

	}

	TEST_METHOD(cxx_math_matrix3x3_access)
	{
		float3x3 v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		CXX_FEQUAL(v._00, v[0]);
		CXX_FEQUAL(v._01, v[1]);
		CXX_FEQUAL(v._02, v[2]);
		CXX_FEQUAL(v._10, v[3]);
		CXX_FEQUAL(v._11, v[4]);
		CXX_FEQUAL(v._12, v[5]);
		CXX_FEQUAL(v._20, v[6]);
		CXX_FEQUAL(v._21, v[7]);
		CXX_FEQUAL(v._22, v[8]);

		CXX_FEQUAL(v._00, v.r[0].x);
		CXX_FEQUAL(v._01, v.r[0].y);
		CXX_FEQUAL(v._02, v.r[0].z);
		CXX_FEQUAL(v._10, v.r[1].x);
		CXX_FEQUAL(v._11, v.r[1].y);
		CXX_FEQUAL(v._12, v.r[1].z);
		CXX_FEQUAL(v._20, v.r[2].x);
		CXX_FEQUAL(v._21, v.r[2].y);
		CXX_FEQUAL(v._22, v.r[2].z);
	}

	TEST_METHOD(cxx_math_matrix3x3_orothogonal)
	{
		float3x3 a = float3x3::rotate_x(radian<float>(0.5f));

		CXX_IS_TRUE(is_orthogonal(a));
		CXX_IS_TRUE(can_invert(a));

		a._11 += 20.0f;
		CXX_IS_FALSE(is_orthogonal(a));
	}

	TEST_METHOD(cxx_math_matrix3x3_transform)
	{
		float3 p(1, 1, 1);

		auto pt = float3x3::scale(2) * p;
		CXX_IS_TRUE(float3(2, 2, 2) == pt);

		pt = float3x3::flip_x() * pt;
		CXX_IS_TRUE(float3(-2, 2, 2) == pt);

		pt = float3x3::rotate_y(degree<float>(180)) * pt;
		CXX_IS_TRUE(float3(2, 2, -2) == pt);

		pt = float3x3::flip_y() * pt;
		CXX_IS_TRUE(float3(2, -2, -2) == pt);

		pt = float3x3::scale(0.5, 0.5, -0.5) * pt;
		CXX_IS_TRUE(float3(1, -1, 1) == pt);

		pt = float3x3::flip_y() * pt;
		CXX_IS_TRUE(float3::one() == pt);
	}
};