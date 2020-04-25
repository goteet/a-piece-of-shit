#include "stdafx.h"


TEST_CLASS(SwizzleTest)
{
public:
	TEST_METHOD(Vector2SwizzleTest)
	{
		float2 v2(1, 2), sw2, sw3, sw4;
		float3 v3(v2, 3);
		float4 v4(v2, 3, 4);

		sw2 = swizzle<_Y, _X>(v2);
		CXX_FEQUAL(sw2.x, v2.y);
		CXX_FEQUAL(sw2.y, v2.x);

		sw3 = swizzle<_Y, _Z>(v3);
		CXX_FEQUAL(sw3.x, v3.y);
		CXX_FEQUAL(sw3.y, v3.z);

		sw4 = swizzle<_Z, _W>(v4);
		CXX_FEQUAL(sw4.x, v4.z);
		CXX_FEQUAL(sw4.y, v4.w);

		sw2 = swizzle<_0, _1>(v4);
		CXX_FEQUAL(0.0f, sw2.x);
		CXX_FEQUAL(1.0f, sw2.y);
	}

	TEST_METHOD(Vector3SwizzleTest)
	{
		float2 v2(1, 2);
		float3 v3(v2, 3), sw2, sw3, sw4;
		float4 v4(v3, 4);

		sw2 = swizzle<_Y, _X, _Y>(v2);
		CXX_FEQUAL(sw2.x, v2.y);
		CXX_FEQUAL(sw2.y, v2.x);
		CXX_FEQUAL(sw2.z, v2.y);

		sw3 = swizzle<_Y, _Z, _X>(v3);
		CXX_FEQUAL(sw3.x, v3.y);
		CXX_FEQUAL(sw3.y, v3.z);
		CXX_FEQUAL(sw3.z, v3.x);

		sw4 = swizzle<_W, _X, _Z>(v4);
		CXX_FEQUAL(sw4.x, v4.w);
		CXX_FEQUAL(sw4.y, v4.x);
		CXX_FEQUAL(sw4.z, v4.z);

		sw2 = swizzle<_0, _1, _X>(v4);
		CXX_FEQUAL(0.0f, sw2.x);
		CXX_FEQUAL(1.0f, sw2.y);
		CXX_FEQUAL(v4.x, sw2.z);
	}

	TEST_METHOD(Vector4SwizzleTest)
	{
		float2 v2(1, 2);
		float3 v3(v2, 3);
		float4 v4(v3, 4), sw2, sw3, sw4;

		sw2 = swizzle<_Y, _X, _Y, _X>(v2);
		CXX_FEQUAL(sw2.x, v2.y);
		CXX_FEQUAL(sw2.y, v2.x);
		CXX_FEQUAL(sw2.z, v2.y);
		CXX_FEQUAL(sw2.w, v2.x);

		sw3 = swizzle<_Y, _Z, _X, _Z>(v3);
		CXX_FEQUAL(sw3.x, v3.y);
		CXX_FEQUAL(sw3.y, v3.z);
		CXX_FEQUAL(sw3.z, v3.x);
		CXX_FEQUAL(sw3.w, v3.z);

		sw4 = swizzle<_W, _X, _Z, _Y>(v4);
		CXX_FEQUAL(sw4.x, v4.w);
		CXX_FEQUAL(sw4.y, v4.x);
		CXX_FEQUAL(sw4.z, v4.z);
		CXX_FEQUAL(sw4.w, v4.y);

		sw2 = swizzle<_W, _0, _1, _X>(v4);
		CXX_FEQUAL(v4.w, sw2.x);
		CXX_FEQUAL(0.0f, sw2.y);
		CXX_FEQUAL(1.0f, sw2.z);
		CXX_FEQUAL(v4.x, sw2.w);
	}

	TEST_METHOD(Color3SwizzleTest)
	{
		color3 c3(0.0, 0.5f, 1.0f), sw3, sw4;
		color4 c4(c3, 0.3f);

		sw3 = swizzle<_B, _G, _R>(c3);
		CXX_FEQUAL(sw3.r, c3.b);
		CXX_FEQUAL(sw3.g, c3.g);
		CXX_FEQUAL(sw3.b, c3.r);

		sw4 = swizzle<_A, _R, _G>(c4);
		CXX_FEQUAL(sw4.r, c4.a);
		CXX_FEQUAL(sw4.g, c4.r);
		CXX_FEQUAL(sw4.b, c4.g);

		sw4 = swizzle<_1, _0, _G>(c4);
		CXX_FEQUAL(1.0f, sw4.r);
		CXX_FEQUAL(0.0f, sw4.g);
		CXX_FEQUAL(c4.g, sw4.b);
	}

	TEST_METHOD(Color4SwizzleTest)
	{
		color3 c3(0.0, 0.5f, 1.0f);
		color4 c4(c3, 0.3f), sw3, sw4;

		sw3 = swizzle<_B, _G, _R, _G>(c3);
		CXX_FEQUAL(sw3.r, c3.b);
		CXX_FEQUAL(sw3.g, c3.g);
		CXX_FEQUAL(sw3.b, c3.r);
		CXX_FEQUAL(sw3.a, c3.g);

		sw4 = swizzle<_A, _R, _G, _B>(c4);
		CXX_FEQUAL(sw4.r, c4.a);
		CXX_FEQUAL(sw4.g, c4.r);
		CXX_FEQUAL(sw4.b, c4.g);
		CXX_FEQUAL(sw4.a, c4.b);

		sw4 = swizzle<_1, _0, _1, _A>(c4);
		CXX_FEQUAL(1.0f, sw4.r);
		CXX_FEQUAL(0.0f, sw4.g);
		CXX_FEQUAL(1.0f, sw4.b);
		CXX_FEQUAL(c4.a, sw4.a);
	}
};