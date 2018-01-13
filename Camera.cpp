#include "Camera.h"
#include "GameManager.h"

extern GameManager manager;

Camera::Camera()
{
    //ctor
}

void Camera::Center(SDL_Rect obj){
    //Center the camera over the object
    this->position.x = ( obj.x + obj.w / 2 ) - manager.native_screen_size.w / 2;
    this->position.y = ( obj.y + obj.h / 2 ) - manager.native_screen_size.h / 2;

    //Keep the camera in bounds
    if( this->position.x < 0 )
    {
        this->position.x = 0;
    }
    if( this->position.y < 0 )
    {
        this->position.y = 0;
    }
    if( this->position.x > manager.maps[manager.current_map].width - this->position.w )
    {
        this->position.x = manager.maps[manager.current_map].width - this->position.w;
    }
    if( this->position.y > manager.maps[manager.current_map].height - this->position.h )
    {
        this->position.y = manager.maps[manager.current_map].height - this->position.h;
    }
    //cout << "Obj:" << obj.x << "," << obj.y << "," << obj.w << "," << obj.h << endl;
    //cout << "Camera: " << position.x << "," << position.y << "," << position.w << "," << position.h << endl;
}

void Camera::Init(unsigned int x, unsigned int y, unsigned int w, unsigned int h){
    this->position.x = x;
    this->position.y = y;
    this->position.w = w;
    this->position.h = h;
}
