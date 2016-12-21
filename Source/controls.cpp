#include "controls.h"
int screenWidth;
int screenHeight;

void hitNextButton(HWND hWnd)
{
	sendClick(1311,627,hWnd); 
	Sleep(1000);
}
			
void hitConfirmEndButton(HWND hWnd)
{
	sendClick(835,515,hWnd); 
	Sleep(1000);
}
			
			
void hitBarracksXButton(HWND hWnd)
{
	sendClick(1117,177,hWnd); 
	Sleep(1000);
}
			
void hitBarracks(HWND hWnd)
{
	sendClick(170,520,hWnd); 
	Sleep(1000);
}

void hitNextBarracksButton(HWND hWnd)
{
	sendClick(1167,435,hWnd); 
	Sleep(500);
}

void hitTrainTroopsButton(HWND hWnd)
{
	sendClick(973,763,hWnd); 
	Sleep(1000);
}

void hitBarbarianButton(HWND hWnd)
{
	sendClick(443,417,hWnd); 
	Sleep(10);
}

void hitEndBattleButton(HWND hWnd)
{
	sendClick(94,649,hWnd); 
	Sleep(1000);
}
void hitAttackButton(HWND hWnd)
{
	sendClick(93,809,hWnd); 
	Sleep(1000);
}
void hitFindMatchButton(HWND hWnd)
{
	sendClick(286,650,hWnd);
	Sleep(1000);
}
void hitReturnHomeButton(HWND hWnd)
{
	sendClick(739,739,hWnd);
	Sleep(1000);
}
void hitOpenChatButton(HWND hWnd)
{
	sendClick(40,434,hWnd);
	Sleep(1000);
}
void hitOpenDonateButton(HWND hWnd)
{

}
void setControlDims(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
}
void sendClick(int x, int y, HWND hWnd)
{
	sendMouseDown(x,y,hWnd);
	sendMouseUp(x,y,hWnd);

	std::cout << "Click: " << x << ", " << y << "\n";
}
void sendMouseDown(int x, int y, HWND hWnd)
{
	SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x,y));
}
void sendMouseMove(int x, int y, HWND hWnd)
{
	SendMessage(hWnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(x,y));
}
void zoomOut(HWND hWnd)
{
	for( int i = 0; i < 10; i++ )
	{
		SendMessage(hWnd, WM_KEYDOWN , VK_DOWN, 1);
		SendMessage(hWnd, WM_KEYUP, VK_DOWN, 1);
		Sleep(200);
	}
}
void zoomIn(HWND hWnd)
{
	cout << "ZOOMIN\n";
	for( int i = 0; i < 1; i++ )
	{
		SendMessage(hWnd, WM_KEYDOWN , VK_UP, 1);
		SendMessage(hWnd, WM_KEYUP, VK_UP, 1);
		Sleep(200);
	}
}
void sendMouseUp(int x, int y, HWND hWnd)
{
	SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x,y));
}
void dragMouse(int x0, int y0, int xf, int yf, HWND hWnd)
{
	sendMouseDown(x0, y0, hWnd);
	Sleep(50);
	sendMouseMove(xf,yf, hWnd);
	Sleep(50);
	sendMouseUp(xf, yf, hWnd);
	Sleep(50);
}

void dragUp(HWND hWnd)
{
	dragMouse(1050, 300, 1050, 500, hWnd);
	dragMouse(1050, 300, 1050, 500, hWnd);
}
void dragDown(HWND hWnd)
{
	cout << "Dragging\n";
	dragMouse(1050, 300, 1050, 500, hWnd);
	dragMouse(1050, 300, 1050, 500, hWnd);
}
void LeftClick ( )
{
	LeftDown();
	LeftUp();
}
void LeftDown()
{
  INPUT    Input={0};
  // left down 
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));

  // left up
  ::ZeroMemory(&Input,sizeof(INPUT));
}
void LeftUp()
{
  INPUT    Input={0};
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  ::SendInput(1,&Input,sizeof(INPUT));
  ::ZeroMemory(&Input,sizeof(INPUT));
}

void MouseMove (int x, int y )
{  
	double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
	double fx = x*(65535.0f/fScreenWidth);
	double fy = y*(65535.0f/fScreenHeight);
	INPUT  Input={0};
	Input.type      = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	::SendInput(1,&Input,sizeof(INPUT));
}

void test(HWND hWnd)
{
	std::cout << "INPUT\n";
	//SendMessage(hWnd, WM_CHAR, (0x53), NULL);
	Sleep(100); 
  
  // Tells ryan to shut up in chat
	SendMessage(hWnd, WM_KEYDOWN, (0x53), 1); // S
	SendMessage(hWnd, WM_KEYUP, (0x53), 1);
	Sleep(10);
	SendMessage(hWnd, WM_KEYDOWN, (0x48), 1); // H
	SendMessage(hWnd, WM_KEYUP, (0x48), 1);
	Sleep(10);
	SendMessage(hWnd, WM_KEYDOWN, (0x55), 1); // U
	SendMessage(hWnd, WM_KEYUP, (0x55), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x54), 1); // T
	PostMessage(hWnd, WM_KEYUP, (0x54), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x20), 1); // SPACE
	PostMessage(hWnd, WM_KEYUP, (0x20), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x55), 1); // U
	PostMessage(hWnd, WM_KEYUP, (0x55), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x50), 1); // P
	PostMessage(hWnd, WM_KEYUP, (0x50), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x20), 1); // SPACE
	PostMessage(hWnd, WM_KEYUP, (0x20), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x52), 1); // R
	PostMessage(hWnd, WM_KEYUP, (0x52), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x59), 1); // Y
	PostMessage(hWnd, WM_KEYUP, (0x59), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x41), 1); // A
	PostMessage(hWnd, WM_KEYUP, (0x41), 1);
	Sleep(10);
	PostMessage(hWnd, WM_KEYDOWN, (0x4E), 1); // N
	PostMessage(hWnd, WM_KEYUP, (0x4E), 1);

	/*INPUT  Input={0};
	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = 0x53;
	::SendInput(1,&Input,sizeof(INPUT));s
	::SendInput(1,&Input,sizeof(INPUT));
	KEYEVENTF_KEYUPs*/
}