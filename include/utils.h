#ifndef UTILS_H
#define UTILS_H

#include <window.h>

/*
 * Contains helpful functions and constants.
 */

// TODO: turn these constants into command line parameters.

const int GOBLINS_TO_TRAIN = 100;
const int RAID_RADIUS = 100;
const int TROOP_SEPARATION = 10;
const int SCREEN_BOTTOM_LIMIT = 640;
const int MIN_MINE_LEVEL = 8;
const int MAX_MINE_LEVEL = 12;
const double MATCH_ACCURACY = 0.75;

double degToRad(double deg);
double degSin(double deg);
double degCos(double deg);
void pauseTime(int milliseconds);

#endif