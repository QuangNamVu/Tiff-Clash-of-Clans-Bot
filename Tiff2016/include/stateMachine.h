#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "controls.h"
#include "window.h"

class State;


class StateMachine
{
public:
    void transitionState(State* nextState);
    State* getCurrentState();

    void init(HWND window);
    vector<cv::Mat> getMatches();
private:
    State* currentState;
    HWND hWnd;
    vector<cv::Mat> matches;
};

class State
{
public:
    virtual void enter(HWND window, StateMachine* sm) {
        hWnd = window;
        stateMachine = sm;
    };
protected:
    StateMachine* stateMachine;
    HWND hWnd;
};

class InitState : public State
{
public:
    virtual void enter(HWND window, StateMachine* sm);
};

class SearchState : public State
{
public:
    void enter(HWND window, StateMachine* sm);
};

class RaidState : public State
{
public:
    RaidState(vector<cv::Point> matches);
    void enter(HWND window, StateMachine* sm);
private:
    vector<cv::Point> matchArray;
};


class HomeState : public State
{
public:
    void enter(HWND window, StateMachine* sm);
};

#endif