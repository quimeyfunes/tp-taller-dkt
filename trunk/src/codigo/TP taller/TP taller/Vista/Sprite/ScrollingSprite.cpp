#include "ScrollingSprite.h"


ScrollingSprite::ScrollingSprite(SDL_Renderer* renderer, SDL_Rect rect, string path): DibujableTextura(){

	this->rec = rect;
	this->textura = IMG_LoadTexture(renderer, path.c_str());
	this->velocidadRefresco = timeStepNubes;
	
	this->anchoEscenario = ParserYaml::getParser()->getEscenario()->anchoPx;
	this->contador=0;
}


ScrollingSprite::~ScrollingSprite(){
}

void ScrollingSprite::dibujar(SDL_Renderer* renderer, int corrimiento, int escalaZoom, int posZoomX, int posZoomY){

	SDL_Rect rectAux = rec;
	rectAux.x += corrimiento;
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rectAux = realizarZoom(rectAux, posZoomX, posZoomY, escalaZoom);
		SDL_RenderCopy(renderer, this->textura , NULL , &rectAux);
	} else {
		SDL_RenderCopy(renderer, this->textura, NULL, &rectAux);
	}
	
	contador++;
	if(contador >= this->velocidadRefresco){
		rec.x++;
		contador = 0;
	}

	if(rec.x >= this->anchoEscenario) rec.x = -rec.w;
	
}