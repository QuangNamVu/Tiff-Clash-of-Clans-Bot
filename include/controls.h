#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>
#include <iostream>
#include <windows.h>
#include "button.h"
#include "utils.h"

using namespace std;

/*
 * Simulates user controls like clicks and key actions.
 */

const int UP_ARROW_KEYCODE = 0x26;
const int DOWN_ARROW_KEYCODE = 0x28;
const int KEY_PRESS_TRIES = 10;

void sendClickAndWait(HWND hWnd, Button coords);
void sendClick(HWND hWnd, Button coords);
void sendClick(HWND hWnd, int x, int y);
void sendMouseDown(HWND hWnd, int x, int y);
void sendMouseUp(HWND hWnd, int x, int y);

void zoomOut(HWND hWnd);
void zoomIn(HWND hWnd);

void raidCoordinates(vector<cv::Point> v, HWND hWnd);

#endif