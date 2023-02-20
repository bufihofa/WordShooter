#ifndef MENU__H
#define MENU__H

#include "Entity.h"
#include <bits/stdc++.h>

class mButton: public Entity{
private:
    bool isHighlight = false;
    Point po;
public:
    mButton(){}
    mButton(double x, double y, string path, SDL_Renderer* renderer, double scale, bool isCenter){
        setXY(x, y);

        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);

        setHW(getPos().h, getPos().w);

        this->setScale(scale);
        if(isCenter) this->setCenterX(1200);
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
class Menu{
private:
    Entity panel;
    deque<mButton> buttonList;
    SDL_Renderer* renderer;
public:
    Menu(){}
    Menu(SDL_Renderer* renderer, string path){
        this->setRenderer(renderer);
        panel = Entity(600, 400, path, renderer);
        panel.setAC(600, 400);
    }
    void setRenderer(SDL_Renderer* renderer){
        this->renderer = renderer;
    }
    void addButton(double x, double y, string path, SDL_Renderer* renderer, double scale, bool isCenter){
        buttonList.push_back(mButton(x, y, path, renderer, scale, isCenter));
    }
    Entity* getPanel(){
        return &panel;
    }
    mButton* getButton(int stt_button){
        if(stt_button < buttonList.size())
            return &buttonList.at(stt_button);
    }
    void checkMouseMove(double x, double y){
        if(!buttonList.empty()){
            for(int i=0;i<buttonList.size();++i){
                buttonList.at(i).unHL();
            }
            for(int i=0;i<buttonList.size();++i){
                if(buttonList.at(i).isClicked(x, y)){
                    buttonList.at(i).HL();
                    return;
                }
            }
        }
    }
    int checkMouseClick(double x, double y){
        if(!buttonList.empty()){
            for(int i=0;i<buttonList.size();++i){
                if(buttonList.at(i).isClicked(x, y)){
                    return i;
                }
            }
        }
    }
    void render(){
        panel.render();
        if(!buttonList.empty()){
            for(int i=0;i<buttonList.size();++i){
                buttonList.at(i).render();
            }
        }
    }
};

void openMenu(SDL_Window* window, SDL_Renderer* renderer, int _width, int _height);
#endif //ENTITY__H

