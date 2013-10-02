#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "resource.h"

#define ADD 1
#define TORIGHT 2
#define DEL 3
#define CLEAR 4
#define EDIT 5

static TCHAR szWindowClass[] = _T("win32app");

static TCHAR szTitle[] = _T("Lab2");

HWND hAddBtn, hTextBox, hToRightBtn, hDeleteBtn, hClearBtn, hListBox1, hListBox2; 

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**/
void AddText(HWND hwnd)
{
	WCHAR* buf = new TCHAR[128];

	int len = SendMessage(hTextBox, EM_LINELENGTH, 0, 0);
	GetDlgItemText(hwnd, EDIT, buf, len+1);

	bool match = FALSE;
	int count = SendMessageW(hListBox1, LB_GETCOUNT, 0, 0);

	for (int i = 0; i < count; i++) 
	{
		WCHAR* data = (WCHAR*)SendMessageW(hListBox1, LB_GETITEMDATA, i, 0);
		if (ua_lstrcmpW(data, buf) == 0)
		{
			match = TRUE;
		}
	}

	if (!match && ua_lstrlenW(buf) > 0)
	{
		int idx = SendMessageW(hListBox1, LB_ADDSTRING, 0, (LPARAM)buf);
		SendMessageW(hListBox1, LB_SETITEMDATA, idx, (LPARAM)buf);
	}

	SetWindowText(hTextBox, L"");
}

void MoveTextToRight()
{
	WCHAR* buf = new WCHAR[128];
	int selectedItemIndex = SendMessageW(hListBox1, LB_GETCURSEL, 0, 0);
	buf = (WCHAR*)SendMessageW(hListBox1, LB_GETITEMDATA, selectedItemIndex, 0);

	bool match = FALSE;
	int count = SendMessageW(hListBox2, LB_GETCOUNT, 0, 0);

	for (int i = 0; i < count; i++) 
	{
		WCHAR* tmp = (WCHAR*)SendMessageW(hListBox2, LB_GETITEMDATA, i, 0);
		if (ua_lstrcmpW(tmp, buf) == 0)
		{
			match = TRUE;
		}
	}

	if (!match && ua_lstrlenW(buf) > 0)
	{
		int idx = SendMessageW(hListBox2, LB_ADDSTRING, 0, (LPARAM)buf);
		SendMessageW(hListBox2, LB_SETITEMDATA, idx, (LPARAM)buf);
	}
}

void DeleteText()
{
	int selectedItemIndex1 = SendMessage(hListBox1, LB_GETCURSEL, 0, 0);
	int selectedItemIndex2 = SendMessage(hListBox2, LB_GETCURSEL, 0, 0);
	SendMessage(hListBox1, LB_DELETESTRING, selectedItemIndex1, 0);
	SendMessage(hListBox2, LB_DELETESTRING, selectedItemIndex2, 0);
}

void ClearListBoxes()
{
	SendMessage(hListBox1, LB_RESETCONTENT, 0, 0);
	SendMessage(hListBox2, LB_RESETCONTENT, 0, 0);
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
        670, 250,
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
    switch (message)
    {
		case WM_CREATE:
		{
			// Adding ListBoxes
    hListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE
        , L"LISTBOX", NULL
        , WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL
        , 10, 10, 200, 200
        , hWnd, NULL, hInst, NULL);

	hListBox2 = CreateWindowExW(WS_EX_CLIENTEDGE
        , L"LISTBOX", NULL
        , WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL
        , 220, 10, 200, 200
        , hWnd, NULL, hInst, NULL);

	// Adding a TextBox
    hTextBox = CreateWindowExW(WS_EX_CLIENTEDGE
        , L"EDIT", NULL
        , WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL
        , 440, 10, 200, 25
        , hWnd, (HMENU)EDIT, hInst, NULL);
    //SetWindowTextW(hTextBox, L"Input text here...");

   // Adding Buttons
    hAddBtn = CreateWindowExW(WS_EX_APPWINDOW
        , L"BUTTON", NULL
        , WS_CHILD | WS_VISIBLE
        , 440, 45, 200, 25
        , hWnd, (HMENU)ADD, hInst, NULL); 
	SetWindowTextW(hAddBtn, L"Add");

	hToRightBtn = CreateWindowExW(WS_EX_APPWINDOW
        , L"BUTTON", NULL
        , WS_CHILD | WS_VISIBLE
        , 440, 80, 200, 25
		, hWnd, (HMENU)TORIGHT, hInst, NULL); 
	SetWindowTextW(hToRightBtn, L"To Right");

	hDeleteBtn = CreateWindowExW(WS_EX_APPWINDOW
        , L"BUTTON", NULL
        , WS_CHILD | WS_VISIBLE
        , 440, 115, 200, 25
		, hWnd, (HMENU)DEL, hInst, NULL); 
	SetWindowTextW(hDeleteBtn, L"Delete");

	hClearBtn = CreateWindowExW(WS_EX_APPWINDOW
        , L"BUTTON", NULL
        , WS_CHILD | WS_VISIBLE
        , 440, 150, 200, 25
		, hWnd, (HMENU)CLEAR, hInst, NULL); 
	SetWindowTextW(hClearBtn, L"Clear");
			break;
		}
		case WM_COMMAND:
			{
				switch(LOWORD(wParam))
				{
				case ADD:
					{
						AddText(hWnd);
						break;
					}
				case TORIGHT:
					{
						MoveTextToRight();
						break;
					}
				case DEL:
					{
						DeleteText();
						break;
					}
				case CLEAR:
					{
						ClearListBoxes();
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