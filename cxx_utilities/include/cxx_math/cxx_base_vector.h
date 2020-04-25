#pragma once
#include "cxx_mathcommon.h"

namespace cxx
{
#ifdef _DEBUG
	template<dimension D>
	using vector_index = index_t<(size_t)D>;
#else
	template<dimension kDimension>
	using vector_index = size_t;
#endif

	template<typename Element, dimension D>
	struct vector_t;

	template<typename Element, dimension D>
	struct normalized_vector_t;

	template<typename E, dimension D>
	struct color_t;

	// type defines
	template<typename E> using vector2 = vector_t<E, dimension::_2>;
	template<typename E> using vector3 = vector_t<E, dimension::_3>;
	template<typename E> using vector4 = vector_t<E, dimension::_4>;

	template<typename E> using nvector2 = normalized_vector_t<E, dimension::_2>;
	template<typename E> using nvector3 = normalized_vector_t<E, dimension::_3>;

	using byte2 = vector2<unsigned char>;
	using byte3 = vector3<unsigned char>;
	using byte4 = vector4<unsigned char>;

	using int2 = vector2<int>;
	using int3 = vector3<int>;
	using int4 = vector4<int>;

	using float2 = vector2<float>;
	using float3 = vector3<float>;
	using float4 = vector4<float>;

	using double2 = vector2<double>;
	using double3 = vector3<double>;
	using double4 = vector4<double>;

	using nfloat2 = nvector2<float>;
	using nfloat3 = nvector3<float>;
	using ndouble2 = nvector2<double>;
	using ndouble3 = nvector3<double>;

	enum class swizzle : size_t
	{
		xx = 0x11, yx = 0x21, zx = 0x31, wx = 0x41, rr = 0x11, gr = 0x21, br = 0x31, ar = 0x41,
		xy = 0x12, yy = 0x22, zy = 0x32, wy = 0x42, rg = 0x12, gg = 0x22, bg = 0x32, ag = 0x42,
		xz = 0x13, yz = 0x23, zz = 0x33, wz = 0x43, rb = 0x13, gb = 0x23, bb = 0x33, ab = 0x43,
		xw = 0x14, yw = 0x24, zw = 0x34, ww = 0x44, ra = 0x14, ga = 0x24, ba = 0x34, aa = 0x44,

		xxx = 0x111, yxx = 0x211, zxx = 0x311, wxx = 0x411, rrr = 0x111, grr = 0x211, brr = 0x311, arr = 0x411,
		xxy = 0x112, yxy = 0x212, zxy = 0x312, wxy = 0x412, rrg = 0x112, grg = 0x212, brg = 0x312, arg = 0x412,
		xxz = 0x113, yxz = 0x213, zxz = 0x313, wxz = 0x413, rrb = 0x113, grb = 0x213, brb = 0x313, arb = 0x413,
		xxw = 0x114, yxw = 0x214, zxw = 0x314, wxw = 0x414, rra = 0x114, gra = 0x214, bra = 0x314, ara = 0x414,
		xyx = 0x121, yyx = 0x221, zyx = 0x321, wyx = 0x421, rgr = 0x121, ggr = 0x221, bgr = 0x321, agr = 0x421,
		xyy = 0x122, yyy = 0x222, zyy = 0x322, wyy = 0x422, rgg = 0x122, ggg = 0x222, bgg = 0x322, agg = 0x422,
		xyz = 0x123, yyz = 0x223, zyz = 0x323, wyz = 0x423, rgb = 0x123, ggb = 0x223, bgb = 0x323, agb = 0x423,
		xyw = 0x124, yyw = 0x224, zyw = 0x324, wyw = 0x424, rga = 0x124, gga = 0x224, bga = 0x324, aga = 0x424,
		xzx = 0x131, yzx = 0x231, zzx = 0x331, wzx = 0x431, rbr = 0x131, gbr = 0x231, bbr = 0x331, abr = 0x431,
		xzy = 0x132, yzy = 0x232, zzy = 0x332, wzy = 0x432, rbg = 0x132, gbg = 0x232, bbg = 0x332, abg = 0x432,
		xzz = 0x133, yzz = 0x233, zzz = 0x333, wzz = 0x433, rbb = 0x133, gbb = 0x233, bbb = 0x333, abb = 0x433,
		xzw = 0x134, yzw = 0x234, zzw = 0x334, wzw = 0x434, rba = 0x134, gba = 0x234, bba = 0x334, aba = 0x434,
		xwx = 0x141, ywx = 0x241, zwx = 0x341, wwx = 0x441, rar = 0x141, gar = 0x241, bar = 0x341, aar = 0x441,
		xwy = 0x142, ywy = 0x242, zwy = 0x342, wwy = 0x442, rag = 0x142, gag = 0x242, bag = 0x342, aag = 0x442,
		xwz = 0x143, ywz = 0x243, zwz = 0x343, wwz = 0x443, rab = 0x143, gab = 0x243, bab = 0x343, aab = 0x443,
		xww = 0x144, yww = 0x244, zww = 0x344, www = 0x444, raa = 0x144, gaa = 0x244, baa = 0x344, aaa = 0x444,

