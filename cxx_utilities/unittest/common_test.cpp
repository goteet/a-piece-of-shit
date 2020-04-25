#include "stdafx.h"
#include "cxx_scope.h"
struct Interface
{
	void Release() {  }
};

TEST_CLASS(TestCXXCommonFunctionsGroup)
{
public:
	TEST_METHOD(cxx_math_deleter)
	{
		// SafeDelete
		{
			int* pi_f = new int(1);
			cxx::safe_delete(pi_f);

			int* pinull = nullptr;
			cxx::safe_delete(pinull);

			pi_f = new int[2];
			cxx::safe_delete(pi_f);
		}


		// SafeRelease
		{
			Interface* pi_f = new Interface();
			cxx::safe_release(pi_f);

			Interface* pinull = nullptr;
			cxx::safe_release(pinull);
		}

		// ScopeGuard
		{
			ON_EXIT
			{
				/*
				std::cout << "ON_EXIT function A Executed"
				<< std::endl;
				*/
			};

			ON_EXIT
			{
				/*
				std::cout << "ON_EXIT function B Executed"
				<< std::endl;
				*/
			};

			struct Functor
			{
				void operator()() { /*std::cout << "struct Functor::operator() Executed";*/ }
			};
			auto lambda = [] { /*std::cout << "scope lambda function Executed;" << std::endl;*/ };
			auto scope1 = cxx::make_scope_guard(lambda);
			auto scope2 = std::move(scope1);
			auto scope3 = cxx::make_scope_guard(Functor());

			auto scope4 = cxx::on_exit_helper() << lambda;
			auto scope5 = cxx::on_exit_helper() << Functor();

			//std::cout << "scope1 is " << (scope1.is_dismiss() ? "Dismissed" : "Activated") << std::endl;
			//std::cout << "scope2 is " << (scope2.is_dismiss() ? "Dismissed" : "Activated") << std::endl;
		}
	}

	TEST_METHOD(cxx_math_misc)
	{
		{
			//float2 v2;
			//float3 v3;
			//float4 v4;
			auto dot2 = dot(float2(1.0f), float2(1, 2));

			constexpr auto min2float2 = min_v(float2(2.0f, 1.0f), float2(1.0f, 2.0f));
			constexpr auto max2float2 = max_v(float2(2.0f, 1.0f), float2(1.0f, 2.0f));
			constexpr auto minmax2float2 = minmax_v(float2(2.0f, 1.0f), float2(1.0f, 2.0f));

			constexpr auto min2float3 = min_v(float3(2.0f, 1.0f, 2.0f), float3(1.0f, 2.0f, 1.0f));
			constexpr auto max2float3 = max_v(float3(2.0f, 1.0f, 2.0f), float3(1.0f, 2.0f, 1.0f));
			constexpr auto minmax2float3 = minmax_v(float3(2.0f, 1.0f, 2.0f), float3(1.0f, 2.0f, 1.0f));

			constexpr auto min2float4 = min_v(float4(2.0f, 1.0f, 2.0f, 1.0f), float4(1.0f, 2.0f, 1.0f, 2.0f));
			constexpr auto max2float4 = max_v(float4(2.0f, 1.0f, 2.0f, 1.0f), float4(1.0f, 2.0f, 1.0f, 2.0f));
			constexpr auto minmax2float4 = minmax_v(float4(2.0f, 1.0f, 2.0f, 1.0f), float4(1.0f, 2.0f, 1.0f, 2.0f));

			constexpr auto min3float2 = min_v(float2(2.0f, 1.0f), float2(1.0f, 3.0f), float2(3.0f, 2.0f));
			constexpr auto max3float2 = max_v(float2(2.0f, 1.0f), float2(1.0f, 3.0f), float2(3.0f, 2.0f));
			constexpr auto minmax3float2 = minmax_v(float2(2.0f, 1.0f), float2(1.0f, 3.0f), float2(3.0f, 2.0f));

			constexpr auto min3float3 = min_v(float3(1, 2, 3), float3(2, 3, 1), float3(3, 1, 2));
			constexpr auto max3float3 = max_v(float3(1, 2, 3), float3(2, 3, 1), float3(3, 1, 2));
			constexpr auto minmax3float3 = minmax_v(float3(1, 2, 3), float3(2, 3, 1), float3(3, 1, 2));

			constexpr auto min3float4 = min_v(float4(1, 2, 3, 1), float4(2, 3, 1, 2), float4(3, 1, 2, 3));
			constexpr auto max3float4 = max_v(float4(1, 2, 3, 1), float4(2, 3, 1, 2), float4(3, 1, 2, 3));
			constexpr auto minmax3float4 = minmax_v(float4(1, 2, 3, 1), float4(2, 3, 1, 2), float4(3, 1, 2, 3));

			constexpr auto swizzle2_2 = _<swizzle::xy>(float2(2, 1));

			constexpr auto swizzle3_2 = _<swizzle::xxy>(float2(2, 1));
			constexpr auto swizzle3_3 = _<swizzle::xxx>(float3(4, 3, 2));
			constexpr auto swizzle3_4 = _<swizzle::zwy>(float4(2, 1, 3, 4));

			constexpr auto swizzle4_2 = _<swizzle::grrg>(float2(2, 1));
			constexpr auto swizzle4_3 = _<swizzle::bgrr>(float3(4, 3, 2));
			constexpr auto swizzle4_4 = _<swizzle::rgar>(float4(2, 1, 3, 4));

			auto c = swizzle4_4[2];
		}

		{
			constexpr auto x = pow_i(2, 10);
			constexpr auto blended = lerp(float2(1, 1), float2(2, 2), 0.5f);
		}

		{
			matrix_impl::is_matrix<float2x2>::value;
			matrix_impl::is_matrix<float2x3>::value;
			matrix_impl::is_matrix<float3x3>::value;
			matrix_impl::is_matrix<float4x4>::value;
			matrix_impl::is_square_matrix<float2x2>::value;
			matrix_impl::is_square_matrix<float2x3>::value;
			matrix_impl::is_square_matrix<float3x3>::value;
			matrix_impl::is_square_matrix<float4x4>::value;
			matrix_impl::is_matrix<float4x4>::value;
			matrix_impl::is_matrix<float2>::value;
			matrix_impl::is_matrix<float3>::value;
			matrix_impl::is_matrix<float4>::value;

			std::is_pod_v<float4x4>;
			constexpr float2x2 a2x2, b2x2;
			constexpr float2x3 a2x3, b2x3;
			constexpr float3x3 a3x3, b3x3;
			constexpr float4x4 a4x4, b4x4;

			constexpr bool same2x2 = a2x2 == b2x2;
			constexpr bool same2x3 = a2x3 == b2x3;
			constexpr bool same3x3 = a3x3 == b3x3;
			constexpr bool same4x4 = a4x4 == b4x4;

			auto c2x2 = a2x2 * b2x2;
			auto c2x3 = a2x3 * b2x3;
			auto c3x3 = a3x3 * b3x3;
			auto c4x4 = a4x4;


			auto d2x2 = c2x2 * 2.0f;
			auto d2x3 = c2x3 * 2.0f;
			auto d3x3 = c3x3 * 2.0f;
			auto d4x4 = c4x4 * 2.0f;

			c2x2 *= b2x2;
			c2x3 *= b2x3;
			c3x3 *= b3x3;
			c4x4 *= b4x4;

			c2x2 *= 2.0f;
			c2x3 *= 2.0f;
			c3x3 *= 2.0f;
			c4x4 *= 2.0f;

			auto r32 = float2x3::rotate(degree<float>(45));
			auto t32 = float2x3::translate(2, 3);
			auto transform32 = t32 * r32;
			auto v = transform(transform32, point2d<float>(1.0f, 0.0f));
			inverse(transform32);
			auto translation = get_translation(transform32);

			constexpr auto zzzz = can_invert(a2x2);
		}

		{
			constexpr auto size_quaternion = sizeof(quaternion<float>);
		}

		{
			//aabb3d_f box3d(float3::zero(), float3::one());
			//aabb2d<float> box2d;
		}
	}
};