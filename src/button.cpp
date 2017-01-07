#include "button.h"

// Contains the screen coordinates of each button and its delay time.
const Button Button::ATTACK     = Button(80, 700, 1000);
const Button Button::FIND_MATCH = Button(250, 550, 5000);
const Button Button::NEXT       = Button(1200, 600, 5000);
const Button Button::SURRENDER  = Button(100, 600, 1000);
const Button Button::CONFIRM    = Button(800, 500, 1000);
const Button Button::RETURN     = Button(650, 650, 4000);
const Button Button::ARMY       = Button(50, 600, 1000);
const Button Button::TRAIN      = Button(550, 130, 1000);
const Button Button::GOBLIN     = Button(350, 550, 10);
const Button Button::EXIT_ARMY  = Button(1080, 130, 1000);

Button::Button(int x, int y) {
    Button(x, y, 0);
}

Button::Button(int x, int y, int t) {
    xCoord = x;
    yCoord = y;
    delay = t;
}

int Button::getX() {
    return xCoord;
}

int Button::getY() {
    return yCoord;
}

int Button::getDelay() {
    return delay;
}

