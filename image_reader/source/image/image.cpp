#include <algorithm>
#include "image.h"

image_data::image_data(image_data && other)
	: format(other.format)
	, line_pitch(other.line_pitch)
	, width(other.width)
	, height(other.height)
	, pixel_stride(other.pixel_stride)
	, buffer(std::move(other.buffer))
{		}

image_data & image_data::operator=(image_data && other)
{
	format = other.format;
	line_pitch = other.line_pitch;
	width = other.width;
	height = other.height;
	pixel_stride = other.pixel_stride;
	buffer = std::move(other.buffer);
	return *this;
}

size_t dds_block_width(const image_data & image)
{
	return std::max<size_t>(1, (image.width + 3) / 4);
}

size_t dds_block_height(const image_data & image)
{
	return std::max<size_t>(1, (image.height + 3) / 4);
}

void gen_image_buffer_bgr24(image_data & image, size_t width, size_t height)
{
	image.format = pixel_format::bgr24;
	image.pixel_stride = 3;
	image.width = width;
	image.height = height;
	image.line_pitch = image.pixel_stride * width;
	image.resize_buffer(image.line_pitch * image.height);
}

void gen_image_buffer_bgra32(image_data & image, size_t width, size_t height)
{
	image.format = pixel_format::bgra32;
	image.pixel_stride = 4;
	image.width = width;
	image.height = height;
	image.line_pitch = image.pixel_stride * width;
	image.resize_buffer(image.line_pitch * image.height);
}

bool parse_image(char* data, size_t length, image_data& img_data)
{
	if (data_image_is_dds(data, length)) return parse_image_from_dds(data, length, img_data);
	if (data_image_is_png(data, length)) return parse_image_from_png(data, length, img_data);
	if (data_image_is_bmp(data, length)) return parse_image_from_bmp(data, length, img_data);
	if (data_image_is_tga(data, length)) return parse_image_from_tga(data, length, img_data);
	return false;
}
unsigned char* get_pixel_offset(image_data & img, size_t line_id, size_t column_id)
{
	return img.buffer_ptr() + img.line_pitch * line_id + column_id * img.pixel_stride;
}
unsigned char* get_pixel_line_offset(image_data & img, size_t line_id)
{
	return img.buffer_ptr() + img.line_pitch * line_id;
}
unsigned char* get_block_line_offset(image_data & img, size_t line_id)
{
	return img.buffer_ptr() + img.line_pitch * line_id;
}