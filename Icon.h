#pragma once
#ifndef ICON_H
#define ICON_H

#include <map>
#include <string>
using namespace std;

class IconState;

class Icon
{
    public:
        Icon();
        map<string, IconState*> icon_states;
        unsigned int texture_id;
        unsigned int frame_width;
        unsigned int frame_height;
    protected:
    private:
};

#endif // ICON_H
