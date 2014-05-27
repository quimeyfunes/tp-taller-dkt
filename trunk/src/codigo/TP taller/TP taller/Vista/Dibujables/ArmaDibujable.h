#ifndef __ARMA_DIBUJABLE_H__
#define __ARMA_DIBUJABLE_H__

#include "DibujableTextura.h"
#include "../../Modelo/Armas/Bazooka.h"
#include "../../Observador/Observador.h"
#include <string>

class ArmaDibujable: public DibujableTextura, public Observador
{
public:
	ArmaDibujable(void);
	ArmaDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	virtual void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	~ArmaDibujable(void);
};

#endif
