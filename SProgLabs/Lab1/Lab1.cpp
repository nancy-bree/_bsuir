#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "resource1.h"

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Lab1");

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int startPosition = 0;
bool isRunning = FALSE;


void Start(HWND hwnd)
{
	if(!isRunning)
	{
		SetTimer(hwnd, 1, 5, NULL);
	}
	isRunning = TRUE;
}

void Stop(HWND hwnd)
{
	if(isRunning)
	{
		KillTimer(hwnd, 1);
	}
	isRunning = FALSE;
}

void Paint(HDC hdc)
{
	RECT rect;
	rect.left = startPosition;
	rect.top = 100;
	rect.bottom = 150;
	rect.right = startPosition + 50;
	SetTextColor(hdc, RGB(149,53,174));
	DrawText(hdc, L"Hello, World!", -1, &rect, DT_LEFT | DT_NOCLIP);
}

void Timer(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	startPosition += 1;
	if(startPosition >= rect.right - rect.left)
	{
		startPosition = -5;
	}
	InvalidateRect(hwnd, NULL, TRUE);
}

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
	wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCE(IDM_MENU);
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
        500, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

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
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_START:
			Start(hWnd);
			break;
		case IDM_STOP:
			Stop(hWnd);
			break;
		}
		break;
	case WM_TIMER:
		Timer(hWnd);
		break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		Paint(hdc);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}