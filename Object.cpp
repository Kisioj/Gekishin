#include "Object.h"
#include "GameManager.h"
#include "IconState.h"
#include "Frame.h"
#include "Texture.h"
#include <cassert>
#include <iostream>//delete
using namespace std;//delete
extern GameManager manager;

Object::Object()
{
    this->density = false;
    this->icon_state = NULL;
    this->pixel_x = 0;
    this->pixel_y = 0;
    this->dir = SOUTH;

}

Object::Object(string icon, string icon_state, bool density, Uint8 pixel_x, Uint8 pixel_y)
{
    this->density = density;
    this->setIcon(icon, icon_state);
    this->pixel_x = pixel_x;
    this->pixel_y = pixel_y;
}

void Object::setIcon(string icon, string icon_state){
    this->icon_state = manager.icons[icon]->icon_states[icon_state];
}

void Object::setPosition(int x, int y, int w, int h){
    this->position.x = x;
    this->position.y = y;
    this->position.w = w;
    this->position.h = h;
}

void Object::render( int camX, int camY ){
    int frame = this->icon_state->next_frame_index;

    if(this->icon_state->frames[0].time==100 && this->icon_state->frames[1].time==100 && this->icon_state->frames[2].time==3000 ){
       //cout << "Object::render" << endl;
       // int x = 1;
       //cout << "Frame: " << frame << endl;
    }


    assert(this->icon_state->dirs>0);

    if(this->icon_state->frames_count/this->icon_state->dirs>1 && SDL_GetTicks() >= this->icon_state->next_frame_time + this->icon_state->frames[frame].time){
        this->icon_state->next_frame_time += this->icon_state->frames[frame].time;

        if(this->icon_state->rewind){
            if(this->icon_state->frames_left_to_right) this->icon_state->next_frame_index++;
            else this->icon_state->next_frame_index--;

            if(this->icon_state->next_frame_index >= this->icon_state->frames_count/this->icon_state->dirs){
                this->icon_state->next_frame_index = this->icon_state->frames_count/this->icon_state->dirs-1;
                this->icon_state->frames_left_to_right = false;
            }
            if(this->icon_state->next_frame_index < 0){
                this->icon_state->next_frame_index = 0;
                this->icon_state->frames_left_to_right = true;
            }


        }else{
            this->icon_state->next_frame_index++;
            if(this->icon_state->next_frame_index >= this->icon_state->frames_count){
                this->icon_state->next_frame_index = 0;
            }
        }
    }
    if(this->icon_state->dirs>1){
        switch(this->dir){
            case SOUTH:
                break;
            case NORTH:
                frame+=this->icon_state->frames_count;
                break;
            case EAST:
                frame+=this->icon_state->frames_count*2;
                break;
            case WEST:
                frame+=this->icon_state->frames_count*3;
                break;

            case NORTHEAST:
                frame+=this->icon_state->frames_count*6;
                break;
            case SOUTHEAST:
                frame+=this->icon_state->frames_count*4;
                break;
            case NORTHWEST:
                frame+=this->icon_state->frames_count*7;
                break;
            case SOUTHWEST:
                frame+=this->icon_state->frames_count*5;
                break;
        }
    }

    manager.textures[this->icon_state->icon->texture_id]->render( this->position.x - camX + this->pixel_x, this->position.y - camY + this->pixel_y, &this->icon_state->frames[frame].clip );
}
