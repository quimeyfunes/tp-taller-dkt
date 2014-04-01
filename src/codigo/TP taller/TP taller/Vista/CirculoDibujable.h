#ifndef __CIRCULO_DIBUJABLE_H__
#define __CIRCULO_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "DibujableGFX.h"
#include "../Observador.h"
#include "../Modelo/Figura.h"

using namespace std;

class CirculoDibujable : public Observador, public DibujableGFX
{
	b2Vec2 posicion;
	int radioHorizontal;
	int radioVertical;
public:
	CirculoDibujable(int x, int y,int radioHorizontal,int radioVertical);
	~CirculoDibujable();
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer);
};

#endif