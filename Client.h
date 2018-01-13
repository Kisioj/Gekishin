#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include "Object.h"

using namespace std;

union SDL_Event;

class Client
{
    public:
		Client();
		void handleEvent( SDL_Event& e );
		void move();

        Object obj;
        int vx,vy;
		int speed;
};


#endif // CLIENT_H
