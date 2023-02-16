#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include "Game.h"
#include "NewGame.h"
#include <thread>
#include <chrono>
using namespace std;

Game* game;

void renderThreadFunc(){

    int FPS = 60;
    int frameDelay = 1000 / FPS;
    while(game->isRunning()){
        if(!game->isPause()){
            game->render();
        }
        this_thread::sleep_for(chrono::milliseconds(frameDelay));
    }
    cout<<"end render\n";
}

void newGame(SDL_Window* window, SDL_Renderer* renderer, int diffMode){

    game = new Game(window, renderer);
    thread renderThread(renderThreadFunc);

    int UPS = 65;
    int updateDelay = 1000 / UPS;

    while(game->isRunning()){
        game->handleEvents();

        if(!game->isPause()){
            game->handleEvents();
            game->update();
        }

        this_thread::sleep_for(chrono::milliseconds(updateDelay));


    }
}
