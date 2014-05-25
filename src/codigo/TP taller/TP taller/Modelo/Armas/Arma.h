#ifndef __ARMA_H__
#define __ARMA_H__

#include <iostream>
#include <string>
#include "Box2D/Box2D.h"
#include "../../Modelo/Figuras/Figura.h"
#include "../../constantes.h"
#include "../../constantesFisicas.h"
#include "../../constantesArmas.h"

class Arma: public Figura
{
protected:
	bool explota;
	float radio;
public:
	tipoArma armaTipo;
	Arma(void);
	Arma(float x, float y, short int rotacion, b2World* world, bool estatico, bool rotacionFija,float radio);
	virtual void disparar(bool sentido, int potencia, float angulo);
	virtual void explotar(bool explotar);
	bool getExplotar();
	float getRadio();
	virtual ~Arma(void);
};

#endif
