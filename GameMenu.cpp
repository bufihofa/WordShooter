#include "GameMenu.h"



MainMenu* mainMenu;
PlayMenu* playMenu;

void openMenu(SDL_Window* window, SDL_Renderer* renderer, int _width, int _height){
    if(mainMenu == NULL){
        mainMenu = new MainMenu(window, renderer, _width, _height);
        playMenu = new PlayMenu(window, renderer);
    }
    Entity snip = Entity(0, 300, "resources/pointer.png", renderer);
    snip.setScale(0.45);
    snip.setAC(0 , 0);
    snip.render2();
    SDL_RenderPresent(renderer);
    SDL_Event ev;

    int FPS = 60;
    int frameDelay = 1000 / FPS;
    int frameTime = 0;
    int menuIsOpen = 0;
    bool running = true;

    while(running){
        frameTime = SDL_GetTicks();

        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT)
                running = false;
            else if(menuIsOpen == 0){
                if(ev.type == SDL_MOUSEMOTION){
                    snip.setAC(ev.button.x , ev.button.y);
                    mainMenu->checkHL(ev.button.x , ev.button.y);
                }
                else if(ev.type == SDL_MOUSEBUTTONDOWN){
                    int temp = mainMenu->checkClick(ev.button.x , ev.button.y);
                    if(temp == 1){
                        //play
                        menuIsOpen = 1;
                    }
                    else if(temp == 2){
                        //setting
                    }
                    else if(temp == 3){
                        //guide
                    }
                    else if(temp == 4){
                        //quit
                        ev.type = SDL_QUIT;
                        return;
                    }
                }
            }
            else if(menuIsOpen == 1){
                if(ev.type == SDL_MOUSEMOTION){
                    snip.setAC(ev.button.x , ev.button.y);
                    playMenu->checkHL(ev.button.x , ev.button.y);
                }
                else if(ev.type == SDL_MOUSEBUTTONDOWN){
                    int temp = playMenu->checkClick(ev.button.x , ev.button.y);
                    if(temp == 1){
                        newGame(window, renderer, 1);
                    }
                    else if(temp == 2){
                        //medium
                    }
                    else if(temp == 3){
                        //hard
                    }
                    else if(temp == 4){
                        //back
                        menuIsOpen = 0;
                    }
                }
            }
        }


        mainMenu->render(renderer);
        if(menuIsOpen == 1){
            playMenu->render(renderer);
        }
        snip.setAngle(snip.getAngle()+3);
        snip.render2();

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameTime;
        this_thread::sleep_for(chrono::milliseconds(frameDelay-frameTime));
    }
}




