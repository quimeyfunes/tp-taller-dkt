#ifndef __BAZOOKA_H__
#define __BAZOOKA_H__

#include "ExplosivaPorContacto.h"

class Bazooka: public ExplosivaPorContacto
{
public:
	Bazooka(void);
	Bazooka(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa);
	void disparar();
	~Bazooka(void);
};

#endif