		xxxx = 0x1111, xyxx = 0x1211, xzxx = 0x1311, xwxx = 0x1411, yxxx = 0x2111, yyxx = 0x2211, yzxx = 0x2311, ywxx = 0x2411,
		xxxy = 0x1112, xyxy = 0x1212, xzxy = 0x1312, xwxy = 0x1412, yxxy = 0x2112, yyxy = 0x2212, yzxy = 0x2312, ywxy = 0x2412,
		xxxz = 0x1113, xyxz = 0x1213, xzxz = 0x1313, xwxz = 0x1413, yxxz = 0x2113, yyxz = 0x2213, yzxz = 0x2313, ywxz = 0x2413,
		xxxw = 0x1114, xyxw = 0x1214, xzxw = 0x1314, xwxw = 0x1414, yxxw = 0x2114, yyxw = 0x2214, yzxw = 0x2314, ywxw = 0x2414,
		xxyx = 0x1121, xyyx = 0x1221, xzyx = 0x1321, xwyx = 0x1421, yxyx = 0x2121, yyyx = 0x2221, yzyx = 0x2321, ywyx = 0x2421,
		xxyy = 0x1122, xyyy = 0x1222, xzyy = 0x1322, xwyy = 0x1422, yxyy = 0x2122, yyyy = 0x2222, yzyy = 0x2322, ywyy = 0x2422,
		xxyz = 0x1123, xyyz = 0x1223, xzyz = 0x1323, xwyz = 0x1423, yxyz = 0x2123, yyyz = 0x2223, yzyz = 0x2323, ywyz = 0x2423,
		xxyw = 0x1124, xyyw = 0x1224, xzyw = 0x1324, xwyw = 0x1424, yxyw = 0x2124, yyyw = 0x2224, yzyw = 0x2324, ywyw = 0x2424,
		xxzx = 0x1131, xyzx = 0x1231, xzzx = 0x1331, xwzx = 0x1431, yxzx = 0x2131, yyzx = 0x2231, yzzx = 0x2331, ywzx = 0x2431,
		xxzy = 0x1132, xyzy = 0x1232, xzzy = 0x1332, xwzy = 0x1432, yxzy = 0x2132, yyzy = 0x2232, yzzy = 0x2332, ywzy = 0x2432,
		xxzz = 0x1133, xyzz = 0x1233, xzzz = 0x1333, xwzz = 0x1433, yxzz = 0x2133, yyzz = 0x2233, yzzz = 0x2333, ywzz = 0x2433,
		xxzw = 0x1134, xyzw = 0x1234, xzzw = 0x1334, xwzw = 0x1434, yxzw = 0x2134, yyzw = 0x2234, yzzw = 0x2334, ywzw = 0x2434,
		xxwx = 0x1141, xywx = 0x1241, xzwx = 0x1341, xwwx = 0x1441, yxwx = 0x2141, yywx = 0x2241, yzwx = 0x2341, ywwx = 0x2441,
		xxwy = 0x1142, xywy = 0x1242, xzwy = 0x1342, xwwy = 0x1442, yxwy = 0x2142, yywy = 0x2242, yzwy = 0x2342, ywwy = 0x2442,
		xxwz = 0x1143, xywz = 0x1243, xzwz = 0x1343, xwwz = 0x1443, yxwz = 0x2143, yywz = 0x2243, yzwz = 0x2343, ywwz = 0x2443,
		xxww = 0x1144, xyww = 0x1244, xzww = 0x1344, xwww = 0x1444, yxww = 0x2144, yyww = 0x2244, yzww = 0x2344, ywww = 0x2444,

