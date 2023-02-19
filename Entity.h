#ifndef ENTITY__H
#define ENTITY__H

#include <iostream>
#include "SDL_utils.h"

class Point{
private:
    double x,y,h,w;
public:
    Point(){
        x = 0;
        y = 0;
        h = 500;
        w = 500;
    }
    double getX()  {return this->x;}
    void setX(double x)    {this->x=x;}

    double getY()  {return this->y;}
    void setY(double y)    {this->y=y;}

    double getH()  {return this->h;}
    void setH(double h)    {this->h=h;}

    double getW()  {return this->w;}
    void setW(double w)    {this->w=w;}
};
class Entity{

private:
    Point pos_d;
    SDL_Rect pos;
    SDL_Rect pos_c;
    SDL_Renderer* renderer;
	SDL_Texture* image;
	double angle = 0;

public:
    double getX()  {return pos_d.getX();}
    void setX(double x)    {pos_d.setX(x);  pos.x=pos_d.getX();}

    double getY()  {return pos_d.getY();}
    void setY(double y)    {pos_d.setY(y);  pos.y=pos_d.getY();}

    double getH()  {return pos_d.getH();}
    void setH(double h)    {pos_d.setH(h);  pos.h=pos_d.getH();}

    double getW()  {return pos_d.getW();}
    void setW(double w)    {pos_d.setW(w);  pos.w=pos_d.getW();}
    double getCenterX(){
        return getX()+getW()/2;
    }
    double getCenterY(){
        return getY()+getH()/2;
    }
    void setXY(double x,double y){
        this->setX(x);
        this->setY(y);
    }
    void setHW(double h,double w){
        this->setH(h);
        this->setW(w);
    }
    double getAngle(){
        return this->angle;
    }
    void setAngle(double angle){
        this->angle = angle;
    }
    void setAC(double x, double y){
        this->setX(x-getW()/2.0);
        this->setY(y-getH()/2.0);
    }
    void setScale(double x){
        double _h = this->getH();
        double _w = this->getW();
        this->setH(this->getH()*x);
        this->setW(this->getW()*x);
        this->addX(-0.5*(this->getW()-_w));
        this->addY(-0.5*(this->getH()-_h));
    }
    void addX(double x){this->setX(this->getX()+x);}
    void addY(double y){this->setY(this->getY()+y);}
    void addH(double h){this->setH(this->getH()+h);}
    void addW(double w){this->setW(this->getW()+w);}
    void loadIMG(string path){image = loadTexture(path, renderer);}

    bool isClicked(int x, int y){
        return ((x >= this->getX()) && (x <= this->getX()+this->getW()) && (y >= this->getY()) && (y <= this->getY()+this->getH()) );
    }

    Entity(){}
    Entity(double x, double y, string path, SDL_Renderer* renderer){
        setXY(x, y);
        setRenderer(renderer);
        loadIMG(path);
        SDL_QueryTexture(getImage(), NULL, NULL, &getPos().w, &getPos().h);
        setHW(getPos().h, getPos().w);
    }
    void render()   {SDL_RenderCopyEx(renderer, image, NULL, &pos, angle, NULL, SDL_FLIP_NONE);}
    void render2() {
        //angle+=0.015;
        SDL_RenderCopyEx(renderer, image, NULL, &pos, angle, NULL, SDL_FLIP_NONE);
    }
    void renderCenter(){
        pos_c.h = getH();
        pos_c.w = getW();
        pos_c.x = getX() - getH()/2.0;
        pos_c.y = getY() - getW()/2.0;
        SDL_RenderCopyEx(renderer, image, NULL, &pos_c, angle, NULL, SDL_FLIP_NONE);
    }
    void setPos(double x, double y, double h, double w){
        pos.x=x;
        pos.y=y;
        pos.h=h;
        pos.w=w;
    }
    void setCenterX(double _w)  {this->setX(_w/2 - this->getW()/2);}
    void setRenderer(SDL_Renderer* renderer)    {this->renderer = renderer;}
    void setImage(SDL_Texture* image)   {this->image = image;}

    SDL_Renderer* getRenderer() {return renderer;}
    SDL_Texture* getImage() {return image;}
    SDL_Rect& getPos()  {return pos;}

    void clearEntity(){
        SDL_DestroyTexture(image);
        renderer = NULL;
        image = NULL;
    }
};



#endif //ENTITY__H

