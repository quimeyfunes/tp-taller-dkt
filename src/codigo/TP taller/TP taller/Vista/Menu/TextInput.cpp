#include "TextInput.h"


TextInput::TextInput(void)
{
}

TextInput::TextInput(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF, string texto)
: DibujableTextura(renderer, rect, pathImagen, pathDEF){

	this->texto = texto;
	this->textoColor.a = 0xFF;
	this->textoColor.b = 0;
	this->textoColor.g = 0;
	this->textoColor.r = 0;
	this->font = TTF_OpenFont(rutaFuente , 25);
	this->surf =  TTF_RenderText_Solid(this->font, this->texto.c_str() , this->textoColor);
	SDL_DestroyTexture(this->imagen);
	this->imagen = SDL_CreateTextureFromSurface( renderer, this->surf );
	SDL_FreeSurface(this->surf);
}

void TextInput::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{

	SDL_Rect rect = this->rect;
	rect.x = corrimientoX;
	rect.y = corrimientoY;
	rect.w = 500;
	rect.h = 60;

	SDL_RenderCopyEx(renderer,this->imagen, NULL, &rect, this->anguloRotacion,NULL,SDL_FLIP_NONE);
	
}

bool TextInput::loadFromRenderedText(SDL_Renderer* renderer, string textureText, SDL_Color textColor )
{
	if( this->imagen != NULL )
	{
		SDL_DestroyTexture( this->imagen );
		this->imagen = NULL;
	}

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( this->font, textureText.c_str(), textColor );
	
	//Create texture from surface pixels
	this->imagen = SDL_CreateTextureFromSurface( renderer, textSurface );

	//Get rid of old surface
	SDL_FreeSurface( textSurface );
	
	return this->imagen != NULL;
}

TextInput::~TextInput(void)
{
	if(this->font != NULL){
		TTF_CloseFont( this->font );
		this->font = NULL;
	}

	if(this->imagen != NULL){
		SDL_DestroyTexture(this->imagen);
		this->imagen = NULL;
	}
}
