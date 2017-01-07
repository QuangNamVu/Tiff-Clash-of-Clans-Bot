#include "bot.h"

/*
 * Loads the gold mine sprites from Images directory.
 */
vector<cv::Mat> Bot::loadSprites() {
    vector<cv::Mat> matches;
    stringstream pathStream;

    string pathPrefix = "Images/gold";
    string pathSuffix = ".png";

    for (int i = MIN_MINE_LEVEL; i <= MAX_MINE_LEVEL; i++) {
        pathStream << pathPrefix << i << pathSuffix;
        cv::Mat image = cv::imread(pathStream.str(), 1);
        matches.push_back(image);

        pathStream.str(std::string());
        pathStream.clear();
    }

    return matches;
}

/*
 * Initializes the window handle and sprite list.
 */
Bot::Bot() {
    hWnd = getHWND("Nox App Player");
    matches = loadSprites();
}

void Bot::begin() {
    InitState* initState = new InitState();
    currentState = initState;
    
    currentState->enter(hWnd, this);
}

/*
 * Free the previous state and enter the next state.
 */
void Bot::transitionState(State* nextState) {
    delete currentState;
    currentState = nextState;
    currentState->enter(hWnd, this);
}

State* Bot::getCurrentState() {
    return currentState;
}

vector<cv::Mat> Bot::getMatches() {
    return matches;
}

/*
 * Init State
 * Aligns the screen and starts the raid search.
 * Transitions into search state.
 */
void InitState::enter(HWND window, Bot* b) {
    hWnd = window;
    bot = b;

    zoomOut(hWnd);
    sendClickAndWait(hWnd, Button::ATTACK);
    sendClickAndWait(hWnd, Button::FIND_MATCH);

    SearchState* searchState = new SearchState();
    bot->transitionState(searchState);
}

/*
 * Search State
 * Takes a screenshot of the base and runs opencv image detection on
 * the gold mine sprites. Transitions to RaidState once a match is found,
 * otherwise keep searching.
 */
void SearchState::enter(HWND window, Bot* b) {
    bot = b;
    hWnd = window;

    cv::Mat base;
    vector<cv::Mat> goldMatches = bot->getMatches();
    vector<cv::Point> goldMinesFound;
    bool finished = false;

    while (!finished) {
        // Run image detection on sprites to find gold mines
        captureScreenshot(hWnd);
        base = cv::imread("fullscreen.bmp");
        goldMinesFound = getArrayMatches(base, goldMatches);

        if (goldMinesFound.size() > 1) {
            // Found gold mines, break to raid
            break;
        } else {
            // Not found, keep searching
            sendClick(hWnd, Button::NEXT);
        }
    }

    RaidState* raidState = new RaidState(goldMinesFound);
    bot->transitionState(raidState);
}

/*
 * Raid State
 * Attacks the base, targetting the gold mines.
 * Transitions to HomeState after the raid is completed.
 */
void RaidState::enter(HWND window, Bot* b) {
    hWnd = window;
    bot = b;
    
    // Takes each the coordinate of each gold mine found and deploys
    // troops in a circle around it.
    raidCoordinates(matchArray, hWnd);
    pauseTime(Button::RAID_DELAY);
    
    // Exit the raid and return home.
    sendClickAndWait(hWnd, Button::SURRENDER);
    sendClickAndWait(hWnd, Button::CONFIRM);
    sendClickAndWait(hWnd, Button::RETURN);

    HomeState* homeState = new HomeState();
    bot->transitionState(homeState);
}

RaidState::RaidState(vector<cv::Point> matches) {
    matchArray = matches;
}

/*
 * Home State
 * Trains a new army for next attack and transitions back into InitState.
 */
void HomeState::enter(HWND window, Bot* b) {
    bot = b;
    hWnd = window;

    // Open the barracks
    sendClickAndWait(hWnd, Button::ARMY);
    sendClickAndWait(hWnd, Button::TRAIN);

    // Trains a bunch of goblins for the next raid.
    for (int i = 0; i < GOBLINS_TO_TRAIN; i++) {
        sendClickAndWait(hWnd, Button::GOBLIN);
    }
    
    sendClickAndWait(hWnd, Button::EXIT_ARMY);

    InitState* initState = new InitState();
    bot->transitionState(initState);
}