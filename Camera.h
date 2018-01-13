#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

class Camera
{
    public:
        Camera();
        void Center(SDL_Rect obj);
        void Init(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

        SDL_Rect position;

};

#endif // CAMERA_H
