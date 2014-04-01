#ifndef __POLIGONO_DIBUJABLE_H__
#define __POLIGONO_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "DibujableGFX.h"
#include "../Observador.h"
#include "../Modelo/Figura.h"

using namespace std;

class PoligonoDibujable : public Observador, public DibujableGFX
{
	b2Vec2* posicionesX;
	b2Vec2* posicionesY;
	short int escala;
	short int n;
	float angulo;
public:
	PoligonoDibujable(int* posicionesX, int* posicionesY, short int n);
	~PoligonoDibujable();
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer);
};

#endif