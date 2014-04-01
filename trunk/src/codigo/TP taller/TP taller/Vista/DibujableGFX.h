#ifndef __DIBUJABLE_GFX_H__
#define __DIBUJABLE_GFX_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Dibujable.h"
#include "../Observador.h"
#include "../Modelo/Figura.h"

using namespace std;

class DibujableGFX: public Observador, public Dibujable
{
	int color[3];
public:
	DibujableGFX();
	~DibujableGFX();
	virtual void dibujar(SDL_Renderer* renderer);
	int* getColor();
	void setColor(int* rgb);
};

#endif