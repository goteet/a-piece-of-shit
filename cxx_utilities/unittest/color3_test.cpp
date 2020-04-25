#include "stdafx.h"


TEST_CLASS(TestCXXMathTestColor3TGroup)
{
public:
	TEST_METHOD(cxx_math_color3_generic)
	{
		CXX_IEQUAL(sizeof(float) * 3, sizeof(color3f));
	}

	TEST_METHOD(cxx_math_color3_initialization)
	{
		// color3f()
		color3f c1;
		CXX_FEQUAL(0.0f, c1.r);
		CXX_FEQUAL(0.0f, c1.g);
		CXX_FEQUAL(0.0f, c1.b);

		// color3f(float, float, float)
		color3f c2(0.1f, 0.2f, 1.3f);
		CXX_FEQUAL(0.1f, c2.r);
		CXX_FEQUAL(0.2f, c2.g);
		CXX_FEQUAL(1.3f, c2.b);

		// color3f(color4f)
		auto cccc = color4f::red();
		color3f c3(cccc);
		CXX_FEQUAL(1.0f, c3.r);
		CXX_FEQUAL(0.0f, c3.g);
		CXX_FEQUAL(0.0f, c3.b);

		// color3f(unsigned int)
		color3f c4 = color3f::from_rgba(0xFFFF00FFu);
		CXX_FEQUAL(1.0f, c4.r);
		CXX_FEQUAL(0.0f, c4.g);
		CXX_FEQUAL(1.0f, c4.b);
	}

	TEST_METHOD(cxx_math_color3_constants)
	{
		const auto black = color3f::black();
		CXX_FEQUAL(0.0f, black.r);
		CXX_FEQUAL(0.0f, black.g);
		CXX_FEQUAL(0.0f, black.b);

		const auto gray = color3f::gray();
		CXX_FEQUAL(0.5f, gray.r);
		CXX_FEQUAL(0.5f, gray.g);
		CXX_FEQUAL(0.5f, gray.b);

		const auto white = color3f::white();
		CXX_FEQUAL(1.0f, white.r);
		CXX_FEQUAL(1.0f, white.g);
		CXX_FEQUAL(1.0f, white.b);

		const auto red = color3f::red();
		CXX_FEQUAL(1.0f, red.r);
		CXX_FEQUAL(0.0f, red.g);
		CXX_FEQUAL(0.0f, red.b);

		const auto green = color3f::green();
		CXX_FEQUAL(0.0f, green.r);
		CXX_FEQUAL(1.0f, green.g);
		CXX_FEQUAL(0.0f, green.b);

		const auto blue = color3f::blue();
		CXX_FEQUAL(0.0f, blue.r);
		CXX_FEQUAL(0.0f, blue.g);
		CXX_FEQUAL(1.0f, blue.b);

		const auto yellow = color3f::yellow();
		CXX_FEQUAL(1.0f, yellow.r);
		CXX_FEQUAL(1.0f, yellow.g);
		CXX_FEQUAL(0.0f, yellow.b);

		const auto cyan = color3f::cyan();
		CXX_FEQUAL(0.0f, cyan.r);
		CXX_FEQUAL(1.0f, cyan.g);
		CXX_FEQUAL(1.0f, cyan.b);

		const auto purple = color3f::purple();
		CXX_FEQUAL(1.0f, purple.r);
		CXX_FEQUAL(0.0f, purple.g);
		CXX_FEQUAL(1.0f, purple.b);
	}

	TEST_METHOD(cxx_math_color3_random)
	{
		auto c1 = color3f::random();
		auto c2 = color3f::random();

		CXX_IS_FALSE(c1 == c2);
	}

	TEST_METHOD(cxx_math_color3_compare)
	{
		color3f c1(0.1f, 0.2f, 1.0f);
		color3f c2(0.1f, 0.2f, 1.0f);
		auto c3 = c1;
		color3f c4(0.1f, 0.1f, 0.1f);

		// operator==
		CXX_IS_TRUE(c1 == c1);
		CXX_IS_TRUE(c1 == c2);
		CXX_IS_TRUE(c1 == c3);

		// opeartor!=
		CXX_IS_TRUE(c1 != c4);
	}

	TEST_METHOD(cxx_math_color3_add)
	{
		color3f c1(0.1f, 0.1f, 0.1f);
		color3f c2(0.2f, 0.2f, 1.2f);

		// operator+(color3f)
		auto b = c1 + c2;
		CXX_FEQUAL(0.3f, b.r);
		CXX_FEQUAL(0.3f, b.g);
		CXX_FEQUAL(1.3f, b.b);

		// operator+=(color3f)
		auto d = c1;
		d += c2;
		CXX_FEQUAL(0.3f, d.r);
		CXX_FEQUAL(0.3f, d.g);
		CXX_FEQUAL(1.3f, d.b);
	}

	TEST_METHOD(cxx_math_color3_multiply)
	{
		color3f c1(0.1f, 0.1f, 0.1f);
		color3f c2(0.2f, 0.2f, 1.2f);
		float f = 0.5f;

		// operator*(float)
		auto a = c1 * f;
		CXX_FEQUAL(0.05f, a.r);
		CXX_FEQUAL(0.05f, a.g);
		CXX_FEQUAL(0.05f, a.b);

		// operator*(color3f)
		auto b = c1 * c2;
		CXX_FEQUAL(0.02f, b.r);
		CXX_FEQUAL(0.02f, b.g);
		CXX_FEQUAL(0.12f, b.b);

		// operator*=(color3f)
		auto d = c1;
		d *= c2;
		CXX_FEQUAL(0.02f, d.r);
		CXX_FEQUAL(0.02f, d.g);
		CXX_FEQUAL(0.12f, d.b);
	}

	TEST_METHOD(cxx_math_color3_index)
	{
		// operator[]
		color3f c;
		c[0] = 0.2f;
		c[1] = 0.3f;
		c[2] = 0.5f;
		CXX_FEQUAL(0.2f, c.r);
		CXX_FEQUAL(c[0], c.r);
		CXX_FEQUAL(0.3f, c.g);
		CXX_FEQUAL(c[1], c.g);
		CXX_FEQUAL(0.5f, c.b);
		CXX_FEQUAL(c[2], c.b);
	}

	TEST_METHOD(cxx_math_color3_topointer)
	{
		// float*
		color3f c(0.1f, 0.2f, 0.3f);
		float *fv = c.value.v;
		CXX_FEQUAL(0.1f, fv[0]);
		CXX_FEQUAL(0.2f, fv[1]);
		CXX_FEQUAL(0.3f, fv[2]);

		const color3f& cc = c;
		const float* cfv = cc.value.v;
		CXX_FEQUAL(0.1f, cfv[0]);
		CXX_FEQUAL(0.2f, cfv[1]);
		CXX_FEQUAL(0.3f, cfv[2]);

		float *nfv = c.value.v;
		nfv[0] = 1.0f;
		nfv[1] = 1.0f;
		nfv[2] = 1.0f;
		CXX_FEQUAL(1.0f, nfv[0]);
		CXX_FEQUAL(1.0f, nfv[1]);
		CXX_FEQUAL(1.0f, nfv[2]);
		CXX_IS_TRUE(color3f::white() == c);
	}

	TEST_METHOD(cxx_math_color3_set)
	{
		color3f c1, c2;
		color4f cccc(0.1f, 0.2f, 0.3f, 0.4f);

		// set(float, float, float)
		c1 = color3f(0.1f, 0.2f, 0.3f);
		CXX_FEQUAL(0.1f, c1.r);
		CXX_FEQUAL(0.2f, c1.g);
		CXX_FEQUAL(0.3f, c1.b);

		// set(color4f)
		c2 = color3f(cccc);
		CXX_FEQUAL(0.1f, c2.r);
		CXX_FEQUAL(0.2f, c2.g);
		CXX_FEQUAL(0.3f, c2.b);

		c2 = color3f(2, 0, 2);
		CXX_FEQUAL(2, c2.r);
		CXX_FEQUAL(0, c2.g);
		CXX_FEQUAL(2, c2.b);
		clamp(c2);
		CXX_FEQUAL(1, c2.r);
		CXX_FEQUAL(0, c2.g);
		CXX_FEQUAL(1, c2.b);

		c2 = color3f(2, -1, 0);
		CXX_FEQUAL(2, c2.r);
		CXX_FEQUAL(-1, c2.g);
		CXX_FEQUAL(0, c2.b);
		clamp(c2);
		CXX_FEQUAL(1, c2.r);
		CXX_FEQUAL(0, c2.g);
		CXX_FEQUAL(0, c2.b);
	}

	TEST_METHOD(cxx_math_color3_bgra)
	{
		auto c1 = color3f::red();
		auto c2 = color3f::white();
		auto c3 = color3f::black();

		CXX_IEQUAL(0xFFFF0000u, to_bgra(c1, 255));
		CXX_IEQUAL(0xFFFFFFFFu, to_bgra(c2, 255));
		CXX_IEQUAL(0xFF000000u, to_bgra(c3, 255));

		CXX_IS_TRUE(color3f::from_bgra(0xFFFF0000u) == c1);
	}

	TEST_METHOD(cxx_math_color3_rgba)
	{
		auto c1 = color3f::red();
		auto c2 = color3f::white();
		auto c3 = color3f::black();

		CXX_IEQUAL(0xFF0000FFu, to_rgba(c1, 255));
		CXX_IEQUAL(0xFFFFFFFFu, to_rgba(c2, 255));
		CXX_IEQUAL(0xFF000000u, to_rgba(c3, 255));

		CXX_IS_TRUE(color3f::from_rgba(0xFF0000FFu) == c1);
	}
};
