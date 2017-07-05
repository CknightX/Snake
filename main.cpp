
#include <windows.h>  
#include "map.h"


Map m(20,20,20);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       //�ص�����  

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Snake");       //����һ����̬����չ�ַ���  
	HWND hwnd;      //����һ�����  
	MSG msg;        //����һ����Ϣ�ṹ  
	WNDCLASS wndclass;      //����һ��������ṹ  

	RECT rect = m.genRECT();
	auto result = AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, 0, 0);


	wndclass.style = CS_HREDRAW | CS_VREDRAW;       //���ڷ��  
	wndclass.lpfnWndProc = WndProc;                 //�ص�����  
	wndclass.cbClsExtra = 0;                        //Ԥ���Ķ���ռ�  
	wndclass.cbWndExtra = 0;                        //Ԥ���Ķ���ռ�  
	wndclass.hInstance = hInstance;                 //�ô��ڵľ��  
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);       //Ϊ���л��ڸô�����Ĵ����趨һ��ͼ��  
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);         //Ϊ���л��ڸô�����Ĵ����趨һ�����ָ��  
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);       //ָ�����ڱ���ɫ  
	wndclass.lpszMenuName = NULL;           //ָ�����ڲ˵�  
	wndclass.lpszClassName = szAppName;     //ָ����������,�������Ա  

	if (!RegisterClass(&wndclass))      //ע���Ҳ��Ըô�����  
	{
		MessageBox(NULL, TEXT("���������Ҫ�� Windows NT ����ִ�У�"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(        //��������  
		szAppName,              //��������,�������Ա  
		TEXT("Snake"),        //������,�����û�  
		WS_OVERLAPPEDWINDOW,    //���ڷ��  
		CW_USEDEFAULT,          //ָ�����ڵĳ�ʼˮƽλ��  
		CW_USEDEFAULT,          //ָ�����ڵĳ�ʼ��ֱλ��  
		rect.right-rect.left+1,          //ָ�����ڵĿ��  
		rect.bottom-rect.top+1,          //ָ�����ڵĸ߶�  
		NULL,                   //ָ�򱻴������ڵĸ����ڻ������ߴ��ڵľ��  
		NULL,                   //�˵����  
		hInstance,              //�봰���������ģ�������ľ��  
		NULL);                  //�ݺ���  

		
	ShowWindow(hwnd, iCmdShow); //����ָ�����ڵ���ʾ״̬  
	UpdateWindow(hwnd);         //�ݺ���  
	while (GetMessage(&msg, NULL, 0, 0))    //����Ϣ�����л�ȡ��Ϣ������msg�ṹ��  
	{
		TranslateMessage(&msg);     //���������Ϣת��Ϊ�ַ���Ϣ  
		DispatchMessage(&msg);      //����һ����Ϣ�����ڳ���  
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool conflict = false;
	static DIRECT curr_direct=RIGHT; //Ĭ������
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