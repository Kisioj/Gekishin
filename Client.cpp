#include "Client.h"
#include "GameManager.h"
#include <iostream>//delete
using namespace std;

extern GameManager manager;

Client::Client(){
    //position 20,20,20,20

    this->vx = 0;
    this->vy = 0;

    this->speed = 2;
}
//http://sdl.beuc.net/sdl.wiki/Handling_the_Keyboard
void Client::handleEvent( SDL_Event& e ){
	if( e.type == SDL_KEYDOWN  )
    {
        if(manager.console.state==2){

            SDL_Keycode k = e.key.keysym.sym;
            Uint16 mod = e.key.keysym.mod;

            bool modified_input = false;
            int input_size = manager.console.input.size();
            int input_max_size = manager.console.line_width;


            if(k=='`') manager.console.state=0;
            else if(k==SDLK_BACKSPACE && input_size){
                string& input = manager.console.input;
                input=input.substr(0,input.size()-1);
            }else if(input_size<input_max_size && (k>=' ' && k<= '~')){
                if(mod & KMOD_SHIFT){
                    if( k>='a' && k<='z' ) k-=32;
                    else if( k == '1') k = '!';else if( k == '2') k = '@';
                    else if( k == '3') k = '#';else if( k == '4') k = '$';
                    else if( k == '5') k = '%';else if( k == '6') k = '^';
                    else if( k == '7') k = '&';else if( k == '8') k = '*';
                    else if( k == '9') k = '(';else if( k == '0') k = ')';
                    else if( k == '-') k = '_';else if( k == '=') k = '+';
                    else if( k == '[') k = '{';else if( k == ']') k = '}';
                    else if( k == '\\') k = '|';else if( k == ';') k = ':';
                    else if( k == '\'') k = '"';else if( k == ',') k = '<';
                    else if( k == '.') k = '>';else if( k == '/') k = '?';
                }
                manager.console.input.push_back(k);
            }else if(k==SDLK_RETURN && input_size){
                string& input = manager.console.input;
                int& cmd = manager.console.current_command;
                if(cmd!=-1 && manager.console.commands[cmd]==input){
                    manager.console.commands.erase(manager.console.commands.begin() + cmd);
                }
                manager.console.parse(input);
                input = "";
                cmd = -1;
            }else if(k==SDLK_UP || k== SDLK_DOWN){
                int& cmd = manager.console.current_command;

                if(k==SDLK_UP){
                    cmd++;
                    if(cmd>=int(manager.console.commands.size())) cmd=-1;
                }else if(k==SDLK_DOWN){
                    cmd--;
                    if(cmd<-1) cmd=-1;
                }
                if(cmd==-1){
                    manager.console.input = "";
                }else{
                    manager.console.input = manager.console.commands[cmd];
                }

            }



            if(modified_input){
                manager.console.show_ambulance = true;
                manager.console.prompt_time = SDL_GetTicks() + 500;
            }
        }else if(e.key.repeat == 0){
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: this->vy -= this->speed; break;
                case SDLK_DOWN: this->vy += this->speed; break;
                case SDLK_LEFT: this->vx -= this->speed; break;
                case SDLK_RIGHT: this->vx += this->speed; break;
                case '`': manager.console.state=2;
            }
        }

    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        if(manager.console.state!=2){
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: this->vy += this->speed; break;
                case SDLK_DOWN: this->vy -= this->speed; break;
                case SDLK_LEFT: this->vx += this->speed; break;
                case SDLK_RIGHT: this->vx -= this->speed; break;
            }
        }
    }
}


