#include <memory.h>
#include "zlib.h"
#include "cxx_math/cxx_color.h"
#include "image.h"
#include "image.hpp"


using png_color_palette = cxx::view_t<cxx::color3b>;
using png_alpha_palette = cxx::view_t<unsigned char>;

#pragma pack(push)
#pragma pack(1)
enum class png_pixel_format : int
{
	grayscale = 0,
	unknown = 1,
	truecolor = 2,
	indexed_color = 3,
};

enum class png_filter_method : uint8_t
{
	none = 0, sub = 1, up = 2, average = 3, paeth = 4
};

struct png_file_head
{
	uint32_t magic_png;		//0x89+'PNG'
	uint32_t magic_crlf;	//CRLF
};

struct png_chunk_head
{
	uint32_t length;
	uint32_t type;
};

struct png_chunk_IHDR
{
	uint32_t width;
	uint32_t height;
	uint8_t  color_bits;
	uint8_t  format;
	uint8_t  compression;
	uint8_t  filter;
	uint8_t  interlace;
};
#pragma pack(pop)

constexpr uint32_t magic_png = 0x474E5089;
constexpr uint32_t magic_crlf = 0x0A1A0A0D;
constexpr int size_png_file_head = sizeof(png_file_head);
constexpr int size_png_chunk_head = sizeof(png_chunk_head);
constexpr int size_png_chunk_crc = sizeof(uint32_t);

#define FourCC(x) constexpr uint32_t CHUNK_##x = make_four_cc(#x);
FourCC(IHDR);
FourCC(PLTE);
FourCC(IDAT);
FourCC(IEND);
FourCC(tRNS);
#undef FourCC

constexpr bool check_magic_png(const png_file_head& head)
{
	return head.magic_png == magic_png
		&& head.magic_crlf == magic_crlf;
}

inline uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c)
{
	int p = a + b - c;
	int pa = abs(p - a);
	int pb = abs(p - b);
	int pc = abs(p - c);

	if (pa <= pb && pa <= pc)
		return a;
	if (pb <= pc)
		return b;
	return c;
}

constexpr png_pixel_format get_png_pixel_format(const png_chunk_IHDR& ihdr)
{
	return (png_pixel_format)(ihdr.format & 0x3);
}

constexpr bool png_has_alpha_channel(const png_chunk_IHDR& ihdr)
{
	return (ihdr.format & 0x4) > 0;
}

constexpr bool png_is_compressed(const png_chunk_IHDR& ihdr)
{
	return ihdr.compression != 0;
}

constexpr bool png_is_interlaced(const png_chunk_IHDR& ihdr)
{
	return ihdr.interlace != 0;
}

template<size_t bit_count>
void expand_indexed_color_data(std::vector<uint8_t>& indexed_data, uint32_t pitch, uint32_t height)
{
	using helper = index_expand_helper<bit_count>;

	size_t src_width = pitch - 1;
	size_t dst_width = src_width * 8 / bit_count;
	size_t dst_pitch = dst_width + 1;

	std::vector<uint8_t> expanded_data(dst_pitch* height);

	for (uint32_t h = 0; h < height; h++)
	{
		size_t offset = dst_pitch * h;
		uint8_t* line_data = &(indexed_data[h * pitch]);
		uint8_t* dst_index = &(expanded_data[offset + 1]);

		expanded_data[offset] = *line_data++;
		for (uint32_t w = 0; w < src_width; w++)
		{
			for (uint32_t index = 0; index < helper::count; index++)
			{
				*dst_index++ = (line_data[w] & helper::bit_mask[index]) >> (helper::shift[index]);
			}
		}
	}

	indexed_data.swap(expanded_data);
}

void expand_indexed_color_data(std::vector<uint8_t>& decoded_data, uint32_t pitch, uint32_t height, uint32_t bit_count)
{
	if (bit_count == 1)
	{
		expand_indexed_color_data<1>(decoded_data, pitch, height);
	}
	else if (bit_count == 2)
	{
		expand_indexed_color_data<2>(decoded_data, pitch, height);
	}
	else if (bit_count == 4)
	{
		expand_indexed_color_data<4>(decoded_data, pitch, height);
	}
}

