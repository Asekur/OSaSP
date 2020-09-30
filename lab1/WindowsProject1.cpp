#include <windows.h>
#include <gdiplus.h>
#include <time.h>

#define TIMER_SECOND 1 
//отрисовка по таймеру

const SIZE MIN_WINDOW_SIZE = { 300,200 };
const COLORREF BACKGROUND_COLOR = RGB(255, 255, 255);
const int MOVE_DISTANCE = 3;
//скорость анимации
const int RECT_WIDTH = 150;
const int RECT_HEIGHT = 150;

static POINTFLOAT modifierDirection = { 1.0, 1.0 };
//направление движения анимации
static RECT rect = { 0, 0, RECT_WIDTH, RECT_HEIGHT };

bool noTimer = false;
clock_t timeStart, timeEnd;

void updateRectValues(RECT& rectToChange, int horizontalPx, int verticalPx) {
	rectToChange.left += horizontalPx;
	rectToChange.top += verticalPx;
	rectToChange.right += horizontalPx;
	rectToChange.bottom += verticalPx;
}

bool calculateBorders(RECT windowRect, int horizontalPx, int verticalPx) {
	RECT tmpRect = rect;
	updateRectValues(tmpRect, horizontalPx, verticalPx);
	if (tmpRect.left < windowRect.left || tmpRect.top < windowRect.top || tmpRect.right > windowRect.right ||
		tmpRect.bottom > windowRect.bottom)
		return false;
	updateRectValues(rect, horizontalPx, verticalPx);
	return true;
	//проверка следующего положения анимации
}

void backgroundCreate(HDC hdc, PAINTSTRUCT ps) {
	HBRUSH brush = CreateSolidBrush(BACKGROUND_COLOR);
	FillRect(hdc, &ps.rcPaint, brush);
	DeleteObject(brush);
}

void paintObj(HWND hWnd, int horizontalPx, int verticalPx) {
	RECT windowRect;
	//размеры окна
	if (!GetClientRect(hWnd, &windowRect))
		GetLastError();
	if (!calculateBorders(windowRect, horizontalPx, verticalPx)) return;
	//очистка окна
	InvalidateRect(hWnd, nullptr, FALSE);

	int left = windowRect.left;
	int top = windowRect.top;
	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;

	//возможность отрисовки на окне
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP hBM = CreateCompatibleBitmap(hdc, width, height);
	//двойная буферизация
	HBITMAP oldbmp = (HBITMAP)SelectObject(memDC, hBM);

	backgroundCreate(memDC, ps);
	Gdiplus::Graphics graphics(memDC);
	Gdiplus::Rect destRect(rect.left, rect.top, RECT_WIDTH, RECT_HEIGHT);
	Gdiplus::Image image(L"D:\\Для учебы\\ОСИСП\\lab1\\assets\\1.png");
	graphics.DrawImage(&image, destRect);

	BitBlt(hdc, left, top, width, height, memDC, left, top, SRCCOPY);
	SelectObject(memDC, oldbmp);
	DeleteObject(hBM);
	DeleteDC(memDC);

	EndPaint(hWnd, &ps);
}

void iconMovement(HWND hWnd) {
	RECT windowRect;
	int xDistance = MOVE_DISTANCE * modifierDirection.x;
	int yDistance = MOVE_DISTANCE * modifierDirection.y;
	if (!GetClientRect(hWnd, &windowRect))
		GetLastError();
	if (windowRect.bottom == 0) return;
	if (rect.bottom + yDistance > windowRect.bottom && rect.right + xDistance < windowRect.right &&
		modifierDirection.y > 0) {
		modifierDirection.y *= -1.0;
	}
	else if (rect.bottom + yDistance > windowRect.bottom && rect.left + xDistance > windowRect.left
		&& modifierDirection.y > 0) {
		modifierDirection.y *= -1.0;
	}
	else if (rect.right + xDistance > windowRect.right && rect.bottom + yDistance <= windowRect.bottom &&
		modifierDirection.x > 0) {
		modifierDirection.x = -1.0;
	}
	else if (rect.top + yDistance < windowRect.top && rect.left + xDistance > windowRect.left &&
		modifierDirection.y < 0) {
		modifierDirection.y *= -1.0;
	}
	else if (rect.left + xDistance < windowRect.left && rect.top + yDistance >= windowRect.top &&
		modifierDirection.x < 0) {
		modifierDirection.x *= -1.0;
	}
	else if (rect.top + yDistance < windowRect.top && rect.left + xDistance < windowRect.left &&
		modifierDirection.y < 0) {
		modifierDirection.y *= -1.0;
	}
	paintObj(hWnd, MOVE_DISTANCE * modifierDirection.x, MOVE_DISTANCE * modifierDirection.y);
}

