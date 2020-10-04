#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const SIZE MIN_WINDOW_SIZE = { 300,200 };
const COLORREF BACKGROUND_COLOR = RGB(255, 255, 255);

const int columnAmount = 6;
vector<string> text;

int readFile() {
    string line;
	ifstream fs("C:\\Users\\Angelina\\Desktop\\NewTest.txt"); //окрываем файл дл€ чтени€
	if (fs.is_open())
	{
		while (getline(fs, line))
		{
			text.push_back(line);
		}
	}
	fs.close();     //закрываем файл
	return 0;
}

void drawVerticalBorders(HDC hdc, int windowWidth, int windowHeight) {
	//прорисовка вертикальных линий
	for (int i = 0; i < columnAmount; i++) {
		MoveToEx(hdc, i * windowWidth, 0, NULL);
		LineTo(hdc, i * windowWidth, windowHeight);
	}
}

void drawHorizontalBorder(HDC hdc, int windowWidth, int lineHeight) {
	//прорисовка горизонтальных линий
	MoveToEx(hdc, 0, lineHeight, NULL);
	LineTo(hdc, windowWidth, lineHeight);
}

void drawTable(HDC hdc, int windowWidth, int windowHeight) {
	int colomnWidth = windowWidth / columnAmount;
	int rowOffset = 0; //верхн€€ координата y текущей строки

	for (int rowIndex = 0; rowIndex < text.size(); rowIndex++) {
		int maxTextHeight = 0;
		RECT cell;
		//границы €чейки
		cell.top = rowOffset + 5;
		cell.bottom = windowHeight;

		for (int colIndex = 0; colIndex < columnAmount && rowIndex < text.size(); colIndex++) {
			cell.left = colIndex * colomnWidth + 5;
			cell.right = (colIndex + 1) * colomnWidth - 5;

			const CHAR* str = text[rowIndex++].c_str();

			int currTextHeight = DrawTextA(hdc, (LPCSTR)str, strlen(str), &cell, DT_VCENTER | DT_WORDBREAK | DT_NOCLIP | DT_EDITCONTROL);

			//вычисление максимальной высоты €чейки
			if (currTextHeight > maxTextHeight)
				maxTextHeight = currTextHeight;
		}

		rowOffset += maxTextHeight + 5;
		drawHorizontalBorder(hdc, windowWidth, rowOffset);
		drawVerticalBorders(hdc, colomnWidth, rowOffset);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	RECT rect;
	if (!GetClientRect(hWnd, &rect))
		GetLastError();

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		drawTable(hdc, rect.right, rect.bottom);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	readFile();

	WNDCLASSEX windClass;
	HWND handWind;
	MSG msg;

	windClass.cbSize = sizeof(WNDCLASSEX);
	windClass.style = CS_HREDRAW | CS_VREDRAW;
	windClass.lpfnWndProc = WndProc;
	windClass.cbClsExtra = 0; //без выделени€ дополнительных байтов
	windClass.cbWndExtra = 0;
	windClass.hInstance = hInstance; //экземпл€р окна
	windClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windClass.hbrBackground = CreateSolidBrush(BACKGROUND_COLOR);
	windClass.lpszMenuName = nullptr;
	windClass.lpszClassName = "Main";
	windClass.hIconSm = windClass.hIcon;
	RegisterClassEx(&windClass);

	handWind = CreateWindow("Main", //им€ класса окна
		"“аблица c текстом", //заголовок окна
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, //дескриптор родительского и дочернего окна
		hInstance, nullptr); //дескриптор приложени€

	ShowWindow(handWind, nCmdShow);
	UpdateWindow(handWind);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}