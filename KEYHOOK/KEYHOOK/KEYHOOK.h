#pragma once
#include <windows.h>

HHOOK g_hook;

//安装钩子
extern "C" __declspec(dllexport) BOOL InstallHook();


//卸载钩子
extern "C" __declspec(dllexport) BOOL UninstallHook();

//钩子处理函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