void Client::move(){
    this->obj.position.x += this->vx;

    if( ( this->obj.position.x < 0 ) || ( this->obj.position.x + this->obj.position.w > manager.maps[manager.current_map].width ) )
    {
        //cout << this->obj.position.x + this->obj.position.w << " VS " << manager.maps[manager.current_map].width << endl;
        //this->obj.position.x = manager.maps[manager.current_map].width - this->obj.position.w;
        this->obj.position.x -= this->vx;
    }else if(this->vx){
        int for_tile_x, from_tile_y, to_tile_y;
        if(this->vx>0){ //klient idzie w prawo
            for_tile_x = (this->obj.position.x+this->obj.position.w-1)/manager.tile_size;
            from_tile_y = this->obj.position.y / manager.tile_size;
            to_tile_y = (this->obj.position.y+this->obj.position.h-1) / manager.tile_size;
        }else if(this->vx<0){ //klient idzie w lewo
            for_tile_x = this->obj.position.x/manager.tile_size;
            from_tile_y = this->obj.position.y / manager.tile_size;
            to_tile_y = (this->obj.position.y+this->obj.position.h-1) / manager.tile_size;
        }

        if(for_tile_x < 0) for_tile_x = 0;
        if(for_tile_x > manager.maps[manager.current_map].maxx){for_tile_x = manager.maps[manager.current_map].maxx;}
        //cout << "for_tile_x:" <<for_tile_x << ",from_tile_y:"<<from_tile_y<<",to_tile_y:"<<to_tile_y << endl;

        for(int y=from_tile_y;y<=to_tile_y;y++){
            bool should_break = false;
            for (auto it=manager.maps[manager.current_map].tiles[for_tile_x][y].begin(); it != manager.maps[manager.current_map].tiles[for_tile_x][y].end(); ++it){
                Object& tile = *it;
                if(tile.density && this->obj.density){
                    if(this->vx>0 && this->obj.position.x+this->obj.position.w-tile.position.x){ //klient idzie w prawo
                        this->obj.position.x -= this->vx;
                        should_break = true; break;
                    }else if(this->vx<0 && this->obj.position.x-tile.position.x-tile.position.w){ //klient idzie w lewo
                        this->obj.position.x -= this->vx;
                        should_break = true; break;
                    }
                }
            }
            if(should_break) break;

        }

    }



    this->obj.position.y += this->vy;
    if( ( this->obj.position.y < 0 ) || ( this->obj.position.y + this->obj.position.h > manager.maps[manager.current_map].height ) )
    {
        this->obj.position.y -= this->vy;
    }else if(this->vy){
        //cout << "client.y: " << this->position.y << ",client.h:" << this->position.h << ", map.h:" <<manager.maps[manager.current_map].height << endl;
        int for_tile_y, from_tile_x, to_tile_x;
        if(this->vy>0){ //klient idzie na południe
            for_tile_y = (this->obj.position.y+this->obj.position.h-1)/manager.tile_size;
            from_tile_x = this->obj.position.x / manager.tile_size;
            to_tile_x = (this->obj.position.x+this->obj.position.w-1) / manager.tile_size;
        }else if(this->vy<0){ //klient idzie na północ
            for_tile_y = this->obj.position.y/manager.tile_size;
            from_tile_x = this->obj.position.x / manager.tile_size;
            to_tile_x = (this->obj.position.x+this->obj.position.w-1) / manager.tile_size;
        }

        if(for_tile_y < 0) for_tile_y = 0;
        if(for_tile_y > manager.maps[manager.current_map].maxy){for_tile_y = manager.maps[manager.current_map].maxy;}

        for(int x=from_tile_x;x<=to_tile_x;x++){
            bool should_break = false;
            for (auto it=manager.maps[manager.current_map].tiles[x][for_tile_y].begin(); it != manager.maps[manager.current_map].tiles[x][for_tile_y].end(); ++it){
                Object& tile = *it;
                if(tile.density && this->obj.density){
                    if(this->vy>0 && this->obj.position.y+this->obj.position.h-tile.position.y){ //klient idzie na południe
                        this->obj.position.y -= this->vy;
                        should_break = true; break;
                    }else if(this->vy<0 && this->obj.position.y-tile.position.y-tile.position.h){ //klient idzie na południe
                        this->obj.position.y -= this->vy;
                        should_break = true; break;
                    }
                }
            }
            if(should_break) break;
        }

    }

}


