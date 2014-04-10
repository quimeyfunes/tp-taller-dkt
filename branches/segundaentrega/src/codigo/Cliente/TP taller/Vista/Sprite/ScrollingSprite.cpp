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

void ScrollingSprite::dibujar(SDL_Renderer* renderer){

	SDL_RenderCopy(renderer, this->textura, NULL, &rec);

	contador++;
	if(contador >= this->velocidadRefresco){
		rec.x++;
		contador = 0;
	}

	if(rec.x >= this->anchoEscenario) rec.x= -rec.w;
	
}