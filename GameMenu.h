#ifndef MENU__H
#define MENU__H

#include "SDL_utils.h"
#include "Entity.h"
#include "NewGame.h"
#include <thread>
#include <chrono>

class mButton: public Entity{
private:
    bool isHighlight = false;
    Point po;
public:
    mButton(){}
    mButton(double x, double y, string path, SDL_Renderer* renderer, double _scale, bool ct){
        setXY(x, y);
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
        this->setScale(_scale);
        if(ct) this->setCenterX(1200);
    }
    bool isHL(){return isHighlight;}
    void HL(){
        if(!isHighlight){
            this->setScale(1.15);
            isHighlight = true;
        }
    }
    void unHL(){
        if(isHighlight){
            this->setScale(100.0/115.0);
            isHighlight = false;
        }
    }
};
class PlayMenu{
private:
    mButton difficultyPanel;
    mButton difficultyText;
    mButton menuPanel;
    mButton easyMode, mediumMode, hardMode;
    mButton backButton;
public:
    PlayMenu(){};
    PlayMenu(SDL_Window* window, SDL_Renderer* renderer){
        int height = 1200;
        int width = 800;
        menuPanel = mButton(0, height/4, "resources/MenuPanel.png", renderer, height/400, true);
        menuPanel.setAC(width/2.0, height/2.0);

        difficultyPanel = mButton(0, 0, "resources/DiffPanel.png", renderer, 1.3, true);
        difficultyPanel.setH(difficultyPanel.getH()/1.5);
        difficultyPanel.setY(175);

        easyMode = mButton(50, 250, "resources/EasyMode.png", renderer, 0.8, false);
        mediumMode = mButton(250, 250, "resources/MediumMode.png", renderer, 0.8, true);
        hardMode = mButton(750, 250, "resources/HardMode.png", renderer, 0.8, false);

        difficultyText = mButton(50, 190, "resources/select.png", renderer, 1, true);

        backButton = mButton(920, 70, "resources/CloseButton.png", renderer, 0.15, false);
    }
    void checkHL(int x, int y){
        backButton.unHL();
        easyMode.unHL();
        mediumMode.unHL();
        hardMode.unHL();
        if(easyMode.isClicked(x, y)){
            easyMode.HL();
        }
        else if(mediumMode.isClicked(x, y)){
            mediumMode.HL();
        }
        else if(hardMode.isClicked(x, y)){
            hardMode.HL();
        }
        else if(backButton.isClicked(x, y)){
            backButton.HL();
        }
    }
    int checkClick(int x, int y){
        if(easyMode.isClicked(x, y)){
            return 1;
        }
        else if(mediumMode.isClicked(x, y)){
            return 2;
        }
        else if(hardMode.isClicked(x, y)){
            return 3;
        }
        else if(backButton.isClicked(x, y)){
            return 4;
        }
        return 0;
    }
     void render(SDL_Renderer* renderer){
        difficultyPanel.render();
        difficultyText.render();
        easyMode.render();
        mediumMode.render();
        hardMode.render();
        backButton.render();
    }

};

class MainMenu{
private:
    SDL_Texture* background;
    mButton menuPanel;
    mButton playButton, settingButton, guideButton, quitButton;
public:
    MainMenu(){};
    MainMenu(SDL_Window* window, SDL_Renderer* renderer, int width, int height){
        background = loadTexture("resources/MenuBackGround.png", renderer);

        menuPanel = mButton(0, 0, "resources/MenuPanel.png", renderer, 2, true);
        menuPanel.setAC(width/2.0, height/2.0);

        playButton = mButton(0, height/4, "resources/PlayButton.png", renderer, 1.2, true);
        settingButton = mButton(0, height/2.5, "resources/SettingButton.png", renderer, 1, true);
        guideButton = mButton(0, height/2, "resources/GuideButton.png", renderer, 1, true);
        quitButton = mButton(0, height/1.5, "resources/QuitButton.png", renderer, 1, true);
    }

    void checkHL(int x, int y){
        quitButton.unHL();
        playButton.unHL();
        settingButton.unHL();
        guideButton.unHL();
        if(playButton.isClicked(x, y)) {
            playButton.HL();
        }
        else if(settingButton.isClicked(x, y)){
            settingButton.HL();
        }
        else if(guideButton.isClicked(x, y)){
            guideButton.HL();
        }
        else if(quitButton.isClicked(x, y)){
            quitButton.HL();
        }
    }

    int checkClick(int x, int y){
        if(playButton.isClicked(x, y)){
            return 1;
        }
        else if(settingButton.isClicked(x, y)){
            return 2;
        }
        else if(guideButton.isClicked(x, y)){
            return 3;
        }
        else if(quitButton.isClicked(x, y)){
            return 4;
        }
        return 0;
    }

    void render(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, background, NULL, NULL);
        menuPanel.render();
        playButton.render();
        settingButton.render();
        guideButton.render();
        quitButton.render();
    }


};
void openMenu(SDL_Window* window, SDL_Renderer* renderer, int _width, int _height);
#endif //ENTITY__H

