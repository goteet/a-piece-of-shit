#include "stdafx.h"

TEST_CLASS(TestCXXMathTestMatrix2x2TGroup)
{
public:
	TEST_METHOD(cxx_math_matrix2x2_generic)
	{
		CXX_IEQUAL(sizeof(float) * 2 * 2, sizeof(float2x2));
	}

	float2x2 memberByStaticFunc = float2x2::identity();
	float2x2 memberByList = { 1,2,3,4 };

	TEST_METHOD(cxx_math_matrix2x2_intialization)
	{
		CXX_FEQUALM(1.0f, memberByStaticFunc._00, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._01, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._10, "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc._11, "member init by static function");

		CXX_FEQUALM(1.0f, memberByList._00, "member init by init-list");
		CXX_FEQUALM(2.0f, memberByList._01, "member init by init-list");
		CXX_FEQUALM(3.0f, memberByList._10, "member init by init-list");
		CXX_FEQUALM(4.0f, memberByList._11, "member init by init-list");

		float2x2 local = { 5,6,7,8 };
		CXX_FEQUALM(5.0f, local._00, "local init by init-list");
		CXX_FEQUALM(6.0f, local._01, "local init by init-list");
		CXX_FEQUALM(7.0f, local._10, "local init by init-list");
		CXX_FEQUALM(8.0f, local._11, "local init by init-list");
	}

	TEST_METHOD(cxx_math_matrix2x2_access)
	{
		float2x2 v = { 1, 2, 3, 4 };
		CXX_FEQUAL(v._00, v[0]);
		CXX_FEQUAL(v._01, v[1]);
		CXX_FEQUAL(v._10, v[2]);
		CXX_FEQUAL(v._11, v[3]);

		CXX_FEQUAL(v._00, v.r[0].x);
		CXX_FEQUAL(v._01, v.r[0].y);
		CXX_FEQUAL(v._10, v.r[1].x);
		CXX_FEQUAL(v._11, v.r[1].y);
	}

	TEST_METHOD(cxx_math_matrix2x2_opeartions)
	{
		auto rotMatrixP = float2x2::rotate((radian<float>)degree<float>(90));
		auto rotMatrixN = float2x2::rotate((radian<float>)degree<float>(-90));
		float2 v(1.0f, 0.0f);
		float2 vtp = rotMatrixP * v;
		float2 vtn = rotMatrixN * v;

		CXX_FEQUALM(0.0f, vtp.x, "float2x2::rotation is wrong");
		CXX_FEQUALM(1.0f, vtp.y, "float2x2::rotation is wrong");

		CXX_FEQUALM(0.0f, vtn.x, "float2x2::rotation is wrong");
		CXX_FEQUALM(-1.0f, vtn.y, "float2x2::rotation is wrong");
	}

	TEST_METHOD(cxx_math_matrix2x2_orothogonal)
	{
		float2x2 a = float2x2::rotate(radian<float>(0.5f));

		CXX_IS_TRUE(is_orthogonal(a));
		CXX_IS_TRUE(can_invert(a));

		a._11 += 20.0f;
		CXX_IS_FALSE(is_orthogonal(a));


		//a._11 = 0.0f;
		//CXX_IS_FALSE(a.can_invert());
		//radian<float> r(0.5f);
		//float2x2::rotate(r);
		//a = a * b;
		//a.inverse();
	}

	TEST_METHOD(cxx_math_matrix2x2_transform)
	{
		float2 p(1, 0);

		auto pt = float2x2::scale(2) * p;
		CXX_IS_TRUE(float2(2, 0) == pt);

		pt = float2x2::flip_x() * pt;
		CXX_IS_TRUE(float2(-2, 0) == pt);

		pt = float2x2::rotate(degree<float>(90)) * pt;
		CXX_IS_TRUE(float2(0, -2) == pt);

		pt = float2x2::flip_y() * pt;
		CXX_IS_TRUE(float2(0, 2) == pt);
		pt.x = 2;

		pt = float2x2::scale(0.5, 0.5) * pt;
		CXX_IS_TRUE(float2(1, 1) == pt);
	}
};