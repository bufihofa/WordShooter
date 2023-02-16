#ifndef PLAYER__CPP
#define PLAYER__CPP

#include "Entity.h"
#include <SDL.h>
#include <bits/stdc++.h>
class moveEntity: public Entity{
private:
    double speed = 20;
    double toX = 0;
    double toY = 0;
    double oldX = 0;
    double oldY = 0;
public:
    moveEntity(){}
    moveEntity(double x, double y, string path, SDL_Renderer* renderer){
        toX = x;
        toY = y;
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
    }
    double getPercent(){
        return (sqrt((getX() - toX)*(getX() - toX)+(getY() - toY)*(getY() - toY))/sqrt((toX - oldX)*(toX - oldX)+(toY - oldY)*(toY - oldY)));
    }
    double getToX(){ return toX;}
    double getToY(){ return toY;}
    double getOldX(){ return oldX;}
    double getOldY(){ return oldY;}
    void setToX(double toX){
        this->oldX = getX();
        this->toX = toX;
    }
    void setToY(double toY){
        this->oldY = getY();
        this->toY = toY;
    }
    void setToXY(double toX, double toY){
        this->oldX = getX();
        this->oldY = getY();
        this->toX = toX;
        this->toY = toY;
    }
    double getSpeed(){return this->speed;}
    void setSpeed(double speed){this->speed = speed;}

    void update(){
        double pc = getPercent();
        if(sqrt((getX() - toX)*(getX() - toX)+(getY() - toY)*(getY() - toY))<100){
            setAngle(getAngle()+4);
            addX(0.15*(toX-getX()));
            addY(0.15*(toY-getY()));
        }
        else{
            setAngle(getAngle()+7+pc*10);
            addX(0.04*(toX-getX())*3-pc*2);
            addY(0.04*(toY-getY())*3-pc*2);
        }
    }

};
class Bullet: public Entity{
private:
    double speed = 40;
    double frameSpeed = 30;
    int numberOfFrame = 4;
    int nowFrame = 0;
    vector<SDL_Texture* > Animation;
public:

    void loadAnimation(int _numberOfFrame, int _frameSpeed, string _path){
        numberOfFrame = _numberOfFrame;
        frameSpeed = _frameSpeed;
        for(int i=0;i<numberOfFrame;i++){
            Animation.push_back(loadTexture(_path + "_" + char(i+48) + ".png", getRenderer()));
            SDL_QueryTexture(Animation[i], NULL, NULL, &getPos().w, &getPos().h);
        }
        this->setScale(0.5);
    }

    Bullet(){}
    Bullet(double x, double y, string path, SDL_Renderer* renderer){
        setXY(x, y);
        setRenderer(renderer);
        loadAnimation(4, 30, path);

        loadIMG(path+ "_" + char(48) + ".png");
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
    }


    double getSpeed(){return this->speed;}
    void setSpeed(double speed){this->speed = speed;}
    double getFrameSpeed(){return frameSpeed;}
    int getNumberOfFrame(){return numberOfFrame;}
    void setFrameSpeed(double frameSpeed)   {this->frameSpeed = frameSpeed;}
    void nextFrame(){
        nowFrame++;
        nowFrame %= (numberOfFrame*3);
        setImage(Animation[nowFrame/3]);
    }
};
class Enemy: public moveEntity{
private:
    queue<char> word;
    //Entity
public:
    Enemy(){}
    Enemy(double x, double y, double _x, double _y, string path, double speed, double scale, SDL_Renderer* renderer){
        setXY(_x, _y);
        setToXY(x, y);
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
        setSpeed(speed);
        setScale(scale);

    }
    void update2(){
        this->addX(-0.005*(getOldX()-getToX()));
        this->addY(-0.005*(getOldY()-getToY()));
    }
    bool outOfRange(){
        return getY()>700;
    }
};
class Player: public Entity{
private:
    vector<Bullet> bull;
    deque<Enemy> ene;
public:
    Player(){}
    Player(double x, double y, string path, SDL_Renderer* renderer){
        setXY(x, y);
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
    }
    void addEnemy(double _x, double _y, string path, double speed, double scale){
        ene.push_back(Enemy(_x, 750, _x, _y, path, speed, scale, getRenderer()));
    }
    void renderEnemy(){
        for(int i=0;i<ene.size();i++){
             ene.at(i).render();
             //cout<<i<<" rendered\n";
        }
    }
    void updateEnemy(){
        for(int i=0;i<ene.size();i++){
            ene.at(i).update2();
            //ene.at(i).nextFrame();
            if(ene.at(i).outOfRange()) ene.pop_front();
        }
    }
    double getPX(){
        if(ene.size()>0)
        return ene.at(0).getX()+ene.at(0).getW()/2;
        return 600;
    }
    double getPY(){
        if(ene.size()>0)
        return ene.at(0).getY()+ene.at(0).getH();
        return 400;
    }
    void shootBullet(){
        bull.push_back(Bullet(0, 0, "resources/Pu", getRenderer()));

    }
    void renderBullet(){
        for(int i=0;i<bull.size();i++){
             bull.at(i).render();
        }
    }
    void updateBullet(){
        for(int i=0;i<bull.size();i++){
            bull.at(i).addX(bull.at(i).getSpeed());
            bull.at(i).nextFrame();
        }
    }
};
#endif //ENTITY__H

