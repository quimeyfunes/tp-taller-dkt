#include "Bazooka.h"


Bazooka::Bazooka(void)
{
}

Bazooka::Bazooka(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa)
	: ExplosivaPorContacto(x,y,rotacion,world,estatico,ancho,alto,masa)
{
}

void Bazooka::disparar(int potencia){

	this->getBody()->SetLinearVelocity(b2Vec2(potencia,0));

}

Bazooka::~Bazooka(void)
{
}
