#ifndef GAME__H
#define GAME__H

#include <SDL.h>

class Game{
private:
    bool running = true;
    bool pause = false;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int diffMode = 1;
public:
    Game();
    Game(SDL_Window* window, SDL_Renderer* renderer, int diffMode);

    void init(const char* title, int x, int y, int h, int w);

    void handleEvents();
    void update();
    void render();

    bool isRunning(){ return running; }
    bool isPause(){ return pause; }
    void stop(){ this-> running = false;}
    void conti(){this->pause = false;}
    void pau(){this->pause = true;}

    int getDiff(){
        return diffMode;
    }
    void setDiff(int diffMode){
        this->diffMode = diffMode;
    }
    void gamePause();
    void gameContinue();
    void gameStop();
};


#endif // GAME__H
