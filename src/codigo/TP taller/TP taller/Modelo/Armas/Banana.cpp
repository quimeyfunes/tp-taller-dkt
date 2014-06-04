#include "Banana.h"


Banana::Banana(void)
{
}

Banana::Banana(float x, float y, short int rotacion, b2World* world, bool estatico,float radioExplosion, float radioArma, float masa, int tiempoExplosion)
	: ExplosivaPorTiempo(x,y,rotacion, world, estatico, radioExplosion, masa, tiempoExplosion)
{
	this->contactos=0;
	this->armaTipo = BANANA;
	b2CircleShape circleShape;
	circleShape.m_radius = radioArma;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape; 
	float areaCirculo = b2_pi * radioArma * radioArma;
	fixtureDef.density = masa/areaCirculo;
	fixtureDef.restitution = 0.7f;
	fixtureDef.friction = friccion;
	fixtureDef.userData = this;
	this->getBody()->CreateFixture(&fixtureDef);
}

void Banana::disparar(bool sentido, float potencia, float angulo){

	ExplosivaPorTiempo::disparar(sentido,potencia,angulo);
	
	//Reproductor::getReproductor()->reproducirSonido(SOLTARGRANADA);

}

void Banana::BeginContact(){

	Reproductor::getReproductor()->reproducirSonido(IMPACTOBANANA);
}



Banana::~Banana(void)
{
}

