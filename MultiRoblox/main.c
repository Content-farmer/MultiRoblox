#include <windows.h>

#include "gui.h"

HWND g_hwnd;
HWND g_title;
HWND g_textbox;
HWND g_status;

HANDLE g_singleton_mutex;
HANDLE g_singleton_event;
DWORD g_singleton_mutex_error;
DWORD g_singleton_event_error;

void StartRobloxLocks(void)
{
	static BOOL started = FALSE;
	if (started)
	{
		return;
	}

	started = TRUE;
	SetLastError(ERROR_SUCCESS);
	g_singleton_mutex = CreateMutexW(NULL, TRUE, L"ROBLOX_singletonMutex");
	g_singleton_mutex_error = GetLastError();

	SetLastError(ERROR_SUCCESS);
	g_singleton_event = CreateEventW(NULL, TRUE, FALSE, L"ROBLOX_singletonEvent");
	g_singleton_event_error = GetLastError();
}

void StopRobloxLocks(void)
{
	if (g_singleton_mutex != NULL)
	{
		ReleaseMutex(g_singleton_mutex);
		CloseHandle(g_singleton_mutex);
		g_singleton_mutex = NULL;
	}

	if (g_singleton_event != NULL)
	{
		CloseHandle(g_singleton_event);
		g_singleton_event = NULL;
	}
}

int main()
{
	StartRobloxLocks();

	HINSTANCE hInstance = GetModuleHandle(NULL);

	return WinMain(hInstance, NULL, NULL, SW_SHOWDEFAULT);
}
