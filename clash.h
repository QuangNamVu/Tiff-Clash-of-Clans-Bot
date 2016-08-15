#ifndef _clashh_
#define _clashh_

#include "stdafx.h"
#include "controls.h"
#include <string>
#include "string.h"
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <sstream>
#include "math.h"
#include "highgui.h"
#include "imgproc/imgproc.hpp"

using namespace std;

void dropBarbs(int x, int y, int num, HWND hWnd);
void lineOfUnits(int x0, int y0, int xf, int yf, int num, HWND hWnd);
void barch(HWND hWnd);
//void barchLeft(int number);
void selectTroops(int slot, HWND hWnd);;
int getGold(string text);
int getElixir(string text);
void barch4Sides(int num, HWND hWnd);
void refillBarracks(HWND hWnd);
void barchTop(int num, HWND hWnd);
void barchCoordinates(vector<cv::Point> v, HWND hWnd);

double degToRad(double deg);
#endif
//180 565
// 980 800
//457 450
//1175 470