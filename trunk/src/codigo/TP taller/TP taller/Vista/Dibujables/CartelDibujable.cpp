#include "CartelDibujable.h"


CartelDibujable::CartelDibujable(void)
{
}

CartelDibujable::CartelDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: DibujableTextura(renderer, rect, pathImagen, pathDEF)
{

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
}