		zxxx = 0x3111, zyxx = 0x3211, zzxx = 0x3311, zwxx = 0x3411, wxxx = 0x4111, wyxx = 0x4211, wzxx = 0x4311, wwxx = 0x4411,
		zxxy = 0x3112, zyxy = 0x3212, zzxy = 0x3312, zwxy = 0x3412, wxxy = 0x4112, wyxy = 0x4212, wzxy = 0x4312, wwxy = 0x4412,
		zxxz = 0x3113, zyxz = 0x3213, zzxz = 0x3313, zwxz = 0x3413, wxxz = 0x4113, wyxz = 0x4213, wzxz = 0x4313, wwxz = 0x4413,
		zxxw = 0x3114, zyxw = 0x3214, zzxw = 0x3314, zwxw = 0x3414, wxxw = 0x4114, wyxw = 0x4214, wzxw = 0x4314, wwxw = 0x4414,
		zxyx = 0x3121, zyyx = 0x3221, zzyx = 0x3321, zwyx = 0x3421, wxyx = 0x4121, wyyx = 0x4221, wzyx = 0x4321, wwyx = 0x4421,
		zxyy = 0x3122, zyyy = 0x3222, zzyy = 0x3322, zwyy = 0x3422, wxyy = 0x4122, wyyy = 0x4222, wzyy = 0x4322, wwyy = 0x4422,
		zxyz = 0x3123, zyyz = 0x3223, zzyz = 0x3323, zwyz = 0x3423, wxyz = 0x4123, wyyz = 0x4223, wzyz = 0x4323, wwyz = 0x4423,
		zxyw = 0x3124, zyyw = 0x3224, zzyw = 0x3324, zwyw = 0x3424, wxyw = 0x4124, wyyw = 0x4224, wzyw = 0x4324, wwyw = 0x4424,
		zxzx = 0x3131, zyzx = 0x3231, zzzx = 0x3331, zwzx = 0x3431, wxzx = 0x4131, wyzx = 0x4231, wzzx = 0x4331, wwzx = 0x4431,
		zxzy = 0x3132, zyzy = 0x3232, zzzy = 0x3332, zwzy = 0x3432, wxzy = 0x4132, wyzy = 0x4232, wzzy = 0x4332, wwzy = 0x4432,
		zxzz = 0x3133, zyzz = 0x3233, zzzz = 0x3333, zwzz = 0x3433, wxzz = 0x4133, wyzz = 0x4233, wzzz = 0x4333, wwzz = 0x4433,
		zxzw = 0x3134, zyzw = 0x3234, zzzw = 0x3334, zwzw = 0x3434, wxzw = 0x4134, wyzw = 0x4234, wzzw = 0x4334, wwzw = 0x4434,
		zxwx = 0x3141, zywx = 0x3241, zzwx = 0x3341, zwwx = 0x3441, wxwx = 0x4141, wywx = 0x4241, wzwx = 0x4341, wwwx = 0x4441,
		zxwy = 0x3142, zywy = 0x3242, zzwy = 0x3342, zwwy = 0x3442, wxwy = 0x4142, wywy = 0x4242, wzwy = 0x4342, wwwy = 0x4442,
		zxwz = 0x3143, zywz = 0x3243, zzwz = 0x3343, zwwz = 0x3443, wxwz = 0x4143, wywz = 0x4243, wzwz = 0x4343, wwwz = 0x4443,
		zxww = 0x3144, zyww = 0x3244, zzww = 0x3344, zwww = 0x3444, wxww = 0x4144, wyww = 0x4244, wzww = 0x4344, wwww = 0x4444,

