#include "window.h"
#include "bot.h"

/*
 * Requires Nox App Player to be open, and Clash of Clans loaded 
 * on the home screen. Starts off the raid bot.
 */
int main(void) {
    Bot* bot = new Bot();
    bot->begin();

    delete bot;
    
    std::cin.ignore();
    return EXIT_SUCCESS;
}