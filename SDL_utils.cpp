#include <iostream>
#include "SDL_utils.h"
using namespace std;



void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
     os << msg << " Error: " << SDL_GetError() << std::endl;
     if (fatal) {
         SDL_Quit();
         exit(1);
     }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT, const string &WINDOW_TITLE)
{
     if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(std::cout, "SDL_Init", true);

     window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

     if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
     SDL_RENDERER_PRESENTVSYNC);

     if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

     SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
     SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
     SDL_Event e;
     while (true) {
         if ( SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
         SDL_Delay(100);
     }
}


SDL_Texture* loadTexture(string path, SDL_Renderer* renderer){
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr){
        cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << endl;
    }
    else{
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == nullptr){
            cout << "Unable to create texture from " << path << " SDL_image Error: " << IMG_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

SDL_Texture* NewLayer(SDL_Renderer* renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT){
    SDL_Texture* my_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetTextureBlendMode(my_layer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, my_layer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    return my_layer;
}














