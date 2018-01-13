#include "GameManager.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include "Frame.h"
#include "Texture.h"
#include "IconState.h"
#include "PNGParser.cpp"



GameManager::GameManager(){
    cout << "GameManager::GameManager()" << endl;
    this->tile_size = 16;
    this->textures_count = 0;

    this->native_screen_size.x = 0;
    this->native_screen_size.y = 0;
    this->native_screen_size.w = 352;//256;
    this->native_screen_size.h = 352;//224;

    this->current_screen_size.x = 0;
    this->current_screen_size.y = 0;
    this->current_screen_size.w = this->native_screen_size.w;
    this->current_screen_size.h = this->native_screen_size.h;

    this->resized = false;

    this->camera.Init(0, 0, this->native_screen_size.w, this->native_screen_size.h);

    this->music = nullptr;
    this->music_volume = 100;

    this->obj_to_place = nullptr;
}

void GameManager::Close(){
    cout << "GameManager::Close" << endl;

    map<string,Icon*>::iterator icon_it;
    for(icon_it=this->icons.begin(); icon_it!=this->icons.end(); ++icon_it){
        map<string,IconState*>::iterator icon_st_it;
        for(icon_st_it=icon_it->second->icon_states.begin(); icon_st_it!=icon_it->second->icon_states.end(); ++icon_st_it){
            delete[] icon_st_it->second->frames;
            delete icon_st_it->second;
        }
        delete icon_it->second;
    }

    map<string,Object*>::iterator obj_it;
    for(obj_it=this->objects.begin(); obj_it!=this->objects.end(); ++obj_it){
        delete obj_it->second;
    }


    for(unsigned int i=0;i<this->maps.size();i++){
        maps[i].Destroy();
    }

	for(unsigned int i=0;i<this->textures_count;i++){
        this->textures[i]->Free();
        delete this->textures[i];
	}


	SDL_DestroyRenderer( this->renderer );
	SDL_DestroyWindow( this->window );
	this->window = NULL;
	this->renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool GameManager::Init(){
    cout << "GameManager::Init()" << endl;
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{

		//Enable VSync
		if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" ) )
		{
			cout << "Warning: VSync not enabled!" << endl;
		}

		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) )//0 - widac pixele, 1 - rozmywa sie
		{
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}

		//Create window
		this->window = SDL_CreateWindow("Gekishin Freeza",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        this->native_screen_size.w,
                                        this->native_screen_size.h,
                                        SDL_WINDOW_SHOWN |
                                        SDL_WINDOW_RESIZABLE );


        SDL_Surface* icon = IMG_Load("media/icon.png");
        SDL_SetWindowIcon(this->window, icon);
        SDL_FreeSurface(icon);


		//SDL_SetWindowSize(gWindow, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		if( this->window == NULL )
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Create renderer for window
			this->renderer = SDL_CreateRenderer(this->window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_TARGETTEXTURE ); // SDL_RENDERER_PRESENTVSYNC
			if( this->renderer == NULL )
			{
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );

                this->back_buffer = SDL_CreateTexture(this->renderer,
                                                SDL_GetWindowPixelFormat(this->window),
                                                SDL_TEXTUREACCESS_TARGET,
                                                this->native_screen_size.w,
                                                this->native_screen_size.h);

                //IMPORTANT Set the back buffer as the target
                SDL_SetRenderTarget(this->renderer, this->back_buffer);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}

				if( TTF_Init() == -1 )
                {
                    cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
                    success = false;
                }else{

                    cout << "SDL_ttf initialized!" << endl;
                }

			}
		}
	}

	return success;
}

void GameManager::Resize(){
    cout << "GameManager::Resize()" << endl;
    int w, h;
    SDL_GetWindowSize(this->window, &w, &h);

    this->scale_ratio_w = w / this->native_screen_size.w;
    this->scale_ratio_h = h / this->native_screen_size.h;

    this->current_screen_size.w = w;
    this->current_screen_size.h = h;

    SDL_DestroyTexture(this->back_buffer);
    this->back_buffer = SDL_CreateTexture(  this->renderer,
                                            SDL_GetWindowPixelFormat(this->window),
                                            SDL_TEXTUREACCESS_TARGET, //Again, must be created using this
                                            this->native_screen_size.w,
                                            this->native_screen_size.h);

    SDL_Rect view_port;
    SDL_RenderGetViewport(this->renderer, &view_port);
    if(view_port.w != this->current_screen_size.w || view_port.h != this->current_screen_size.h)
    {
        //VERY IMPORTANT - Change the viewport over to the new size. It doesn't do this for you.
        SDL_RenderSetViewport(this->renderer, &this->current_screen_size);
    }
}

