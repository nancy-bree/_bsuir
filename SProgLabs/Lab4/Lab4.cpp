#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "resource.h"

#define START 40003
#define STOP 40004
#define CLEAR 40005

static TCHAR szWindowClass[] = _T("win32app");

static TCHAR szTitle[] = _T("Lab4");

HWND hStartBtn, hStopBtn, hClearBtn;

HANDLE hThread1 = 0;
HANDLE hThread2 = 0;
HANDLE hThread3 = 0;

bool isActive = FALSE;
bool isFirstTime = TRUE;

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**/
void SetWindowStyle(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	HGDIOBJ penBlack = CreatePen(NULL, 1, RGB(0,0,0));

	int y = 55;
	int offset = 150;

	SelectObject(hdc, penBlack);

	MoveToEx(hdc, 0, y, NULL);
	LineTo(hdc, 450, y);

	MoveToEx(hdc, 0, 355, NULL);
	LineTo(hdc, 450, 355);

	MoveToEx(hdc, offset, y, NULL);
	LineTo(hdc, offset, 355);

	MoveToEx(hdc, 2*offset, y, NULL);
	LineTo(hdc, 2*offset, 355);


}

void DrawLine(HWND hwnd, int x0, bool isDraw)
{
	HDC hdc = GetDC(hwnd);
	int sleepParam = 100;
	HGDIOBJ penRed = CreatePen(NULL, 2, RGB(255,0,0));
	SelectObject(hdc, penRed);

	for (int i = 60; i <= 355; i+=5)
	{
		MoveToEx(hdc, x0, i-5, NULL);
		LineTo(hdc, x0, i);
		Sleep(sleepParam);
	}
}

DWORD WINAPI Thread1(LPVOID lpParam)
{
	int x = 75;
	HANDLE  hStdout = NULL;
	HWND hwnd = (HWND)lpParam;
    
    // Get Handle To screen.
    if( (hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE )  
		return 1;

	DrawLine(hwnd, x, TRUE);

	return 0;

}

DWORD WINAPI Thread2(LPVOID lpParam)
{
	int x = 225;
	HANDLE  hStdout = NULL;
	HWND hwnd = (HWND)lpParam;
    
    // Get Handle To screen.
    if( (hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE )  
		return 1;

	DrawLine(hwnd, x, TRUE);

	return 0;
}

DWORD WINAPI Thread3(LPVOID lpParam)
{
	int x = 375;
	HANDLE  hStdout = NULL;
	HWND hwnd = (HWND)lpParam;
    
    // Get Handle To screen.
    if( (hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE )  
		return 1;

	DrawLine(hwnd, x, TRUE);

	return 0;
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
	wcex.hCursor        = LoadCursor(hInstance, IDI_APPLICATION);
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
        465, 400,
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

	SuspendThread(hThread1);
					SuspendThread(hThread2);
					SuspendThread(hThread3);

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
	case WM_CREATE:
		{
			hStartBtn = CreateWindowExW(WS_EX_APPWINDOW
				, L"BUTTON", NULL
				, WS_CHILD | WS_VISIBLE
				, 10, 10, 50, 25
				, hWnd, (HMENU)START, hInst, NULL); 
			SetWindowTextW(hStartBtn, L"Start");

			hStopBtn = CreateWindowExW(WS_EX_APPWINDOW
				, L"BUTTON", NULL
				, WS_CHILD | WS_VISIBLE
				, 80, 10, 50, 25
				, hWnd, (HMENU)STOP, hInst, NULL); 
			SetWindowTextW(hStopBtn, L"Stop");

			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case START:
				if (isFirstTime)
				{
					hThread1 = CreateThread( NULL, 0, Thread1, hWnd, 0, NULL);
					hThread2 = CreateThread( NULL, 0, Thread2, hWnd, 0, NULL);
					hThread3 = CreateThread( NULL, 0, Thread3, hWnd, 0, NULL);

					isFirstTime = FALSE;
				}
				if (!isActive)
				{
					ResumeThread(hThread1);
					ResumeThread(hThread2);
					ResumeThread(hThread3);
				}
				isActive = TRUE;
				break;
			case STOP:
				if (isActive)
				{
					SuspendThread(hThread1);
					SuspendThread(hThread2);
					SuspendThread(hThread3);
				}
				isActive = FALSE;
				break;
			}
			break;
		}
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		SetWindowStyle(hWnd);
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