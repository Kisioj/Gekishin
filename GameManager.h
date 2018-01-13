#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <png.h>

#include "LuaConsole.h"
#include <SDL2/SDL.h> //not needed if LuaConsole.h included
typedef struct _TTF_Font TTF_Font; //not needed if LuaConsole.h included

#include <vector>
#include "Client.h"
#include "Camera.h"
#include "Map.h"
#include "Icon.h"


using namespace std;

typedef struct _Mix_Music Mix_Music;
class Object;
class Texture;
class Icon;

class GameManager
{
    public:
        GameManager();
        void Close();
        bool Init();
        void Resize();
        void Render();
        void RenderText(int x, int y, string s, TTF_Font* font, SDL_Color& color);
        void Update();
        bool HandleEvents();
        bool LoadMedia();
        bool LoadIcon(string path);
        SDL_Rect native_screen_size;
        SDL_Rect current_screen_size;
        float scale_ratio_w;
        float scale_ratio_h;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* back_buffer;
        bool resized;
        vector<Map> maps;
        unsigned int current_map;
        Camera camera;

        SDL_Event event;

        unsigned int textures_count;
        Texture* textures[256]; //bez kontenera STL bo sie porypie

        int tile_size;
        map<string, Icon*> icons; //was <string, int> before
        map<string, Object*> objects; //needed to create map
        Object* obj_to_place;

        LuaConsole console;
        Client client;

        Mix_Music* music;
        int music_volume;
        TTF_Font* font;


};

#endif // GAMEMANAGER_H
