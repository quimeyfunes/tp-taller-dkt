#ifndef __POLIGONO_H__
#define __POLIGONO_H__

#include "Figura.h"

class Poligono: public Figura
{
public:
	Poligono();
	Poligono(float x, float y, short int rotacion, b2World world, bool estatico, float escala, float masa);
	~Poligono(void);
};

#endif