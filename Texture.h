#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <string>
using namespace std;

class Texture
{
	public:
		Texture();
		~Texture();

		void Free();
		bool LoadFromFile( string path );

		bool LoadFromRenderedText( string textureText, SDL_Color textColor );

		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		void setBlendMode( SDL_BlendMode blending );
		void setAlpha( Uint8 alpha );
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        int width;
		int height;
		SDL_Texture* tex;
};
#endif // TEXTURE_H
