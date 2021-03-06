// WindowsProject2.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include "stdafx.h"
#include "WindowsProject2.h"

#define MAX_LOADSTRING 100
UINT INTER_PROCESS_MESSAGE = 10101;

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;										// current window
HWND ok_button;									// send button
HWND clear_button;									// clear button
HWND line_edit_field;							// line edit
HWND show_text_field;							// line edit
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	line_edit_field = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, 10, 285, 25, hWnd, (HMENU)10001, NULL, NULL);
	show_text_field = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_READONLY, 10, 70, 285, 85, hWnd, (HMENU)10002, NULL, NULL);
	ok_button = CreateWindow(L"BUTTON", L"Send", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 10, 40, 100, 25, hWnd, (HMENU)10000, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	clear_button = CreateWindow(L"BUTTON", L"Clear", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 195, 40, 100, 25, hWnd, (HMENU)10003, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

	MSG msg;
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            // Parse the menu selections:
            switch (LOWORD(wParam))
            {
			case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case 10000:
			//Handling when user clicking on "Send" button
			{
				int len = GetWindowTextLength(line_edit_field) + 1;
				wchar_t* text = new wchar_t[len];
				GetWindowText(line_edit_field, &text[0], len);
				//Append the text to box 2.
				//SendMessage(show_text_field, EM_SETSEL, 0, -1);
				//SendMessage(show_text_field, EM_REPLACESEL, 0, (LPARAM)&text[0]);
				//delete[] text;
				HWND another_process = FindWindowA(NULL, "WindowsProject3");
				if (another_process)
				{
					COPYDATASTRUCT cds;
					cds.dwData = 1;
					cds.cbData = sizeof(wchar_t) * (_tcslen(text) + 1);
					cds.lpData = text;
					SendMessage(another_process, WM_COPYDATA, (WPARAM)(HWND)another_process, (LPARAM)&cds);
					SetWindowText(show_text_field, L"Message send");
					//MessageBox(hWnd, L"Message send", L"MyApp", MB_OK);
				}
				else
				{
					MessageBox(hWnd, L"Can't send WM_COPYDATA", L"MyApp", MB_OK);
				}

				//Delete the text from box 1.
				//SetWindowText(line_edit_field, L"");
				break;
			}
			case 10003:
				SetWindowText(line_edit_field, L"");
				SetWindowText(show_text_field, L"");
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}