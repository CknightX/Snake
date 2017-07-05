
#include <windows.h>  
#include "map.h"


Map m(20,20,20);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       //回调函数  

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Snake");       //定义一个静态的扩展字符名  
	HWND hwnd;      //定义一个句柄  
	MSG msg;        //定义一个消息结构  
	WNDCLASS wndclass;      //定义一个窗口类结构  

	RECT rect = m.genRECT();
	auto result = AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, 0, 0);


	wndclass.style = CS_HREDRAW | CS_VREDRAW;       //窗口风格  
	wndclass.lpfnWndProc = WndProc;                 //回调函数  
	wndclass.cbClsExtra = 0;                        //预留的额外空间  
	wndclass.cbWndExtra = 0;                        //预留的额外空间  
	wndclass.hInstance = hInstance;                 //该窗口的句柄  
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);       //为所有基于该窗口类的窗口设定一个图标  
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);         //为所有基于该窗口类的窗口设定一个鼠标指针  
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);       //指定窗口背景色  
	wndclass.lpszMenuName = NULL;           //指定窗口菜单  
	wndclass.lpszClassName = szAppName;     //指定窗口类名,面向程序员  

	if (!RegisterClass(&wndclass))      //注册且测试该窗口类  
	{
		MessageBox(NULL, TEXT("这个程序需要在 Windows NT 才能执行！"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(        //创建窗口  
		szAppName,              //窗口类名,面向程序员  
		TEXT("Snake"),        //窗口名,面向用户  
		WS_OVERLAPPEDWINDOW,    //窗口风格  
		CW_USEDEFAULT,          //指定窗口的初始水平位置  
		CW_USEDEFAULT,          //指定窗口的初始垂直位置  
		rect.right-rect.left+1,          //指明窗口的宽度  
		rect.bottom-rect.top+1,          //指明窗口的高度  
		NULL,                   //指向被创建窗口的父窗口或所有者窗口的句柄  
		NULL,                   //菜单句柄  
		hInstance,              //与窗口相关联的模块事例的句柄  
		NULL);                  //暂忽略  

		
	ShowWindow(hwnd, iCmdShow); //设置指定窗口的显示状态  
	UpdateWindow(hwnd);         //暂忽略  
	while (GetMessage(&msg, NULL, 0, 0))    //从消息队列中获取消息并放在msg结构中  
	{
		TranslateMessage(&msg);     //将虚拟键消息转换为字符消息  
		DispatchMessage(&msg);      //调度一个消息给窗口程序  
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool conflict = false;
	static DIRECT curr_direct=RIGHT; //默认向右
	HDC hdc;
	PAINTSTRUCT ps;
	m.setHWND(hwnd);
	//m.changeDirect(curr_direct);
	switch (message)
	{
	case WM_CREATE:
		m.randFood();
		m.changeDirect(RIGHT);
		SetTimer(hwnd, 1, 10, NULL);
		return 0;
	case WM_TIMER:
		if (!m.moveSnakeOneStep())
		{
			KillTimer(hwnd, 1);
			m.gameOver();
			PostQuitMessage(0);
		}
		m.genmatrix();
		m.gameAI();
		conflict = false;
		m.clearMap();
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		m.genPane(hdc);
		m.drawMap(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (!conflict)
		{
			switch (wParam)
			{
			case VK_LEFT:
				if (curr_direct != RIGHT)
					curr_direct = LEFT;
				break;
			case VK_RIGHT:
				if (curr_direct != LEFT)
					curr_direct = RIGHT;
				break;
			case VK_UP:
				if (curr_direct != DOWN)
					curr_direct = UP;
				break;
			case VK_DOWN:
				if (curr_direct != UP)
					curr_direct = DOWN;
				break;
			}
		}
		conflict = true;
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}