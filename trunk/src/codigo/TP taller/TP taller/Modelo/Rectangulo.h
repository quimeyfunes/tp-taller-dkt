#ifndef __RECTANGULO_H__
#define __RECTANGULO_H__

#include "Figura.h"

class Rectangulo: public Figura
{
public:
	Rectangulo();
	Rectangulo(float x, float y, short int rotacion, b2World world, bool estatico, float ancho, float alto, float masa);
	~Rectangulo(void);
};

#endif