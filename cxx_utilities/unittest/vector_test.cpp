#include "stdafx.h"


TEST_CLASS(TestCXXMathVectorTGroup)
{
public:

	TEST_METHOD(cxx_math_vector_t_pod)
	{
		CXX_IS_TRUE(std::is_trivially_copyable_v<float2>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<float3>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<float4>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<double2>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<double3>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<double4>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<int2>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<int3>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<int4>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<byte2>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<byte3>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<byte4>);


		CXX_IS_TRUE(std::is_trivially_copyable_v<color3f>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<color4f>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<color3d>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<color4d>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<color3b>);
		CXX_IS_TRUE(std::is_trivially_copyable_v<color4b>);
	}

	TEST_METHOD(cxx_math_vector_t_dimension)
	{
		CXX_IEQUAL(2, static_cast<int>(float2::Dimension));
		CXX_IEQUAL(3, static_cast<int>(float3::Dimension));
		CXX_IEQUAL(4, static_cast<int>(float4::Dimension));
		CXX_IEQUAL(2, static_cast<int>(int2::Dimension));
		CXX_IEQUAL(3, static_cast<int>(int3::Dimension));
		CXX_IEQUAL(4, static_cast<int>(int4::Dimension));
		CXX_IEQUAL(2, static_cast<int>(byte2::Dimension));
		CXX_IEQUAL(3, static_cast<int>(byte3::Dimension));
		CXX_IEQUAL(4, static_cast<int>(byte4::Dimension));

		CXX_IEQUAL(3, static_cast<int>(color3f::Dimension));
		CXX_IEQUAL(4, static_cast<int>(color4f::Dimension));

		CXX_IEQUAL(3, static_cast<int>(color3d::Dimension));
		CXX_IEQUAL(4, static_cast<int>(color4d::Dimension));

		CXX_IEQUAL(3, static_cast<int>(color3b::Dimension));
		CXX_IEQUAL(4, static_cast<int>(color4b::Dimension));
	}
};