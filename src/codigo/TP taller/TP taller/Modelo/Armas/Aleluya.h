#ifndef __ALELUYA_H__
#define __ALELUYA_H__

#include "ExplosivaPorTiempo.h"

class Aleluya:public ExplosivaPorTiempo
{
public:
	Aleluya(void);
	Aleluya(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa);
	~Aleluya(void);
	void disparar(bool sentido, float potencia, float angulo);
};

#endif