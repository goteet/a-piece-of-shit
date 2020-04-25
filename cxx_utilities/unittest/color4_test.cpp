#include "stdafx.h"


TEST_CLASS(TestCXXMathTestColor4TGroup)
{
public:
	TEST_METHOD(cxx_math_color4_generic)
	{
		CXX_IEQUAL(sizeof(float) * 4, sizeof(color4f));
	}

	TEST_METHOD(cxx_math_color4_initialization)
	{
		// color4f(float, float, float, float)
		color4f c2(0.1f, 0.2f, 1.3f, 0.5f);
		CXX_FEQUAL(0.1f, c2.r);
		CXX_FEQUAL(0.2f, c2.g);
		CXX_FEQUAL(1.3f, c2.b);
		CXX_FEQUAL(0.5f, c2.a);

		// color4f(color4f)
		color3f ccc = color3f::red();
		color4f c3(ccc, 0.1f);
		CXX_FEQUAL(1.0f, c3.r);
		CXX_FEQUAL(0.0f, c3.g);
		CXX_FEQUAL(0.0f, c3.b);
		CXX_FEQUAL(0.1f, c3.a);

		// color4f(unsigned int)
		color4f c4 = color4f::from_rgba(0xFFFF00FFu);
		CXX_FEQUAL(1.0f, c4.r);
		CXX_FEQUAL(0.0f, c4.g);
		CXX_FEQUAL(1.0f, c4.b);
		CXX_FEQUAL(1.0f, c4.a);
	}

	TEST_METHOD(cxx_math_color4_constants)
	{
		const auto black = color4f::black();
		CXX_FEQUAL(0.0f, black.r);
		CXX_FEQUAL(0.0f, black.g);
		CXX_FEQUAL(0.0f, black.b);
		CXX_FEQUAL(1.0f, black.a);

		const auto gray = color4f::gray();
		CXX_FEQUAL(0.5f, gray.r);
		CXX_FEQUAL(0.5f, gray.g);
		CXX_FEQUAL(0.5f, gray.b);
		CXX_FEQUAL(1.0f, gray.a);

		const auto white = color4f::white();
		CXX_FEQUAL(1.0f, white.r);
		CXX_FEQUAL(1.0f, white.g);
		CXX_FEQUAL(1.0f, white.b);
		CXX_FEQUAL(1.0f, white.a);

		const auto red = color4f::red();
		CXX_FEQUAL(1.0f, red.r);
		CXX_FEQUAL(0.0f, red.g);
		CXX_FEQUAL(0.0f, red.b);
		CXX_FEQUAL(1.0f, red.a);

		const auto green = color4f::green();
		CXX_FEQUAL(0.0f, green.r);
		CXX_FEQUAL(1.0f, green.g);
		CXX_FEQUAL(0.0f, green.b);
		CXX_FEQUAL(1.0f, green.a);

		const auto blue = color4f::blue();
		CXX_FEQUAL(0.0f, blue.r);
		CXX_FEQUAL(0.0f, blue.g);
		CXX_FEQUAL(1.0f, blue.b);
		CXX_FEQUAL(1.0f, blue.a);

		const auto yellow = color4f::yellow();
		CXX_FEQUAL(1.0f, yellow.r);
		CXX_FEQUAL(1.0f, yellow.g);
		CXX_FEQUAL(0.0f, yellow.b);
		CXX_FEQUAL(1.0f, yellow.a);

		const auto cyan = color4f::cyan();
		CXX_FEQUAL(0.0f, cyan.r);
		CXX_FEQUAL(1.0f, cyan.g);
		CXX_FEQUAL(1.0f, cyan.b);
		CXX_FEQUAL(1.0f, cyan.a);

		const auto purple = color4f::purple();
		CXX_FEQUAL(1.0f, purple.r);
		CXX_FEQUAL(0.0f, purple.g);
		CXX_FEQUAL(1.0f, purple.b);
		CXX_FEQUAL(1.0f, purple.a);
	}

	TEST_METHOD(cxx_math_color4_random)
	{
		auto c1 = color4f::random();
		auto c2 = color4f::random();

		CXX_IS_FALSE(c1 == c2);
	}

	TEST_METHOD(cxx_math_color4_compare)
	{
		color4f c1(0.1f, 0.2f, 1.0f, 0.4f);
		color4f c2(0.1f, 0.2f, 1.0f, 0.4f);
		auto c3 = c1;
		color4f c4(0.1f, 0.1f, 0.1f, 0.1f);

		// operator==
		CXX_IS_TRUE(c1 == c1);
		CXX_IS_TRUE(c1 == c2);
		CXX_IS_TRUE(c1 == c3);

		// opeartor!=
		CXX_IS_TRUE(c1 != c4);
	}

	TEST_METHOD(cxx_math_color4_add)
	{
		color4f c1(0.1f, 0.1f, 0.1f, 0.1f);
		color4f c2(0.2f, 0.2f, 1.2f, 0.2f);

		// operator+(color4f)
		auto b = c1 + c2;
		CXX_FEQUAL(0.3f, b.r);
		CXX_FEQUAL(0.3f, b.g);
		CXX_FEQUAL(1.3f, b.b);
		CXX_FEQUAL(0.3f, b.a);

		// operator+=(color4f)
		auto d = c1;
		d += c2;
		CXX_FEQUAL(0.3f, d.r);
		CXX_FEQUAL(0.3f, d.g);
		CXX_FEQUAL(1.3f, d.b);
		CXX_FEQUAL(0.3f, d.a);
	}

	TEST_METHOD(cxx_math_color4_multiply)
	{
		color4f c1(0.1f, 0.1f, 0.1f, 0.1f);
		color4f c2(0.2f, 0.2f, 1.2f, 0.2f);
		float f = 0.5f;

		// operator*(float)
		auto a = c1 * f;
		CXX_FEQUAL(0.05f, a.r);
		CXX_FEQUAL(0.05f, a.g);
		CXX_FEQUAL(0.05f, a.b);
		CXX_FEQUAL(0.05f, a.a);

		// operator*(color4f)
		auto b = c1 * c2;
		CXX_FEQUAL(0.02f, b.r);
		CXX_FEQUAL(0.02f, b.g);
		CXX_FEQUAL(0.12f, b.b);
		CXX_FEQUAL(0.02f, b.a);

		// operator*=(float)
		auto c = c1;
		c *= f;
		CXX_FEQUAL(0.05f, c.r);
		CXX_FEQUAL(0.05f, c.g);
		CXX_FEQUAL(0.05f, c.b);
		CXX_FEQUAL(0.05f, c.a);

		// operator*=(color4f)
		auto d = c1;
		d *= c2;
		CXX_FEQUAL(0.02f, d.r);
		CXX_FEQUAL(0.02f, d.g);
		CXX_FEQUAL(0.12f, d.b);
		CXX_FEQUAL(0.02f, d.a);
	}

	TEST_METHOD(cxx_math_color4_index)
	{
		// operator[]
		color4f c;
		c[0] = 0.2f;
		c[1] = 0.3f;
		c[2] = 0.5f;
		c[3] = 0.1f;
		CXX_FEQUAL(0.2f, c.r);
		CXX_FEQUAL(c[0], c.r);
		CXX_FEQUAL(0.3f, c.g);
		CXX_FEQUAL(c[1], c.g);
		CXX_FEQUAL(0.5f, c.b);
		CXX_FEQUAL(c[2], c.b);
		CXX_FEQUAL(0.1f, c.a);
		CXX_FEQUAL(c[3], c.a);
	}

	TEST_METHOD(cxx_math_color4_topointer)
	{
		// float*
		color4f c(0.1f, 0.2f, 0.3f, 0.4f);
		float *fv = c.value.v;
		CXX_FEQUAL(0.1f, fv[0]);
		CXX_FEQUAL(0.2f, fv[1]);
		CXX_FEQUAL(0.3f, fv[2]);
		CXX_FEQUAL(0.4f, fv[3]);

		const color4f& cc = c;
		const float* cfv = cc.value.v;
		CXX_FEQUAL(0.1f, cfv[0]);
		CXX_FEQUAL(0.2f, cfv[1]);
		CXX_FEQUAL(0.3f, cfv[2]);
		CXX_FEQUAL(0.4f, cfv[3]);

		float *nfv = c.value.v;
		nfv[0] = 1.0f;
		nfv[1] = 1.0f;
		nfv[2] = 1.0f;
		nfv[3] = 1.0f;
		CXX_FEQUAL(1.0f, nfv[0]);
		CXX_FEQUAL(1.0f, nfv[1]);
		CXX_FEQUAL(1.0f, nfv[2]);
		CXX_FEQUAL(1.0f, nfv[3]);
		CXX_IS_TRUE(color4f::white() == c);
	}

	TEST_METHOD(cxx_math_color4_set)
	{
		color4f c1, c2;

		// set(float, float, float, float)
		c1 = color4f(0.1f, 0.2f, 0.3f, 0.4f);
		CXX_FEQUAL(0.1f, c1.r);
		CXX_FEQUAL(0.2f, c1.g);
		CXX_FEQUAL(0.3f, c1.b);
		CXX_FEQUAL(0.4f, c1.a);

		c2 = color4f(2, 0, 2, 0.5);
		CXX_FEQUAL(2, c2.r);
		CXX_FEQUAL(0, c2.g);
		CXX_FEQUAL(2, c2.b);
		CXX_FEQUAL(0.5f, c2.a);

		clamp(c2);
		CXX_FEQUAL(1, c2.r);
		CXX_FEQUAL(0, c2.g);
		CXX_FEQUAL(1, c2.b);
		CXX_FEQUAL(0.5f, c2.a);

		c2 = color4f(2, -1, 3, -5);
		CXX_FEQUAL(2, c2.r);
		CXX_FEQUAL(-1, c2.g);
		CXX_FEQUAL(3, c2.b);
		CXX_FEQUAL(-5, c2.a);
		clamp(c2);
		CXX_FEQUAL(1, c2.r);
		CXX_FEQUAL(0, c2.g);
		CXX_FEQUAL(1, c2.b);
		CXX_FEQUAL(0, c2.a);

	}

	TEST_METHOD(cxx_math_color4_bgra)
	{
		auto c1 = color4f::red();
		auto c2 = color4f::white();
		auto c3 = color4f::black();

		CXX_IEQUAL(0xFFFF0000u, to_bgra(c1));
		CXX_IEQUAL(0xFFFFFFFFu, to_bgra(c2));
		CXX_IEQUAL(0xFF000000u, to_bgra(c3));

		CXX_IS_TRUE(color4f::from_bgra(0xFFFF0000u) == c1);
	}

	TEST_METHOD(cxx_math_color4_rgba)
	{
		auto c1 = color4f::red();
		auto c2 = color4f::white();
		auto c3 = color4f::black();

		CXX_IEQUAL(0xFF0000FFu, to_rgba(c1));
		CXX_IEQUAL(0xFFFFFFFFu, to_rgba(c2));
		CXX_IEQUAL(0xFF000000u, to_rgba(c3));

		CXX_IS_TRUE(color4f::from_rgba(0xFF0000FFu) == c1);
	}
};