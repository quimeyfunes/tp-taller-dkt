#ifndef __DIBUJABLE_GFX_H__
#define __DIBUJABLE_GFX_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Dibujable.h"
#include "../../Modelo/Escenario/Escenario.h"
#include "../../Observador/Observador.h"
#include "../../Modelo/Figuras/Figura.h"

using namespace std;

class DibujableGFX: public Observador, public Dibujable
{
	int color[3];
public:
	DibujableGFX();
	~DibujableGFX();
	virtual void dibujar(SDL_Renderer* renderer, int corrimiento);
	int* getColor();
	void setColor(int* rgb);
};

#endif