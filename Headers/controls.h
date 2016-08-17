#ifndef _controlsh_
#define _controlsh_


#include <iostream>
#include <windows.h>
using namespace std;
//175 557   835 515
			//715 531--- 1117 177
void hitNextButton(HWND hWnd);
void hitBarracks(HWND hWnd);
void hitNextBarracksButton(HWND hWnd);
void hitTrainTroopsButton(HWND hWnd);
void hitBarbarianButton(HWND hWnd);
void hitEndBattleButton(HWND hWnd);
void hitAttackButton(HWND hWnd);
void hitFindMatchButton(HWND hWnd);
void hitReturnHomeButton(HWND hWnd);
void hitConfirmEndButton(HWND hWnd);
void hitBarracksXButton(HWND hWnd);

void hitOpenChatButton(HWND hWnd);
void hitOpenDonateButton(HWND hWnd);

void setControlDims(int w, int h);
void LeftClick ( );
void MouseMove ( int x, int y );
void LeftUp();
void LeftDown();
void dragUp(HWND hWnd);
void dragDown(HWND hWnd);

void zoomOut(HWND hWnd);
void zoomIn(HWND hWnd);

void sendMouseMove(int x, int y, HWND hWnd);
void sendClick(int x, int y, HWND hWnd);
void sendMouseDown(int x, int y, HWND hWnd);
void sendMouseUp(int x, int y, HWND hWnd);
void dragMouse(int x0, int y0, int xf, int yf, HWND hWnd);

void test(HWND hWnd);
#endif