bool reverse_filtering(uint8_t* data, uint32_t width, uint32_t height, uint32_t color_space)
{
	auto line_size = width * color_space;
	auto line_pitch = 1 + line_size;

	for (uint32_t i = 0; i < height; i++)
	{
		size_t index = line_pitch * i;
		uint8_t* line_data = data + 1 + index;
		png_filter_method filtering_method = (png_filter_method)data[index];
		switch (filtering_method)
		{
		case png_filter_method::sub:
			for (uint32_t j = 1; j < width; j++)
			{
				auto recon_a = line_data + (j - 1) * color_space;
				auto src_data = line_data + j * color_space;
				for (uint32_t c = 0; c < color_space; c++)
				{
					src_data[c] = (src_data[c] + recon_a[c]) & 0xFF;
				}
			}
			break;
		case png_filter_method::up:
			if (i > 0)
			{
				auto recon_b = data + line_pitch * (i - 1) + 1;
				for (uint32_t j = 0; j < line_size; j++)
				{
					line_data[j] = (line_data[j] + recon_b[j]) & 0xFF;
				}
			}
			break;
		case png_filter_method::average:
			if (i == 0)
			{
				for (uint32_t j = 1; j < width; j++)
				{
					auto recon_a = line_data + (j - 1) * color_space;
					auto src_data = line_data + j * color_space;
					for (uint32_t c = 0; c < color_space; c++)
					{
						src_data[c] = (src_data[c] + recon_a[c] / 2) & 0xFF;
					}
				}
			}
			else
			{
				auto recon_bs = data + line_pitch * (i - 1) + 1;
				for (uint32_t c = 0; c < color_space; c++)
				{
					line_data[c] = (line_data[c] + recon_bs[c] / 2) & 0xFF;
				}

				for (uint32_t j = 1; j < width; j++)
				{
					auto recon_a = line_data + (j - 1) * color_space;
					auto recon_b = recon_bs + j * color_space;
					auto src_data = line_data + j * color_space;
					for (uint32_t c = 0; c < color_space; c++)
					{
						src_data[c] = (src_data[c] + (recon_a[c] + recon_b[c]) / 2) & 0xFF;
					}
				}
			}
			break;
		case png_filter_method::paeth:
			if (i == 0)
			{
				for (uint32_t j = 1; j < width; j++)
				{
					auto recon_a = line_data + (j - 1) * color_space;
					auto src_data = line_data + j * color_space;
					for (uint32_t c = 0; c < color_space; c++)
					{
						//color[c] = (color[c] + PaethPredictor(reconA[c], 0, 0))&  0xFF;
						src_data[c] = (src_data[c] + recon_a[c]) & 0xFF;
					}
				}
			}
			else
			{
				auto recon_bs = data + line_pitch * (i - 1) + 1;
				for (uint32_t c = 0; c < color_space; c++)
				{
					//lineData[c] = (lineData[c] + PaethPredictor(0, reconBs[c], 0))&  0xFF;
					line_data[c] = (line_data[c] + recon_bs[c]) & 0xFF;
				}

				for (uint32_t j = 1; j < width; j++)
				{
					auto recon_a = line_data + (j - 1) * color_space;
					auto recon_b = recon_bs + j * color_space;
					auto recon_c = recon_bs + (j - 1) * color_space;
					auto src_data = line_data + j * color_space;
					for (uint32_t c = 0; c < color_space; c++)
					{
						src_data[c] = (src_data[c] + paeth_predictor(recon_a[c], recon_b[c], recon_c[c])) & 0xFF;
					}
				}
			}
			break;
		case png_filter_method::none:
			break;
		default:
			return false;
		}
	}
	return true;
}

struct interlace_block
{
	uint32_t width;
	uint32_t height;
	uint8_t* buffer_ptr;
	uint8_t* get_pixel(uint32_t row, uint32_t col, uint32_t colorSpace)
	{
		return buffer_ptr + 1 + row * (width * colorSpace + 1) + col * colorSpace;
	}
};

const size_t interlace_pass_count = 7;
constexpr uint32_t interlace_row_start[interlace_pass_count] = { 0, 0, 4, 0, 2, 0, 1 };
constexpr uint32_t interlace_col_start[interlace_pass_count] = { 0, 4, 0, 2, 0, 1, 0 };
constexpr uint32_t interlace_row_stride[interlace_pass_count] = { 8, 8, 8, 4, 4, 2, 2 };
constexpr uint32_t interlace_col_stride[interlace_pass_count] = { 8, 8, 4, 4, 2, 2, 1 };
constexpr uint32_t interlace_block_width[interlace_pass_count] = { 8, 4, 4, 2, 2, 1, 1 };
constexpr uint32_t interlace_block_height[interlace_pass_count] = { 8, 8, 4, 4, 2, 2, 1 };

