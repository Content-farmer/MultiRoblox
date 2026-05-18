#include <windows.h>
#include <stdio.h>

#include "gui.h"

static BOOL LockIsActive(HANDLE handle, DWORD error)
{
    return handle != NULL && error != ERROR_ALREADY_EXISTS;
}

static void SetStatusText(void)
{
    WCHAR status[512];
    BOOL mutexActive = LockIsActive(g_singleton_mutex, g_singleton_mutex_error);
    BOOL eventActive = LockIsActive(g_singleton_event, g_singleton_event_error);

    if (mutexActive || eventActive)
    {
        swprintf_s(status, 512,
            L"Status: ready. Keep this window open while launching Roblox.\r\n"
            L"Legacy lock: %s\r\n"
            L"Current lock: %s",
            mutexActive ? L"active" : L"not created",
            eventActive ? L"active" : L"not created");
    }
    else
    {
        swprintf_s(status, 512,
            L"Status: Roblox may already be running. Close every Roblox window, "
            L"restart MultiRoblox, then launch Roblox again.");
    }

    SetWindowTextW(g_status, status);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        StopRobloxLocks();
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    StartRobloxLocks();

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MULTIROBLOX_CLASS";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    g_hwnd = CreateWindowExW(0, L"MULTIROBLOX_CLASS", L"MultiRoblox", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 430, 360, NULL, NULL, hInstance, NULL);
    if (g_hwnd == NULL)
    {
        return 0;
    }

    g_title = CreateWindowW(L"STATIC", L"MultiRoblox 0.0.4", WS_VISIBLE | WS_CHILD, 14, 14, 390, 22, g_hwnd, NULL, hInstance, NULL);
    g_status = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 14, 44, 390, 64, g_hwnd, NULL, hInstance, NULL);
    g_textbox = CreateWindowW(L"EDIT",
        L"How to use:\r\n\r\n"
        L"1. Close every Roblox window.\r\n"
        L"2. Open MultiRoblox and leave this window open.\r\n"
        L"3. Launch Roblox on each account you want to use.\r\n\r\n"
        L"Closing MultiRoblox releases the lock, so Roblox may return to one instance.",
        WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        14, 120, 390, 180, g_hwnd, NULL, hInstance, NULL);

    SetStatusText();

    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    MSG msg = { 0 };
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
