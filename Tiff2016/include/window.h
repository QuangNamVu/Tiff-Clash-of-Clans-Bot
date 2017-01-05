#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>
#include "utils.h"

using namespace std;

/*
 * Contains functions for the Windows API and opencv manipuation
 * of images.
 */

HWND getHWND(string text);
void captureScreenshot(HWND hWnd);
void captureScreenshot(HWND hWnd, int x, int y, int w, int h, string name);
bool SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
cv::Point matchImages(cv::Mat base, cv::Mat match, float acc);
vector<cv::Point> getArrayMatches(cv::Mat base, vector<cv::Mat> matches);

#endif