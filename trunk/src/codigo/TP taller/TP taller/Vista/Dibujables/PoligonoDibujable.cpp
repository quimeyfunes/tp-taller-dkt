#include "PoligonoDibujable.h"


PoligonoDibujable::PoligonoDibujable(int* posicionesX, int* posicionesY, short int n)
{
	this->posicionesX = new b2Vec2[n];
	this->posicionesY = new b2Vec2[n];
};

PoligonoDibujable::~PoligonoDibujable()
{
	delete[] this->posicionesX;
	delete[] this->posicionesY;
}



void PoligonoDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	this->angulo = fig->getAngulo();
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	//Actualizar cada posicion de los vectores...
	/*this->posicion.x = fig->getPosicion().x * e->anchoPx / e->anchoU;
	this->posicion.y = fig->getPosicion().y * e->altoPx / e->altoU;*/
}

void PoligonoDibujable::dibujar(SDL_Renderer* renderer){

}