void GameManager::Render(){
    //render map
    int from_tile_x = this->camera.position.x / this->tile_size;
    int from_tile_y = this->camera.position.y / this->tile_size;
    int to_tile_x = from_tile_x + this->native_screen_size.w/16;//16; //-1
    int to_tile_y = from_tile_y + this->native_screen_size.h/16;//14; //-1

    if(to_tile_x >= this->maps[this->current_map].maxx)
        to_tile_x = this->maps[this->current_map].maxx-1;
    if(to_tile_y >= this->maps[this->current_map].maxy)
        to_tile_y = this->maps[this->current_map].maxy-1;



    for(int y=from_tile_y;y<=to_tile_y;y++){
        for(int x=from_tile_x;x<=to_tile_x;x++){
            for (auto it=this->maps[this->current_map].tiles[x][y].begin(); it != this->maps[this->current_map].tiles[x][y].end(); ++it){
                Object* obj = &(*it);//maybe &(*it);
                obj->render(camera.position.x, camera.position.y);
            }
        }
    }



    this->client.obj.render( camera.position.x, camera.position.y );
    this->console.render();

    SDL_SetRenderTarget(this->renderer, NULL); //Set the target back to the window
    if(this->resized)
    {
        this->Resize();
        this->resized = false;
    }
    SDL_RenderCopy(this->renderer, this->back_buffer, &this->native_screen_size, &this->current_screen_size); //Render the gBackBuffer onto the screen at (0,0)
    /* START: Textures here will be the always the same size: */

    /*SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
    SDL_Rect rect2 = {0,160,200,50};
    SDL_SetRenderDrawColor(this->renderer, 0,0,0,255);
    SDL_RenderDrawRect(this->renderer, &rect2);
    SDL_SetRenderDrawColor(this->renderer, 255,50,50,200);
    SDL_RenderFillRect(this->renderer, &rect2);*/

    //this->RenderText(10,10,"super partia", this->font);
    /* END */

    SDL_RenderPresent(this->renderer);
    SDL_RenderClear(this->renderer); //Clear the window buffer

    SDL_SetRenderTarget(this->renderer, this->back_buffer); //Set the target back to the back buffer
    SDL_RenderClear(this->renderer); //Clear the back buffer
}

void GameManager::RenderText(int x, int y, string s, TTF_Font* font, SDL_Color& color){
    //SDL_Color outline = {255,255,255};
    Texture* text = new Texture();
    //text->LoadFromRenderedText(s, outline);
    //text->render(x+1,y+1);
    //text->Free();
    text->LoadFromRenderedText(s, color);
    text->render(x,y);
    text->Free();
    delete text;
}


void GameManager::Update(){
    this->client.move();
    SDL_Rect center = {client.obj.position.x, client.obj.position.y, client.obj.position.w, client.obj.position.h};
    this->camera.Center(center);
}

bool GameManager::HandleEvents(){
    while( SDL_PollEvent( &this->event ) )
    {
        if( this->event.type == SDL_QUIT )
        {
            return true;
        }
        else if(this->event.type == SDL_WINDOWEVENT)
        {
            if(this->event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                this->resized = true;
            }
        }/*else if(this->event.type == SDL_MOUSEBUTTONDOWN && this->event.button.button == SDL_BUTTON_LEFT){
            if(this->obj_to_place!=NULL){
                double scale_w = 1.0*this->current_screen_size.w/this->native_screen_size.w;// * this->tile_size;
                double scale_h = 1.0*this->current_screen_size.h/this->native_screen_size.h;// * this->tile_size;
                int tile_x = ((this->event.button.x/scale_w) + this->camera.position.x)/this->tile_size;
                int tile_y = ((this->event.button.y/scale_h) + this->camera.position.y)/this->tile_size;
                cout << "tile x:" << tile_x << ",y:" << tile_y << endl;

                Object* tile = &this->maps[this->current_map].tiles[tile_x][tile_y];
                tile->icon_state = this->obj_to_place->icon_state;
                tile->density = this->obj_to_place->density;
            }
        }*/
        client.handleEvent( this->event );
    }
    return false;
}


