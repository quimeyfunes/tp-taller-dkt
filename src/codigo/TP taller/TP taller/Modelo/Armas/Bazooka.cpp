#include "Bazooka.h"


Bazooka::Bazooka(void)
{
}

Bazooka::Bazooka(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa)
	: ExplosivaPorContacto(x,y,rotacion,world,estatico,ancho,alto,masa)
{
	this->armaTipo = BAZOOKA;
}

void Bazooka::disparar(){
	this->disparada=true;
	//this->getBody()->SetLinearVelocity(b2Vec2(potencia*cos(this->anguloInicial*DEGTORAD), potencia*sin(this->anguloInicial*DEGTORAD)));
	this->getBody()->SetLinearVelocity(b2Vec2(-100,-50));
	this->potencia = 0;


}

Bazooka::~Bazooka(void)
{
}
