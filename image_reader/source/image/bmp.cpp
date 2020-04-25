#include "image.h"
#include "image.hpp"
#include "cxx_math/cxx_color.h"

using bmp_palette = cxx::view_t<cxx::color4b>;

#pragma pack(push)
#pragma pack(2)
struct bmp_file_head
{
	uint16_t	magic_bm;	// 'B' 'M' = 0x4D42
	uint32_t	file_size;	// whole file size.
	uint16_t	reserved1;
	uint16_t	reserved2;
	uint32_t	data_offset;// in bytes. rgbÊý¾ÝÆ«ÒÆÖµ¡£
};

struct bmp_info_head
{
	uint32_t	head_size;	// =sizeof(BitmapInfoHead) =sizeof(this)
	int32_t		width;
	int32_t		height;
	uint16_t	plane;		// always = 1, compare to other format like yuv, i420, etc.
	uint16_t	data_bits;	// normally use 24or32, 1, 4 is special used for compressed.
	uint32_t	format;		// must be 0 for uncompressed here.
	uint32_t	data_length;// raw data size.
	int32_t		ppm_x;
	int32_t		ppm_y;
	uint32_t	clr_used;	//palette used
	uint32_t	clr_important;
};
#pragma pack(pop)

constexpr size_t size_bmp_file_head = sizeof(bmp_file_head);
constexpr size_t size_bmp_info_head = sizeof(bmp_info_head);
constexpr uint16_t magic_bm = 0x4D42; // 'B' 'M'

constexpr bool check_magic_bm(const bmp_file_head& head)
{
	return head.magic_bm == magic_bm;
}

constexpr bool bmp_is_uncompressed(const bmp_info_head& head)
{
	return head.format == 0;
}

constexpr bool bmp_has_palette(const bmp_info_head& head)
{
	return head.clr_used > 0;
}

constexpr bool bmp_has_alpha_channel(const bmp_info_head& head)
{
	return head.data_bits == 32;
}

constexpr size_t get_bmp_palette_count(const bmp_info_head& head)
{
	return head.clr_used;
}


void bmp_parse_scanline_truecolor(
	unsigned char * src_data,
	size_t src_pitch,
	image_data& img,
	size_t line_id,
	const bmp_palette& palette)
{
	auto src_ptr = src_data + line_id * src_pitch;
	auto dst_ptr = get_pixel_line_offset(img, img.height - line_id - 1);
	memcpy(dst_ptr, src_ptr, img.line_pitch);
}

template<size_t indexed_color_bit>
size_t expand_index(unsigned char v, size_t index)
{
	using index_expander = index_expand_helper<indexed_color_bit>;

	return (v &  index_expander::bit_mask[index])
		>> (index_expander::shift[index]);
}

template<size_t indexed_color_bit>
void bmp_parse_scanline_index_t(
	unsigned char * src_data,
	size_t src_pitch,
	image_data& img,
	size_t line_id,
	const bmp_palette& palette)
{
	using index_expander = index_expand_helper<indexed_color_bit>;

	unsigned char* src_line = src_data + line_id * src_pitch;
	unsigned char* dst_color = get_pixel_line_offset(img, img.height - line_id - 1);

	size_t width = img.width * indexed_color_bit / 8;
	for (size_t col = 0; col < width; col++)
	{
		for (uint32_t index = 0; index < index_expander::count; index++)
		{
			size_t palette_index = expand_index<indexed_color_bit>(src_line[col], index);

			copy_rgb_from_palette<3>(dst_color, palette_index, palette);
			dst_color += img.pixel_stride;
		}
	}
}

template<>
void bmp_parse_scanline_index_t<8>(
	unsigned char * src_data,
	size_t src_pitch,
	image_data& img,
	size_t line_id,
	const bmp_palette& palette)
{
	for (uint32_t col = 0; col < img.width; col++)
	{
		auto srcPtr = src_data + line_id * src_pitch + col;
		auto dstPtr = get_pixel_offset(img, img.height - line_id - 1, col);
		copy_rgb_from_palette<3>(dstPtr, *srcPtr, palette);
	}
}

bool data_image_is_bmp(char* file_data, size_t length)
{
	bmp_file_head* fileHead = (bmp_file_head*)file_data;
	if (!check_magic_bm(*fileHead))
	{
		return false;
	}

	bmp_info_head* infoHead = (bmp_info_head*)(file_data + size_bmp_file_head);
	if (bmp_is_uncompressed(*infoHead) && infoHead->plane == 1)
	{
		if (bmp_has_palette(*infoHead))
		{
			return infoHead->data_bits == 1
				|| infoHead->data_bits == 2
				|| infoHead->data_bits == 4
				|| infoHead->data_bits == 8;
		}
		else
		{
			return infoHead->data_bits == 24
				|| infoHead->data_bits == 32;
		}
	}
	else
	{
		return false;
	}
}

bool parse_image_from_bmp(char* file_data, size_t length, image_data& img_data)
{
	bmp_file_head* file_head = (bmp_file_head*)file_data;
	bmp_info_head* info_head = (bmp_info_head*)(file_data + size_bmp_file_head);

	const size_t width = info_head->width;
	const size_t height = info_head->height;
	const size_t palette_size = get_bmp_palette_count(*info_head);

	bmp_palette palette = cxx::make_view<cxx::color4b>(file_data
		+ size_bmp_file_head
		+ size_bmp_info_head,
		palette_size);

	void(*pase_scanline_func)(unsigned char*, size_t, image_data&, size_t, const bmp_palette&) = nullptr;

	// choose the right parse function.
	if (bmp_has_palette(*info_head))
	{
		switch (info_head->data_bits)
		{
		case 1:pase_scanline_func = bmp_parse_scanline_index_t<1>; break;
		case 2:pase_scanline_func = bmp_parse_scanline_index_t<2>; break;
		case 4:pase_scanline_func = bmp_parse_scanline_index_t<4>; break;
		case 8:pase_scanline_func = bmp_parse_scanline_index_t<8>; break;
		}
	}
	else
	{
		pase_scanline_func = bmp_parse_scanline_truecolor;
	}

	// fill scanline datas.
	unsigned char* src_data = ((unsigned char*)file_data + file_head->data_offset);
	size_t src_pitch = line_pitch_align_bits<4>(info_head->width, info_head->data_bits);

	if (bmp_has_alpha_channel(*info_head))
	{
		gen_image_buffer_bgra32(img_data, width, height);
	}
	else
	{
		gen_image_buffer_bgr24(img_data, width, height);
	}

	for (uint32_t row = 0; row < img_data.height; row++)
	{
		pase_scanline_func(src_data, src_pitch, img_data, row, palette);
	}

	return true;
}