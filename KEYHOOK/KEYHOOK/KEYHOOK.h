#pragma once
#include <windows.h>

HHOOK g_hook;

//��װ����
extern "C" __declspec(dllexport) BOOL InstallHook();


//ж�ع���
extern "C" __declspec(dllexport) BOOL UninstallHook();

//���Ӵ�����
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
