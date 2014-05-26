#ifndef __GRANADA_H__
#define __GRANADA_H__

#include "ExplosivaPorTiempo.h"

class Granada: public ExplosivaPorTiempo
{
public:
	Granada(void);
	Granada(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa);
	~Granada(void);
	void disparar(bool sentido, int potencia, float angulo);
};

#endif