#pragma once
#include <vector>
#include <stdint.h>

enum class pixel_format : int
{
	unknown,// unsupported data.
	bgr24,	// 0xBBGGRR
	bgra32,	// 0xBBGGRRAA
	dxt1x,	// BC1 without alpha channel
	dxt1a,	// BC1 with alpha channel
	dxt2,	// BC2 color data is premultiplied by alpha
	dxt3,	// BC2 color data is not premultiplied by alpha
	dxt4,	// BC3 color data is premultiplied by alpha
	dxt5,	// BC3 color data is not premultiplied by alpha
};

enum class image_type
{
	bmp, tga, dds, png, err
};

class image_data
{
	std::vector<unsigned char> buffer;
public:
	pixel_format format = pixel_format::unknown;
	size_t line_pitch = 0;
	size_t width = 0;
	size_t height = 0;
	size_t pixel_stride = 0;

	image_data() = default;

	image_data(image_data&& other);

	void resize_buffer(size_t length) { buffer.resize(length); }

	operator bool() const { return format != pixel_format::unknown && buffer.size() > 0; }

	image_data& operator= (image_data&& other);

	size_t buffer_length() const { return buffer.size(); }

	unsigned char* buffer_ptr() { return &(buffer[0]); }

	const unsigned char* buffer_ptr() const { return &(buffer[0]); }
};

bool parse_image(char* data, size_t length, image_data& img_data);

size_t dds_block_width(const image_data& image);

size_t dds_block_height(const image_data& image);

void gen_image_buffer_bgr24(image_data& image, size_t width, size_t height);

void gen_image_buffer_bgra32(image_data& image, size_t width, size_t height);

unsigned char* get_pixel_offset(image_data& img, size_t line_id, size_t column_id);

unsigned char* get_pixel_line_offset(image_data& img, size_t line_id);

unsigned char* get_block_line_offset(image_data& img, size_t line_id);

bool data_image_is_bmp(char*, size_t);

bool data_image_is_tga(char*, size_t);

bool data_image_is_png(char*, size_t);

bool data_image_is_dds(char*, size_t);

bool parse_image_from_bmp(char*, size_t, image_data& img_data);

bool parse_image_from_tga(char*, size_t, image_data& img_data);

bool parse_image_from_png(char*, size_t, image_data& img_data);

bool parse_image_from_dds(char*, size_t, image_data& img_data);