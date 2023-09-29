#include <Windows.h>
#include <vector>

WNDCLASS createWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInstance, HICON icon, LPCWSTR windowName, WNDPROC windowProcedure);
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WNDCLASS mainWindow = createWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInstance, LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", windowProcedure);

	if (!RegisterClassW(&mainWindow))
		return -1;

	CreateWindow(L"MainWndClass", L"MainWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 800, NULL, NULL, NULL, NULL);

	MSG message = { 0 };
	while (GetMessage(&message, NULL, NULL, NULL)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

WNDCLASS createWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInstance, HICON icon, LPCWSTR windowName, WNDPROC windowProcedure)
{
	WNDCLASS wc = { 0 };

	wc.hCursor = cursor;
	wc.hIcon = icon;
	wc.hInstance = hInstance;
	wc.lpszClassName = windowName;
	wc.hbrBackground = bgColor;
	wc.lpfnWndProc = windowProcedure;

	return wc;
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hdc = NULL;
	static auto color = RGB(0, 0, 0);
	static HPEN myPen;
	POINT p;
	static POINT a[3];
	PAINTSTRUCT pt;

	static bool flag = false;
	static int status = 0;

	switch (msg)
	{
	case WM_CREATE:
		break;
	case WM_INITDIALOG: status = false;
		return TRUE;
		break;
	case WM_RBUTTONDOWN:
		InvalidateRect(hWnd, NULL, true);
		GetCursorPos(&p);
		ScreenToClient(hWnd, &p);
		a[status % 3] = p;
		status = (status + 1) % 3;
		break;
	case WM_KEYDOWN:
		if (wp == 0x43)
		{
			InvalidateRect(hWnd, NULL, TRUE);
			flag = true;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &pt);
		switch (status)
		{
		case 2:
			if (flag)
			{
				color = RGB(0, 255, 0);
				flag = false;
			}
			else
				color = RGB(255, 0, 0);
			myPen = CreatePen(PS_SOLID, 3, color);
			SelectObject(hdc, myPen);
			MoveToEx(hdc, a[0].x, a[0].y, &a[2]);
			LineTo(hdc, a[1].x, a[1].y);
			break;
		default:
			break;
		}
		EndPaint(hWnd, &pt);
		break;
	case WM_CLOSE:
		ExitProcess(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
