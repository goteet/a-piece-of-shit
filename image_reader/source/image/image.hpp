#include "cxx_view.h"

template<size_t bits, size_t bit_cap = bits - 1, class T>
T align_bit_div(T num_bits)
{
	return (num_bits + bit_cap) / bits;
}

template<size_t bits, class T>
T align_bits(T num_bits)
{
	return align_bit_div<bits>(num_bits) * bits;
}

template<size_t bytes, size_t bits = bytes * 8, class T>
T align_bytes(T num_bytes)
{
	return align_bit_div<bits>(num_bytes * 8) * bytes;
}

template<size_t bytes, size_t bits = bytes * 8, class T>
T line_pitch_align_bits(T width, size_t num_bits)
{
	return align_bit_div<bits>(width * num_bits) * bytes;
}

template<size_t bytes, class T>
T line_pitch_align_bytes(T width, size_t num_byte)
{
	return line_pitch_align_bits<bytes>(width, num_byte * 8);
}

constexpr uint32_t make_four_cc(const char fourcc[4])
{
	return (((((fourcc[3] << 8) | fourcc[2]) << 8) | fourcc[1]) << 8) | fourcc[0];
};

template<size_t N, class T, size_t Size = (N < sizeof(T) ? N : sizeof(T)) >
void copy_rgb_from_palette(unsigned char* dstColor, size_t index, const cxx::view_t<T>& palette)
{
	if (index < palette.size())
	{
		memcpy(dstColor, &(palette[index]), Size);
	}
	else
	{
		memset(dstColor, 0, N);
	}
}

template<size_t color_bits>
struct index_expand_helper;

template<>
struct index_expand_helper<1>
{
	static constexpr size_t count = 8;
	static constexpr unsigned char bit_mask[count] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
	static constexpr size_t shift[count] = { 7, 6, 5, 4, 3, 2, 1, 0 };
};
template<> struct index_expand_helper<2>
{
	static constexpr size_t count = 4;
	static constexpr unsigned char bit_mask[count] = { 0xC0, 0x30, 0x0C, 0x03 };
	static constexpr size_t shift[count] = { 6, 4, 2, 0 };
};

template<> struct index_expand_helper<4>
{
	static constexpr size_t count = 2;
	static constexpr unsigned char bit_mask[count] = { 0xF0, 0x0F };
	static constexpr size_t shift[count] = { 4, 0 };
};