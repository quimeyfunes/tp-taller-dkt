#include "ScrollingSprite.h"


ScrollingSprite::ScrollingSprite(SDL_Renderer* renderer, SDL_Rect rect, string path): DibujableTextura(){

	this->rect = rect;
	this->imagen = IMG_LoadTexture(renderer, path.c_str());
	this->velocidadRefresco = timeStepNubes;
	
	this->anchoEscenario = ParserYaml::getParser()->getEscenario()->anchoU*relacionPPU;
	//printf("%i", this->anchoEscenario);
	this->contador=0;
}


ScrollingSprite::~ScrollingSprite(){
}

void ScrollingSprite::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx){

	SDL_Rect rectAux = rect;
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rectAux = realizarZoom(rectAux, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopy(renderer, this->imagen , NULL , &rectAux);
	} else {
		rectAux.x -= corrimientoX;
		rectAux.y -= corrimientoY;
		SDL_RenderCopy(renderer, this->imagen, NULL, &rectAux);
	}
	
	contador++;
	if(contador >= this->velocidadRefresco){
		rect.x++;
		contador = 0;
	}

	if(rect.x >= this->anchoEscenario) rect.x = -rect.w;
	
}