bool GameManager::LoadIcon(string path){
    string file;
    unsigned found = path.find_last_of("/");
    if(found != string::npos ) file = path.substr(found+1);
    else file = path;

    Texture* texture = new Texture();
    if(!texture->LoadFromFile(path)){
        delete texture;
        cout << "Failed to load texture: " << path << endl;
        return false;
    }else{
        this->textures[textures_count] = texture;
        this->icons[file] = new Icon();
        this->icons[file]->texture_id = textures_count;

        stringstream s;
        read_png_file(path, s);
        string str="";
        string state="";
        string attribute;
        int *delay = nullptr;
        int width=0;
        int height=0;
        char c=0;

        int frames=1;
        int dirs=1;
        int rewind=0;
        int loop=0;

        int frame_no=0;
        int texture_frames_x=0;

        while(!s.eof()){

            s >> str;
            if(str=="width"){s >> c >> width;texture_frames_x=texture->width/width;}
            else if(str=="height") s >> c >> height;
            else if(str=="version"){
                s >> c >> str;
            }else if(str=="state"){
                attribute="";
                if(state!=""){
                    cout << "(" <<  (frame_no%texture_frames_x)*width << "," << (frame_no/texture_frames_x)*height << "," << width << ","<< height << ")";
                    cout << "}\n";
                    SDL_Rect rect = {(frame_no%texture_frames_x)*width,(frame_no/texture_frames_x)*height,width,height};
                    IconState* icon_state = new IconState(texture->width, rect, frames, delay, dirs, rewind);
                    icon_state->icon = this->icons[file];
                    this->icons[file]->icon_states[state] = icon_state;
                    delete delay;

                    frame_no += dirs*frames;
                }
                s >> c >> state;
                state.erase(remove(state.begin(),state.end(),'"'),state.end());
                cout << "state: " << state << " {";
                frames=1;dirs=1;rewind=0;loop=0;
            }else if(str=="#"){
                s >> str;
                if(str=="END"){
                    cout << "(" <<  (frame_no%texture_frames_x)*width << (frame_no/texture_frames_x)*height << width << height << ")";
                    cout << "}\n";
                    SDL_Rect rect = {(frame_no%texture_frames_x)*width,(frame_no/texture_frames_x)*height,width,height};
                    IconState* icon_state = new IconState(texture->width, rect, frames, delay, dirs, rewind);
                    icon_state->icon = this->icons[file];
                    this->icons[file]->icon_states[state] = icon_state;
                    delete delay;
                    break;
                }else if(str=="BEGIN") s>>str;
            }else{
                if(str[0]==',') continue;
                if(attribute!="") cout << "; ";
                attribute = str;
                s >> c;
                if(attribute=="frames"){
                    s >> frames;
                    cout << "frames:" << frames;
                    delay = new int[frames*dirs];
                }
                else if(attribute=="dirs"){
                    s >> dirs;
                    cout << "dirs:" << dirs;
                }else if(attribute=="loop"){
                    s >> loop;
                    cout << "loop:" << loop;
                }else if(attribute=="rewind"){
                    s >> rewind;
                    cout << "rewind:" << (rewind?"true":"false");
                }
                else if(attribute=="delay"){
                    cout << "delay:";

                    double d;
                    for(int i=0;i<frames;i++){
                        s >> d;
                        delay[i] = d*100;
                        cout << delay[i];
                        if(i!=frames-1){
                            s >> c;
                            cout << ",";
                        }
                    }
                }
            }
        }
        this->icons[file]->frame_width = width;
        this->icons[file]->frame_height = height;
        this->textures_count++;
        return true;
    }
}

