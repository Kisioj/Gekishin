#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <string>
using namespace std;

class IconState;

#define NORTH 1
#define SOUTH 2
#define EAST 4
#define WEST 8
#define NORTHEAST 5
#define SOUTHEAST 6
#define NORTHWEST 9
#define SOUTHWEST 10

#define AREA_LAYER 1
#define TURF_LAYER 2
#define OBJ_LAYER 3
#define MOB_LAYER 4
#define FLY_LAYER 5



class Object
{
    public:
        Object();
        Object(string icon, string icon_state, bool density=false, Uint8 pixel_x=0, Uint8 pixel_y=0);
        void render( int camX, int camY );
        void setIcon(string icon, string icon_state);
        void setPosition(int x, int y, int w, int h);

        IconState* icon_state;

        SDL_Rect position;

        Uint8 dir;
        Uint8 density;
        int8_t pixel_x,pixel_y;
};

#endif // OBJECT_H
