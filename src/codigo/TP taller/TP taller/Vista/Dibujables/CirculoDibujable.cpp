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
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	this->posicion.x = fig->getPosicion().x * e->anchoPx / e->anchoU;
	this->posicion.y = fig->getPosicion().y * e->altoPx / e->altoU;
}

void CirculoDibujable::dibujar(SDL_Renderer* renderer) {
	filledEllipseRGBA(renderer,this->posicion.x,this->posicion.y,this->radioHorizontal,this->radioVertical,this->getColor()[0],this->getColor()[1],this->getColor()[2],255);
}
