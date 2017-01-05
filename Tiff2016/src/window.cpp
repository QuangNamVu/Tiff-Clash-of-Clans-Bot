#include <window.h>

/*
 * Finds a window with the provided name, and returns a handle 
 * to that window.
 */ 
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

/*
 * Gets the Rect information from the window handle. 
 */
void getRECT(HWND hWnd, LPRECT prect) {
    bool cont = true;
    while (cont == true) {
        if(GetWindowRect(hWnd, prect) != 0) {
            cout << "Rect found" << endl;
            return;
        } else {
            cout << "Rect not found, try again.\n";
            system("pause");
        }
    }
}

/*
 * Takes a screenshot of the entire provided window. 
 * Saves the screenshot to "fullscreen.bmp".
 */
void captureScreenshot(HWND hWnd) {
    RECT rect;
    getRECT(hWnd, &rect);
    captureScreenshot(hWnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, "fullscreen.bmp");
}

/*
 * Takes a screenshot of the provided window at specific coordinates.
 * Saves the screenshot to the given filename;
 */
void captureScreenshot(HWND hWnd, int x, int y, int w, int h, string name) {
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hDesktopWnd = hWnd;
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, w, h);
    SelectObject(hCaptureDC,hCaptureBitmap); 

    BitBlt(hCaptureDC, -x, -y, w+x, h+y,
           hDesktopDC, 0, 0, SRCCOPY|CAPTUREBLT);
    
    SaveToFile(hCaptureBitmap, name.c_str()); 
  
    ReleaseDC(hDesktopWnd, hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);
}

/*
 * Save an image to file.
 * Code modified from MSDN documentation:
 * https://msdn.microsoft.com/en-us/library/dd145119.aspx
 */
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

    bmfHdr.bfType = 0x4D42;
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

/*
 * Takes a base image and a template, and finds the first point where
 * the template matches the image, within an accuracy range [0, 1].
 * If no match is found, returns the point (-1, -1).
 */
cv::Point matchImages(cv::Mat base, cv::Mat match, float acc) {
    cv::Mat img_display;
    cv::Mat result;

    base.copyTo(img_display);
    int result_cols = base.cols - match.cols + 1;
    int result_rows = base.rows - match.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    matchTemplate(img_display, match, result, 5);

    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    matchLoc = maxLoc;
    float err = result.at<float>(matchLoc.y, matchLoc.x);

    if( err > acc )
        return matchLoc;
    else
        return cv::Point(-1,-1);
}

/*
 * Draws a black rectangle at the coordinates specified.
 * matchImages() only finds one match, so this covers up the found
 * match so it can run the search again.
 */
cv::Mat drawRectAt(cv::Mat image, int x, int y, int w, int h) {
    cv::Mat copy;
    image.copyTo(copy);
    rectangle(copy, cv::Point(x,y), cv::Point(x+w,y+h), cv::Scalar::all(0), CV_FILLED, 8, 0);
    return copy;
}

/*
 * Finds an array of points where the base image matches the template.
 */
vector<cv::Point> getArrayMatches(cv::Mat base, vector<cv::Mat> templates) {
    cv::Mat match;
    cv::Point matchLoc;
    vector<cv::Point> v;

    // Try each template image (gold mine)
    for(int i = 0; i < templates.size(); i++) {
        match = templates.at(i);
        int matchW = match.cols;
        int matchH = match.rows;

        // Record each match, draw a black rectangle over it, and keep
        // searching until no more matches.
        while(matchLoc.x != -1) {
            matchLoc = matchImages(base, match, MATCH_ACCURACY);
            if( matchLoc.x == -1 )
                break;

            base = drawRectAt(base, matchLoc.x, matchLoc.y, matchW, matchH);
            v.push_back(matchLoc);
        }
        matchLoc = cv::Point(0,0);
    }
  
    return v;
}
