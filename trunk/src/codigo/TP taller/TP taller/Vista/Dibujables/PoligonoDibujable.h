#ifndef __POLIGONO_DIBUJABLE_H__
#define __POLIGONO_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "DibujableGFX.h"
#include "../../Observador/Observador.h"
#include "../../Modelo/Figuras/Figura.h"

using namespace std;

class PoligonoDibujable : public Observador, public DibujableGFX
{
	short int* posicionesX;
	short int* posicionesY;
	float escalaAncho;
	float escalaAlto;
	short int n;
public:
	PoligonoDibujable(short int n, float escalaAncho, float escalaAlto);
	PoligonoDibujable();
	~PoligonoDibujable();
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	virtual string serializar();
	virtual int deserealizar(string aDeserealizar);
};

#endif