#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../task_graph/task_graph.h"
const wchar_t* APP_NAME = L"win32_test";
HINSTANCE hInst;
HWND hWindow;

bool Initialize(HWND hWnd);
void Uninitialize(HWND hWnd);
void RenderScene();
void NextImage();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	::ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = APP_NAME;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWindow = CreateWindow(APP_NAME, APP_NAME, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWindow)
	{
		return FALSE;
	}

	ShowWindow(hWindow, nCmdShow);
	UpdateWindow(hWindow);


	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONUP:
		NextImage();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ wchar_t* lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	TaskScheduler::Instance();
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow) ||
		!Initialize(hWindow))
	{
		TaskScheduler::Shutdown();
		return FALSE;
	}

	MSG msg;
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			RenderScene();
		}
	}

	TaskScheduler::Shutdown();
	Uninitialize(hWindow);
	return (int)msg.wParam;
}