#include "image.h"
#include "image.hpp"

enum class DXGI_FORMAT : uint32_t
{
	DXGI_FORMAT_UNKNOWN = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
	DXGI_FORMAT_R32G32B32A32_UINT = 3,
	DXGI_FORMAT_R32G32B32A32_SINT = 4,
	DXGI_FORMAT_R32G32B32_TYPELESS = 5,
	DXGI_FORMAT_R32G32B32_FLOAT = 6,
	DXGI_FORMAT_R32G32B32_UINT = 7,
	DXGI_FORMAT_R32G32B32_SINT = 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
	DXGI_FORMAT_R16G16B16A16_UNORM = 11,
	DXGI_FORMAT_R16G16B16A16_UINT = 12,
	DXGI_FORMAT_R16G16B16A16_SNORM = 13,
	DXGI_FORMAT_R16G16B16A16_SINT = 14,
	DXGI_FORMAT_R32G32_TYPELESS = 15,
	DXGI_FORMAT_R32G32_FLOAT = 16,
	DXGI_FORMAT_R32G32_UINT = 17,
	DXGI_FORMAT_R32G32_SINT = 18,
	DXGI_FORMAT_R32G8X24_TYPELESS = 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
	DXGI_FORMAT_R10G10B10A2_UNORM = 24,
	DXGI_FORMAT_R10G10B10A2_UINT = 25,
	DXGI_FORMAT_R11G11B10_FLOAT = 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
	DXGI_FORMAT_R8G8B8A8_UNORM = 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
	DXGI_FORMAT_R8G8B8A8_UINT = 30,
	DXGI_FORMAT_R8G8B8A8_SNORM = 31,
	DXGI_FORMAT_R8G8B8A8_SINT = 32,
	DXGI_FORMAT_R16G16_TYPELESS = 33,
	DXGI_FORMAT_R16G16_FLOAT = 34,
	DXGI_FORMAT_R16G16_UNORM = 35,
	DXGI_FORMAT_R16G16_UINT = 36,
	DXGI_FORMAT_R16G16_SNORM = 37,
	DXGI_FORMAT_R16G16_SINT = 38,
	DXGI_FORMAT_R32_TYPELESS = 39,
	DXGI_FORMAT_D32_FLOAT = 40,
	DXGI_FORMAT_R32_FLOAT = 41,
	DXGI_FORMAT_R32_UINT = 42,
	DXGI_FORMAT_R32_SINT = 43,
	DXGI_FORMAT_R24G8_TYPELESS = 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
	DXGI_FORMAT_R8G8_TYPELESS = 48,
	DXGI_FORMAT_R8G8_UNORM = 49,
	DXGI_FORMAT_R8G8_UINT = 50,
	DXGI_FORMAT_R8G8_SNORM = 51,
	DXGI_FORMAT_R8G8_SINT = 52,
	DXGI_FORMAT_R16_TYPELESS = 53,
	DXGI_FORMAT_R16_FLOAT = 54,
	DXGI_FORMAT_D16_UNORM = 55,
	DXGI_FORMAT_R16_UNORM = 56,
	DXGI_FORMAT_R16_UINT = 57,
	DXGI_FORMAT_R16_SNORM = 58,
	DXGI_FORMAT_R16_SINT = 59,
	DXGI_FORMAT_R8_TYPELESS = 60,
	DXGI_FORMAT_R8_UNORM = 61,
	DXGI_FORMAT_R8_UINT = 62,
	DXGI_FORMAT_R8_SNORM = 63,
	DXGI_FORMAT_R8_SINT = 64,
	DXGI_FORMAT_A8_UNORM = 65,
	DXGI_FORMAT_R1_UNORM = 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
	DXGI_FORMAT_BC1_TYPELESS = 70,
	DXGI_FORMAT_BC1_UNORM = 71,
	DXGI_FORMAT_BC1_UNORM_SRGB = 72,
	DXGI_FORMAT_BC2_TYPELESS = 73,
	DXGI_FORMAT_BC2_UNORM = 74,
	DXGI_FORMAT_BC2_UNORM_SRGB = 75,
	DXGI_FORMAT_BC3_TYPELESS = 76,
	DXGI_FORMAT_BC3_UNORM = 77,
	DXGI_FORMAT_BC3_UNORM_SRGB = 78,
	DXGI_FORMAT_BC4_TYPELESS = 79,
	DXGI_FORMAT_BC4_UNORM = 80,
	DXGI_FORMAT_BC4_SNORM = 81,
	DXGI_FORMAT_BC5_TYPELESS = 82,
	DXGI_FORMAT_BC5_UNORM = 83,
	DXGI_FORMAT_BC5_SNORM = 84,
	DXGI_FORMAT_B5G6R5_UNORM = 85,
	DXGI_FORMAT_B5G5R5A1_UNORM = 86,
	DXGI_FORMAT_B8G8R8A8_UNORM = 87,
	DXGI_FORMAT_B8G8R8X8_UNORM = 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
	DXGI_FORMAT_BC6H_TYPELESS = 94,
	DXGI_FORMAT_BC6H_UF16 = 95,
	DXGI_FORMAT_BC6H_SF16 = 96,
	DXGI_FORMAT_BC7_TYPELESS = 97,
	DXGI_FORMAT_BC7_UNORM = 98,
	DXGI_FORMAT_BC7_UNORM_SRGB = 99,
	DXGI_FORMAT_AYUV = 100,
	DXGI_FORMAT_Y410 = 101,
	DXGI_FORMAT_Y416 = 102,
	DXGI_FORMAT_NV12 = 103,
	DXGI_FORMAT_P010 = 104,
	DXGI_FORMAT_P016 = 105,
	DXGI_FORMAT_420_OPAQUE = 106,
	DXGI_FORMAT_YUY2 = 107,
	DXGI_FORMAT_Y210 = 108,
	DXGI_FORMAT_Y216 = 109,
	DXGI_FORMAT_NV11 = 110,
	DXGI_FORMAT_AI44 = 111,
	DXGI_FORMAT_IA44 = 112,
	DXGI_FORMAT_P8 = 113,
	DXGI_FORMAT_A8P8 = 114,
	DXGI_FORMAT_B4G4R4A4_UNORM = 115,
	DXGI_FORMAT_P208 = 130,
	DXGI_FORMAT_V208 = 131,
	DXGI_FORMAT_V408 = 132,
	DXGI_FORMAT_FORCE_UINT = 0xffffffff
};

