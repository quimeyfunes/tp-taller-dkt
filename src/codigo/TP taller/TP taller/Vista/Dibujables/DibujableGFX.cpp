#include "DibujableGFX.h"

DibujableGFX::DibujableGFX(){
}

DibujableGFX::~DibujableGFX(){
}

void DibujableGFX::dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, int escalaZoom, int posZoomX, int posZoomY){
}

int* DibujableGFX::getColor(){
	return this->color;
}
void DibujableGFX::setColor(int* rgb){
	for (int i = 0; i < 3; i++) {
		this->color[i] = rgb[i];
	}
}

DibujableSerializado DibujableGFX::getDibujableSerializado(int& tamano){
	DibujableSerializado serializado;
	return serializado;
}