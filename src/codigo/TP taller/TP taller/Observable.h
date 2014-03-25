#pragma once

#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__

#include <list>
#include "Observador.h"


using namespace std;

class Observable

{
	list<Observador*>* listaObservadores;

public:
	Observable();
	~Observable();
	void agregarObservador(Observador* observador);
	void notificar();

};

#endif
