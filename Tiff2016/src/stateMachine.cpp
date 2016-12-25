#include "stateMachine.h"

vector<cv::Mat> loadImages() {
    cv::Mat base = cv::imread("fullscreen.bmp", 1);
    cv::Mat gold12 = cv::imread("Images/gold12.png", 1);
    cv::Mat gold11 = cv::imread("Images/gold11.png", 1);
    cv::Mat gold10 = cv::imread("Images/gold10.png", 1);
    cv::Mat gold9 = cv::imread("Images/gold9.png", 1);
    cv::Mat gold8 = cv::imread("Images/gold8.png", 1);

    vector<cv::Mat> matches;
    matches.push_back(gold12);
    matches.push_back(gold11);
    matches.push_back(gold10);
    matches.push_back(gold9);
    matches.push_back(gold8);

    return matches;
}

void StateMachine::init(HWND window) {
    hWnd = window;
    matches = loadImages();
    
    InitState* initState = new InitState();
    currentState = initState;
    currentState->enter(hWnd, this);
}

void StateMachine::transitionState(State* nextState) {
    delete currentState;
    currentState = nextState;
    currentState->enter(hWnd, this);
}

State* StateMachine::getCurrentState() {
    return currentState;
}

vector<cv::Mat> StateMachine::getMatches() {
    return matches;
}

// Init State

void InitState::enter(HWND window, StateMachine* sm) {
    cout << "\n*******State Init*******\n\n";
    hWnd = window;
    stateMachine = sm;

    zoomOut(hWnd);

    sendClick(hWnd, Coords::ATTACK);
    Sleep(1000);
    sendClick(hWnd, Coords::FIND_MATCH);
    Sleep(5000);

    SearchState* searchState = new SearchState();
    stateMachine->transitionState(searchState);
}

// Search State

void SearchState::enter(HWND window, StateMachine* sm) {
    cout << "\n*******State Search*******\n\n";
    stateMachine = sm;
    hWnd = window;
    // Screenshot
    bool finished = false;
    cv::Mat base = cv::imread("fullscreen.bmp");
    vector<cv::Mat> goldMatches = stateMachine->getMatches();
    vector<cv::Point> matchArray;

    while (!finished) {
        captureScreenshot(hWnd);
        base = cv::imread("fullscreen.bmp");
        matchArray = getArrayMatches(base, goldMatches);

        if (matchArray.size() > 1) {
            finished = true;
            cout << "Gold found, beginning raid\n";
        } else {
            sendClick(hWnd, Coords::NEXT);
            Sleep(4000);
            cout << "Gold not found, skipping\n\n";
        }
    }

    RaidState* raidState = new RaidState(matchArray);
    stateMachine->transitionState(raidState);
}

// Raid State

RaidState::RaidState(vector<cv::Point> matches) {
    matchArray = matches;
}

void RaidState::enter(HWND window, StateMachine* sm) {
    cout << "\n*******State Raid*******\n\n";
    hWnd = window;
    stateMachine = sm;
    
    raidCoordinates(matchArray, hWnd);
    Sleep(20000);
    
    sendClick(hWnd, Coords::SURRENDER);
    Sleep(1000);
    sendClick(hWnd, Coords::CONFIRM);
    Sleep(1000);
    sendClick(hWnd, Coords::RETURN);
    Sleep(4000);

    HomeState* homeState = new HomeState();
    stateMachine->transitionState(homeState);
}

// Home State

void HomeState::enter(HWND window, StateMachine* sm) {
    stateMachine = sm;
    hWnd = window;

    cout << "\n*******State Home*******\n\n";
    sendClick(hWnd, Coords::ARMY);
    Sleep(1000);
    sendClick(hWnd, Coords::TRAIN);
    Sleep(1000);

    for (int i = 0; i < 100; i++) {
        sendClick(hWnd, Coords::GOBLIN);
        Sleep(10);
    }
    
    sendClick(hWnd, Coords::EXIT_ARMY);
    Sleep(1000);


    InitState* initState = new InitState();
    stateMachine->transitionState(initState);
}