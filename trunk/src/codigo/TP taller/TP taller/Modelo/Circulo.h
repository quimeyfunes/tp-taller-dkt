#ifndef __CIRCULO_H__
#define __CIRCULO_H__

#include "Figura.h"

class Circulo: public Figura
{
public:
	Circulo();
	Circulo(float x, float y, short int rotacion, b2World world, bool estatico, float radio, float masa);
	~Circulo(void);
};

#endif