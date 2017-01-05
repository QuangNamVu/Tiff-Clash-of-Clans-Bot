#include "controls.h"

void sendClick(HWND hWnd, Coords coords) {
    sendClick(hWnd, coords.getX(), coords.getY());
}

void sendClick(HWND hWnd, int x, int y) {
    sendMouseDown(hWnd, x, y);
    sendMouseUp(hWnd, x, y);
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

void zoomIn(HWND hWnd) {
    for( int i = 0; i < KEY_PRESS_TRIES; i++ ) {
        PostMessage(hWnd, WM_KEYDOWN, UP_ARROW_KEYCODE, 1); // Up arrow, press
        PostMessage(hWnd, WM_KEYUP, UP_ARROW_KEYCODE, 1);   // Up arrow, unpress
    }
}

void zoomOut(HWND hWnd) {
    for( int i = 0; i < KEY_PRESS_TRIES; i++ ) {
        PostMessage(hWnd, WM_KEYDOWN, DOWN_ARROW_KEYCODE, 1); // Down arrow, press
        PostMessage(hWnd, WM_KEYUP, DOWN_ARROW_KEYCODE, 1);   // Down arrow, unpress
    }
}

void raidCoordinates(vector<cv::Point> v, HWND hWnd) {
    vector<cv::Point> deployCoordinates;

    for(int i = 0; i < v.size(); i++) {
        cv::Point coord = v[i];
        
        // Gets a circle centered around coord,
        // with radius of RAID_RADIUS pixels,
        // separated by TROOP_SEPARATION degrees.
        for(int i = 0; i < 360; i += TROOP_SEPARATION) {
            cv::Point pt = cv::Point(
                coord.x + degSin(i)*RAID_RADIUS,
                coord.y + degCos(i)*RAID_RADIUS);

            // Skip if coordinate goes past bottom bar.
            if(coord.y < SCREEN_BOTTOM_LIMIT)
                deployCoordinates.push_back(pt);
        }
    }

    for(int i = 0; i < deployCoordinates.size(); i++) {
        cv::Point coord = deployCoordinates[i];
        sendClick(hWnd, coord.x, coord.y);
        pauseTime(CLICK_DELAY);
    }
}