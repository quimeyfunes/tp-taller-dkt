#include "RelojDibujable.h"


RelojDibujable::RelojDibujable(void)
{
}

RelojDibujable::RelojDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF):DibujableTextura(renderer, rect, pathImagen, pathDEF)
{

}

RelojDibujable::~RelojDibujable(void)
{
}


void RelojDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{
	
		SDL_Rect rect = this->rect;
	
		if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
			//rect = this->realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
			SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
		} else {
			rect.x -= corrimientoX;
			rect.y -= corrimientoY;
			SDL_RenderCopyEx(renderer,this->imagen,NULL, &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
		}

		
	
}