		rrrr = 0x1111, rgrr = 0x1211, rbrr = 0x1311, rarr = 0x1411, grrr = 0x2111, ggrr = 0x2211, gbrr = 0x2311, garr = 0x2411,
		rrrg = 0x1112, rgrg = 0x1212, rbrg = 0x1312, rarg = 0x1412, grrg = 0x2112, ggrg = 0x2212, gbrg = 0x2312, garg = 0x2412,
		rrrb = 0x1113, rgrb = 0x1213, rbrb = 0x1313, rarb = 0x1413, grrb = 0x2113, ggrb = 0x2213, gbrb = 0x2313, garb = 0x2413,
		rrra = 0x1114, rgra = 0x1214, rbra = 0x1314, rara = 0x1414, grra = 0x2114, ggra = 0x2214, gbra = 0x2314, gara = 0x2414,
		rrgr = 0x1121, rggr = 0x1221, rbgr = 0x1321, ragr = 0x1421, grgr = 0x2121, gggr = 0x2221, gbgr = 0x2321, gagr = 0x2421,
		rrgg = 0x1122, rggg = 0x1222, rbgg = 0x1322, ragg = 0x1422, grgg = 0x2122, gggg = 0x2222, gbgg = 0x2322, gagg = 0x2422,
		rrgb = 0x1123, rggb = 0x1223, rbgb = 0x1323, ragb = 0x1423, grgb = 0x2123, gggb = 0x2223, gbgb = 0x2323, gagb = 0x2423,
		rrga = 0x1124, rgga = 0x1224, rbga = 0x1324, raga = 0x1424, grga = 0x2124, ggga = 0x2224, gbga = 0x2324, gaga = 0x2424,
		rrbr = 0x1131, rgbr = 0x1231, rbbr = 0x1331, rabr = 0x1431, grbr = 0x2131, ggbr = 0x2231, gbbr = 0x2331, gabr = 0x2431,
		rrbg = 0x1132, rgbg = 0x1232, rbbg = 0x1332, rabg = 0x1432, grbg = 0x2132, ggbg = 0x2232, gbbg = 0x2332, gabg = 0x2432,
		rrbb = 0x1133, rgbb = 0x1233, rbbb = 0x1333, rabb = 0x1433, grbb = 0x2133, ggbb = 0x2233, gbbb = 0x2333, gabb = 0x2433,
		rrba = 0x1134, rgba = 0x1234, rbba = 0x1334, raba = 0x1434, grba = 0x2134, ggba = 0x2234, gbba = 0x2334, gaba = 0x2434,
		rrar = 0x1141, rgar = 0x1241, rbar = 0x1341, raar = 0x1441, grar = 0x2141, ggar = 0x2241, gbar = 0x2341, gaar = 0x2441,
		rrag = 0x1142, rgag = 0x1242, rbag = 0x1342, raag = 0x1442, grag = 0x2142, ggag = 0x2242, gbag = 0x2342, gaag = 0x2442,
		rrab = 0x1143, rgab = 0x1243, rbab = 0x1343, raab = 0x1443, grab = 0x2143, ggab = 0x2243, gbab = 0x2343, gaab = 0x2443,
		rraa = 0x1144, rgaa = 0x1244, rbaa = 0x1344, raaa = 0x1444, graa = 0x2144, ggaa = 0x2244, gbaa = 0x2344, gaaa = 0x2444,

		brrr = 0x3111, bgrr = 0x3211, bbrr = 0x3311, barr = 0x3411, arrr = 0x4111, agrr = 0x4211, abrr = 0x4311, aarr = 0x4411,
		brrg = 0x3112, bgrg = 0x3212, bbrg = 0x3312, barg = 0x3412, arrg = 0x4112, agrg = 0x4212, abrg = 0x4312, aarg = 0x4412,
		brrb = 0x3113, bgrb = 0x3213, bbrb = 0x3313, barb = 0x3413, arrb = 0x4113, agrb = 0x4213, abrb = 0x4313, aarb = 0x4413,
		brra = 0x3114, bgra = 0x3214, bbra = 0x3314, bara = 0x3414, arra = 0x4114, agra = 0x4214, abra = 0x4314, aara = 0x4414,
		brgr = 0x3121, bggr = 0x3221, bbgr = 0x3321, bagr = 0x3421, argr = 0x4121, aggr = 0x4221, abgr = 0x4321, aagr = 0x4421,
		brgg = 0x3122, bggg = 0x3222, bbgg = 0x3322, bagg = 0x3422, argg = 0x4122, aggg = 0x4222, abgg = 0x4322, aagg = 0x4422,
		brgb = 0x3123, bggb = 0x3223, bbgb = 0x3323, bagb = 0x3423, argb = 0x4123, aggb = 0x4223, abgb = 0x4323, aagb = 0x4423,
		brga = 0x3124, bgga = 0x3224, bbga = 0x3324, baga = 0x3424, arga = 0x4124, agga = 0x4224, abga = 0x4324, aaga = 0x4424,
		brbr = 0x3131, bgbr = 0x3231, bbbr = 0x3331, babr = 0x3431, arbr = 0x4131, agbr = 0x4231, abbr = 0x4331, aabr = 0x4431,
		brbg = 0x3132, bgbg = 0x3232, bbbg = 0x3332, babg = 0x3432, arbg = 0x4132, agbg = 0x4232, abbg = 0x4332, aabg = 0x4432,
		brbb = 0x3133, bgbb = 0x3233, bbbb = 0x3333, babb = 0x3433, arbb = 0x4133, agbb = 0x4233, abbb = 0x4333, aabb = 0x4433,
		brba = 0x3134, bgba = 0x3234, bbba = 0x3334, baba = 0x3434, arba = 0x4134, agba = 0x4234, abba = 0x4334, aaba = 0x4434,
		brar = 0x3141, bgar = 0x3241, bbar = 0x3341, baar = 0x3441, arar = 0x4141, agar = 0x4241, abar = 0x4341, aaar = 0x4441,
		brag = 0x3142, bgag = 0x3242, bbag = 0x3342, baag = 0x3442, arag = 0x4142, agag = 0x4242, abag = 0x4342, aaag = 0x4442,
		brab = 0x3143, bgab = 0x3243, bbab = 0x3343, baab = 0x3443, arab = 0x4143, agab = 0x4243, abab = 0x4343, aaab = 0x4443,
		braa = 0x3144, bgaa = 0x3244, bbaa = 0x3344, baaa = 0x3444, araa = 0x4144, agaa = 0x4244, abaa = 0x4344, aaaa = 0x4444,
	};