void png_parse_scanline_truecolor(
	unsigned char* src_data,
	size_t src_pitch,
	image_data& img,
	size_t line_id,
	const png_color_palette& palette)
{
	auto src_ptr = src_data + line_id * src_pitch + 1;
	for (uint32_t col = 0; col < img.width; col++)
	{
		auto src_color = src_ptr + col * img.pixel_stride;
		auto dst_color = get_pixel_offset(img, line_id, col);
		memcpy(dst_color, src_color, img.pixel_stride);
		cxx::swap(dst_color[0], dst_color[2]);
	}
}

void png_parse_scanline_indexed8(
	unsigned char* src_data,
	size_t src_pitch,
	image_data& img,
	size_t line_id,
	const png_color_palette& palette)
{
	auto src_ptr = src_data + src_pitch * line_id + 1;
	auto dst_ptr = get_pixel_line_offset(img, line_id);
	for (uint32_t col = 0; col < img.width; col++)
	{
		auto& index = src_ptr[col];
		auto dstColor = dst_ptr + col * 3;
		copy_rgb_from_palette<3>(dstColor, index, palette);
	}
}


png_chunk_head* retrieve_chunk_head(char* chunk_ptr)
{
	return (png_chunk_head*)chunk_ptr;
}

template<class T>
T* retrieve_chunk_content(char* chunk_ptr)
{
	return (T*)(chunk_ptr + size_png_chunk_head);
}

char* move_to_next_chunk(char* chunk_ptr)
{
	png_chunk_head* chunk_head_ptr = (png_chunk_head*)chunk_ptr;

	chunk_ptr += size_png_chunk_head
		+ cxx::reverse_endian(chunk_head_ptr->length)
		+ size_png_chunk_crc;

	return chunk_ptr;
}

template<class T>
size_t expand_buffer_length(std::vector<T>& buffer, size_t expand_length)
{
	auto old_buffer_size = buffer.size();
	buffer.resize(old_buffer_size + expand_length);
	return old_buffer_size;
}

void parse_interlaced_png_indexed_x(
	unsigned char* dst_color,
	unsigned char* src_color,
	size_t color_space,
	const png_color_palette& color_palette,
	const png_alpha_palette& alpha_palette)
{
	copy_rgb_from_palette<3>(dst_color, *src_color, color_palette);
}

void parse_interlaced_png_indexed_a(
	unsigned char* dst_color,
	unsigned char* src_color,
	size_t color_space,
	const png_color_palette& color_palette,
	const png_alpha_palette& alpha_palette)
{
	copy_rgb_from_palette<3>(dst_color, *src_color, color_palette);
	dst_color[3] = alpha_palette[*src_color];
}

void parse_interlaced_png_truecolor(
	unsigned char* dst_color,
	unsigned char* src_color,
	size_t color_space,
	const png_color_palette&,
	const png_alpha_palette&)
{
	memcpy(dst_color, src_color, color_space);
	cxx::swap(dst_color[0], dst_color[2]);
}

bool find_color_datas(char* chunk_ptr, size_t data_length,
	std::vector<uint8_t>& color_data_buffer,
	cxx::color3b*& palette_ptr, size_t& palette_count,
	unsigned char*& alpha_ptr, size_t& alpha_count)
{
	char* curr_chunk_ptr = chunk_ptr;
	size_t length = 0;
	png_chunk_head* chunk_head_ptr = retrieve_chunk_head(curr_chunk_ptr);

	while (length < data_length && chunk_head_ptr->type != CHUNK_IEND)
	{
		// color table
		switch (chunk_head_ptr->type)
		{
		case CHUNK_PLTE:
			palette_ptr = retrieve_chunk_content<cxx::color3b>(curr_chunk_ptr);
			palette_count = cxx::reverse_endian(chunk_head_ptr->length) / 3;
			break;
		case CHUNK_IDAT:// compressed image data
		{
			auto idat_length = cxx::reverse_endian(chunk_head_ptr->length);
			size_t start = expand_buffer_length(color_data_buffer, idat_length);
			memcpy(&(color_data_buffer[start]), retrieve_chunk_content<uint8_t>(curr_chunk_ptr), idat_length);
			break;
		}
		case CHUNK_tRNS:
			alpha_ptr = retrieve_chunk_content<unsigned char>(curr_chunk_ptr);
			alpha_count = cxx::reverse_endian(chunk_head_ptr->length);
			break;
		}

		curr_chunk_ptr = move_to_next_chunk(curr_chunk_ptr);
		length = curr_chunk_ptr - chunk_ptr;
		chunk_head_ptr = retrieve_chunk_head(curr_chunk_ptr);
	}

	return color_data_buffer.size() > 0;
}

