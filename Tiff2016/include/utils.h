#ifndef UTILS_H
#define UTILS_H

/*
 * Contains helpful functions and constants.
 */
const int GOBLINS_TO_TRAIN = 100;
const int RAID_RADIUS = 100;
const int TROOP_SEPARATION = 10;
const int SCREEN_BOTTOM_LIMIT = 640;
const int MIN_MINE_LEVEL = 8;
const int MAX_MINE_LEVEL = 12;
const double MATCH_ACCURACY = 0.75;

const int KEY_PRESS_TRIES = 10;
const int UP_ARROW_KEYCODE = 0x26;
const int DOWN_ARROW_KEYCODE = 0x28;

const int CLICK_DELAY = 10;
const int ATTACK_DELAY = 1000;
const int FIND_MATCH_DELAY = 5000;
const int NEXT_DELAY = 4000;
const int SURRENDER_DELAY = 1000;
const int CONFIRM_DELAY = 1000;
const int RETURN_DELAY = 4000;
const int ARMY_DELAY = 1000;
const int TRAIN_DELAY = 1000;
const int EXIT_DELAY = 1000;
const int RAID_DELAY = 20000;

double degToRad(double deg) {
    return deg * 0.0174532925;
}

double degSin(double deg) {
    std::sin(degToRad(deg));
}

double degCos(double deg) {
    std::cos(degToRad(deg));
}

void pauseTime(int milliseconds) {
    Sleep(milliseconds);
}

#endif