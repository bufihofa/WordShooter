#include "Game.h"
#include "Player.cpp"
#include <bits/stdc++.h>
#include "GameMenu.h"
using namespace std;

Player player;

Entity bgr, pauseButton, faqButton, settingButton, menuButton;
Entity pausetext;
double startTime = 0;

moveEntity point;

Game::Game(SDL_Window* window, SDL_Renderer* renderer, int diffMode){

    this->setDiff(diffMode);

    this->renderer = renderer;
    this->window = window;

    bgr = Entity(0, 0, "resources/bvb.jpg", renderer);
    player = Player(0, 0, "resources/Ship_1.png", renderer);
    player.setDiff(diffMode);

    player.setAC(1200/2, 750);
    player.setScale(0.3);
    bgr.setAC(600,400);

    pauseButton = Entity(1050, -40, "resources/Pause_BTN.png", renderer);
    faqButton = Entity(1050, 30, "resources/FAQ_BTN.png", renderer);
    settingButton = Entity(1050, 100, "resources/Settings_BTN.png", renderer);
    menuButton = Entity(1050, 170, "resources/Menu_BTN.png", renderer);

    pausetext = Entity(0, 0, "resources/pausetext.png", renderer);
    pausetext.setAC(600, 400);

    pauseButton.setScale(0.3);
    faqButton.setScale(0.3);
    settingButton.setScale(0.3);
    menuButton.setScale(0.3);

    point = moveEntity(500, 500, "resources/pointer.png", renderer);
    point.setScale(0.3);

    startTime = SDL_GetTicks();
}
void Game::gamePause(){
    this->pau();
    pauseButton.setImage(loadTexture("resources/Play_BTN.png", this->renderer));
    pausetext.render();
}
void Game::gameContinue(){
    pauseButton.setImage(loadTexture("resources/Pause_BTN.png", this->renderer));
    this->conti();
}
void Game::gameStop(){
    this->pau();
    this->stop();

    player.clearAll();
    player.clearEntity();
    bgr.clearEntity();
    pauseButton.clearEntity();
    faqButton.clearEntity();
    settingButton.clearEntity();
    menuButton.clearEntity();
    point.clearEntity();
    pausetext.clearEntity();

    openMenu(window,renderer, 1200, 800);

}
void Game::handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_MOUSEBUTTONDOWN){
        //cout<<player.getShootAngle(event.button.x, event.button.y)<<"\n";
        if(pauseButton.isClicked(event.button.x, event.button.y)){
            if(this->isPause()){
                this->gameContinue();
            }
            else{
                this->gamePause();
            }
            pauseButton.render();
            SDL_RenderPresent(renderer);
            return;
        }
        else if( menuButton.isClicked(event.button.x, event.button.y) ){
            this->gameStop();
            return;
        }
    }

    if(!this->isPause() && event.type == SDL_KEYDOWN){
        //cout<<char(event.key.keysym.sym)<<" "<<event.type<<"\n";
        //if(event.key.keysym.sym == SDLK_SPACE){
        //    player.addEnemy(SDL_GetTicks()%1000, 0, "resources/HardShip.png", 1.5, 0.3, 180);
        //}
        if((event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z) || (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)){
            player.shootBullet(event.key.keysym.sym);

        }

    }

}
void Game::render(){
    SDL_RenderClear(this->renderer);
    bgr.render2();
    player.render();

    player.renderBullet();
    player.renderEnemy();
    player.boomRender();
    player.drawScoreBoard();

    player.renderXP();
    point.renderCenter();

    pauseButton.render();
    faqButton.render();
    settingButton.render();
    menuButton.render();

    SDL_RenderPresent(renderer);
}

void Game::update(){

    player.updateBullet();
    player.updateEnemy();
    player.updateXP();
    //player.debugCOUT();
    point.setToXY(player.getPX(), player.getPY());
    point.update();

}