bool decoded_image_data(std::vector<uint8_t>& color_data_buffer, uint32_t width, uint32_t height, size_t color_space)
{
	unsigned long encoded_data_length = (unsigned long)color_data_buffer.size();
	unsigned long decoded_data_length = (width + 1) * height * color_space;
	std::vector<uint8_t> decoded_image_data(decoded_data_length);

	auto result = uncompress(
		&(decoded_image_data[0]), &decoded_data_length,
		&(color_data_buffer[0]), encoded_data_length);

	while (Z_BUF_ERROR == result)
	{
		decoded_data_length += decoded_data_length / 2;
		decoded_image_data.resize(decoded_data_length);
		result = uncompress(
			&(decoded_image_data[0]), &decoded_data_length,
			&(color_data_buffer[0]), encoded_data_length);
	}

	if (Z_OK == result)
	{
		if (decoded_image_data.size() != decoded_data_length)
			decoded_image_data.resize(decoded_data_length);

		color_data_buffer.swap(decoded_image_data);
		return true;
	}
	else
	{
		return false;
	}
}

bool data_image_is_png(char* file_data, size_t length)
{
	png_file_head* file_head = (png_file_head*)file_data;

	if (!check_magic_png(*file_head))
		return false;

	char* chunk_ptr = file_data + size_png_file_head;
	png_chunk_head* chunk_head = (png_chunk_head*)chunk_ptr;
	png_chunk_IHDR* IHDR = (png_chunk_IHDR*)(chunk_ptr + size_png_chunk_head);

	//不支持压缩格式
	if (chunk_head->type != CHUNK_IHDR ||
		png_is_compressed(*IHDR) ||
		IHDR->filter != 0)
	{
		return false;
	}

	// 判断是否支持相应格式
	switch (get_png_pixel_format(*IHDR))
	{
	case png_pixel_format::truecolor:
		return IHDR->color_bits == 8;
	case png_pixel_format::indexed_color:
		if (png_is_interlaced(*IHDR))
		{
			return IHDR->color_bits == 8;
		}
		else
		{
			return IHDR->color_bits == 1
				|| IHDR->color_bits == 2
				|| IHDR->color_bits == 4
				|| IHDR->color_bits == 8;
		}
	default:
		return false;
	}
}

