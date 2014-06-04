#ifndef __EXPLOSIVA_POR_CONTACTO_H__
#define __EXPLOSIVA_POR_CONTACTO_H__

#include "Arma.h"
#include "time.h"

class ExplosivaPorTiempo: public Arma
{
protected:
	int tiempoExplosion;
	int tiempoInicial;
	int contactos;
public:
	ExplosivaPorTiempo(void);
	ExplosivaPorTiempo(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float masa, int tiempoExplosion);
	~ExplosivaPorTiempo(void);
	void BeginContact();
	void EndContact();
	bool getExplotar();
	virtual void disparar(bool sentido, float potencia, float angulo);
	
};

#endif