enum class D3D10_RESOURCE_DIMENSION : int
{
	D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4,
	D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
	D3D10_RESOURCE_DIMENSION_BUFFER = 1,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
};

constexpr uint32_t magic_dds = 0x20534444; //'DDS'
constexpr uint32_t ddsd_caps = 0x00000001;
constexpr uint32_t ddsd_height = 0x00000002;
constexpr uint32_t ddsd_width = 0x00000004;
constexpr uint32_t ddsd_pitch = 0x00000008;
constexpr uint32_t ddsd_pixel_format = 0x00001000;
constexpr uint32_t ddsd_mipmaps_count = 0x00020000;
constexpr uint32_t ddsd_linear_size = 0x00080000;
constexpr uint32_t ddsd_depth = 0x00800000;

constexpr uint32_t dds_caps_complex = 0x00000008;
constexpr uint32_t dds_caps_mipmap = 0x00400000;
constexpr uint32_t dds_caps_texture = 0x00001000;

constexpr uint32_t ddpf_has_alpha_mask = 0x00000001;
constexpr uint32_t ddpf_alpha_channel_mask = 0x00000002;	//Alpha uncompressed data
constexpr uint32_t ddpf_use_fourcc_mask = 0x00000004;	// contains compressed RGB
constexpr uint32_t ddpf_rgb_channel_mask = 0x00000040;	// contains uncompressed RGB data
constexpr uint32_t ddpf_yuv_channel_mask = 0x00000200;	// YUV uncompressed data
constexpr uint32_t ddpf_lum_channel_mask = 0x00020000;	// uncompressed data, Can be combined with AlphaChannel for a two channel.

#define FourCC(x) constexpr uint32_t DDS_##x = make_four_cc(#x);
FourCC(DXT1);
FourCC(DXT2);
FourCC(DXT3);
FourCC(DXT4);
FourCC(DXT5);
FourCC(DX10);
#undef FourCC

