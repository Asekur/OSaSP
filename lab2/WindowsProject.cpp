#include <windows.h>
#include <gdiplus.h>
#include <time.h>

const SIZE MIN_WINDOW_SIZE = { 300,200 };
const COLORREF BACKGROUND_COLOR = RGB(255, 255, 255);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		break;
	case WM_GETMINMAXINFO: {
		break;
	}
	case WM_TIMER:
		break;
	case WM_KEYDOWN:
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	//хранение состояния
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	WNDCLASSEX windClass;
	HWND handWind;
	MSG msg;

	windClass.cbSize = sizeof(WNDCLASSEX);
	windClass.style = CS_DROPSHADOW;
	windClass.lpfnWndProc = WndProc;
	windClass.cbClsExtra = 0; //без выделения дополнительных байтов
	windClass.cbWndExtra = 0;
	windClass.hInstance = hInstance; //экземпляр окна
	windClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windClass.hbrBackground = CreateSolidBrush(BACKGROUND_COLOR);
	windClass.lpszMenuName = nullptr;
	windClass.lpszClassName = "Main";
	windClass.hIconSm = windClass.hIcon;
	RegisterClassEx(&windClass);

	handWind = CreateWindow("Main", "Таблица текста", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(handWind, nCmdShow);
	UpdateWindow(handWind);
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}