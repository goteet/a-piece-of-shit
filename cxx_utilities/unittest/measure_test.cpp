#include "stdafx.h"

TEST_CLASS(TestCXXMathMeasurementGroup)
{
public:
	TEST_METHOD(cxx_math_measure_constructors)
	{
		// default constructor
		degree<float> d_i;
		radian<float> r_i;
		CXX_FEQUAL(0, d_i.value);
		CXX_FEQUAL(0, r_i.value);

		// explicit float constructor
		degree<float> d_half_pi(90);
		radian<float> r_half_pi(constants<float>::pi_half);
		CXX_FEQUAL(90, d_half_pi.value);
		CXX_FEQUAL(constants<float>::pi_half, r_half_pi.value);

		// converted constructor
		degree<float> d_180 = radian<float>(constants<float>::pi);
		radian<float> r_pi = degree<float>(180);
		CXX_FEQUAL(180.0f, d_180.value);
		CXX_FEQUAL(constants<float>::pi, r_pi.value);

		// user-literal

		degree<float> d_45_l = 45_degf;
		radian<float> r_pi_l = 3.141592657_radf;
		CXX_FEQUAL(45.0f, d_45_l .value);
		CXX_FEQUAL(constants<float>::pi, r_pi_l.value);
	}

	TEST_METHOD(cxx_math_measure_unary_operators)
	{
		degree<float> d_180 = radian<float>(constants<float>::pi);
		radian<float> r_pi = degree<float>(180);

		// operator +
		CXX_FEQUAL(d_180.value, (+d_180).value);
		CXX_FEQUAL(r_pi.value, (+r_pi).value);
		CXX_FEQUAL(180.0f, (+d_180).value);
		CXX_FEQUAL(constants<float>::pi, (+r_pi).value);

		// operator -
		CXX_FEQUAL(-180.0f, (-d_180).value);
		CXX_FEQUAL(-constants<float>::pi, (-r_pi).value);

		degree<float> d_positive(45);
		degree<float> d_negetive(-45);
		radian<float> r_positive(constants<float>::pi * 0.25);
		radian<float> r_negetive(-constants<float>::pi * 0.25);

		// operator + & operator==
		CXX_IS_TRUE(d_positive == +d_positive);
		CXX_IS_TRUE(r_positive == +r_positive);
		CXX_IS_TRUE(d_positive == -d_negetive);
		CXX_IS_TRUE(r_positive == -r_negetive);

	}

	TEST_METHOD(cxx_math_measure_compare_operators)
	{
		degree<float> d45(45);
		degree<float> d90(90);
		degree<float> d180(180);
		radian<float> rpi_4(constants<float>::pi / 4);
		radian<float> rpi_2(constants<float>::pi_half);
		radian<float> rpi(constants<float>::pi);

		// operator==
		CXX_IS_TRUE(degree<float>(30) == degree<float>(30));
		CXX_IS_TRUE(degree<float>(30) == radian<float>(constants<float>::pi / 6));
		CXX_IS_TRUE(d45 == rpi_4);
		CXX_IS_TRUE(rpi == d180);
		CXX_IS_FALSE(rpi == rpi_2);
		CXX_IS_FALSE(d90 == d45);

		// operator!=
		CXX_IS_TRUE(d45 != rpi_2);
		CXX_IS_FALSE(rpi != d180);


		// operator<
		CXX_IS_TRUE(d45 < d90);
		CXX_IS_TRUE(d45 < rpi);
		CXX_IS_TRUE(rpi_2 < rpi);
		CXX_IS_TRUE(rpi_2 < d180);

		// operator>
		CXX_IS_TRUE(d180 > d90);
		CXX_IS_TRUE(d180 > rpi_4);
		CXX_IS_TRUE(rpi > rpi_2);
		CXX_IS_TRUE(rpi > d90);

		// operator<=
		CXX_IS_TRUE(d90 <= rpi_2);
		CXX_IS_TRUE(rpi_2 <= d90);

		// operator>=
		CXX_IS_TRUE(d90 >= rpi_2);
		CXX_IS_TRUE(rpi_2 >= d90);
	}

	//...
	TEST_METHOD(cxx_math_measure_calculations)
	{
		// operator+
		degree<float> sum = degree<float>(30) + (degree<float>)radian<float>(constants<float>::pi / 6);
		CXX_FEQUAL(60.0f, sum.value);

		// operator-
		degree<float> dif = degree<float>(30) - (degree<float>)radian<float>(constants<float>::pi / 6);
		CXX_FEQUAL(0.0f, dif.value);

		// operator*
		CXX_IS_TRUE(radian<float>(constants<float>::pi / 6) * 6 == degree<float>(180));
		CXX_IS_TRUE(3 * radian<float>(constants<float>::pi / 3) == degree<float>(180));

		// operator/
		CXX_IS_TRUE(radian<float>(constants<float>::pi) / 6 == degree<float>(30));
		CXX_IS_TRUE(radian<float>(constants<float>::pi) / 3 == degree<float>(60));

		// operator+=
		dif += radian<float>(constants<float>::pi / 6);
		CXX_FEQUAL(30.0f, dif.value);

		// operator-=
		sum -= radian<float>(constants<float>::pi / 6);
		CXX_FEQUAL(30.0f, sum.value);



		radian<float> mr(constants<float>::pi);
		mr *= 2;

		// operator*=
		CXX_FEQUAL(mr.value, constants<float>::pi2);

		mr /= 2;
		// operator/=
		CXX_FEQUAL(mr.value, constants<float>::pi);
	}

	TEST_METHOD(cxx_math_measure_functions)
	{
		degree<float> d60(60);
		radian<float> r_pi_3(constants<float>::pi / 3.0f);
		// sin
		CXX_FEQUAL(sin(r_pi_3), sin(d60));
		CXX_FEQUAL(sin(r_pi_3.value), sin(d60));
		CXX_FEQUAL(0.5f * sqrt(3.0f), sin(d60));

		// cos
		CXX_FEQUAL(cos(r_pi_3), cos(d60));
		CXX_FEQUAL(cos(r_pi_3.value), cos(d60));
		CXX_FEQUAL(0.5f, cos(d60));

		// tan
		CXX_FEQUAL(tan(r_pi_3), tan(d60));
		CXX_FEQUAL(tan(r_pi_3.value), tan(d60));

		float a = 5;
		radian<float> rpiz(constants<float>::pi2 + a);
		degree<float> d361(361);

		// normalize
		CXX_FEQUAL(normalized(d361).value, 1);
		CXX_FEQUAL(normalized(rpiz).value, a);
	}
};