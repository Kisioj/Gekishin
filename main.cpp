//http://www.youtube.com/watch?v=2fVvqtmIqio - filmik z 1 czesci gry
//http://www.zophar.net/translations/nes/english.html - linki do wszystkich 4 czesci tej gry
//http://dragonball.wikia.com/wiki/Dragon_Ball_Z_II:_Gekishin_Freeza - info o grze
//http://lazyfoo.net/tutorials/SDL/index.php
//http://www.experts-exchange.com/Programming/Languages/CPP/A_2812-Which-STL-Container.html - o kontenerach
//http://librocket.com - dobre interfejsy mozna robic w tym w HTMLu i CSSie i zastosowac w SDLu
//http://mdqinc.com/blog/2013/01/integrating-librocket-with-sdl-2/ - implementacja tej bilbioteki (ps. podobno jest cos w stylu jquery do tego, zeby animacje byly czadowe)

//protecting resources:
//https://gamedevelopment.tutsplus.com/tutorials/create-custom-binary-file-formats-for-your-games-data--gamedev-206
//http://gamedev.stackexchange.com/questions/37648/how-can-you-put-all-images-from-a-game-to-1-file

//luabind przyklady:
//http://www.gamedev.net/topic/196231-anyone-have-a-luabind-example/
//http://stackoverflow.com/questions/2581377/pass-c-object-to-lua-function
//http://www.dholm.com/2010/10/07/binding-c-to-lua-part-3/
//http://www.gamedev.net/topic/601684-luabind-push-object/
//http://rubenlaguna.com/wp/2012/11/26/first-steps-lua-cplusplus-integration/ - luaL_newstate() jest, wiec jest dobrze
//http://halmd.org/developer/luabind.html

//http://www.labbookpages.co.uk/software/imgProc/libPNG.html - pisanie do plikow png z libPNG, mozna pisac komentarze jak byond
//trzeba bedzie zrobic edytor plikow graficznych, mozna uzyc Qt i Qt-color-picker-dialog
//http://stackoverflow.com/questions/1972239/qt-color-picker-dialog
//standardy cpp http://technojazda.wikidot.com/standardcpp#toc20
//http://baptiste-wicht.com/posts/2012/12/cpp-benchmark-vector-list-deque.html - porownanie roznych przypadkow listy, vectora i deque, świetne, z wykresami
//http://stackoverflow.com/questions/9688200/difference-between-shared-objects-so-static-libraries-a-and-dlls-so - roznica miedzy bibliotekami starycznymi, dynamicznymi a shared objects

//http://www.gamefaqs.com/nes/562880-dragon-ball-z-ii-gekigami-freeza/cheats - kody na postacie
#include "GameManager.h"

using namespace std;

GameManager manager;


int main( int argc, char* args[] )
{
	if( !manager.Init() ) return 0;
	if( !manager.LoadMedia() ) return 0;

    bool quit = false;
    while( !quit )
    {
        quit = manager.HandleEvents();
        manager.Update();
        manager.Render();
    }
    manager.Close();
	return 0;
}

/*
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
                this->objects[obj] = new Object(icon,icon_state,density,pixel_x,pixel_y);
                cout << obj << ", ";
            }
            if(str[0]=='#') break;
            obj=str;
            pixel_x=0;pixel_y=0;density=0;
        }
    }
*/