bool GameManager::LoadMedia(){
    cout << "GameManager::LoadMedia()" << endl;

    bool success = true;
    string files[] = {
                        "media/tiles/space.png",
                        "media/tiles/space_radar.png"
                        };
    int files_num = sizeof(files) / sizeof(string);

    for(int i=0;i<files_num;i++){
        if(!this->LoadIcon(files[i])) success=false;
    }

    this->client.obj.setIcon("space.png","pointer");
    this->client.obj.setPosition(16,16,16,16);
    this->client.obj.density = false;

    cout << this->client.obj.icon_state->frames[0].time << endl;
    cout << this->client.obj.icon_state->frames[1].time << endl;
    cout << this->client.obj.icon_state->frames[2].time << endl;


    /*
    string obj="";
    string attribute;
    */


    string str="";

    stringstream s;

    string obj = "", icon = "", icon_state="";
    int pixel_x, pixel_y, density;

    vector<string> types{"obj","turf"};
    vector<string> attributes{"icon","icon_state","pixel_x","pixel_y","density"};
    unsigned char c;

    ifstream file("media/maps/objs.dm");
    while(!file.eof()){
        file >> std::noskipws >> c;
        if(c=='\''||c=='"'||c=='=') s << ' ';
        else s << c;
    }
    s << " #END"; // end
    file.close();

    //cout << s.str() << endl;

    while(!s.eof()){
        s >> str;
        if( find( begin(types),end(types), str ) != end(types) ) continue;
        if( find( begin(attributes),end(attributes), str ) != end(attributes) ){
            if(str=="icon") s >> icon; //will suck in some cases, when parent has icon, children's icon will overwrite it for next children
            else if(str=="icon_state") s >> icon_state;
            else if(str=="pixel_x") s >> pixel_x;
            else if(str=="pixel_y") s >> pixel_y;
            else if(str=="density") s >> density;
        }else if( (str[0]>='a'&&str[0]<='z') || (str[0]>='A'&&str[0]<='Z') || str[0]=='#' ){   //obj name can start only from letter
            if(obj!=""){
                this->objects[obj] = new Object(icon,icon_state,density,-pixel_x,-pixel_y);
                cout << obj << ", ";
            }
            if(str[0]=='#') break;
            obj=str;
            pixel_x=0;pixel_y=0;density=0;
        }
    }





/*
    this->objects["path"] = new Object("space.png","path",false);
    this->objects["path_arrow"] = new Object("space.png","path_arrow",false);
    this->objects["black"] = new Object("space.png","black",true);
    this->objects["white"] = new Object("space.png","white",true);
    this->objects["edge"] = new Object("space.png","edge",true);
    this->objects["rocks"] = new Object("space.png","rocks",true);

    this->objects["ship1"] = new Object("space.png","ship1",false);
    this->objects["ship2"] = new Object("space.png","ship1",false);
    this->objects["pointer"] = new Object("space.png","pointer",false);

    this->objects["stars1"] = new Object("space.png","stars1",true);
    this->objects["stars2"] = new Object("space.png","stars2",true);
    this->objects["stars3"] = new Object("space.png","stars3",true);

    this->objects["planet1"] = new Object("space.png","planet1",true);
    this->objects["planet2"] = new Object("space.png","planet2",true);
    this->objects["planet3"] = new Object("space.png","planet3",true);
    this->objects["planet4"] = new Object("space.png","planet4",true);
    this->objects["planet5"] = new Object("space.png","planet5",true);

    this->objects["star1"] = new Object("space.png","star1",true);
    this->objects["star2"] = new Object("space.png","star2",true);
    this->objects["star3"] = new Object("space.png","star3",true);
    this->objects["star4"] = new Object("space.png","star4",true);
*/


    s.str(string()); s.clear();

    this->maps.push_back(Map());
    this->maps[0].Load("media/maps/space1.dmm");

    //return 0;
    /*
    this->maps[0].width = 32*16;//1280;
    this->maps[0].height = 60*16;//960;
    this->maps[0].Create(32,60);
    */
    this->current_map = 0;

    this->font = TTF_OpenFont( "media/fonts/Courier_Prime.ttf", 14 );
	if( this->font == NULL )
	{
		printf( "Failed to load poke font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
		return false;
	}

    if(!console.loadFont()){
        success = false;
        return false;
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);


	return success;
}


