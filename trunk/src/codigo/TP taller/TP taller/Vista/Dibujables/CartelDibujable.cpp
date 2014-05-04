#include "CartelDibujable.h"


CartelDibujable::CartelDibujable(void)
{
}

CartelDibujable::CartelDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
	this->texto = "Rasta";
	this->font = TTF_OpenFont(rutaFuente , 25);
	this->textColor.b = 0;
	this->textColor.g = 0;
	this->textColor.r = 0;
	this->surf =  TTF_RenderText_Solid(this->font, this->texto.c_str() , this->textColor);
	this->imagen = SDL_CreateTextureFromSurface( renderer, this->surf );
	SDL_FreeSurface(this->surf);

	this->rect.w = this->texto.length() * 7;

}

void CartelDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{

	SDL_Rect rect = this->rect;
	rect.x -= corrimientoX;
	rect.y -= corrimientoY;

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->imagen, NULL, &rect, this->anguloRotacion,NULL,SDL_FLIP_NONE);
	}
}

CartelDibujable::~CartelDibujable(void)
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
