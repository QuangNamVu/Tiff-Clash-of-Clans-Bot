#include <window.h>

HWND getHWND(string text) {
    bool cont = true;
    while (cont == true) {
        HWND hWnd = FindWindow(NULL, text.c_str()); 
        if (hWnd != 0) {
             cout << "Window found" << endl;
            return hWnd;
        } else {
            cout << "Window not found, please open " << text << ".\n";
            system("pause");
        }
    }
    return NULL;
}

void getRECT(HWND hWnd, LPRECT prect) {
    bool cont = true;
    while (cont == true) {
        if(GetWindowRect(hWnd, prect) != 0) {
            cout << "Rect found" << endl;
            return;
        } else {
            cout << "Window not found, try again.\n";
            system("pause");
        }
    }
}

// Only works for directX not opengl!!
void captureScreenshot(HWND hWnd) {
    RECT rect;
    getRECT(hWnd, &rect);
    captureScreenshot(hWnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, "fullscreen.bmp");
}

void captureScreenshot(HWND hWnd, int x, int y, int w, int h, string name) {
    int clashX = x;
    int clashY = y;
    int clashW = w;
    int clashH = h;

    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = hWnd;
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, w, h);
    SelectObject(hCaptureDC,hCaptureBitmap); 

    BitBlt(hCaptureDC, -clashX, -clashY, clashW+clashX, clashH+clashY,
           hDesktopDC, 0, 0, SRCCOPY|CAPTUREBLT);
    
    SaveToFile(hCaptureBitmap, name.c_str()); 
  
    ReleaseDC(hDesktopWnd, hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}


bool SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName) {
    HDC hDC;
    int iBits;
    WORD wBitCount;
    DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
    BITMAP Bitmap;
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    HANDLE fh, hDib, hPal,hOldPal=NULL;
 
    hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else
    wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap.bmWidth;
    bi.biHeight = Bitmap.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;
    dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

    hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal) {
        hDC = GetDC(NULL);
        hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }

    GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
    +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

    if (hOldPal) {
        SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }

    fh = CreateFile(lpszFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
 
    if (fh == INVALID_HANDLE_VALUE)
    return FALSE;

    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

cv::Point matchImages(cv::Mat base, cv::Mat match, float acc)
{
    cv::Mat img_display;
    cv::Mat result;

    base.copyTo(img_display);
    int result_cols = base.cols - match.cols + 1;
    int result_rows = base.rows - match.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    cv::Mat beforeNorm;
    matchTemplate(img_display, match, result, 5);

    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    matchLoc = maxLoc;
    int num = 0;
    //cout << "Acc: " << result.at<float>(matchLoc.y + num, matchLoc.x + num) << endl;
    float err = result.at<float>(matchLoc.y, matchLoc.x);

    if( err > acc )
        return matchLoc;
    else
        return cv::Point(-1,-1);
}

cv::Mat drawRectAt(cv::Mat image, int x, int y, int w, int h) {
    cv::Mat copy;
    image.copyTo(copy);
    rectangle(copy, cv::Point(x,y), cv::Point(x+w,y+h), cv::Scalar::all(0), CV_FILLED, 8, 0);
    return copy;
}

vector<cv::Point> getArrayMatches(cv::Mat base, vector<cv::Mat> matches) {
    cv::Mat match;
    cv::Point matchLoc = cv::Point(0,0);
    vector<cv::Point> v;

    for(int i = 0; i < matches.size(); i++) {
        match = matches.at(i);
        int matchW = match.cols;
        int matchH = match.rows;

        while(matchLoc.x != -1) {
            matchLoc = matchImages(base, match, 0.75);
            if( matchLoc.x == -1 )
                break;
            base=drawRectAt(base, matchLoc.x, matchLoc.y, matchW, matchH);
            v.push_back(matchLoc);
        }
        matchLoc = cv::Point(0,0);
    }
  
    return v;
}
