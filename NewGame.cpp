#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include "Game.h"
#include "NewGame.h"
#include <thread>
#include <chrono>
using namespace std;

Game* game;
/*
void renderThreadFunc(){

    int FPS = 60;
    int frameDelay = 1000 / FPS;
    int frameTime = 0;
    while(game->isRunning()){
        frameTime = SDL_GetTicks();
        if(!game->isPause()){
            game->render();
        }
        frameTime = frameDelay - SDL_GetTicks() + frameTime;
        cout<<frameTime<<"render! \n";
        if(frameTime > 0){
            this_thread::sleep_for(chrono::milliseconds(frameTime));
        }
    }
}
*/
void newGame(SDL_Window* window, SDL_Renderer* renderer, int diffMode){

    game = new Game(window, renderer);

    bool render = true;
    int UPS = 60;
    int updateDelay = 1000 / UPS;
    int updateTime = 0;
    //thread renderThread;
    while(game->isRunning()){
        updateTime = SDL_GetTicks();
        game->handleEvents();

        if(!game->isPause()){
            game->handleEvents();
            game->update();
            game->render();
        }
        if(render){
            render = false;
            //renderThread = thread(renderThreadFunc);
        }
        updateTime = updateDelay - SDL_GetTicks() + updateTime;

        if(updateTime>0){
            this_thread::sleep_for(chrono::milliseconds(updateTime));
        }
    }
}
