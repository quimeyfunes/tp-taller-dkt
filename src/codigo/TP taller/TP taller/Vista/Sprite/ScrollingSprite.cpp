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
	int resize = 150;
	int newH = ((rectAux.h) * (resize / 100));
	int newW = ((rectAux.w) * (resize / 100));


	if (escalaZoom != escalaZoomDefault) {
		int scrollLeft = (( posZoomX * (resize / 100 )) - ((newW / 2) /2));
		int scrollTop = ((posZoomY * (resize / 100)) - (( newH / 2 ) / 2));
		rectAux.x -= scrollLeft;
		rectAux.y -= scrollTop;
		rectAux.h = newH*escalaZoom;
		rectAux.w = newW*escalaZoom;
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