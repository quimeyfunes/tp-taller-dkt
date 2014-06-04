#ifndef __MISILES_H__
#define __MISILES_H__

#include "ExplosivaPorContacto.h"


class Misiles: public ExplosivaPorContacto
{

	private:
		bool pedidoDeLanzamiento;

public:
	Misiles(void);
	Misiles(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, float radio);
	void disparar(bool sentido, float potencia, float angulo);
	void notificar();
	void setPedidoDeLanzamiento(bool pedido);
	bool getPedidoDeLanzamiento();

	~Misiles(void);
};

#endif