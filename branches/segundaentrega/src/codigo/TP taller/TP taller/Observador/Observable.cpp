#include "Observable.h"

Observable::Observable()
{
	this->listaObservadores = new list<Observador*>();
}


Observable::~Observable()
{


}

void Observable::agregarObservador(Observador* observador)
{
	this->listaObservadores->push_back(observador);
}


void Observable::notificar(){
	for (list<Observador*>::iterator it = this->listaObservadores->begin(); it != this->listaObservadores->end(); it++) {
		(*it)->actualizar(this);
	}
}