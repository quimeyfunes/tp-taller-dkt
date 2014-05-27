#ifndef __DINAMITA_H__
#define __DINAMITA_H__

#include "ExplosivaPorTiempo.h"

class Dinamita: public ExplosivaPorTiempo{
public:
	Dinamita(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float ancho, float alto, float masa, int tiempoExplosion);
	~Dinamita(void);
	void disparar(bool sentido, float potencia, float angulo);
};

#endif

