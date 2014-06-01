#ifndef __BANANA_H__
#define __BANANA_H__

#include "ExplosivaPorTiempo.h"

class Banana: public ExplosivaPorTiempo 
{

private:
public:
	Banana(void);
	Banana(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa, int tiempoExplosion);
	~Banana(void);
	void disparar(bool sentido, float potencia, float angulo);
	void BeginContact();
};

#endif