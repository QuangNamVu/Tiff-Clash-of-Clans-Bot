#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
    static const Button ATTACK;
    static const Button FIND_MATCH;
    static const Button NEXT;
    static const Button SURRENDER;
    static const Button CONFIRM;
    static const Button RETURN;
    static const Button ARMY;
    static const Button TRAIN;
    static const Button GOBLIN;
    static const Button EXIT_ARMY;
    
    static const int RAID_DELAY = 20000;
    static const int CLICK_DELAY = 10;
    
    Button(int x, int y);
    Button(int x, int y, int t);

    int getX();
    int getY();
    int getDelay();
private:
    int xCoord;
    int yCoord;
    int delay;
};

#endif