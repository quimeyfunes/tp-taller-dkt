#ifndef __FIGURA_H__
#define __FIGURA_H__

#include <iostream>
#include <string>
#include "Box2D/Box2D.h"
#include "../../Observador/Observable.h"
#include "../../constantes.h"


using namespace std;

class Figura : public Observable
{
private:

	b2Body* body;
	b2Vec2 posicionInicial;
	float anguloInicial;
	int tipo;
	bool movimientoDer;
	bool movimientoIzq;

public:

	Figura();
	Figura(float x, float y, short int rotacion, b2World* world, bool estatico, bool rotacionFija);
	void setPosicion(float x, float y, float angulo);
	b2Body* getBody();
	b2Vec2 getPosicion();
	virtual void reiniciar();
	void quieto();
	bool seMueveALaDer();
	void setMovimientoDer(bool movDer);
	bool seMueveALaIzq();
	void setMovimientoIzq(bool movIzq);
	float getAngulo();
	void setTipo(int tipo);
	int getTipo();
	virtual bool meClickeo(float x, float y);
	virtual void simularAgua(int nivelAgua);
	~Figura();
};

#endif