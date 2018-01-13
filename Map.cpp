#include "Map.h"
#include "GameManager.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


extern GameManager manager;

Map::Map()
{
    this->maxx = 0;
    this->maxy = 0;
    this->width = 0;
    this->height = 0;
}



void Map::Create(int maxx, int maxy){
    /*this->maxx = maxx;
    this->maxy = maxy;
    tiles = new Object*[maxx];
    for(int x=0;x<maxx;x++){
        tiles[x] = new Object[maxy];
    }

    for(int y=0;y<maxy;y++){
        for(int x=0;x<maxx;x++){
            if(x==0||x==maxx-1||y==0||y==maxy-1){
                this->tiles[x][y] = *manager.objects["edge"]; //wykonuje sie domyslny operator przypisania
            }else if(x%5&&y%5) {
                this->tiles[x][y] = *manager.objects["path"];
            }else{
                this->tiles[x][y] = *manager.objects["stars3"];
                if(rand() % 2){
                    this->tiles[x][y].setIcon("space.png", "star3");
                    this->tiles[x][y].density = true;
                }
            }
            this->tiles[x][y].position.x = x*manager.tile_size;
            this->tiles[x][y].position.y = y*manager.tile_size;
        }
    }*/

}

void Map::Load(string filename){
    string symbol="";
    string obj="";
    map<string,list<Object>> objects;
    vector<string> tile_symbols;

    string str="";
    char c;
    stringstream s;

    vector<string> keywords{"turf","obj","area"};
    vector<string> attributes{"dir"};

    ifstream file(filename);
    while(!file.eof()){
        file >> std::noskipws >> c;
        if(c=='/'||c=='"'||c=='='||c==',') s << ' ';
        else if(c=='{'||c=='}') s << ' ';
        else if(c=='('||c==')') s << " " << c << " ";
        else s << c;
    }
    file.close();
    s << " #END"; // end

    cout << s.str() << endl;

    int symbol_size=0, dir=0;

    bool settings = true; //file consist of settings and then map info, so when settings are over, this goes false
    while(!s.eof()){
        s >> str;
        if(settings){
            if(!symbol_size) symbol_size=str.size();

            if( find( begin(keywords),end(keywords), str ) != end(keywords) ) continue;
            else if( find( begin(attributes),end(attributes), str ) != end(attributes) ){
                if(str=="dir") s >> dir;
                cout << str << ":"<<dir<<", ";
                objects[symbol].front().dir = dir;
                continue;
            }
            else if(str=="("){
                if(symbol==""){
                    s >> str >> str >> str >> str;
                    settings = false;
                    continue;
                }else{

                }

            }else if(str==")"){
                symbol.clear();
            }else if(symbol==""){
                symbol = str;

            }else{
                cout << "OBJ:";
                obj = str;
                objects[symbol].push_front(*manager.objects[obj]);
            }
        }else if(str=="#END"){
                break;
        }else{
            this->maxy++;
            if(!this->maxx) this->maxx = str.size();
            for(int x=0;x<maxx;x++){
                string tile="";
                if(symbol_size==1){
                    tile+=str[x];
                }else if(symbol_size==2){
                    tile+=str[x*2];
                    tile+=str[(x*2)+1];
                }else if(symbol_size==3){
                    tile+=str[x*3];
                    tile+=str[(x*3)+1];
                    tile+=str[(x*3)+2];
                }

                tile_symbols.push_back(tile);
            }
        }

        cout << str << ", ";

    }
    cout << endl << "symbol_size:" << int(symbol_size) << endl;
    /*for(unsigned int i=0;i<tile_symbols.size();i++){
        cout << tile_symbols[i]<<" | ";
    }*/
    cout << endl << "DZIWNE" << endl;
    cout << "maxx:" << maxx << ", maxy" << maxy << endl;

    tiles = new list<Object>*[maxx];
    for(int x=0;x<maxx;x++){
        tiles[x] = new list<Object>[maxy];
    }


    for(int y=0;y<maxy;y++){
        for(int x=0;x<maxx;x++){

            for (auto it=objects[tile_symbols[y*maxx+x]].begin(); it != objects[tile_symbols[y*maxx+x]].end(); ++it){
                this->tiles[x][y].push_back(*it);
            }

            for (auto it=manager.maps[manager.current_map].tiles[x][y].begin(); it != manager.maps[manager.current_map].tiles[x][y].end(); ++it){
                it->position.x = x*manager.tile_size;
                it->position.y = y*manager.tile_size;

            }

        }
    }

    /*
    for(int y=0;y<maxy;y++){
        for(int x=0;x<maxx;x++){

            cout << "<"<<x<<","<<y<<">: ";

            for (auto it=manager.maps[manager.current_map].tiles[x][y].begin(); it != manager.maps[manager.current_map].tiles[x][y].end(); ++it){
                cout << "("<<it->position.x<<","<<it->position.y<<") ";
            }
            cout << endl;

        }
    }
    */


    this->width = this->maxx * manager.tile_size;
    this->height = this->maxy * manager.tile_size;
}

void Map::Destroy(){
    for (int x = 0; x < this->maxx; x++){
        delete [] tiles[x];
    }
    delete [] tiles;
}
