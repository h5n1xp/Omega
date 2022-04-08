//
//  SDLText.h
//  
//
//  Created by Matt Parsons on 20/10/2020.
//
//


#ifndef sdltext_h
#define sdltext_h

#include <SDL2/SDL.h>


void SDL_TextInit(SDL_Renderer*);
void SDL_Print(char* string,int x, int y);


#endif
