#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <d3d9.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include "image.h"
#include "cxx_file.h"
#include <chrono>
#include "../task_graph/task_graph.h"

extern HWND hWindow;
namespace
{
	IDirect3DDevice9* d3dDevice = nullptr;
	IDirect3DVertexBuffer9* d3dVertexBuffer = nullptr;
	D3DMATRIX identityMatrix;

	struct ImageTexture
	{
		std::string name;
		IDirect3DTexture9* texture;
		uint32_t width;
		uint32_t height;
	};
	std::vector<ImageTexture> textures;
	int currentTexture;
}

struct ImageVertex
{
	float x, y, z;
	DWORD color;
	float u, v;
};

constexpr DWORD CUSTOM_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

void CenterWindow(HWND hWnd, int width, int height)
{
	RECT clientRect;
	RECT windowRect;
	::GetClientRect(hWnd, &clientRect);
	::GetWindowRect(hWnd, &windowRect);


	int borderWidth = (windowRect.right - windowRect.left - clientRect.right) / 2;
	int borderHeight = (windowRect.bottom - windowRect.top - clientRect.bottom) / 2;

	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi);
	int centerX = (mi.rcWork.right + mi.rcWork.left) / 2;
	int centerY = (mi.rcWork.top + mi.rcWork.bottom) / 2;

	int halfWidth = width / 2 + borderWidth;
	int halfHeight = height / 2 + borderHeight;
	if (halfWidth > centerX)halfWidth = centerX;
	if (halfHeight > centerY)halfHeight = centerY;

	width = (width + 2 * borderWidth) * 2;
	height = (height + 2 * borderHeight) * 2;
	::SetWindowPos(hWnd, NULL,
		centerX - halfWidth,
		centerY - halfHeight,
		width, height,
		SWP_SHOWWINDOW);
}
void NextImage()
{
	//用来测试能拉起work thread的代码
	/*
	Task::StartTask(ThreadName::WorkThread, [](Task&) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		std::stringstream ostre;
		ostre << std::this_thread::get_id();
		std::string x = ostre.str();
		::MessageBoxA(NULL, x.c_str(), NULL, MB_OK);
	});
	*/

	currentTexture = (currentTexture + 1) % textures.size();
	CenterWindow(hWindow, textures[currentTexture].width, textures[currentTexture].height);
	::SetWindowTextA(hWindow, textures[currentTexture].name.c_str());
}
struct ImageLoader
{
	ImageLoader(const char* path) :img_path(path)
	{
	}

	int load_file = 2;
	int parse_img = 2;
	std::string img_path;
	cxx::file_buffer img_file;
	image_data img_data;

	IDirect3DTexture9* d3dTexture = nullptr;

