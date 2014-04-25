#include "Dibujable.h"

Dibujable::Dibujable(){}

Dibujable::~Dibujable() 
{
}

void Dibujable::setColor(int* rgb, int a){
}

void Dibujable::dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, float escalaZoom, int posZoomX, int posZoomY){
}

DibujableSerializado Dibujable::getDibujableSerializado(int& tamano){
	DibujableSerializado serializado;
	return serializado;
}