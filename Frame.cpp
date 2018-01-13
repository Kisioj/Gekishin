#include "Frame.h"

Frame::Frame()
{
    //ctor
    this->time = 100;
}

void Frame::Set(int x, int y, int w, int h, int time){
    this->clip.x = x;
    this->clip.y = y;
    this->clip.w = w;
    this->clip.h = h;
    this->time = time;
}
