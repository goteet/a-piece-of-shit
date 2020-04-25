#include "stdafx.h"

TEST_CLASS(TestCXXMathTestDualQuaternionTGroup)
{
public:
	TEST_METHOD(cxx_math_dual_quaternion_generictest)
	{
		dual_quaternion<float> DualQualT(float3(1, 0, 0));
		dual_quaternion<float> DualQuatR(float3(0, 1, 0), degree<float>(90));
		float4x4 MatrixT = to_matrix4x4(DualQualT);
		float4x4 MatrixR = to_matrix4x4(DualQuatR);

		auto T_R = DualQualT * DualQuatR;
		auto R_T = DualQuatR * DualQualT;

		float4x4 MatrixR_T = to_matrix4x4(R_T);
		float4x4 MatrixT_R = to_matrix4x4(T_R);
		point3d<float> p(0, 0, 0);

		point3d<float> tp = transform(MatrixT, p);
		CXX_FEQUAL(1, tp.x);
		CXX_FEQUAL(0, tp.y);
		CXX_FEQUAL(0, tp.z);

		auto tret = transform(DualQualT, p);
		CXX_FEQUAL(1, tret.x);
		CXX_FEQUAL(0, tret.y);
		CXX_FEQUAL(0, tret.z);

		point3d<float> rp = transform(MatrixR, tp);
		CXX_FEQUAL(0, rp.x);
		CXX_FEQUAL(0, rp.y);
		CXX_FEQUAL(-1, rp.z);

		tret = transform(DualQuatR, tp);
		CXX_FEQUAL(0, tret.x);
		CXX_FEQUAL(0, tret.y);
		CXX_FEQUAL(-1, tret.z);

		tret = transform(T_R, p);
		CXX_FEQUAL(1, tret.x);
		CXX_FEQUAL(0, tret.y);
		CXX_FEQUAL(0, tret.z);

		auto tretm = transform(MatrixT_R, point3d<float>(p));
		CXX_FEQUAL(tret.x, tretm.x);
		CXX_FEQUAL(tret.y, tretm.y);
		CXX_FEQUAL(tret.z, tretm.z);

		tret = transform(R_T, p);
		CXX_FEQUAL(0, tret.x);
		CXX_FEQUAL(0, tret.y);
		CXX_FEQUAL(-1, tret.z);

		tretm = transform(MatrixR_T, point3d<float>(p));
		CXX_FEQUAL(tret.x, tretm.x);
		CXX_FEQUAL(tret.y, tretm.y);
		CXX_FEQUAL(tret.z, tretm.z);
	}

	TEST_METHOD(cxx_math_dual_quaternion_intializationtest)
	{
		dual_quaternion<float> dq;
		point3d<float> op = point3d<float>(-1, 0, 0);
		point3d<float> left = transform(dq, op);
		CXX_IS_TRUE(left == op);
	}
};