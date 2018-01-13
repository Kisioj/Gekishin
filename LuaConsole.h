#pragma once
#ifndef LUACONSOLE_H
#define LUACONSOLE_H

#include <SDL2/SDL.h>
#include <deque>
#include <string>
using namespace std;

struct lua_State;
typedef struct _TTF_Font TTF_Font;

class Object;

struct ConsoleLine{
    string text;
    SDL_Color color;
};

class LuaConsole
{
    public:
        lua_State* L;

        int state;
        bool map_editor_on;
        int line_width;
        int lines_num;
        int line_width_px;
        int line_height_px;
        unsigned int lines_history_size;
        deque<ConsoleLine> lines;
        unsigned int commands_history_size;
        deque<string> commands;
        int current_command;
        string input;
        ConsoleLine extra_input; //can be result of a command
        TTF_Font* font;
        int current_line;
        string command_prompt;
        Uint32 prompt_time;
        bool show_ambulance;

        SDL_Color text_color;
        SDL_Color bg_color;

        LuaConsole();
        ~LuaConsole();
        void render();
        bool loadFont();
        void parse(string line);
};

#endif // LUACONSOLE_H