	namespace vector_impl
	{
		enum class hint { is_normalized };

		template<typename E, dimension D, size_t Index>
		class const_vector_component_t
		{
			friend struct normalized_vector_t<E, D>;

			vector_t<E, D> value;

			constexpr const_vector_component_t() = default;

			constexpr void operator=(const const_vector_component_t& rhs) { value = rhs.value; }

		public:
			constexpr const_vector_component_t(const const_vector_component_t&) = default;

			inline operator const E&() const { return value.v[Index]; }
		};

		template<typename E>
		constexpr void minmax_element(E& minv, E& maxv)
		{
			if (minv > maxv) swap(minv, maxv);
		}

		template<typename E>
		constexpr void minmax_element(E f0, E f1, E f2, E& outMin, E& outMax)
		{
			if (f0 < f1)
			{
				if (f0 < f2)
				{
					outMin = f0; outMax = max2(f1, f2);
				}
				else
				{
					outMin = f2; outMax = f1;
				}
			}
			else
			{
				if (f1 < f2)
				{
					outMin = f1; outMax = max2(f0, f2);
				}
				else
				{
					outMin = f2; outMax = f0;
				}
			}
		}
	}

	namespace swizzle_impl
	{
		template<typename E, dimension D, size_t>
		struct vector_component_accessor;

		template<typename E, dimension D>
		struct vector_component_accessor<E, D, 1>
		{
			static constexpr E get(const vector_t<E, D>& _v) { return _v.x; }
			static constexpr E get(const color_t<E, D>& _v) { return _v.value.x; }
		};

		template<typename E, dimension D>
		struct vector_component_accessor<E, D, 2>
		{
			static constexpr E get(const vector_t<E, D>& _v) { return _v.y; }
			static constexpr E get(const color_t<E, D>& _v) { return _v.value.y; }
		};

		template<typename E, dimension D>
		struct vector_component_accessor<E, D, 3>
		{
			static constexpr E get(const vector_t<E, D>& _v) { return _v.z; }
			static constexpr E get(const color_t<E, D>& _v) { return _v.value.z; }
		};

		template<typename E, dimension D>
		struct vector_component_accessor<E, D, 4>
		{
			static constexpr E get(const vector_t<E, D>& _v) { return _v.w; }
			static constexpr E get(const color_t<E, D>& _v) { return _v.value.w; }
		};

		template<dimension Dimension>
		static constexpr bool swizzle_component_support(size_t index)
		{
			return index <= Dimension;
		}

		template<dimension D, size_t Swizzle0, size_t Swizzle1>
		struct swizzle_unpack2
		{
			static const dimension Dimension = dimension::_2;

			static const bool support =
				swizzle_component_support<D>(Swizzle0) &&
				swizzle_component_support<D>(Swizzle1);

			template<typename E>
			static constexpr vector_t<E, Dimension> get(const vector_t<E, D>& v)
			{
				return vector_t<E, Dimension>(
					vector_component_accessor<E, D, Swizzle0>::get(v),
					vector_component_accessor<E, D, Swizzle1>::get(v)
					);
			}
		};
		template<dimension D, size_t Swizzle0, size_t Swizzle1, size_t Swizzle2>
		struct swizzle_unpack3
		{
			static const dimension Dimension = dimension::_3;

			static const bool support =
				swizzle_component_support<D>(Swizzle0) &&
				swizzle_component_support<D>(Swizzle1) &&
				swizzle_component_support<D>(Swizzle2);

