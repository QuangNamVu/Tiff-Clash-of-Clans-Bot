#ifndef COORDS_H
#define COORDS_H

class Coords {
public:
    Coords(int x, int y);
    static const Coords ATTACK;
    static const Coords FIND_MATCH;
    static const Coords NEXT;
    static const Coords SURRENDER;
    static const Coords CONFIRM;
    static const Coords RETURN;
    static const Coords ARMY;
    static const Coords TRAIN;
    static const Coords GOBLIN;
    static const Coords EXIT_ARMY;

    int getX();
    int getY();
private:
    int xCoord;
    int yCoord;
};

#endif