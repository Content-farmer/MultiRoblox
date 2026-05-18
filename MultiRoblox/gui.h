#pragma once

#include <windows.h>

extern HWND g_hwnd;
extern HWND g_title;
extern HWND g_textbox;
extern HWND g_status;

extern HANDLE g_singleton_mutex;
extern HANDLE g_singleton_event;
extern DWORD g_singleton_mutex_error;
extern DWORD g_singleton_event_error;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void StartRobloxLocks(void);
void StopRobloxLocks(void);
