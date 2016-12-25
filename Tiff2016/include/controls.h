#ifndef CONTROLS_H
#define CONTROLS_H

#include <windows.h>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "coords.h"

using namespace std;

void sendClick(HWND hWnd, Coords coords);
void sendClick(HWND hWnd, int x, int y);
void sendMouseDown(HWND hWnd, int x, int y);
void sendMouseUp(HWND hWnd, int x, int y);

void zoomOut(HWND hWnd);
void zoomIn(HWND hWnd);

void raidCoordinates(vector<cv::Point> v, HWND hWnd);

#endif