	Task firstTask;
	Task finalTask;
};
bool Initialize(HWND hWnd)
{
	srand(static_cast<unsigned>(time(0)));
	std::vector<ImageLoader*> imageFileList;
	imageFileList.push_back(new ImageLoader{ "palette8_100x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "palette8_128x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "palette4_100x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "palette4_128x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "rgb_100x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "rgb_128x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "rgba_100x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "rgba_128x128.bmp" });
	imageFileList.push_back(new ImageLoader{ "grayscale_100x128.tga" });
	imageFileList.push_back(new ImageLoader{ "grayscale_128x128.tga" });
	imageFileList.push_back(new ImageLoader{ "palette_100x128.tga" });
	imageFileList.push_back(new ImageLoader{ "palette_128x128.tga" });
	imageFileList.push_back(new ImageLoader{ "rgb_100x128.tga" });
	imageFileList.push_back(new ImageLoader{ "rgb_128x128.tga" });
	imageFileList.push_back(new ImageLoader{ "rgba_100x128.tga" });
	imageFileList.push_back(new ImageLoader{ "rgba_128x128.tga" });
	imageFileList.push_back(new ImageLoader{ "palette1.png" });
	imageFileList.push_back(new ImageLoader{ "palette2.png" });
	imageFileList.push_back(new ImageLoader{ "palette4.png" });
	imageFileList.push_back(new ImageLoader{ "palette_100x128_interlaced.png" });
	imageFileList.push_back(new ImageLoader{ "palette_128x128_interlaced.png" });
	imageFileList.push_back(new ImageLoader{ "rgb_100x128_interlaced.png" });
	imageFileList.push_back(new ImageLoader{ "rgb_128x128_interlaced.png" });
	imageFileList.push_back(new ImageLoader{ "rgba_100x128_interlaced.png" });
	imageFileList.push_back(new ImageLoader{ "rgba_128x128_interlaced.png" });
	imageFileList.push_back(new ImageLoader{ "palette_100x128.png" });
	imageFileList.push_back(new ImageLoader{ "palette_128x128.png" });
	imageFileList.push_back(new ImageLoader{ "rgb_100x128.png" });
	imageFileList.push_back(new ImageLoader{ "rgb_128x128.png" });
	imageFileList.push_back(new ImageLoader{ "rgba_100x128.png" });
	imageFileList.push_back(new ImageLoader{ "rgba_128x128.png" });
	imageFileList.push_back(new ImageLoader{ "dxt1_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt1_128x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt1a_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt1a_128x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt3_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt3_128x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt3a_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt3a_128x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt5_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt5_128x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt5a_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "dxt5a_128x128.dds" });
	imageFileList.push_back(new ImageLoader{ "rgb_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "rgb_128x128.dds" });
	imageFileList.push_back(new ImageLoader{ "rgba_100x128.dds" });
	imageFileList.push_back(new ImageLoader{ "rgba_128x128.dds" });
	IDirect3D9* d3d9 = ::Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d9 == nullptr)
	{
		return false;
	}

	D3DPRESENT_PARAMETERS presentParam;
	ZeroMemory(&presentParam, sizeof(presentParam));
	presentParam.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.Windowed = TRUE;
	if (S_OK != d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParam, &d3dDevice))
	{
		d3d9->Release();
		return false;
	}
	else
	{
		d3d9->Release();
	}

	auto start = std::chrono::steady_clock::now();

#if 0
	for (ImageLoader* file : imageFileList)
	{
		cxx::file_buffer& f = file->img_file;
		if (file->load_file = cxx::read_file(file->img_path.c_str(), f))
		{
			image_data& data = file->img_data;
			if (file->parse_img = parse_image(f.buffer_ptr(), f.length(), data))
			{

				IDirect3DTexture9* d3dTexture = nullptr;
				D3DFORMAT fmt = D3DFMT_A8R8G8B8;
				if (data.format == pixel_format::dxt1x)
				{
					fmt = D3DFMT_DXT1;
				}
				else if (data.format == pixel_format::dxt2)
				{
					fmt = D3DFMT_DXT2;
				}
				else if (data.format == pixel_format::dxt3)
				{
					fmt = D3DFMT_DXT3;
				}
				else if (data.format == pixel_format::dxt4)
				{
					fmt = D3DFMT_DXT4;
				}
				else if (data.format == pixel_format::dxt5)
				{
					fmt = D3DFMT_DXT5;
				}

				if (S_OK == d3dDevice->CreateTexture(data.width, data.height, 0, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &d3dTexture, NULL))
				{
					D3DLOCKED_RECT lockRect;
					if (S_OK == d3dTexture->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD))
					{
						uint32_t dstPitch;
						if (data.format == pixel_format::bgra32)
						{
							dstPitch = 4 * data.width;
							for (uint32_t i = 0; i < data.height; i++)
							{
								auto dstBuffer = (uint8_t*)lockRect.pBits + i * lockRect.Pitch;
								auto srcBuffer = data.buffer_ptr() + i * dstPitch;
								memcpy(dstBuffer, srcBuffer, dstPitch);
							}
						}
						else if (data.format == pixel_format::bgr24)
						{
							for (uint32_t row = 0; row < data.height; row++)
							{
								for (uint32_t col = 0; col < data.width; col++)
								{
									auto dstBuffer = (uint8_t*)lockRect.pBits + row * lockRect.Pitch + 4 * col;
									auto srcBuffer = get_pixel_offset(data, row, col);
									memcpy(dstBuffer, srcBuffer, 3);
									dstBuffer[3] = 255;
								}
							}
						}
						else if (data.format == pixel_format::dxt1x
							|| data.format == pixel_format::dxt2
							|| data.format == pixel_format::dxt3
							|| data.format == pixel_format::dxt4
							|| data.format == pixel_format::dxt5)
						{
							size_t block_height = dds_block_height(data);
							for (uint32_t i = 0; i < block_height; i++)
							{
								auto dstBuffer = (uint8_t*)lockRect.pBits + i * data.line_pitch;
								auto srcBuffer = data.buffer_ptr() + i * data.line_pitch;
								memcpy(dstBuffer, srcBuffer, data.line_pitch);
							}
						}
						d3dTexture->UnlockRect(0);
					}

					textures.push_back({ file->img_path,d3dTexture, data.width, data.height });
				}
			}
		}

		delete file;
	}
#else
	for (ImageLoader* file : imageFileList)
	{
		file->firstTask = Task::StartTask(ThreadName::DiskIOThread, [file](Task&) {
			file->load_file = cxx::read_file(file->img_path.c_str(), file->img_file);
		});
		file->finalTask = file->firstTask.ContinueWith(ThreadName::WorkThread, [file](Task&) {
			if (file->img_file)
			{
				file->parse_img = parse_image(file->img_file.buffer_ptr(), file->img_file.length(), file->img_data);
			}
		});
	}

	std::vector<ImageLoader*> unfinished;
	while (imageFileList.size() > 0)
	{
		for (ImageLoader* file : imageFileList)
		{
			if (!file->finalTask.IsComplete())
			{
				unfinished.push_back(file);
			}
			else
			{
				if (file->img_data)
				{
					image_data& data = file->img_data;
					IDirect3DTexture9*& d3dTexture = file->d3dTexture;
					D3DFORMAT fmt = D3DFMT_A8R8G8B8;
					if (data.format == pixel_format::dxt1x)
					{
						fmt = D3DFMT_DXT1;
					}
					else if (data.format == pixel_format::dxt2)
					{
						fmt = D3DFMT_DXT2;
					}
					else if (data.format == pixel_format::dxt3)
					{
						fmt = D3DFMT_DXT3;
					}
					else if (data.format == pixel_format::dxt4)
					{
						fmt = D3DFMT_DXT4;
					}
					else if (data.format == pixel_format::dxt5)
					{
						fmt = D3DFMT_DXT5;
					}

					if (S_OK == d3dDevice->CreateTexture(data.width, data.height, 0, D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &d3dTexture, NULL))
					{
						D3DLOCKED_RECT lockRect;
						if (S_OK == d3dTexture->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD))
						{
							uint32_t dstPitch;
							if (data.format == pixel_format::bgra32)
							{
								dstPitch = 4 * data.width;
								for (uint32_t i = 0; i < data.height; i++)
								{
									auto dstBuffer = (uint8_t*)lockRect.pBits + i * lockRect.Pitch;
									auto srcBuffer = data.buffer_ptr() + i * dstPitch;
									memcpy(dstBuffer, srcBuffer, dstPitch);
								}
							}
							else if (data.format == pixel_format::bgr24)
							{
								for (uint32_t row = 0; row < data.height; row++)
								{
									for (uint32_t col = 0; col < data.width; col++)
									{
										auto dstBuffer = (uint8_t*)lockRect.pBits + row * lockRect.Pitch + 4 * col;
										auto srcBuffer = get_pixel_offset(data, row, col);
										memcpy(dstBuffer, srcBuffer, 3);
										dstBuffer[3] = 255;
									}
								}
							}
							else if (data.format == pixel_format::dxt1x
								|| data.format == pixel_format::dxt2
								|| data.format == pixel_format::dxt3
								|| data.format == pixel_format::dxt4
								|| data.format == pixel_format::dxt5)
							{
								size_t block_height = dds_block_height(data);
								for (uint32_t i = 0; i < block_height; i++)
								{
									auto dstBuffer = (uint8_t*)lockRect.pBits + i * data.line_pitch;
									auto srcBuffer = data.buffer_ptr() + i * data.line_pitch;
									memcpy(dstBuffer, srcBuffer, data.line_pitch);
								}
							}
							d3dTexture->UnlockRect(0);
						}

						textures.push_back({ file->img_path,d3dTexture, data.width, data.height });
					}
				}
				else
				{
					assert(false);
					throw;
				}

				file->firstTask = Task::StartTask(ThreadName::DiskIOThread, [file](Task&) {
					file->load_file = cxx::read_file(file->img_path.c_str(), file->img_file);
				});
				file->finalTask = file->firstTask.ContinueWith(ThreadName::WorkThread, [file](Task&) {
					if (file->img_file)
					{
						file->parse_img = parse_image(file->img_file.buffer_ptr(), file->img_file.length(), file->img_data);
					}

					delete file;
				});
			}
		}
		imageFileList.swap(unfinished);
		unfinished.clear();
	}
#endif
	auto end = std::chrono::steady_clock::now();

	std::chrono::duration <double, std::milli> elapsedSecond = end - start;

	if (textures.size() == 0)
	{
		return false;
	}
	else
	{
		char buffer[256];
		sprintf(buffer, "加载%d个图片，共耗时%f毫秒", textures.size(), elapsedSecond.count());
		::MessageBoxA(NULL, buffer, NULL, MB_OK);
	}

	ImageVertex quadVertex[6] =
	{
		{ -1.0f, -1.0f, 0.0f, 0xFFFFFFFF ,0.0f, 1.0f },
		{ +1.0f, +1.0f, 0.0f, 0xFFFFFFFF ,1.0f, 0.0f },
		{ +1.0f, -1.0f, 0.0f, 0xFFFFFFFF ,1.0f, 1.0f },
		{ -1.0f, -1.0f, 0.0f, 0xFFFFFFFF ,0.0f, 1.0f },
		{ -1.0f, +1.0f, 0.0f, 0xFFFFFFFF ,0.0f, 0.0f },
		{ +1.0f, +1.0f, 0.0f, 0xFFFFFFFF ,1.0f, 0.0f },
	};

	if (S_OK != d3dDevice->CreateVertexBuffer(sizeof(quadVertex), 0, CUSTOM_FVF, D3DPOOL_MANAGED, &d3dVertexBuffer, NULL))
	{
		return false;
	}

	BYTE* vbPtr;
	if (S_OK == d3dVertexBuffer->Lock(0, sizeof(quadVertex), (void**)&vbPtr, 0))
	{
		memcpy(vbPtr, quadVertex, sizeof(quadVertex));
		d3dVertexBuffer->Unlock();
	}
	else
	{
		return false;
	}

	ZeroMemory(&identityMatrix, sizeof(identityMatrix));
	identityMatrix._11 = identityMatrix._22 = identityMatrix._33 = identityMatrix._44 = 1.0f;
	d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	currentTexture = -1;
	NextImage();
	return true;
}

void RenderScene()
{

	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF306090, 1.0f, 0);
	if (S_OK == d3dDevice->BeginScene())
	{
		d3dDevice->SetTransform(D3DTS_WORLD, &identityMatrix);
		d3dDevice->SetTransform(D3DTS_VIEW, &identityMatrix);
		d3dDevice->SetTransform(D3DTS_PROJECTION, &identityMatrix);

		d3dDevice->SetStreamSource(0, d3dVertexBuffer, 0, sizeof(ImageVertex));
		d3dDevice->SetFVF(CUSTOM_FVF);
		d3dDevice->SetTexture(0, textures[currentTexture].texture);

		d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		d3dDevice->EndScene();
	}

	d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Uninitialize(HWND hWnd)
{
	for (auto& t : textures)
	{
		t.texture->Release();
	}
	textures.clear();

	if (d3dVertexBuffer != nullptr)
	{
		d3dVertexBuffer->Release();
		d3dVertexBuffer = nullptr;
	}
	if (d3dDevice != nullptr)
	{
		d3dDevice->Release();
		d3dDevice = nullptr;
	}
}