#include "FiguraDibujable.h"


FiguraDibujable::FiguraDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen) : Dibujable(renderer, rect, pathImagen)
{
};

FiguraDibujable::~FiguraDibujable() {
}



void FiguraDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	this->setAngulo(fig->getAngulo() * RADTODEG);
	SDL_Rect recAux = this->getRect();
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	//cout<<fig->getPosicion().y<<endl;
	recAux.x = fig->getPosicion().x * e->anchoPx / e->anchoU - recAux.w/2;
	recAux.y = fig->getPosicion().y * e->altoPx / e->altoU - recAux.h/2;
	this->setRect(recAux);
}
