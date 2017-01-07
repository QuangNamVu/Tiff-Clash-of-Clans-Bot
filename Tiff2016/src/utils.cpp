#include "utils.h"

double degToRad(double deg) {
    return deg * 0.0174532925;
}

double degSin(double deg) {
    return std::sin(degToRad(deg));
}

double degCos(double deg) {
    return std::cos(degToRad(deg));
}

void pauseTime(int milliseconds) {
    Sleep(milliseconds);
}