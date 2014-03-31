#ifndef __FIGURA_H__
#define __FIGURA_H__

#include <iostream>
#include <string>
#include "Box2D/Box2D.h"
#include "../Observable.h"
#include "../constantes.h"


using namespace std;

class Figura : public Observable
{
private:

	b2Body* body;
	b2Vec2 posicionInicial;
	float anguloInicial;

public:

	Figura();
	Figura(float x, float y, short int rotacion, b2World* world, bool estatico);
	void setPosicion(float x, float y, float angulo);
	b2Body* getBody();
	b2Vec2 getPosicion();
	void reiniciar();
	float getAngulo();
	~Figura();
};

#endif