#pragma once
#ifndef ICONSTATE_H
#define ICONSTATE_H

#include <SDL2/SDL.h>

class Icon;
class Frame;
struct SDL_Rect;

class IconState
{
    public:
        IconState();
        IconState(int icon_width, SDL_Rect &start_frame, int frames_count, int frame_times[], int dirs, bool rewind=false);
        void render( int camX, int camY );

        Frame* frames;
        int frames_count;
        int total_time;
        int dirs;

        bool rewind; //animation



        Icon* icon;
        Uint32 next_frame_time;// to trzeba by przeniesc gdzies to icon_state'a
        bool frames_left_to_right;
        int next_frame_index;
};

#endif // ICONSTATE_H