#pragma pack(push)
#pragma pack(1)
struct DDSPixelFormat
{
	uint32_t Size;
	uint32_t Flags;
	uint32_t FourCC;
	uint32_t RGBBitCount;
	uint32_t RBitMask;
	uint32_t GBitMask;
	uint32_t BBitMask;
	uint32_t ABitMask;
};

struct dds_file_head
{
	uint32_t Size;
	uint32_t Flags;
	uint32_t Height;
	uint32_t Width;
	uint32_t PitchOrLinearSize;
	uint32_t Depth;
	uint32_t MipMapCount;
	uint32_t Reserved1[11];
	DDSPixelFormat ddspf;
	uint32_t Caps;
	uint32_t Caps2;
	uint32_t Caps3;
	uint32_t Caps4;
	uint32_t Reserved2;
};

struct DDSDX10Head
{
	DXGI_FORMAT dxgiFormat;
	D3D10_RESOURCE_DIMENSION resourceDimension;
	uint32_t miscFlag;
	uint32_t arraySize;
	uint32_t miscFlags2;
};

#pragma pack(pop)

bool ddpf_use_fourcc(const DDSPixelFormat& ddpf) { return (ddpf.Flags &  ddpf_use_fourcc_mask) > 0; }
bool ddpf_has_alpha(const DDSPixelFormat& ddpf) { return (ddpf.Flags &  ddpf_has_alpha_mask) > 0; }
bool ddpf_has_alpha_channel(const DDSPixelFormat& ddpf) { return (ddpf.Flags &  ddpf_alpha_channel_mask) > 0; }
bool ddpf_has_rgb_channel(const DDSPixelFormat& ddpf) { return (ddpf.Flags &  ddpf_rgb_channel_mask) > 0; }
bool ddpf_has_yuv_channel(const DDSPixelFormat& ddpf) { return (ddpf.Flags &  ddpf_yuv_channel_mask) > 0; }
bool ddpf_has_lum_channel(const DDSPixelFormat& ddpf) { return (ddpf.Flags &  ddpf_lum_channel_mask) > 0; }

pixel_format get_dds_pixel_format(const dds_file_head& header)
{
	if (ddpf_use_fourcc(header.ddspf))
	{
		switch (header.ddspf.FourCC)
		{
		case DDS_DXT1://compressed
			return pixel_format::dxt1x;
		case DDS_DXT2:
			return pixel_format::dxt2;
		case DDS_DXT3:
			return pixel_format::dxt3;
		case DDS_DXT4:
			return pixel_format::dxt4;
		case DDS_DXT5:
			return pixel_format::dxt5;
		default:
			return pixel_format::unknown;
		}
	}
	else if (ddpf_has_rgb_channel(header.ddspf))
	{
		//uncompressed rgb
		return (ddpf_has_alpha(header.ddspf))
			? pixel_format::bgra32
			: pixel_format::bgr24;
	}
	else
	{
		return pixel_format::unknown;
	}
}

int get_dds_color_space(const dds_file_head& header)
{
	if (header.ddspf.RGBBitCount == 32)
		return 4;
	else if (header.ddspf.RGBBitCount == 24)
		return 3;
	return 0;
}

bool dds_has_multi_surface(const dds_file_head& header) { return (header.Caps & dds_caps_complex) > 0; }
bool dds_used_for_mipmap(const dds_file_head& header) { return (header.Caps & dds_caps_mipmap) > 0; }
bool dds_has_pitch(const dds_file_head& header) { return (header.Flags & ddsd_pitch) > 0; }
bool dds_has_mipmap_count(const dds_file_head& header) { return (header.Flags & ddsd_mipmaps_count) > 0; }
bool dds_has_linear_size(const dds_file_head& header) { return (header.Flags & ddsd_linear_size) > 0; }
bool dds_has_depth(const dds_file_head& header) { return (header.Flags & ddsd_depth) > 0; }

bool has_mipmaps(const dds_file_head& header)
{
	return dds_has_mipmap_count(header)
		&& dds_has_multi_surface(header)
		&& header.MipMapCount > 1;
}


