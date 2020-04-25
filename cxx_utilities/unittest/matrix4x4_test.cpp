#include "stdafx.h"

TEST_CLASS(TestCXXMathTestMatrix4x4TGroup)
{
public:
	TEST_METHOD(cxx_math_matrix4x4_generic)
	{
		CXX_IEQUAL(sizeof(float) * 4 * 4, sizeof(float4x4));
	}

	float4x4 memberByStaticFunc = float4x4::identity();
	float4x4 memberByList = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

	TEST_METHOD(cxx_math_matrix4x4_intialization)
	{
		CXX_FEQUALM(1.0f, memberByStaticFunc._00, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._01, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._02, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._03, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._10, "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc._11, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._12, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._13, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._20, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._21, "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc._22, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._23, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._30, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._31, "member init by static function");
		CXX_FEQUALM(0.0f, memberByStaticFunc._32, "member init by static function");
		CXX_FEQUALM(1.0f, memberByStaticFunc._33, "member init by static function");

		CXX_FEQUALM(1.0f, memberByList._00, "member init by init-list");
		CXX_FEQUALM(2.0f, memberByList._01, "member init by init-list");
		CXX_FEQUALM(3.0f, memberByList._02, "member init by init-list");
		CXX_FEQUALM(4.0f, memberByList._03, "member init by init-list");
		CXX_FEQUALM(5.0f, memberByList._10, "member init by init-list");
		CXX_FEQUALM(6.0f, memberByList._11, "member init by init-list");
		CXX_FEQUALM(7.0f, memberByList._12, "member init by init-list");
		CXX_FEQUALM(8.0f, memberByList._13, "member init by init-list");
		CXX_FEQUALM(9.0f, memberByList._20, "member init by init-list");
		CXX_FEQUALM(10.0f, memberByList._21, "member init by init-list");
		CXX_FEQUALM(11.0f, memberByList._22, "member init by init-list");
		CXX_FEQUALM(12.0f, memberByList._23, "member init by init-list");
		CXX_FEQUALM(13.0f, memberByList._30, "member init by init-list");
		CXX_FEQUALM(14.0f, memberByList._31, "member init by init-list");
		CXX_FEQUALM(15.0f, memberByList._32, "member init by init-list");
		CXX_FEQUALM(16.0f, memberByList._33, "member init by init-list");

		float4x4 local = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		CXX_FEQUALM(1.0f, local._00, "local init by init-list");
		CXX_FEQUALM(2.0f, local._01, "local init by init-list");
		CXX_FEQUALM(3.0f, local._02, "local init by init-list");
		CXX_FEQUALM(4.0f, local._03, "local init by init-list");
		CXX_FEQUALM(5.0f, local._10, "local init by init-list");
		CXX_FEQUALM(6.0f, local._11, "local init by init-list");
		CXX_FEQUALM(7.0f, local._12, "local init by init-list");
		CXX_FEQUALM(8.0f, local._13, "local init by init-list");
		CXX_FEQUALM(9.0f, local._20, "local init by init-list");
		CXX_FEQUALM(10.0f, local._21, "local init by init-list");
		CXX_FEQUALM(11.0f, local._22, "local init by init-list");
		CXX_FEQUALM(12.0f, local._23, "local init by init-list");
		CXX_FEQUALM(13.0f, local._30, "local init by init-list");
		CXX_FEQUALM(14.0f, local._31, "local init by init-list");
		CXX_FEQUALM(15.0f, local._32, "local init by init-list");
		CXX_FEQUALM(16.0f, local._33, "local init by init-list");

		float4x4 m433(float3x3(1, 2, 3, 4, 5, 6, 7, 8, 9), float3(0, 0, 0));
		CXX_FEQUAL(1.0f, m433._00);
		CXX_FEQUAL(2.0f, m433._01);
		CXX_FEQUAL(3.0f, m433._02);
		CXX_FEQUAL(0.0f, m433._03);
		CXX_FEQUAL(4.0f, m433._10);
		CXX_FEQUAL(5.0f, m433._11);
		CXX_FEQUAL(6.0f, m433._12);
		CXX_FEQUAL(0.0f, m433._13);
		CXX_FEQUAL(7.0f, m433._20);
		CXX_FEQUAL(8.0f, m433._21);
		CXX_FEQUAL(9.0f, m433._22);
		CXX_FEQUAL(0.0f, m433._23);
		CXX_FEQUAL(0.0f, m433._30);
		CXX_FEQUAL(0.0f, m433._31);
		CXX_FEQUAL(0.0f, m433._32);
		CXX_FEQUAL(1.0f, m433._33);
	}

	TEST_METHOD(cxx_math_matrix4x4_access)
	{
		float4x4 v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
		CXX_FEQUAL(v._00, memberByList[0]);
		CXX_FEQUAL(v._01, memberByList[1]);
		CXX_FEQUAL(v._02, memberByList[2]);
		CXX_FEQUAL(v._03, memberByList[3]);
		CXX_FEQUAL(v._10, memberByList[4]);
		CXX_FEQUAL(v._11, memberByList[5]);
		CXX_FEQUAL(v._12, memberByList[6]);
		CXX_FEQUAL(v._13, memberByList[7]);
		CXX_FEQUAL(v._20, memberByList[8]);
		CXX_FEQUAL(v._21, memberByList[9]);
		CXX_FEQUAL(v._22, memberByList[10]);
		CXX_FEQUAL(v._23, memberByList[11]);
		CXX_FEQUAL(v._30, memberByList[12]);
		CXX_FEQUAL(v._31, memberByList[13]);
		CXX_FEQUAL(v._32, memberByList[14]);
		CXX_FEQUAL(v._33, memberByList[15]);

		CXX_FEQUAL(v._00, v.r[0].x);
		CXX_FEQUAL(v._01, v.r[0].y);
		CXX_FEQUAL(v._02, v.r[0].z);
		CXX_FEQUAL(v._03, v.r[0].w);
		CXX_FEQUAL(v._10, v.r[1].x);
		CXX_FEQUAL(v._11, v.r[1].y);
		CXX_FEQUAL(v._12, v.r[1].z);
		CXX_FEQUAL(v._13, v.r[1].w);
		CXX_FEQUAL(v._20, v.r[2].x);
		CXX_FEQUAL(v._21, v.r[2].y);
		CXX_FEQUAL(v._22, v.r[2].z);
		CXX_FEQUAL(v._23, v.r[2].w);
		CXX_FEQUAL(v._30, v.r[3].x);
		CXX_FEQUAL(v._31, v.r[3].y);
		CXX_FEQUAL(v._32, v.r[3].z);
		CXX_FEQUAL(v._33, v.r[3].w);
	}

	TEST_METHOD(cxx_math_matrix4x4_orothogonal)
	{
		float4x4 a = float4x4::rotate_x(radian<float>(0.5f));

		CXX_IS_TRUE(is_orthogonal(a));
		CXX_IS_TRUE(can_invert(a));

		a._11 += 20.0f;
		CXX_IS_FALSE(is_orthogonal(a));
	}

	TEST_METHOD(cxx_math_matrix4x4_transform)
	{
		point3d<float> p(1, 1, 1);

		auto pt = transform(float4x4::scale(2), p);
		CXX_IS_TRUE(float3(2, 2, 2) == pt);

		pt = transform(float4x4::flip_x(), pt);
		CXX_IS_TRUE(float3(-2, 2, 2) == pt);

		pt = transform(float4x4::rotate_y(degree<float>(180)), pt);
		CXX_IS_TRUE(float3(2, 2, -2) == pt);

		pt = transform(float4x4::flip_y(), pt);
		CXX_IS_TRUE(float3(2, -2, -2) == pt);

		pt = transform(float4x4::scale(0.5, 0.5, -0.5), pt);
		CXX_IS_TRUE(float3(1, -1, 1) == pt);

		pt = transform(float4x4::flip_y(), pt);
		CXX_IS_TRUE(float3::one() == pt);

		pt = transform(float4x4::translate(-1, -1, -1), pt);
		CXX_IS_TRUE(float3::zero() == pt);
	}
};