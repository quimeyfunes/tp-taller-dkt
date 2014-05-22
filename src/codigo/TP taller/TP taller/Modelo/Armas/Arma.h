#ifndef __ARMA_H__
#define __ARMA_H__

#include <iostream>
#include <string>
#include "Box2D/Box2D.h"
#include "../../Observador/Observable.h"
#include "../../constantes.h"
#include "../../constantesFisicas.h"

class Arma: public Observable
{
private:
	b2Body* body;
	b2Vec2 posicionInicial;
	float anguloInicial;
public:
	Arma(void);
	Arma(float x, float y, short int rotacion, b2World* world, bool estatico, bool rotacionFija);
	void setPosicion(float x, float y, float angulo);
	b2Body* getBody();
	b2Vec2 getPosicion();
	virtual void disparar(int potencia);
	virtual void explotar();
	~Arma(void);
};

#endif
