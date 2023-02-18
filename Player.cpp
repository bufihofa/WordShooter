#ifndef PLAYER__CPP
#define PLAYER__CPP
#include "NewWord.h"
#include "Entity.h"
#include <SDL.h>
#include <bits/stdc++.h>


//SDL_Texture* key_img[100];

class moveEntity: public Entity{
private:
    double speed = 20;
    double toX = 0;
    double toY = 0;
    double oldX = 0;
    double oldY = 0;
    double delta = 0;
public:
    moveEntity(){}
    moveEntity(double x, double y, string path, SDL_Renderer* renderer){
        toX = x;
        toY = y;
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
        setDelta();
    }
    void setDelta(){
        delta = speed/sqrt((toX-oldX)*(toX-oldX)+(toY-oldY)*(toY-oldY));
    }
    double getDelta(){
        return this->delta;
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

class Enemy: public moveEntity{
private:
    deque<char> word;
    Entity wordBox;
    int dmg = 0;
    int hp = 0;;
public:
    Enemy(){}
    ~Enemy(){

    }
    Enemy(double x, double y, double _x, double _y, string path, double speed, double scale, SDL_Renderer* renderer, double angle){
        setXY(_x, _y);
        setToXY(x, y);
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
        setSpeed(speed);
        setScale(scale);
        wordBox = Entity(getX(), getY(), "resources/WordBox.png", renderer);
        wordBox.setScale(0.7);
        this->setAngle(angle);
        this->setDelta();
        string s = getNewWord(1);
        cout<<s<<"\n";
        hp = s.size();
        for(int i=0;i<s.size();++i){
            word.push_back(s[i]);
        }
    }

    void update2(){
        this->addX(-0.001*(getOldX()-getToX())*getSpeed());
        this->addY(-0.001*(getOldY()-getToY())*getSpeed());
    }
    bool outOfRange(){
        return getY()>700;
    }
    void renderWordBox(){
        wordBox.setAC(getX(), getY()-getH()/2-10);
        wordBox.render();
    }
    bool checkKey(int key){
        if(word.empty()) return false;
        return (word.at(0)==int(key));
    }
    void addDmg(){
        dmg++;
    }
    int getDmg(){
        return dmg;
    }
    int getHp(){
        return hp;
    }
    void remKey(){
        if(!word.empty()){
            word.pop_front();
        }
    }
    bool isAlive(){
        return(dmg<hp);
    }
};
class Bullet: public moveEntity{
private:
    double speed = 200;
    int numberOfFrame = 1;
    int nowFrame = 0;
    int id = 0;
    vector<SDL_Texture* > Animation;
public:

    void loadAnimation(string _path){
        for(int i=0;i<numberOfFrame;i++){
            Animation.push_back(loadTexture(_path + "_" + char(i+48) + ".png", getRenderer()));
            SDL_QueryTexture(Animation[i], NULL, NULL, &getPos().w, &getPos().h);
        }
        setHW(20, 20);
    }
    int getID(){
        return this->id;
    }
    Bullet(){}
    Bullet(double x, double y, double toX, double toY, string path, SDL_Renderer* renderer, int _id){
        id = _id;
        setXY(x, y);
        setToXY(toX, toY);
        setRenderer(renderer);
        //loadAnimation(path);
        loadIMG(path + "_0.png");
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(20, 20);
        this->setDelta();
    }



    double getSpeed(){return this->speed;}
    void setSpeed(double speed){this->speed = speed;}
    int getNumberOfFrame(){return numberOfFrame;}
    void nextFrame(){
        nowFrame++;
        nowFrame %= (numberOfFrame*5);
        setImage(Animation[nowFrame/5]);
    }
    void update3(){
        this->addX(-1*(getOldX()-getToX())*getDelta());
        this->addY(-1*(getOldY()-getToY())*getDelta());
    }
    bool isHitted(){
        return (getY()<getToY());
    }
};
class Player: public Entity{
private:
    Bullet* bullet[51];
    Enemy* enemy[51];
    int bullet_status[51];
    int enemy_status[51];
    int numberOfEnemy = 0;
    int target_id = 0;

public:
    Player(){}
    Player(double x, double y, string path, SDL_Renderer* renderer){
        setXY(x, y);
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
        for(int i=0;i<=50;++i){
            bullet[i] = new Bullet();
            enemy[i] = new Enemy();
            bullet_status[i] = 0;
            enemy_status[i] = 0;
        }
    }
    int findNearestEnemy(){
        int _max = INT_MIN;
        int _id = 0;
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1) {
                if(enemy[i]->getY() >= _max){
                    _max = enemy[i]->getY();
                    _id = i;
                }
            }
        }
        return _id;
    }
    int findEnemyEmpty(){
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 0) return i;
        }
    }
    void removeEnemy(int id){
        enemy_status[id] = 0;
        numberOfEnemy--;
    }
    void addEnemy(double _x, double _y, string path, double speed, double scale, double angle){
        int id = findEnemyEmpty();
        enemy[id]->clearEntity();
        enemy[id] = new Enemy(_x, 750, _x, _y, path, speed, scale, getRenderer(), angle);
        enemy_status[id] = 1;
        numberOfEnemy++;
    }
    void renderEnemy(){
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1){
                enemy[i]->renderCenter();
                enemy[i]->renderWordBox();
            }
        }
    }
    void updateEnemy(){
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1){
                enemy[i]->update2();
                if(enemy[i]->outOfRange()) {
                    removeEnemy(i);
                }
            }
        }
    }
    double getPX(){
        if(numberOfEnemy>0){
            if(enemy_status[target_id] == 0) target_id = findNearestEnemy();
            return enemy[target_id]->getX();
        }
        return 600;
    }
    double getPY(){
        if(numberOfEnemy>0){
            if(enemy_status[target_id] == 0) target_id = findNearestEnemy();
            return enemy[target_id]->getY();
        }
        return 400;
    }

    int findBulletEmpty(){
        for(int i=0;i<=50;++i){
            if(bullet_status[i] == 0) return i;
        }
    }
    void removeBullet(int id){
        bullet_status[id] = 0;
    }
    void shootBullet(int key){
        if(enemy_status[target_id] == 1) {
            int i = target_id;
            if(enemy[i]->checkKey(key)) {
                target_id = i;
                int id = findBulletEmpty();
                bullet[id]->clearEntity();
                bullet[id] = new Bullet(getCenterX(), getCenterY(), enemy[i]->getX(), enemy[i]->getY(), "resources/bulletanimation/bullet", getRenderer(), i);
                bullet_status[id] = 1;
                enemy[i]->remKey();
                return;
            }
        }
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1){
                if(enemy[i]->checkKey(key)) {
                    target_id = i;
                    int id = findBulletEmpty();
                    bullet[id]->clearEntity();
                    bullet[id] = new Bullet(getCenterX(), getCenterY(), enemy[i]->getX(), enemy[i]->getY(), "resources/bulletanimation/bullet", getRenderer(), i);
                    bullet_status[id] = 1;
                    enemy[i]->remKey();
                    break;
                }
            }
        }

    }
    void renderBullet(){
        for(int i=0;i<=50;++i){
            if(bullet_status[i] == 1){
                bullet[i]->render();
            }
        }
    }
    void updateBullet(){
        for(int i=0;i<=50;++i){
            if(bullet_status[i] == 1){
                bullet[i]->update3();
                //bullet[i]->nextFrame();
                if(bullet[i]->isHitted()){
                    enemy[bullet[i]->getID()]->addDmg();
                    if(!enemy[bullet[i]->getID()]->isAlive()) {
                        cout<<"KILLED\n";
                        removeEnemy(bullet[i]->getID());
                    }
                    removeBullet(i);
                }
            }
        }
    }
    void clearAll(){
        for(int i=0;i<=50;++i){

            if(enemy_status[i] == 1)bullet[i]->clearEntity();
            if(bullet_status[i] == 1)enemy[i]->clearEntity();
        }
    }
};
#endif //ENTITY__H

