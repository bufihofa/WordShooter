#ifndef PLAYER__CPP
#define PLAYER__CPP
#include "NewWord.h"
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
        delta = double(speed)/sqrt((toX-oldX)*(toX-oldX)+(toY-oldY)*(toY-oldY));
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
            addX(0.12*(toX-getX())-pc*2);
            addY(0.12*(toY-getY())-pc*2);
        }
    }

    void update_1(){
        double pc = getPercent();
        setAngle(getAngle()+7+pc*10);
        addX((0.03*(toX-getX())-pc*3.0));
        addY((0.06*(toY-getY())-pc*7.0));
        //addX((1.2-pc)*(0.01*(toX-getX())*3));
        //addY((1.2-pc)*(0.02*(toY-getY())*3));
    }

};

class Enemy: public moveEntity{
private:
    deque<char> word;
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
        this->setAngle(angle);
        this->setDelta();
        string s = getNewWord(1);
        //cout<<s<<"\n";
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
    int getNumber(){
        return word.size();
    }
    char getWord(int i){
        if(word.size()>i) return word.at(i);
        return '0';
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
    int id = 0;
public:
    int getID(){
        return this->id;
    }
    Bullet(){}
    Bullet(double x, double y, double toX, double toY, string path, SDL_Renderer* renderer, int _id){
        id = _id;
        setXY(x, y);
        setToXY(toX, toY);
        setRenderer(renderer);
        loadIMG(path + "_0.png");
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(20, 20);
        this->setDelta();
    }



    double getSpeed(){return this->speed;}
    void setSpeed(double speed){this->speed = speed;}

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
    Bullet* exp[51];
    Bullet* bullet[51];
    Enemy* enemy[51];
    int bullet_status[51];
    int enemy_status[51];
    int exp_status[51];
    SDL_Texture* key_img[150];
    Entity* key_now;
    int numberOfEnemy = 0;
    int target_id = 0;


    Entity* score_board;
    int score_keyPressed = 0;
    int score_wordPressed = 0;
    int score = 0;
    int score_pool = 0;
    Entity* score_pen;
    SDL_Texture* number_img[10];
    double time = 0;
    int time_2 = 1;
    int time_3 = 1;
    int _sp = 400;
public:
    void drawNumberText(int drawW, int drawX, int drawY, double drawSize){
        string s = to_string(drawW);
        double _h = score_pen->getH();
        double _w = score_pen->getW();
        score_pen->setHW(score_pen->getH()*drawSize,score_pen->getW()*drawSize);
        for(int i=0;i<s.size();++i){
            score_pen->setImage(number_img[s[i]-48]);
            score_pen->setXY(drawX+i*score_pen->getW()/1.85, drawY);
            score_pen->render();
        }
        score_pen->setHW(_h, _w);
    }
    void drawScoreBoard(){
        time+=16;
        time_3+=16;
        if(time>(1000-time_2*2)){
            time = time-1000+time_2*2;
            time_2++;
            _sp+=50;
            this->addEnemy(_sp % 1000, 0, "resources/HardShip.png", 1.5, 0.3, 180);
        }
        score += score_pool*0.02;
        score_pool *= 0.98;
        drawNumberText(score, 120, 45, 0.6);
        drawNumberText(score_wordPressed, 125, 83, 0.35);
        drawNumberText(score_keyPressed, 125, 107, 0.35);
        drawNumberText(time_3/1000, 30, 8, 0.4);
        drawNumberText(score_keyPressed*12/(time_3/1000.0), 200, 135, 0.4);
        //DrawNumberText(score_keyPressed, 100, 70, 0.4);
    }
    Player(){}
    Player(double x, double y, string path, SDL_Renderer* renderer){
        setXY(x, y);
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
        /*
        for(int i=0;i<=50;++i){
            bullet[i] = new Bullet();
            enemy[i] = new Enemy();
            exp[i] = new Bullet();
            exp_status[i] = 0;
            bullet_status[i] = 0;
            enemy_status[i] = 0;
        }
        */
        for(int i=0;i<=50;++i){
            bullet[i] = new Bullet();
            enemy[i] = new Enemy();
            exp[i] = new Bullet();
            exp_status[i] = 0;
            bullet_status[i] = 0;
            enemy_status[i] = 0;
        }
        string s = "0-Key";
        for(char i='0';i<='9';++i){
            s[0] = i;
            key_img[i] = loadTexture("resources/Dark/"+s+".png", renderer);
        }
        for(char i='a';i<='z';++i){
            s[0] = i;
            key_img[i] = loadTexture("resources/Dark/"+s+".png", renderer);
        }
        key_now = new Entity(0, 0, "resources/Dark/A-Key.png", renderer);
        key_now->setHW(20,20);
        for(int i=0;i<10;++i){
            s = char(i+48);
            number_img[i] = loadTexture("resources/number/"+s+".png", renderer);
        }
        score_pen = new Entity(0,0, "resources/number/0.png", renderer);
        score_pen->setHW(60,50);
        score_board = new Entity(0,0, "resources/number/box.png", renderer);
        score_board->setScale(0.6);
        score_board->setXY(-10, -10);
    }
    void debugCOUT(){
        int c1=0;
        int c2=0;
        int c3=0;
        int d1=0;
        int d2=0;
        int d3=0;
        for(int i=0;i<=50;i++){
            if(enemy_status[i] == 1) c1++;
            if(bullet_status[i] == 1) c2++;
            if(exp_status[i] == 1) c3++;
            if(enemy[i] != NULL) d1++;
            if(bullet[i] != NULL) d2++;
            if(exp[i] != NULL) d3++;
        }
        cout<<c1<<" "<<c2<<" "<<c3<<" "<<d1<<" "<<d2<<" "<<d3<<"\n";

    }
    int findNearestEnemy(){
        int _max = INT_MIN;
        int _id = 0;
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1 && enemy[i] != NULL && enemy[i]->getNumber()>0) {
                if(enemy[i]->getY() >= _max){
                    _max = enemy[i]->getY();
                    _id = i;
                }
            }
        }
        if(_max == INT_MIN) return -1;
        return _id;
    }

    int findEnemyEmpty(){
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 0 && enemy[i] != NULL) return i;
        }
    }
    void removeEnemy(int id){
        enemy_status[id] = 0;
        if(enemy[id] != NULL) enemy[id]->clearEntity();
        numberOfEnemy--;
    }
    void addEnemy(double _x, double _y, string path, double speed, double scale, double angle){
        int id = findEnemyEmpty();
        enemy[id] = new Enemy(_x, 750, _x, _y, path, speed, scale, getRenderer(), angle);
        enemy_status[id] = 1;
        numberOfEnemy++;
    }
    void renderEnemy(){
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1 && enemy[i] != NULL){
                enemy[i]->renderCenter();
            }
        }
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1 && enemy[i] != NULL){
                int temp = enemy[i]->getNumber();
                for(int j=0;j<temp;++j){
                    key_now->setImage(key_img[enemy[i]->getWord(j)]);
                    key_now->setXY(enemy[i]->getX()-temp*9+j*20, enemy[i]->getY()-45);
                    key_now->render();
                }
            }
        }
        score_board->render();
    }
    void updateEnemy(){
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1 && enemy[i] != NULL){
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
            if(enemy[target_id]->getNumber()==0) target_id = findNearestEnemy();
            if(target_id == -1) return 600;
            return enemy[target_id]->getX();
        }

        //if(numberOfEnemy>0 && enemy[target_id] != NULL && enemy[target_id]->getNumber()>0) return enemy[target_id]->getX();

        return 600;
    }
    double getPY(){

        if(numberOfEnemy>0){
            if(enemy_status[target_id] == 0) target_id = findNearestEnemy();
            if(enemy[target_id]->getNumber()==0) target_id = findNearestEnemy();
            if(target_id == -1) return 400;
            return enemy[target_id]->getY();
        }

        //if(numberOfEnemy>0 && enemy[target_id] != NULL && enemy[target_id]->getNumber()>0) return enemy[target_id]->getY();
        return 400;
    }

    int findBulletEmpty(){
        for(int i=0;i<=50;++i){
            if(bullet_status[i] == 0 && bullet[i] != NULL) return i;
        }
    }
    void removeBullet(int id){
        bullet_status[id] = 0;
        if(bullet[id] != NULL) bullet[id]->clearEntity();
    }


    int findXPEmpty(){
        for(int i=0;i<=50;++i){
            if(exp_status[i] == 0) return i;
        }
    }
    void removeXP(int id){
        exp_status[id] = 0;
        if(bullet[id] != NULL) exp[id]->clearEntity();
    }
    void shootXP(double x, double y){
        int id = findXPEmpty();
        exp[id] = new Bullet(x, y, 6, 55, "resources/xpdot", getRenderer(), id);
        exp[id]->setSpeed(10);
        exp_status[id] = 1;
    }
    void updateXP(){
        for(int i=0;i<=50;++i){
            if(exp_status[i] == 1  && exp[i] != NULL){
                exp[i]->update_1();
                if(exp[i]->getX() <= 7.0){
                    removeXP(i);
                }
            }
        }
    }
    void renderXP(){
        for(int i=0;i<=50;++i){
            if(exp_status[i] == 1 && exp[i] != NULL){
                exp[i]->render();
            }
        }
    }

    void shootBullet(int key){
        if(enemy_status[target_id] == 1  && enemy[target_id] != NULL) {
            int i = target_id;
            if(enemy[i]->checkKey(key)) {
                target_id = i;
                int id = findBulletEmpty();
                bullet[id] = new Bullet(getCenterX(), getCenterY(), enemy[i]->getX(), enemy[i]->getY(), "resources/bulletanimation/bullet", getRenderer(), i);
                bullet_status[id] = 1;
                if (enemy[i] != NULL) enemy[i]->remKey();
                score_keyPressed++;
                return;
            }
        }
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1  && enemy[i] != NULL){
                if(enemy[i]->checkKey(key)) {
                    target_id = i;
                    int id = findBulletEmpty();
                    bullet[id] = new Bullet(getCenterX(), getCenterY(), enemy[i]->getX(), enemy[i]->getY(), "resources/bulletanimation/bullet", getRenderer(), i);
                    bullet_status[id] = 1;
                    if (enemy[i] != NULL) enemy[i]->remKey();
                    score_keyPressed++;
                    break;
                }
            }
        }

    }
    void renderBullet(){
        for(int i=0;i<=50;++i){
            if(bullet_status[i] == 1  && bullet[i] != NULL){
                bullet[i]->render();
            }
        }
    }
    void updateBullet(){
        for(int i=0;i<=50;++i){
            if(bullet_status[i] == 1 && bullet[i] != NULL){
                bullet[i]->update3();
                if(bullet[i]->isHitted()){
                    if(enemy[bullet[i]->getID()] != NULL){
                        enemy[bullet[i]->getID()]->addDmg();
                        if(!enemy[bullet[i]->getID()]->isAlive()) {
                            shootXP(bullet[i]->getX(), bullet[i]->getY()-5);
                            score_wordPressed++;
                            score_pool += enemy[bullet[i]->getID()]->getHp() * getRandomNumber(80,100);
                            removeEnemy(bullet[i]->getID());
                        }
                        removeBullet(i);
                    }
                }
            }
        }
    }
    void clearAll(){
        for(int i=0;i<=50;++i){
            if(enemy_status[i] == 1) enemy[i]->clearEntity();
            if(bullet_status[i] == 1) bullet[i]->clearEntity();
            if(exp_status[i] == 1) exp[i]->clearEntity();
        }
        for(char i='0';i<='9';++i){
            SDL_DestroyTexture(key_img[i]);
            key_img[i] = NULL;
        }
        for(char i='a';i<='z';++i){
            SDL_DestroyTexture(key_img[i]);
            key_img[i] = NULL;
        }
        for(int i=0;i<10;++i){
            SDL_DestroyTexture(number_img[i]);
            number_img[i] = NULL;
        }
        key_now->clearEntity();
        score_pen->clearEntity();
        score_board->clearEntity();
    }
};
#endif //ENTITY__H

