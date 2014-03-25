#pragma once

#ifndef __OBSERVADOR_H__
#define __OBSERVADOR_H__

//#include "Observable.h"

class Observable;

class Observador

{


public:
	Observador();
	~Observador();
	virtual void actualizar(Observable* observable);

};

#endif