			template<typename E>
			static constexpr vector_t<E, Dimension> get(const vector_t<E, D>& v)
			{
				return vector_t<E, Dimension>(
					vector_component_accessor<E, D, Swizzle0>::get(v),
					vector_component_accessor<E, D, Swizzle1>::get(v),
					vector_component_accessor<E, D, Swizzle2>::get(v)
					);
			}
		};

		template<dimension D, size_t Swizzle0, size_t Swizzle1, size_t Swizzle2, size_t Swizzle3>
		struct swizzle_unpack4
		{
			static const dimension Dimension = dimension::_4;

			static const bool support =
				swizzle_component_support<D>(Swizzle0) &&
				swizzle_component_support<D>(Swizzle1) &&
				swizzle_component_support<D>(Swizzle2) &&
				swizzle_component_support<D>(Swizzle3);

			template<typename E>
			static constexpr vector_t<E, Dimension> get(const vector_t<E, D>& v)
			{
				return vector_t<E, Dimension>(
					vector_component_accessor<E, D, Swizzle0>::get(v),
					vector_component_accessor<E, D, Swizzle1>::get(v),
					vector_component_accessor<E, D, Swizzle2>::get(v),
					vector_component_accessor<E, D, Swizzle3>::get(v)
					);
			}
		};

		template<dimension D, size_t Swizzle1, size_t Swizzle2>
		struct swizzle_unpack3<D, 0, Swizzle1, Swizzle2>
			: public swizzle_unpack2<D, Swizzle1, Swizzle2>
		{ };

		template<dimension D, size_t Swizzle1, size_t Swizzle2, size_t Swizzle3>
		struct swizzle_unpack4<D, 0, Swizzle1, Swizzle2, Swizzle3>
			: public swizzle_unpack3<D, Swizzle1, Swizzle2, Swizzle3>
		{ };

		template<swizzle AlignSwizzle, dimension D>
		struct swz_unpacker : public swizzle_unpack4 <D,
			(((unsigned int)AlignSwizzle & 0xF000) >> 12),
			(((unsigned int)AlignSwizzle & 0x0F00) >> 8),
			(((unsigned int)AlignSwizzle & 0x00F0) >> 4),
			((unsigned int)AlignSwizzle & 0x000F)>
		{ };
	}

	template<swizzle T, typename E, dimension D
		, typename = std::enable_if_t<swizzle_impl::swz_unpacker<T, D>::support>>
		constexpr vector_t<E, swizzle_impl::swz_unpacker<T, D>::Dimension> _(const vector_t<E, D>& v)
	{
		return swizzle_impl::swz_unpacker<T, D>::get(v);
	}

	template<swizzle T, typename E, dimension D
		, typename = std::enable_if_t<swizzle_impl::swz_unpacker<T, D>::support>>
		constexpr vector_t<E, swizzle_impl::swz_unpacker<T, D>::Dimension> swz(const vector_t<E, D>& v)
	{
		return swizzle_impl::swz_unpacker<T, D>::get(v);
	}

	template<swizzle T, typename E, dimension D
		, typename = std::enable_if_t<swizzle_impl::swz_unpacker<T, D>::support>>
		constexpr vector_t<E, swizzle_impl::swz_unpacker<T, D>::Dimension> _(const normalized_vector_t<E, D>& v)
	{
		return swizzle_impl::swz_unpacker<T, D>::get(v.value);
	}

	template<swizzle T, typename E, dimension D
		, typename = std::enable_if_t<swizzle_impl::swz_unpacker<T, D>::support>>
		constexpr vector_t<E, swizzle_impl::swz_unpacker<T, D>::Dimension> swz(const normalized_vector_t<E, D>& v)
	{
		return swizzle_impl::swz_unpacker<T, D>::get(v.value);
	}

	template<swizzle T, typename E, dimension D
		, typename = std::enable_if_t<swizzle_impl::swz_unpacker<T, D>::support>>
		constexpr vector_t<E, swizzle_impl::swz_unpacker<T, D>::Dimension> _(const color_t<E, D>& v)
	{
		return swizzle_impl::swz_unpacker<T, D>::get(v.value);
	}

	template<swizzle T, typename E, dimension D
		, typename = std::enable_if_t<swizzle_impl::swz_unpacker<T, D>::support>>
		constexpr vector_t<E, swizzle_impl::swz_unpacker<T, D>::Dimension> swz(const color_t<E, D>& v)
	{
		return swizzle_impl::swz_unpacker<T, D>::get(v.value);
	}
}