#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <CommCtrl.h>
#include "resource.h"
#include <TlHelp32.h>

#define LB1 40007
#define LB2 40008

static TCHAR szWindowClass[] = _T("win32app");

static TCHAR szTitle[] = _T("Lab5");

HINSTANCE hInst;

HWND hListBox1, hListBox2;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**/
WCHAR* GetPriorityClassName(DWORD id)
{
	if (id == ABOVE_NORMAL_PRIORITY_CLASS) return L"Above Nomal";
	if (id == BELOW_NORMAL_PRIORITY_CLASS) return L"Below Normal";
	if (id == HIGH_PRIORITY_CLASS) return L"High";
	if (id == IDLE_PRIORITY_CLASS) return L"Idle";
	if (id == NORMAL_PRIORITY_CLASS) return L"Normal";
	if (id == REALTIME_PRIORITY_CLASS) return L"Real Time";
	return L"---";
}

void GetProcessList()
{
	PROCESSENTRY32 pe32;
	HANDLE hSnapshot;
	HANDLE hProcess;
	DWORD dwPriorityClass;
	
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32);

	Process32First( hSnapshot, &pe32 );

	SendMessage(hListBox1, LB_RESETCONTENT, 0, 0);

	do {
		dwPriorityClass = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		dwPriorityClass = GetPriorityClass(hProcess);

		char text[1024];
		swprintf((wchar_t*)text, L"[%d] %s [%s]", pe32.th32ProcessID, pe32.szExeFile, GetPriorityClassName(dwPriorityClass));
		int index = SendMessage(hListBox1, LB_ADDSTRING, 0, (LPARAM)text);
		SendMessage(hListBox1, LB_SETITEMDATA, index, (LPARAM)pe32.th32ProcessID);

	} while (Process32Next(hSnapshot, &pe32));

	CloseHandle(hSnapshot);	
}

void GetProcessModulesList(DWORD pid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	me32.dwSize = sizeof( MODULEENTRY32 );

	SendMessage(hListBox2, LB_RESETCONTENT, 0, 0);

	do {
		int index = SendMessage(hListBox2, LB_ADDSTRING, 0, (LPARAM)me32.szModule);
		SendMessage(hListBox2, LB_SETITEMDATA, index, (LPARAM)me32.szModule);
	} while (Module32Next(hModuleSnap, &me32));

	CloseHandle( hModuleSnap );
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
	wcex.hCursor        = LoadCursor(hInstance, IDC_ARROW);
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
        550, 530,
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
	GetProcessList();

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
		hListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE
        , L"LISTBOX", NULL
		, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY
        , 10, 10, 250, 480
		, hWnd, (HMENU)LB1, hInst, NULL);
		hListBox2 = CreateWindowEx(WS_EX_CLIENTEDGE
        , L"LISTBOX", NULL
        , WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL
        , 270, 10, 250, 480
		, hWnd, NULL, hInst, NULL);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) >= ID_IDLE && LOWORD(wParam) <= ID_ABOVENORMAL) 
		{
			int index = SendMessage(hListBox1, LB_GETCURSEL, 0, 0);
			if (index == LB_ERR)
				break;
			int pid = SendMessage(hListBox1, LB_GETITEMDATA, index, 0);

			int priority = 0;
			switch(LOWORD(wParam)) 
			{
			case ID_IDLE:
				priority = IDLE_PRIORITY_CLASS;
				break;
			case ID_BELOWNORMAL:
				priority = BELOW_NORMAL_PRIORITY_CLASS;
				break;
			case ID_NORMAL:
				priority = NORMAL_PRIORITY_CLASS;
				break;
			case ID_ABOVENORMAL:
				priority = ABOVE_NORMAL_PRIORITY_CLASS;
			case ID_HIGH:
				priority = HIGH_PRIORITY_CLASS;
				break;
			case ID_REALTIME:
				priority = REALTIME_PRIORITY_CLASS;
				break;
			}

			HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid);
			if (!SetPriorityClass(hProcess, priority))
				MessageBox(hWnd, L"Failed to set priority", L"", MB_ICONERROR);
			CloseHandle(hProcess);
			GetProcessList();
		}
		switch(LOWORD(wParam))
		{
		case LB1:
			{
				if (HIWORD(wParam) == NM_RCLICK) 
				{
					int index = SendMessage(hListBox1, LB_GETCURSEL, 0, 0);
				}
				if (HIWORD(wParam) != 0) 
				{
					int index = SendMessage(hListBox1, LB_GETCURSEL, 0, 0);
					if (index == LB_ERR)
						break;
					int pid = SendMessage(hListBox1, LB_GETITEMDATA, index, 0);

					SendMessage(hListBox2, LB_RESETCONTENT, 0, 0);

					GetProcessModulesList(pid);
				}
			break;
			}
		}
		break;
	case WM_CONTEXTMENU:
		if ((HWND)wParam == hListBox1)
		{
			HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
			hMenu = GetSubMenu(hMenu, 0);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_NOANIMATION, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
		}
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