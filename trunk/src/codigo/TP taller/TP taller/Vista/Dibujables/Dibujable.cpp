#include "Dibujable.h"

Dibujable::Dibujable(){}

Dibujable::~Dibujable() 
{
}

void Dibujable::setColor(int* rgb){
}

void Dibujable::dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, float escalaZoom, int anchoPx, int altoPx){
}

string Dibujable::serializar(){
	string serializado = "";
	return serializado;
}

void Dibujable::deserealizar(string aDeserealizar){
}

bool Dibujable::getDestruido(){
	return this->destruido;
}