bool parse_image_from_png(char* file_data, size_t length, image_data& img_data)
{
	char* chunk_ptr = file_data + size_png_file_head;
	//first is IHDR
	png_chunk_IHDR* IHDR = retrieve_chunk_content<png_chunk_IHDR>(chunk_ptr);
	chunk_ptr = move_to_next_chunk(chunk_ptr);

	// find datas
	std::vector<uint8_t> color_data_buffer;
	cxx::color3b* palette_ptr = nullptr;
	unsigned char* alpha_ptr = nullptr;
	size_t palette_count = 0;
	size_t alpha_count = 0;

	size_t remain_length = length - (chunk_ptr - file_data);
	if (!find_color_datas(chunk_ptr, remain_length,
		color_data_buffer,
		palette_ptr, palette_count,
		alpha_ptr, alpha_count))
	{
		return false;
	}

	uint32_t width = cxx::reverse_endian(IHDR->width);
	uint32_t height = cxx::reverse_endian(IHDR->height);
	png_pixel_format format = get_png_pixel_format(*IHDR);
	uint32_t color_space;

	// color_space
	if (format != png_pixel_format::indexed_color)
	{
		color_space = png_has_alpha_channel(*IHDR) ? 4 : 3;
	}
	else
	{
		if (palette_ptr == nullptr || palette_count == 0)
			return false;

		for (size_t i = 0; i < palette_count; i++)
		{
			cxx::color3b& color = palette_ptr[i];
			cxx::swap(color.r, color.b);
		}
		color_space = 1;
	}

	// LZ77 decode
	if (!decoded_image_data(color_data_buffer, width, height, color_space))
	{
		return false;
	}

	//expand bits to byte
	if (color_space == 1 && IHDR->color_bits != 8)
	{
		expand_indexed_color_data(color_data_buffer, color_data_buffer.size() / height, height, IHDR->color_bits);
	}

	auto color_palette = cxx::make_view(palette_ptr, palette_count);
	auto alpha_palette = cxx::make_view(alpha_ptr, alpha_count);

	//extract datas
	if (png_is_interlaced(*IHDR))
	{
		//interlaced
		interlace_block color_plane[interlace_pass_count];

		// reverse data filtering
		auto color_plane_start = &(color_data_buffer[0]);
		for (int pass = 0; pass < interlace_pass_count; pass++)
		{
			auto& plane = color_plane[pass];
			plane.width = (width + interlace_col_stride[pass] - 1 - interlace_col_start[pass]) / interlace_col_stride[pass];
			plane.height = (height + interlace_row_stride[pass] - 1 - interlace_row_start[pass]) / interlace_row_stride[pass];
			plane.buffer_ptr = color_plane_start;
			color_plane_start += (plane.width* color_space + 1) * plane.height;

			if (!reverse_filtering(plane.buffer_ptr, plane.width, plane.height, color_space))
			{
				return false;
			}
		}

		void(*parse_interlaced_png_image_data)(unsigned char*, unsigned char*, size_t,
			const png_color_palette&, const png_alpha_palette&) = nullptr;

		if (get_png_pixel_format(*IHDR) == png_pixel_format::indexed_color)
		{
			size_t image_data_length = color_data_buffer.size();

			if (alpha_palette.size() > 0)
			{
				gen_image_buffer_bgra32(img_data, width, height);
				parse_interlaced_png_image_data = parse_interlaced_png_indexed_a;

			}
			else
			{
				gen_image_buffer_bgr24(img_data, width, height);
				parse_interlaced_png_image_data = parse_interlaced_png_indexed_x;
			}

		}
		else
		{
			if (png_has_alpha_channel(*IHDR))
			{
				gen_image_buffer_bgra32(img_data, width, height);
			}
			else
			{
				gen_image_buffer_bgr24(img_data, width, height);
			}
			parse_interlaced_png_image_data = parse_interlaced_png_truecolor;
		}

		for (int pass = 0; pass < interlace_pass_count; pass++)
		{
			auto& plane = color_plane[pass];
			for (uint32_t h = 0; h < plane.height; h++)
			{
				auto row = interlace_row_start[pass] + h * interlace_row_stride[pass];
				for (uint32_t w = 0; w < plane.width; w++)
				{
					auto col = interlace_col_start[pass] + w * interlace_col_stride[pass];
					auto src_data = plane.get_pixel(h, w, color_space);
					auto dst_data = get_pixel_offset(img_data, row, col);

					parse_interlaced_png_image_data(
						dst_data, src_data,
						color_space,
						color_palette, alpha_palette);
				}
			}
		}

		return true;
	}
	else
	{
		if (!reverse_filtering(&(color_data_buffer[0]), width, height, color_space))
		{
			return false;
		}

		void(*png_parse_scanline_func)(unsigned char*, size_t, image_data&, size_t, const png_color_palette&) = nullptr;
		if (get_png_pixel_format(*IHDR) == png_pixel_format::indexed_color)
		{
			gen_image_buffer_bgr24(img_data, width, height);
			png_parse_scanline_func = png_parse_scanline_indexed8;
		}
		else
		{
			if (png_has_alpha_channel(*IHDR))
			{
				gen_image_buffer_bgra32(img_data, width, height);
			}
			else
			{
				gen_image_buffer_bgr24(img_data, width, height);
			}

			png_parse_scanline_func = png_parse_scanline_truecolor;
		}

		unsigned char* src_data = &(color_data_buffer[0]);
		size_t src_pitch = color_data_buffer.size() / height;

		for (uint32_t row = 0; row < height; row++)
		{
			png_parse_scanline_func(src_data, src_pitch, img_data, row, color_palette);
		}

		return true;
	}
}