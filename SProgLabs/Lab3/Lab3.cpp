#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "resource.h"

#define DRAW 1
#define CLEAR 2


static TCHAR szWindowClass[] = _T("win32app");

static TCHAR szTitle[] = _T("Lab3");

HINSTANCE hInst;

HWND hBtnDraw, hBtnClear;

HBITMAP hBtnNormal, hBtnPressed;

bool isDrawn = FALSE;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**/
void DrawButton(LPDRAWITEMSTRUCT dis) {
	HDC hDc = dis->hDC;
	BITMAP bitmap;

	HBITMAP hBitmap = hBtnNormal;
	if (dis->itemState & ODS_SELECTED)
		hBitmap = hBtnPressed;

	HDC hdcMem = CreateCompatibleDC(hDc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	BitBlt(hDc, dis->rcItem.left, dis->rcItem.top, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);

	LPWSTR text = L"Draw";
	if (dis->hwndItem == hBtnClear)
		text = L"Clear";

	RECT rect;
	SetTextColor(hDc, RGB(0,0,0));
	SetBkMode(hDc, TRANSPARENT);
	rect.left = 22;
	rect.top = 10;
	rect.right = 60;
	rect.bottom = 10;
	DrawText(hDc, text, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
	DeleteDC(hdcMem);
}

void DrawPicture(HWND hWnd) {
	HDC hDc = GetDC(hWnd);

	HGDIOBJ penGray = CreatePen(NULL, 1, RGB(128, 128, 128));
	HGDIOBJ penBrown = CreatePen(NULL, 1, RGB(128, 128, 0));
	HGDIOBJ penBlack = CreatePen(NULL, 1, RGB(0, 0, 0));
	HGDIOBJ penWhite = CreateSolidBrush(RGB(255, 255, 255));

	HGDIOBJ brushSand = CreateSolidBrush(RGB(236, 223, 180));
	HGDIOBJ brushTurquoise = CreateSolidBrush(RGB(44, 192, 192));
	HGDIOBJ brushGreen = CreateSolidBrush(RGB(128, 255, 128));
	HGDIOBJ brushSwamp = CreateSolidBrush(RGB(154, 144, 114));
	HGDIOBJ brushWhite = CreateSolidBrush(RGB(255, 255, 255));

	SelectObject(hDc, penWhite);
	SelectObject(hDc, brushWhite);
	Rectangle(hDc, 150, 0, 390, 450);

	if (isDrawn) {
		int x = 150, y = 200;
		SelectObject(hDc, penGray); 
		SelectObject(hDc, brushSand);

		Rectangle(hDc, x, y, x + 240, y + 180);	//дом

		SelectObject(hDc, brushSwamp);

		Rectangle(hDc, x + 40, y - 60, x + 60, y - 20);	//труба

		POINT points[3];
		points[0].x = x;
		points[0].y = y;
		points[1].x = x + 120;
		points[1].y = y - 80;
		points[2].x = x + 240;
		points[2].y = y;

		Polygon(hDc, points, 3);	//крыша

		SelectObject(hDc, penGray); 
		SelectObject(hDc, brushTurquoise);

		Ellipse(hDc, x + 100, y - 60, x + 140, y - 20);	// окно на крыше

		SelectObject(hDc, penBlack); 

		MoveToEx(hDc, x + 100, y - 40, NULL);	// рама окна на крыше
		LineTo(hDc, x + 140, y - 40);
		MoveToEx(hDc, x + 120, y - 60, NULL);
		LineTo(hDc, x + 120, y - 20);

		SelectObject(hDc, penGray); 

		Rectangle(hDc, x + 40, y + 40, x + 100, y + 120);	//окно

		SelectObject(hDc, penBlack);

		Rectangle(hDc, x + 60, y + 60, x + 80, y + 120);	//рама

		MoveToEx(hDc, x + 40, y + 60, NULL);	// рама
		LineTo(hDc, x + 100, y + 60);

		Rectangle(hDc, x + 160, y + 60, x + 220, y + 180);	// дверь

		SelectObject(hDc, penBrown);
		SelectObject(hDc, brushGreen);

		Ellipse(hDc, x + 165, y + 115, x + 175, y + 125);	//ручка

		SelectObject(hDc, penGray); 

		Arc(hDc, x + 40, y  - 80, x + 60, y - 60, x + 40, y - 60, x + 50, y  - 80);	//дым
		Arc(hDc, x + 35, y  - 100, x + 50, y - 80, x + 50, y - 80, x + 35, y  - 100);
		Arc(hDc, x + 30, y  - 120, x + 40, y - 100, x + 40, y - 100, x + 30, y  - 120);
	}
}
/**/

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor        = LoadCursor(hInstance, NULL);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Lab1"),
            NULL);

        return 1;
    }

    hInst = hInstance;

    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
        500, 450,
        NULL,
        NULL,
        hInstance,
        NULL
    );

	hBtnNormal = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	hBtnPressed = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			hBtnDraw = CreateWindowExW(WS_EX_APPWINDOW
			, L"BUTTON", NULL
			, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW
			, 10, 10, 80, 30
			, hWnd, (HMENU)DRAW, hInst, NULL); 

			hBtnClear = CreateWindowExW(WS_EX_APPWINDOW
			, L"BUTTON", NULL
			, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW
			, 10, 50, 80, 30
			, hWnd, (HMENU)CLEAR, hInst, NULL); 

			break;
		}
	case WM_DRAWITEM: 
		{
			int itemId = wParam;
			LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
			DrawButton(dis);
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case DRAW:
				{
					isDrawn = TRUE;
					DrawPicture(hWnd);
					break;
				}
			case CLEAR:
				{
					isDrawn = FALSE;
					DrawPicture(hWnd);
					break;
				}
			}
			break;
		}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}