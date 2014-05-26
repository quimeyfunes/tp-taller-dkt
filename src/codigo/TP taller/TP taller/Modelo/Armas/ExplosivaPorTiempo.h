#ifndef __EXPLOSIVA_POR_CONTACTO_H__
#define __EXPLOSIVA_POR_CONTACTO_H__

#include "Arma.h"
#include "time.h"

class ExplosivaPorTiempo: public Arma
{
private:
	int timer;
public:
	ExplosivaPorTiempo(void);
	ExplosivaPorTiempo(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa);
	~ExplosivaPorTiempo(void);
	void setTimer(int timer);
	int getTimer();
	
};

#endif