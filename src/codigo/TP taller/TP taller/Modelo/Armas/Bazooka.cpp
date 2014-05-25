#include "Bazooka.h"


Bazooka::Bazooka(void)
{
}

Bazooka::Bazooka(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, float radio)
	: ExplosivaPorContacto(x,y,rotacion,world,estatico,ancho,alto,masa, radio)
{
	this->armaTipo = BAZOOKA;
}

void Bazooka::disparar(bool sentido, int potencia, float angulo){

	float vX = potencia*cos(angulo*DEGTORAD);
	float vY = -potencia*sin(angulo*DEGTORAD);

	if(sentido) vX *= -1;

	this->getBody()->SetLinearVelocity(b2Vec2(vX, vY));
}


Bazooka::~Bazooka(void)
{
}
