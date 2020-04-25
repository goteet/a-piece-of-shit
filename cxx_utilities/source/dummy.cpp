#include "cxx_common.h"
#include "cxx_file.h"
#include "cxx_math.h"
#include <iostream>

namespace cxx
{
	void vector2_testcase()
	{
		constexpr float2 va(2, 3);

		auto nva = normalized(va);
		auto nvb = abs(nva);
		auto la = length(va);
	}

	void nvector2_testcase()
	{
		constexpr float2 va(2, 3);
		constexpr float2 vb(3, 2);
		constexpr float2 vc(2, 3);
		bool ab_equal = va == vb;
		bool ac_equal = va == vc;

		auto nva = normalized(va);
		nva = nfloat2(1, 2);

		float2 nvavalue = nva;

		auto x = nva.x;
		auto y = nva.y;

		float xv = x;
		float yv = y;
	}

	void vector2_swizzle_testcase()
	{
		constexpr float2 a(1, 2);
		constexpr float2 b(3, 4);

		constexpr float fax = a.x;

		constexpr auto swizzle1 = _<swizzle::xy>(a);

		constexpr float4 v4(5, 6, 7, 8);
		constexpr auto swizzle2 = _<swizzle::yw>(v4);
		constexpr auto swizzle3 = _<swizzle::ww>(v4);

		constexpr auto swizzle4 = a._<swizzle::xyyx>();

		constexpr float fswzzie4x = swizzle4.x;
		constexpr float fswzzie4y = swizzle4.y;
		constexpr float fswzzie4z = swizzle4.z;
		constexpr float fswzzie4w = swizzle4.w;
	}

	void aabb_testcase()
	{
		aabb2d<float> a(float2(1, 1), float2(1, 2));
		aabb2d<float> b(float2(1, 1), float2(1, 2));

		b.move_to(1, 2);
		bool i = cxx::is_intersect(a, b);
		move(a, -1, -1);
		i = is_intersect(a, b);
	}

	void ray_testcase()
	{
		ray2d<float> r2(float2(1, 2), float2(2, 1));
		ray3d<float> r3(float3(1, 2, 3), float3(2, 1, 3));

		auto p = r2.get_offset(1.5f);

		auto d2 = r2.direction();
		nfloat3 d3 = r3.direction();
		float2 x = d2 * 5.0f;

		float3 v3 = d3;
		dot(d2, float2(1, 2));
		dot(d3, float3(1, 2, 4));

	}

	void point2d_testcase()
	{
		constexpr point2d<float> a(1, 2);
		constexpr point2d<float> b(3, 4);
		constexpr point2d<float> a2(4,5 );
		constexpr auto equal_ab = (a2 == b);

		auto c = a + b;

		constexpr auto d = a - b;
		constexpr auto f1 = a * 5;
		constexpr auto f2 = 2 * a2 * 5;
	}

	int dummy()
	{
		vector2_testcase();
		nvector2_testcase();
		vector2_swizzle_testcase();
		point2d_testcase();
		aabb_testcase();
		ray_testcase();

		int a = 5;
		short b = 5;
		a = cxx::reverse_endian(a);
		b = cxx::reverse_endian(b);
		return 0;
	}
}
#ifdef CXX_BUILD_AS_EXE
int main()
{
	return cxx::dummy();
}
#endif