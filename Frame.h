#pragma once
#ifndef FRAME_H
#define FRAME_H

#include <SDL2/SDL.h>

class Frame
{
    public:
        Frame();
        void Set(int x, int y, int w, int h, int time=100);
        unsigned int time;  //in miliseconds
        SDL_Rect clip;

};

#endif // FRAME_H
