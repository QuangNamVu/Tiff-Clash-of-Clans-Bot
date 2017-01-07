#ifndef BOT_H
#define BOT_H

#include <sstream>
#include "utils.h"
#include "window.h"
#include "controls.h"

class State;

/*
 * Contains the state machine and functionality needed to run the auto-raid.
 */
class Bot {
public:
    int radius;
    
    Bot();

    void begin();
    State* getCurrentState();
    void transitionState(State* nextState);
    
    vector<cv::Mat> loadSprites();
    vector<cv::Mat> getMatches();
private:
    HWND hWnd;
    State* currentState;
    vector<cv::Mat> matches;
};


class State {
public:
    virtual void enter(HWND window, Bot* b) {
        hWnd = window;
        bot = b;
    };
protected:
    Bot* bot;
    HWND hWnd;
};


class InitState : public State {
public:
    void enter(HWND window, Bot* b);
};


class SearchState : public State {
public:
    void enter(HWND window, Bot* b);
};


class RaidState : public State {
public:
    RaidState(vector<cv::Point> matches);
    void enter(HWND window, Bot* b);
private:
    vector<cv::Point> matchArray;
};


class HomeState : public State {
public:
    void enter(HWND window, Bot* b);
};

#endif