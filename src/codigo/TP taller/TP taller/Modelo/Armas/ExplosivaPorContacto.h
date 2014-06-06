#ifndef __EXPLOSVIA_POR_CONTACTO_H__
#define __EXPLOSVIA_POR_CONTACTO_H__

#include "Arma.h"

class ExplosivaPorContacto: public Arma
{
public:
	ExplosivaPorContacto(void);
	ExplosivaPorContacto(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa,float radio);
	void BeginContact();
	virtual void actualizar(int anchoU, int altoU);
	virtual ~ExplosivaPorContacto(void);
};

#endif
