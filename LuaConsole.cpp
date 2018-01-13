#include "LuaConsole.h"
#include "GameManager.h"
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

extern GameManager manager;

void print(string s) {
    cout << s << endl;
    manager.console.extra_input.text = s;
    manager.console.extra_input.color.r = 255;
    manager.console.extra_input.color.g = 255;
    manager.console.extra_input.color.b = 255;
}

void play_music(string s, int loop){
    Mix_FreeMusic(manager.music);
    string path = "media/music/";
    manager.music = Mix_LoadMUS(path.append(s).c_str());
    Mix_PlayMusic(manager.music, loop);
}
void music_volume(int volume){
    Mix_VolumeMusic(volume);
}
void stop_music(){
    Mix_FreeMusic(manager.music);
}

void select_object(string s){
    if(manager.objects.count(s)>0){
        manager.obj_to_place = manager.objects[s];
    }else{
        manager.console.extra_input.text = "object doesn't exist!";
        manager.console.extra_input.color.r = 255;
    }
}

extern "C" int init(lua_State* L){
    //using namespace luabind;
    luabind::open(L);
    luabind::module(L)
    [
        luabind::def("print",&print),
        luabind::def("play",&play_music),
        luabind::def("volume", &music_volume),
        luabind::def("stop", &stop_music),
        luabind::def("select", &select_object)
    ];
    return 0;
}


LuaConsole::~LuaConsole(){
    lua_close(L);
}

LuaConsole::LuaConsole()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    init(L);
    //states: 0-off, 1-animation on, 2 - on, 3 - animation off
    this->state = 2;

    this->map_editor_on = false;

    this->line_width = 30; //characters
    this->lines_num = 7;    //112 / 14(height_px) = 8(lines) = 7(lines) + 1(input)

    this->line_width_px = 30 * 8; //courier 14 has character of width 8px
    this->line_height_px = 14; //courier 14 has character of width 14px? od dolu q to gory t jest 12px

    this->input = "";

    this->current_line = 0;
    this->command_prompt="]";

    this->prompt_time=0;
    this->show_ambulance=false;

    this->text_color.r = 255;
    this->text_color.g = 255;
    this->text_color.b = 255;
    this->text_color.a = 255;

    this->bg_color.r = 0;//50
    this->bg_color.g = 0;//50
    this->bg_color.b = 0;//255
    this->bg_color.a = 245;//200

    this->lines_history_size = 100;
    this->commands_history_size = 100;

    this->current_command = -1;

}


void LuaConsole::render(){
    if(state==2){
        SDL_Rect rect = {0,0,256,112};
        SDL_SetRenderDrawBlendMode(manager.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(manager.renderer, 0,0,0,255);
        SDL_RenderDrawRect(manager.renderer, &rect);
        SDL_SetRenderDrawColor(manager.renderer, bg_color.r,bg_color.g,bg_color.b,bg_color.a);
        SDL_RenderFillRect(manager.renderer, &rect);

        int lines_to_show = this->lines.size();
        if(lines_to_show>lines_num) lines_to_show=this->lines_num;

        for(int i=0;i<lines_to_show;i++){
            this->current_line=i;
            int start_line_py = (lines_to_show-1)*this->line_height_px;
            manager.RenderText(2,start_line_py-this->current_line*this->line_height_px,this->lines[i].text, this->font, this->lines[i].color);
        }

        string s = this->command_prompt+this->input;
        if(SDL_GetTicks() > this->prompt_time){
            this->prompt_time=SDL_GetTicks() + 500;
            if(this->show_ambulance) this->show_ambulance=false;
            else this->show_ambulance = true;
        }
        if(this->show_ambulance) s.append("_");

        manager.RenderText(0,-1+this->lines_num*this->line_height_px, s, this->font, this->text_color);

    }
}


bool LuaConsole::loadFont(){
    this->font = TTF_OpenFont( "media/fonts/Courier_Prime.ttf", 14 );
	if( this->font == NULL )
	{
		printf( "Failed to load poke font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}else{
        return true;
	}

	this->line_height_px = TTF_FontHeight(this->font);
}


//https://gist.github.com/randrews/939029 - ciekawy przyklad zastosowania lua w C++, np tworzenie mapy
//http://www.gamedev.net/topic/523067-lua-c-parser-error-messages/
//int color: 1 - red, 2 - green, 3 - blue, 4 - white, 5 - black, 6 - gray
void LuaConsole::parse(string line){
    this->extra_input.text="";
    this->extra_input.color.r=0;
    this->extra_input.color.g=0;
    this->extra_input.color.b=0;
    this->extra_input.color.a=255;

    SDL_Color c = {0,0,0,255};
    if(luaL_dostring(L, line.c_str())){
        cout << "Error:" << lua_tostring(L, -1) << endl;
        c.r=255;
    }else{
        c.g=255;
    }

    ConsoleLine l = {line, c};
    this->lines.push_front(l);
    if(this->lines.size()>this->lines_history_size) this->lines.pop_back();

    if(this->extra_input.text!=""){
        this->lines.push_front(this->extra_input);
        if(this->lines.size()>this->lines_history_size) this->lines.pop_back();
    }

    this->commands.push_front(line);
    if(this->commands.size()>this->commands_history_size) this->commands.pop_back();
}
