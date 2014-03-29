#ifndef __FIGURA_DIBUJABLE_H__
#define __FIGURA_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Dibujable.h"
#include "../Observador.h"
#include "../Modelo/Figura.h"

using namespace std;

class FiguraDibujable : public Dibujable, public Observador
{

public:
	FiguraDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen);
	~FiguraDibujable();
	void actualizar(Observable* observable);
};

#endif