// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//
#pragma once

// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。

// 如果要为以前的 Windows 平台生成应用程序，请包括 WinSDKVer.h，并将
// 将 _WIN32_WINNT 宏设置为要支持的平台，然后再包括 SDKDDKVer.h。
#include <SDKDDKVer.h>

// 用于 CppUnitTest 的头文件
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