#include "IconState.h"
#include "GameManager.h"
#include "Frame.h"
#include <iostream>//to delete
using namespace std;//to delete
extern GameManager manager;

IconState::IconState()
{
    this->rewind = false;

    this->next_frame_time = SDL_GetTicks() + 100; //jakos trzeba inaczej pomyslec jak zrobic te animacje
    this->frames_left_to_right = true;
    this->next_frame_index = 0;
    this->dirs=1;
}
IconState::IconState(int texture_width, SDL_Rect &start_frame, int frames_count, int frame_times[], int dirs, bool rewind){
    cout << "d:" << dirs << endl;
    this->rewind = rewind;
    this->frames_count = frames_count;
    this->dirs = dirs;
    this->frames = new Frame[frames_count*dirs];

    int x = start_frame.x;
    int y = start_frame.y;
    int w = start_frame.w;
    int h = start_frame.h;
    for(int i=0;i<frames_count*dirs;i++){
        this->total_time+=frame_times[i];
        this->frames[i].Set(x,y,w,h,frame_times[i]);
        if(x+w >= texture_width){
            x=0;
            y+=h;
        }else{
            x+=w;
        }
    }

    this->next_frame_time = SDL_GetTicks() + 100; //jakos trzeba inaczej pomyslec jak zrobic te animacje
    this->frames_left_to_right = true;
    this->next_frame_index = 0;
}


void IconState::render( int camX, int camY ){
    /*
    int frame = this->next_frame_index;

    if(this->icon_state.frames_count>1 && SDL_GetTicks() >= next_frame_time){
        next_frame_time += this->icon_state.frames[frame].time;
        if(this->icon_state.rewind){
            if(frames_left_to_right) next_frame_index++;
            else next_frame_index--;

            if(next_frame_index >= this->icon_state.frames_count){
                next_frame_index = this->icon_state.frames_count-1;
                frames_left_to_right = false;
            }
            if(next_frame_index < 0){
                next_frame_index = 0;
                frames_left_to_right = true;
            }


        }else{
            next_frame_index++;
            if(next_frame_index >= this->icon_state.frames_count){
                next_frame_index = 0;
            }
        }
    }


    manager.textures[this->texture_id]->render( this->obj.position.x - camX, this->obj.position.y - camY, &this->icon_state.frames[frame].clip );

	//manager.textures[this->texture_id]->render( this->position.x - camX, this->position.y - camY );
	*/
}
