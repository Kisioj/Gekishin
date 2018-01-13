#include "Texture.h"
#include "GameManager.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

extern GameManager manager;

Texture::Texture()
{
    //cout << "Texture::Texture" << endl;
    this->tex = NULL;
    this->width = 0;
    this->height = 0;
}

Texture::~Texture()
{
    //cout << "Texture::~Texture" << endl;
	Free();
}

void Texture::Free(){

    if( this->tex != NULL )
	{
        //cout << "Texture::Free" << endl;
		SDL_DestroyTexture( this->tex );
		this->tex = NULL;
		this->width = 0;
		this->height = 0;
	}
}

bool Texture::LoadFromFile( std::string path )
{
    printf("Texture::LoadFromFile\n");
	Free();

	SDL_Texture* new_tex = NULL;

	SDL_Surface* surface = IMG_Load( path.c_str() );
	if( surface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ) );

        new_tex = SDL_CreateTextureFromSurface( manager.renderer, surface );
		if( new_tex == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			this->width = surface->w;
			this->height = surface->h;
		}
		SDL_FreeSurface( surface );
	}

	this->tex = new_tex;
	return this->tex != NULL;
}



void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( this->tex, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( this->tex, blending );
}

void Texture::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( this->tex, alpha );
}

void Texture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y, this->width, this->height };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx( manager.renderer, this->tex, clip, &renderQuad, angle, center, flip );
}



bool Texture::LoadFromRenderedText( string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	this->Free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( manager.font, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        this->tex = SDL_CreateTextureFromSurface( manager.renderer, textSurface );
		if( this->tex == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			this->width = textSurface->w;
			this->height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return this->tex != NULL;
}
