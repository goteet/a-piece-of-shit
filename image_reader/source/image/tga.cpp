#include "cxx_math/cxx_color.h"
#include "image.h"
#include "image.hpp"

using tga_palette = cxx::view_t<cxx::color3b>;

enum class tga_pixel_format : int
{
	no_image = 0,		//colormap:no 
	indexed_color = 1,	//colormap:yes
	truecolor = 2,		//colormap:no 
	grayscale = 3,		//colormap:no 
};

#pragma pack(push)
#pragma pack(1)
struct tga_file_head
{
	uint8_t		id_length;        /* 00h  Size of Image ID field */
	uint8_t		color_map_type;  /* 01h  Color map type */
	uint8_t		image_type;      /* 02h  Image type code */
	uint16_t	color_map_start; /* 03h  Color map origin */
	uint16_t	color_map_length;/* 05h  Color map length */
	uint8_t		color_map_depth; /* 07h  Depth of color map entries */
	uint16_t	offset_x;        /* 08h  X origin of image */
	uint16_t	offset_y;        /* 0Ah  Y origin of image */
	uint16_t	width;           /* 0Ch  Width of image */
	uint16_t	height;          /* 0Eh  Height of image */
	uint8_t		pixel_bits;      /* 10h  Image pixel size */
	uint8_t		image_descriptor;/* 11h  Image descriptor byte */
};
#pragma pack(pop)

constexpr int size_tga_file_head = sizeof(tga_file_head);

bool tga_is_encodede(const tga_file_head& head)
{
	return (head.image_type & 0x8) > 0;
}

tga_pixel_format get_color_type(const tga_file_head& head)
{
	return (tga_pixel_format)(head.image_type & 0x7);
}

bool tga_has_palette(const tga_file_head& head)
{
	return head.color_map_type == 1 && head.color_map_length > 0;
}

bool tga_has_no_palette(const tga_file_head& head)
{
	return head.color_map_type == 0 && head.color_map_length == 0;
}

bool tga_has_alpha_channel(const tga_file_head& head)
{
	return head.pixel_bits == 32;
}

uint32_t get_tga_palette_count(const tga_file_head& head)
{
	return head.color_map_length > head.color_map_start ? head.color_map_length - head.color_map_start : 0;
}

void tga_parse_scanline_truecolor(
	unsigned char* src_data,
	size_t src_pitch,
	image_data& image,
	size_t line,
	const tga_palette& palette)
{
	unsigned char* src_ptr = src_data + line * src_pitch;
	unsigned char* dst_ptr = get_pixel_line_offset(image, image.height - line - 1);
	memcpy(dst_ptr, src_ptr, image.line_pitch);
}

void tga_parse_scanline_palette(
	unsigned char* src_data,
	size_t src_pitch,
	image_data& image,
	size_t line,
	const tga_palette& palette)
{
	unsigned char * line_data = (unsigned char*)(src_data + line * src_pitch);

	for (uint32_t col = 0; col < image.width; col++)
	{
		unsigned char * src_ptr = line_data + col;
		unsigned char* dst_ptr = get_pixel_offset(image, image.height - line - 1, col);

		copy_rgb_from_palette<3>(dst_ptr, *src_ptr, palette);
	}
}

void tga_parse_scanline_grayscale(
	unsigned char* src_data,
	size_t src_pitch,
	image_data& image,
	size_t line,
	const tga_palette& palette)
{
	unsigned char* src_ptr = src_data + line * src_pitch;
	for (uint32_t col = 0; col < image.width; col++)
	{
		unsigned char* dst_ptr = get_pixel_offset(image, image.height - line - 1, col);
		memset(dst_ptr, src_ptr[col], 3);
	}
}

bool data_image_is_tga(char* file_data, size_t length)
{
	tga_file_head* file_head = (tga_file_head*)file_data;
	if (file_head->offset_x != 0 ||
		file_head->offset_y != 0 ||
		tga_is_encodede(*file_head))
	{
		return false;
	}

	switch (get_color_type(*file_head))
	{
	case tga_pixel_format::truecolor:
		return tga_has_no_palette(*file_head)
			&& (file_head->pixel_bits == 24 || file_head->pixel_bits == 32);

	case tga_pixel_format::indexed_color:
		return tga_has_palette(*file_head)
			&& file_head->color_map_depth == 24
			&& file_head->pixel_bits == 8;

	case tga_pixel_format::grayscale:
		return file_head->pixel_bits == 8;

	default: return false;
	}
}

bool parse_image_from_tga(char* file_data, size_t length, image_data& img_data)
{
	using tga_palette = cxx::view_t<cxx::color3b>;

	tga_file_head* file_head = (tga_file_head*)file_data;
	tga_pixel_format format = get_color_type(*file_head);
	const size_t width = file_head->width;
	const size_t height = file_head->height;
	const size_t src_pitch = line_pitch_align_bits<4>(width, file_head->pixel_bits);
	const size_t palette_count = get_tga_palette_count(*file_head);

	void(*pase_scanline_func)(unsigned char*, size_t, image_data&, size_t, const tga_palette&) = nullptr;
	unsigned char* src_data = nullptr;

	tga_palette palette = cxx::make_view<cxx::color3b>(file_data
		+ size_tga_file_head
		+ file_head->id_length
		+ file_head->color_map_start * sizeof(tga_palette),
		palette_count);

	if (format == tga_pixel_format::indexed_color)
	{
		pase_scanline_func = tga_parse_scanline_palette;
		src_data = (unsigned char*)file_data
			+ size_tga_file_head
			+ file_head->id_length
			+ file_head->color_map_length * sizeof(cxx::color3b);
	}
	else if (format == tga_pixel_format::truecolor)
	{
		pase_scanline_func = tga_parse_scanline_truecolor;
		src_data = (unsigned char*)file_data
			+ size_tga_file_head
			+ file_head->id_length;
	}
	else // grayscale
	{
		pase_scanline_func = tga_parse_scanline_grayscale;
		src_data = (unsigned char*)file_data
			+ size_tga_file_head
			+ file_head->id_length;
	}

	if (tga_has_alpha_channel(*file_head))
	{
		gen_image_buffer_bgra32(img_data, width, height);
	}
	else
	{
		gen_image_buffer_bgr24(img_data, width, height);
	}

	for (uint32_t line = 0; line < height; line++)
	{
		pase_scanline_func(src_data, src_pitch, img_data, line, palette);
	}

	return true;
}