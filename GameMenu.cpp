#include "GameMenu.h"
#include "NewGame.h"
#include <thread>
#include <chrono>

Menu* mainMenu;
Menu* playMenu;
Entity* background;
void openMenu(SDL_Window* window, SDL_Renderer* renderer, int _width, int _height){
    //khoi tao cac menu
    if(background == NULL){

        background = new Entity(0, 0, "resources/MenuBackGround.png", renderer);
        background->setHW(800, 1200);
        //tao mainmenu
        mainMenu = new Menu(renderer, "resources/MainMenu.png");
        mainMenu->getPanel()->setScale(2);
        mainMenu->addButton(0, 200, "resources/PlayButton.png", renderer, 1.2, true); //0
        mainMenu->addButton(0, 300, "resources/SettingButton.png", renderer, 1, true); //1
        mainMenu->addButton(0, 380, "resources/GuideButton.png", renderer, 1, true); //2
        mainMenu->addButton(0, 500, "resources/QuitButton.png", renderer, 1, true); //3

        //tao playmenu
        playMenu = new Menu(renderer, "resources/PlayMenu.png");
        playMenu->addButton(30, 250, "resources/EasyMode.png", renderer, 0.8, false); //0
        playMenu->addButton(250, 250, "resources/MediumMode.png", renderer, 0.8, true); //1
        playMenu->addButton(770, 250, "resources/HardMode.png", renderer, 0.8, false); //2
        playMenu->addButton(920, 70, "resources/CloseButton.png", renderer, 0.15, false); //3
        playMenu->addButton(50, 190, "resources/difficultyText.png", renderer, 1.3, true); //4

        //playMenu->getPanel()->setH(playMenu->getPanel()->getH() * 1.4);
        playMenu->getPanel()->setW(playMenu->getPanel()->getW() * 1.4);
        playMenu->getPanel()->setAC(600,400);

    }
    //tao con tro chuot
    Entity snip = Entity(0, 300, "resources/pointer.png", renderer);
    snip.setScale(0.45);
    snip.render();

    SDL_RenderPresent(renderer);

    SDL_Event ev;

    int FPS = 60;
    int frameDelay = 1000 / FPS;
    int frameTime = 0;
    int menuIsOpen = 0;
    bool running = true;
    /*
    menuIsOpen:
    0: mainmenu
    1: playmenu
    2: setting
    3: guidemenu
    */
    while(running){
        frameTime = SDL_GetTicks();

        while(SDL_PollEvent(&ev) != 0){
            if(ev.type == SDL_QUIT)
                running = false;
            else if(menuIsOpen == 0){ //dang o mainmenu
                if(ev.type == SDL_MOUSEMOTION){
                    snip.setAC(ev.button.x , ev.button.y);
                    mainMenu->checkMouseMove(ev.button.x , ev.button.y);
                }
                else if(ev.type == SDL_MOUSEBUTTONDOWN){
                    int temp = mainMenu->checkMouseClick(ev.button.x , ev.button.y);
                    if(temp == 0){
                        //mo menu playgame
                        menuIsOpen = 1;
                    }
                    else if(temp == 1){
                        //mo menu settinggame
                    }
                    else if(temp == 2){
                        //mo menu guidegame
                    }
                    else if(temp == 3){
                        //quit game
                        running = false;
                    }
                }
            }
            else if(menuIsOpen == 1){   //dang o playmenu
                if(ev.type == SDL_MOUSEMOTION){
                    snip.setAC(ev.button.x , ev.button.y);
                    playMenu->checkMouseMove(ev.button.x , ev.button.y);
                }
                else if(ev.type == SDL_MOUSEBUTTONDOWN){
                    int temp = playMenu->checkMouseClick(ev.button.x , ev.button.y);
                    if(temp == 0){
                        menuIsOpen = 0;
                        running = false;
                        newGame(window, renderer, 1);
                    }
                    else if(temp == 1){
                        menuIsOpen = 0;
                        running = false;
                        newGame(window, renderer, 2);
                    }
                    else if(temp == 2){
                        menuIsOpen = 0;
                        running = false;
                        newGame(window, renderer, 3);
                    }
                    else if(temp == 3){
                        //quay tro lai mainmenu
                        menuIsOpen = 0;
                    }
                }
            }
        }

        if(!running) break;
        background->render();
        mainMenu->render(); //render mainmenu
        if(menuIsOpen == 1){
            playMenu->render(); //render playmenu
        }

        //render con tro
        snip.setAngle(snip.getAngle()+3);
        snip.render2();

        SDL_RenderPresent(renderer);

        frameTime = frameDelay - SDL_GetTicks() + frameTime;
        if(frameTime>0) this_thread::sleep_for(chrono::milliseconds(frameTime));
    }

}




