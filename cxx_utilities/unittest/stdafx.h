// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//
#pragma once

// ���� SDKDDKVer.h ��������õ���߰汾�� Windows ƽ̨��

// ���ҪΪ��ǰ�� Windows ƽ̨����Ӧ�ó�������� WinSDKVer.h������
// �� _WIN32_WINNT ������ΪҪ֧�ֵ�ƽ̨��Ȼ���ٰ��� SDKDDKVer.h��
#include <SDKDDKVer.h>

// ���� CppUnitTest ��ͷ�ļ�
#include <CppUnitTest.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include "cxx_math.h"
using namespace cxx;

static constexpr float kEpsilon = constants<float>::epsilon;

// Integer Equal Assert
#define CXX_IEQUAL(expected, actual) \
	Assert::AreEqual(expected, actual, nullptr, LINE_INFO())
#define CXX_IEQUALM(expected, actual, message) \
	Assert::AreEqual(expected, actual, L#message, LINE_INFO())

// Float Equal Assert
#define CXX_FEQUAL(expected, actual) \
	Assert::AreEqual((expected), (actual), kEpsilon, nullptr, LINE_INFO())
#define CXX_FEQUALM(expected, actual, message) \
	Assert::AreEqual((expected), (actual), kEpsilon, L#message, LINE_INFO())

// AB Assert
#define CXX_IS_TRUE(cond) \
	Assert::IsTrue((cond), nullptr, LINE_INFO())
#define CXX_IS_FALSE(cond) \
	Assert::IsFalse((cond), nullptr, LINE_INFO());
#define CXX_IS_TRUEM(cond, message) \
	Assert::IsTrue((cond), L#message, LINE_INFO())
#define CXX_IS_FALSEM(cond, message) \
	Assert::IsFalse((cond), L#message, LINE_INFO());