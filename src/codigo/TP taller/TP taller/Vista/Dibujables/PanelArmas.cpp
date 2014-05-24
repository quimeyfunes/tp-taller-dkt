#include "PanelArmas.h"

PanelArmas::PanelArmas(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF):DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
	this->visible = false;
}


PanelArmas::~PanelArmas(){
}

void PanelArmas::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{
	//this->dibujar(renderer, corrimientoX, corrimientoY, escalaZoom, anchoPx, altoPx);
	if(this->visible){
		SDL_Rect rect = this->rect;
	
		if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
			rect = this->realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
			SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
		} else {
			rect.x -= corrimientoX;
			rect.y -= corrimientoY;
			SDL_RenderCopyEx(renderer,this->imagen,NULL, &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
		}
	}
}

void PanelArmas::alternarVisibilidad(){
	if(this->visible){
		this->visible = false;
	}else{
		this->visible = true;
	}
}
