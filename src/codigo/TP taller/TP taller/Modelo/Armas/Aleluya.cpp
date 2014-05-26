#include "Aleluya.h"


Aleluya::Aleluya(void)
{
}

Aleluya::Aleluya(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa)
	: ExplosivaPorTiempo(x,y,rotacion, world, estatico, radioExplosion, radioArma, masa)
{
	this->armaTipo = ALELUYA;
}

Aleluya::~Aleluya(void)
{
}


void Aleluya::disparar(bool sentido, int potencia, float angulo){

	float vX = potencia*cos(angulo*DEGTORAD);
	float vY = -potencia*sin(angulo*DEGTORAD);

	if(sentido) vX *= -1;

	this->getBody()->SetLinearVelocity(b2Vec2(vX, vY));

}