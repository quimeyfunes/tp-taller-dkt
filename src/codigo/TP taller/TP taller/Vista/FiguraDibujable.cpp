#include "FiguraDibujable.h"


FiguraDibujable::FiguraDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen) : Dibujable(renderer, rect, pathImagen)
{
};

FiguraDibujable::~FiguraDibujable() {
}

void FiguraDibujable::setColor(int* rgb){
	SDL_SetTextureColorMod(this->getImagen(),rgb[0],rgb[1],rgb[2]);
}

void FiguraDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	this->setAngulo(fig->getAngulo() * RADTODEG);
	SDL_Rect recAux = this->getRect();
	recAux.x = fig->getPosicion().x;
	recAux.y = fig->getPosicion().y;
	this->setRect(recAux);
}
