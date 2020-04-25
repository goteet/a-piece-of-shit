#include "stdafx.h"

TEST_CLASS(TestCXXMathTestMatrix2x3TGroup)
{
public:
	TEST_METHOD(cxx_math_matrix2x3_generic)
	{
		CXX_IEQUAL(sizeof(float) * 3 * 2, sizeof(float2x3));

	}

	float2x3 memberByStaticFunc = float2x3::identity();
	float2x3 memberByList = { 1,2,3,4,5,6 };

	TEST_METHOD(cxx_math_matrix2x3_intialization)
	{
		CXX_FEQUALM(1.0f, memberByStaticFunc._00, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._01, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._02, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._10, "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc._11, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._12, "member init by static function");

		CXX_FEQUALM(1.0f, memberByList._00, "member init by init-list");
		CXX_FEQUALM(2.0f, memberByList._01, "member init by init-list");
		CXX_FEQUALM(3.0f, memberByList._02, "member init by init-list");
		CXX_FEQUALM(4.0f, memberByList._10, "member init by init-list");
		CXX_FEQUALM(5.0f, memberByList._11, "member init by init-list");
		CXX_FEQUALM(6.0f, memberByList._12, "member init by init-list");

		float2x3 local = { 1,2,3,4,5,6 };
		CXX_FEQUALM(1.0f, local._00, "local init by init-list");
		CXX_FEQUALM(2.0f, local._01, "local init by init-list");
		CXX_FEQUALM(3.0f, local._02, "local init by init-list");
		CXX_FEQUALM(4.0f, local._10, "local init by init-list");
		CXX_FEQUALM(5.0f, local._11, "local init by init-list");
		CXX_FEQUALM(6.0f, local._12, "local init by init-list");

		float2x3 m3(float2x2(1, 2, 3, 4), float2(0));
		CXX_FEQUAL(1.0f, m3._00);
		CXX_FEQUAL(2.0f, m3._01);
		CXX_FEQUAL(0.0f, m3._02);
		CXX_FEQUAL(3.0f, m3._10);
		CXX_FEQUAL(4.0f, m3._11);
		CXX_FEQUAL(0.0f, m3._12);
	}

	TEST_METHOD(cxx_math_matrix2x3_access)
	{
		float2x3 v = { 1, 2, 3, 4, 5, 6 };
		CXX_FEQUAL(v._00, v[0]);
		CXX_FEQUAL(v._01, v[1]);
		CXX_FEQUAL(v._02, v[2]);
		CXX_FEQUAL(v._10, v[3]);
		CXX_FEQUAL(v._11, v[4]);
		CXX_FEQUAL(v._12, v[5]);

		CXX_FEQUAL(v._00, v.r[0].x);
		CXX_FEQUAL(v._01, v.r[0].y);
		CXX_FEQUAL(v._02, v.r[0].z);
		CXX_FEQUAL(v._10, v.r[1].x);
		CXX_FEQUAL(v._11, v.r[1].y);
		CXX_FEQUAL(v._12, v.r[1].z);
	}

	TEST_METHOD(cxx_math_matrix2x3_transform)
	{
		point2d<float> p(1, 0);

		auto pt = transform(float2x3::scale(2), p);
		CXX_IS_TRUE(float2(2, 0) == pt);

		pt = transform(float2x3::flip_x(), pt);
		CXX_IS_TRUE(float2(-2, 0) == pt);

		pt = transform(float2x3::rotate(degree<float>(90)), pt);
		CXX_IS_TRUE(float2(0, -2) == pt);

		pt = transform(float2x3::flip_y(), pt);
		CXX_IS_TRUE(float2(0, 2) == pt);
		pt.x = 2;

		pt = transform(float2x3::scale(0.5, 0.5), pt);
		CXX_IS_TRUE(float2(1, 1) == pt);
	}
};