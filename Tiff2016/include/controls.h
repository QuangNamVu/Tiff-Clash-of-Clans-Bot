#ifndef CONTROLS_H
#define CONTROLS_H

#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>
#include "coords.h"
#include "utils.h"

using namespace std;

/*
 * Simulates user controls like clicks and key actions.
 */

void sendClick(HWND hWnd, Coords coords);
void sendClick(HWND hWnd, int x, int y);
void sendMouseDown(HWND hWnd, int x, int y);
void sendMouseUp(HWND hWnd, int x, int y);

void zoomOut(HWND hWnd);
void zoomIn(HWND hWnd);

void raidCoordinates(vector<cv::Point> v, HWND hWnd);

#endif