#ifndef __MINI_BANANA_H__
#define __MINI_BANANA_H__

#include "ExplosivaPorContacto.h"

class MiniBanana: public ExplosivaPorContacto
{
public:
	MiniBanana(void);
	MiniBanana(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa, int tiempoExplosion);
	void disparar(bool sentido, float potencia, float angulo);
	~MiniBanana(void);
};

#endif