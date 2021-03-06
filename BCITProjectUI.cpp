// BCITProjectUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BCITProjectUI.h"
#include "includes.h"
#include <thread>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int IDC_AIMBOX = 10;
const int IDC_ESPBOX = 11;
const int IDC_TRIGGERBOX = 12;
const int IDC_HOPBOX = 13;

float FOV = 45; //default value of 45
float ESPDistance = 0;

BOOL AimCheck;
BOOL ESPCheck;
BOOL TriggerCheck;
BOOL HopCheck;

HANDLE threadLoop;
HWND FOVtext;
HWND ESPtext;


//Struc Varaible Holders for Passing Variables to Create Thread



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
    LoadStringW(hInstance, IDC_BCITPROJECTUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BCITPROJECTUI));
	
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BCITPROJECTUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BCITPROJECTUI);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 250,300, nullptr, nullptr, hInstance, nullptr);
 
   
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void aimThreadTask()
{
	while (AimCheck)
	{
		pAim->aimAssist(FOV);
	}
		
}

void *ESPThreadTask()
{
	while (ESPCheck)
	{
		pGlow->GlowESP(ESPDistance);
	}

	return 0;
}

void *TriggerThreadTask()
{
	while (TriggerCheck)
	{
		pTrigger->Trigger();
	}

	return 0;
}

void *HopThreadTask()
{
	while (HopCheck)
	{
		pBhop->autobHop();
	}

	return 0;
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
	case WM_CREATE:
	{
		CreateWindowEx(NULL,TEXT("button"), TEXT("Aim"),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 30, 185, 35,
			hWnd, (HMENU)IDC_AIMBOX, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		CreateWindowEx(NULL, TEXT("button"), TEXT("GlowESP"),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 90, 185, 35,
			hWnd, (HMENU)IDC_ESPBOX, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		CreateWindowEx(NULL, TEXT("button"), TEXT("Trigger"),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 150, 185, 35,
			hWnd, (HMENU)IDC_TRIGGERBOX, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		CreateWindowEx(NULL, TEXT("button"), TEXT("BHOP"),
			WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			10, 180, 185, 35,
			hWnd, (HMENU)IDC_HOPBOX, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		CreateWindow(TEXT("static"), TEXT("FOV"), WS_CHILD | WS_VISIBLE | WS_BORDER, 25, 68, 45, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("static"), TEXT("ESP Distance"), WS_CHILD | WS_VISIBLE | WS_BORDER, 15, 128, 95, 20, hWnd, NULL, NULL, NULL);
		FOVtext = CreateWindow(TEXT("Edit"), TEXT("45"), WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 68, 60, 20, hWnd, NULL, NULL, NULL);
		ESPtext = CreateWindow(TEXT("Edit"), TEXT("0"), WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 128, 60, 20, hWnd, NULL, NULL, NULL);

		break;
	}

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:

			AimCheck = IsDlgButtonChecked(hWnd, IDC_AIMBOX);
			ESPCheck = IsDlgButtonChecked(hWnd, IDC_ESPBOX);
			TriggerCheck = IsDlgButtonChecked(hWnd, IDC_TRIGGERBOX);
			HopCheck = IsDlgButtonChecked(hWnd, IDC_HOPBOX);

			
			HANDLE aimThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)aimThreadTask, 0, CREATE_SUSPENDED, NULL);
			HANDLE glowThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ESPThreadTask, 0, CREATE_SUSPENDED, NULL);
			HANDLE triggerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TriggerThreadTask,0, CREATE_SUSPENDED, NULL);
			HANDLE hopThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HopThreadTask,0, CREATE_SUSPENDED, NULL);
			LPWSTR* lpString;

            switch (wmId)
            {

			case IDC_AIMBOX:
			
				if (AimCheck) {
					CheckDlgButton(hWnd, IDC_AIMBOX, BST_CHECKED);
					TCHAR buff[1024];
					GetWindowText(FOVtext, buff, 4);

					FOV = _wtof(buff);
					ResumeThread(aimThread);
					
					SetWindowText(hWnd, TEXT("AIMCHECKED"));
				}
				else if(!AimCheck){
					
					CheckDlgButton(hWnd, IDC_AIMBOX, BST_UNCHECKED);
					SetWindowText(hWnd, TEXT("AIM UNCHECKED"));
					
				}
				break;


			case IDC_ESPBOX:
				
				if (ESPCheck) {
					CheckDlgButton(hWnd, IDC_ESPBOX, BST_CHECKED);
					SetWindowText(hWnd, TEXT("ESP CHECKED"));

					TCHAR buff[1024];
					GetWindowText(ESPtext, buff, 4);

					ESPDistance = _wtof(buff);

					ResumeThread(glowThread);


				}
				else {
					CheckDlgButton(hWnd, IDC_ESPBOX, BST_UNCHECKED);
					SetWindowText(hWnd, TEXT("ESP UNCHECKED"));
				}
				break;

			case IDC_TRIGGERBOX:
				if (TriggerCheck) {
					CheckDlgButton(hWnd, IDC_TRIGGERBOX, BST_CHECKED);
					SetWindowText(hWnd, TEXT("TRIGGER CHECKED"));
					ResumeThread(triggerThread);
				}
				else {
					CheckDlgButton(hWnd, IDC_TRIGGERBOX, BST_UNCHECKED);
					SetWindowText(hWnd, TEXT("TRIGGER UNCHECKED"));
				}
				break;

			case IDC_HOPBOX:
				if (HopCheck) {
					CheckDlgButton(hWnd, IDC_HOPBOX, BST_CHECKED);
					SetWindowText(hWnd, TEXT("HOP CHECKED"));
					ResumeThread(hopThread);
				}
				else {
					CheckDlgButton(hWnd, IDC_HOPBOX, BST_UNCHECKED);
					SetWindowText(hWnd, TEXT("HOP UNCHECKED"));
				}
				break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
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
