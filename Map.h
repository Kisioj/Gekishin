#pragma once
#ifndef MAP_H
#define MAP_H

#include <string>
#include <list>
using namespace std;

class Object;

class Map
{
    public:
        Map();
        void Create(int x, int y);
        void Load(string filename);
        void Destroy();

        int width;
        int height;

        list<Object>** tiles;
        int maxx;
        int maxy;
};

#endif // MAP_H
