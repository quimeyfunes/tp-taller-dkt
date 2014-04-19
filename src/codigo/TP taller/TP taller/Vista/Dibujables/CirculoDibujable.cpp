#include "CirculoDibujable.h"


CirculoDibujable::CirculoDibujable(int x,int y, int radioHorizontal,int radioVertical)
{
	this->posicion.x = x;
	this->posicion.y = y;
	this->radioHorizontal = radioHorizontal;
	this->radioVertical = radioVertical;
};

CirculoDibujable::~CirculoDibujable() {
}



void CirculoDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	this->posicion.x = fig->getPosicion().x * Escenario::getRelacionAncho();
	this->posicion.y = fig->getPosicion().y * Escenario::getRelacionAlto();
}

void CirculoDibujable::dibujar(SDL_Renderer* renderer,int corrimiento) {
	filledEllipseRGBA(renderer,this->posicion.x + corrimiento,this->posicion.y,this->radioHorizontal,this->radioVertical,this->getColor()[0],this->getColor()[1],this->getColor()[2],255);
}
