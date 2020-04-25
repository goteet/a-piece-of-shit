#include "stdafx.h"


TEST_CLASS(TestCXXMathRectGroup)
{
private:
	rect mRect;

public:
	TestCXXMathRectGroup()
	{
		mRect.move_to(int2(10, 10));
		mRect.set_extend(int2(20, 20));
	}

	TEST_METHOD(cxx_math_rect_equal)
	{
		// operator==
		rect r1;
		r1.move_to(int2(10, 10));
		r1.set_extend(int2(20, 20));
		CXX_IS_TRUE(mRect == r1);

		rect r2;
		r2.move_to(int2(11, 10));
		r2.set_extend(int2(20, 20));
		CXX_IS_FALSE(mRect == r2);

		rect r3;
		r3.move_to(int2(10, 10));
		r3.set_extend(int2(21, 21));
		CXX_IS_FALSE(mRect == r3);
	}

	TEST_METHOD(cxx_math_rect_access)
	{
		CXX_IEQUAL(-10, mRect.min().x);
		CXX_IEQUAL(30, mRect.max().x);
		CXX_IEQUAL(-10, mRect.min().y);
		CXX_IEQUAL(30, mRect.max().y);
		auto e = mRect.extend();
		CXX_IEQUAL(20, e.x);
		CXX_IEQUAL(20, e.y);

		int2 center = mRect.center();
		CXX_IEQUAL(10, center.x);
		CXX_IEQUAL(10, center.y);
		
		int2 size = mRect.extend();
		CXX_IEQUAL(20, size.x);
		CXX_IEQUAL(20, size.y);
	}

	TEST_METHOD(cxx_math_rect_contains)
	{

		mRect.move_to(int2(10, 10));
		mRect.set_extend(int2(20, 20));
		CXX_IS_TRUE(mRect.contains(int2(10, 10)));
		CXX_IS_FALSE(mRect.contains(int2(-11, -11)));
		CXX_IS_FALSE(mRect.contains(int2(31, 9)));
		CXX_IS_FALSE(mRect.contains(int2(9, 31)));

		CXX_IS_TRUE(mRect.contains(rect(int2(10, 10), int2(20,20))));
		CXX_IS_TRUE(mRect.contains(rect(int2(-10, -10), int2(30, 30))));
		CXX_IS_FALSE(mRect.contains(rect(int2(-12, -12), int2(-11, -11))));
		
	}

	TEST_METHOD(cxx_math_rect_hittet)
	{
		rect r;
		r.move_to(int2(-12, -12));
		r.set_extend(int2(1, 1));

		CXX_IS_TRUE(mRect.hit_test(r) == intersection::none);

		enlarge(r, int2(5, 5));
		CXX_IS_TRUE(mRect.hit_test(r) == intersection::hit);

		enlarge(r, int2(100, 100));
		CXX_IS_TRUE(mRect.hit_test(r) == intersection::inside);

		move(r, int2(12, 12));
		enlarge(r, -100, -100);
		CXX_IS_TRUE(mRect.hit_test(r) == intersection::contain);

		move(r, 10, 10);
		enlarge(r, 14, 14);
		CXX_IS_TRUE(mRect.hit_test(r) == intersection::same);
	}

	TEST_METHOD(cxx_math_rect_move)
	{
		rect r;
		r.move_to(10, 10);
		r.expand_to(20, 20);

		move(r, 10, 10);
		CXX_IEQUAL(0, r.min().x);
		CXX_IEQUAL(40, r.max().x);
		CXX_IEQUAL(0, r.min().y);
		CXX_IEQUAL(40, r.max().y);
		CXX_IEQUAL(20, r.extend().x);
		CXX_IEQUAL(20, r.extend().y);
	}

	TEST_METHOD(cxx_math_rectenlarge)
	{
		rect r;
		r.move_to(10, 10);
		r.expand_to(20, 20);

		enlarge(r, 10, 10);
		CXX_IEQUAL(-20, r.min().x);
		CXX_IEQUAL(40, r.max().x);
		CXX_IEQUAL(-20, r.min().y);
		CXX_IEQUAL(40, r.max().y);
		CXX_IEQUAL(30, r.extend().x);
		CXX_IEQUAL(30, r.extend().y);
	}
};