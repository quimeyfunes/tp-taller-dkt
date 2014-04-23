#include "DibujableGFX.h"

DibujableGFX::DibujableGFX(){
}

DibujableGFX::~DibujableGFX(){
}

void DibujableGFX::dibujar(SDL_Renderer* renderer, int corrimiento, int escalaZoom, int posZoomX, int posZoomY){
}

int* DibujableGFX::getColor(){
	return this->color;
}
void DibujableGFX::setColor(int* rgb){
	for (int i = 0; i < 3; i++) {
		this->color[i] = rgb[i];
	}
}