bool data_image_is_dds(char* file_data, size_t length)
{
	uint32_t magic = *(uint32_t*)file_data;
	if (magic != magic_dds)
		return false;

	dds_file_head* file_head = (dds_file_head*)(file_data + sizeof(magic_dds));
	return get_dds_pixel_format(*file_head) != pixel_format::unknown;
}

bool parse_image_from_dds(char* file_data, size_t length, image_data& img_data)
{
	file_data += sizeof(magic_dds);
	dds_file_head* file_head = (dds_file_head*)file_data;
	const uint32_t& width = file_head->Width;
	const uint32_t& height = file_head->Height;
	const pixel_format format = get_dds_pixel_format(*file_head);
	/*
	The D3DX library (for example, D3DX11.lib) and other similar libraries unreliably or
	"INCONSISTENTLY" provide the pitch value in the dwPitchOrLinearSize member of the DDS_HEADER structure.
	Therefore, when you read and write to DDS files,
	we recommend that you compute the pitch in one of the following ways for the indicated formats:

	- For block-compressed formats, compute the pitch as:
		  max( 1, ((width+3)/4) ) * block-size
	  The block-size is 8 bytes for DXT1, BC1, and BC4 formats, and 16 bytes for other block-compressed formats.

	- For R8G8_B8G8, G8R8_G8B8, legacy UYVY-packed,
	  and legacy YUY2-packed formats, compute the pitch as:
		  ((width+1) >> 1) * 4

	- For other formats, compute the pitch as:
		  ( width * bits-per-pixel + 7 ) / 8
	  You divide by 8 for byte alignment.

	**Note** The pitch value that you calculate "DOES NOT"
	always equal the pitch that the run0time supplies,
	which is DWORD-aligned in some situations and byte-aligned in other situations.
	Therefore, we recommend that you copy a scan line at a time rather than try to copy the whole image in one copy.
	*/
	img_data.width = width;
	img_data.height = height;
	img_data.format = format;

	bool is_compressed = false;
	unsigned int src_color_space = get_dds_color_space(*file_head);

	switch (format)
	{
	case pixel_format::dxt1x:
		img_data.pixel_stride = 8;
		img_data.line_pitch = dds_block_width(img_data) * img_data.pixel_stride;
		is_compressed = true;
		break;
	case pixel_format::dxt2:
	case pixel_format::dxt3:
	case pixel_format::dxt4:
	case pixel_format::dxt5:
		img_data.pixel_stride = 16;
		img_data.line_pitch = dds_block_width(img_data) * img_data.pixel_stride;
		is_compressed = true;
		break;
	case pixel_format::bgr24:
		if (src_color_space == 0)
		{
			return false;
		}

		img_data.line_pitch = line_pitch_align_bytes<4>(width, 3);
		img_data.pixel_stride = 3;
		break;
	case pixel_format::bgra32:
		if (src_color_space != 4)
		{
			return false;
		}
		img_data.line_pitch = line_pitch_align_bytes<4>(width, 4);
		img_data.pixel_stride = 4;
		break;
	default:
		return false;
	}

	auto data_block_ptr = (file_data + sizeof(dds_file_head));

	if (is_compressed)
	{
		img_data.resize_buffer(img_data.line_pitch * dds_block_height(img_data));
		memcpy(img_data.buffer_ptr(), data_block_ptr, img_data.buffer_length());
	}
	else if (src_color_space == img_data.pixel_stride)
	{
		img_data.resize_buffer(img_data.line_pitch * img_data.height);
		memcpy(img_data.buffer_ptr(), data_block_ptr, img_data.buffer_length());
	}
	else if (src_color_space > img_data.pixel_stride)
	{
		img_data.resize_buffer(img_data.line_pitch * img_data.height);
		auto src_pitch = line_pitch_align_bytes<4>(img_data.width, src_color_space);
		for (uint32_t row = 0; row < height; row++)
		{
			for (uint32_t col = 0; col < img_data.width; col++)
			{
				auto src_offset = data_block_ptr + row * src_pitch + col * src_color_space;
				auto dst_offset = get_pixel_offset(img_data, row, col);
				memcpy(dst_offset, src_offset, img_data.pixel_stride);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}
