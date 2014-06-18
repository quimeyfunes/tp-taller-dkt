#include "DibujableGFX.h"

DibujableGFX::DibujableGFX(){
}

DibujableGFX::~DibujableGFX(){
}

void DibujableGFX::dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, int escalaZoom, int anchoPx, int altoPx){
}

int* DibujableGFX::getColor(){
	return this->color;
}
void DibujableGFX::setColor(int* rgb){
	for (int i = 0; i < 3; i++) {
		this->color[i] = rgb[i];
	}
}

string DibujableGFX::serializar(){
	string serializado = "";
	return serializado;
}

int DibujableGFX::deserealizar(string aDeserealizar){
	return 0;
}