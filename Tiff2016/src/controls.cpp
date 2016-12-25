#include "controls.h"

void sendClick(HWND hWnd, Coords coords) {
    sendClick(hWnd, coords.getX(), coords.getY());
}

void sendClick(HWND hWnd, int x, int y) {
    sendMouseDown(hWnd, x, y);
    sendMouseUp(hWnd, x, y);

    cout << "Clicking: " << x << ", " << y << "\n";
}

void sendMouseDown(HWND hWnd, int x, int y) {
    SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x,y));
}

void sendMouseUp(HWND hWnd, int x, int y) {
    SendMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x,y));
}

void sendMouseMove(HWND hWnd, int x, int y) {
    SendMessage(hWnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(x,y));
}

void zoomOut(HWND hWnd) {
    cout << "Zoom out\n";
    for( int i = 0; i < 10; i++ ) {
        PostMessage(hWnd, WM_KEYDOWN, (0x28), 1); // down, press
        PostMessage(hWnd, WM_KEYUP, (0x28), 1); // down, unpress
    }
}

void zoomIn(HWND hWnd) {
    cout << "Zoom in\n";
    for( int i = 0; i < 1; i++ ) {
        PostMessage(hWnd, WM_KEYDOWN, (0x26), 1); // Up, press
        PostMessage(hWnd, WM_KEYUP, (0x26), 1); // Up, unpress
    }
}

double degToRad(double deg) {
    return deg * 0.0174532925;
}

void raidCoordinates(vector<cv::Point> v, HWND hWnd) {
    vector<cv::Point> deployVec;

    for(int i = 0; i < v.size(); i++) {
        cv::Point coord = v[i];
        
        if(coord.y > 640)
            continue;

        for(int i = 0; i < 360; i+=10) {
            cv::Point pt = cv::Point(
                coord.x + std::sin((double)degToRad(i)) * 100,
                coord.y + std::cos((double)degToRad(i)) * 100);
            deployVec.push_back(pt);
            // cout << degToRad(std::sin((double)i)) * 100; // Print coord
        }
    }

    for(int i = 0; i < deployVec.size(); i++) {
        cv::Point coord = deployVec[i];
        sendClick(hWnd, coord.x, coord.y);
        // cout << coord << endl; // Print coord
        Sleep(10);
    }
}