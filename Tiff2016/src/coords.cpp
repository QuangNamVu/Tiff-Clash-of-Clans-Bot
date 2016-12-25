#include "coords.h"

const Coords Coords::ATTACK     = Coords(80, 700);
const Coords Coords::FIND_MATCH = Coords(250, 550);
const Coords Coords::NEXT       = Coords(1200, 600);
const Coords Coords::SURRENDER  = Coords(100, 600);
const Coords Coords::CONFIRM    = Coords(800, 500);
const Coords Coords::RETURN     = Coords(650, 650);
const Coords Coords::ARMY       = Coords(50, 600);
const Coords Coords::TRAIN       = Coords(550, 130);
const Coords Coords::GOBLIN     = Coords(350, 550);
const Coords Coords::EXIT_ARMY  = Coords(1080, 130);

Coords::Coords(int x, int y) {
    xCoord = x;
    yCoord = y;
}

int Coords::getX() {
    return xCoord;
}

int Coords::getY() {
    return yCoord;
}

