#ifndef NEWGAME__H
#define NEWGAME__H

#include "SDL_utils.h"

void renderThreadFunc();
void newGame(SDL_Window* window, SDL_Renderer* renderer, int diffMode);
#endif //NEWGAME__H

