#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

HWND getHWND(string text);
bool SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
void captureScreenshot(HWND hWnd);
void captureScreenshot(HWND hWnd, int x, int y, int w, int h, string name);
cv::Point matchImages(cv::Mat base, cv::Mat match, float acc);
vector<cv::Point> getArrayMatches(cv::Mat base, vector<cv::Mat> matches);

#endif