void updateRectPos(HWND hWnd) {    
	// перерисовка анимации при касании с границами
	RECT windowRect;
	if (!GetClientRect(hWnd, &windowRect))
		GetLastError();
	if (windowRect.bottom == 0) return;
	if (rect.bottom <= windowRect.bottom && rect.right > windowRect.right) {
		rect.right = windowRect.right;
		rect.left = rect.right - RECT_WIDTH;
	}
	else if (rect.right > windowRect.right && rect.bottom > windowRect.bottom) {
		rect.right = windowRect.right;
		rect.bottom = windowRect.bottom;
		rect.top = rect.bottom - RECT_HEIGHT;
		rect.left = rect.right - RECT_WIDTH;
	}
	else if (rect.right <= windowRect.right && rect.bottom > windowRect.bottom) {
		rect.bottom = windowRect.bottom;
		rect.top = rect.bottom - RECT_WIDTH;
	}
	paintObj(hWnd, 0, 0);
}

void keyDownAnalyse(HWND hWnd, WPARAM wParam) {
	if (GetKeyState(VK_UP) < 0 && GetKeyState(VK_RIGHT) < 0) {
		paintObj(hWnd, MOVE_DISTANCE, -MOVE_DISTANCE);
		return;
	}
	else if (GetKeyState(VK_UP) < 0 && GetKeyState(VK_LEFT) < 0) {
		paintObj(hWnd, -MOVE_DISTANCE, -MOVE_DISTANCE);
		return;
	}
	else if (GetKeyState(VK_DOWN) < 0 && GetKeyState(VK_LEFT) < 0) {
		paintObj(hWnd, -MOVE_DISTANCE, MOVE_DISTANCE);
		return;
	}
	else if (GetKeyState(VK_DOWN) < 0 && GetKeyState(VK_RIGHT) < 0) {
		paintObj(hWnd, MOVE_DISTANCE, MOVE_DISTANCE);
		return;
	}
	switch (wParam) {
	case VK_UP:
		paintObj(hWnd, 0, -MOVE_DISTANCE);
		break;
	case VK_DOWN:
		paintObj(hWnd, 0, MOVE_DISTANCE);
		break;
	case VK_LEFT:
		paintObj(hWnd, -MOVE_DISTANCE, 0);
		break;
	case VK_RIGHT:
		paintObj(hWnd, MOVE_DISTANCE, 0);
		break;
	}
}

void mouseWheelAnalyse(HWND hWnd, WPARAM wParam) {
	if (GetKeyState(VK_SHIFT) < 0) {
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			paintObj(hWnd, 0, -MOVE_DISTANCE);
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			paintObj(hWnd, 0, MOVE_DISTANCE);
		return;
	}
	if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		paintObj(hWnd, MOVE_DISTANCE, 0);
	else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		paintObj(hWnd, -MOVE_DISTANCE, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, TIMER_SECOND, 10, NULL);
		break;
	case WM_GETMINMAXINFO: {
		updateRectPos(hWnd);
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = MIN_WINDOW_SIZE.cx;
		lpMMI->ptMinTrackSize.y = MIN_WINDOW_SIZE.cy;
	}
	case WM_TIMER:
		if (!noTimer) {
			iconMovement(hWnd);
		}
		break;
	case WM_KEYDOWN:
		noTimer = true;
		keyDownAnalyse(hWnd, wParam);
		break;
	case WM_MOUSEWHEEL:
		noTimer = true;
		mouseWheelAnalyse(hWnd, wParam);
		break;
	case WM_PAINT:
		paintObj(hWnd, 0, 0);
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
	windClass.lpszClassName = L"Main";
	windClass.hIconSm = windClass.hIcon;
	RegisterClassEx(&windClass);
	
	handWind = CreateWindow(L"Main", L"Ъуь",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
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