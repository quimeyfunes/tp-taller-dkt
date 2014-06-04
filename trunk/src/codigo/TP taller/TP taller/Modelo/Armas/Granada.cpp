#include "Granada.h"


Granada::Granada(void)
{
}

Granada::Granada(float x, float y, short int rotacion, b2World* world, bool estatico,float radioExplosion, float radioArma, float masa, int tiempoExplosion)
	: ExplosivaPorTiempo(x,y,rotacion, world, estatico, radioExplosion, masa, tiempoExplosion)
{
	this->contactos=0;
	this->armaTipo = GRANADA;
	b2CircleShape circleShape;
	circleShape.m_radius = radioArma;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape; 
	float areaCirculo = b2_pi * radioArma * radioArma;
	fixtureDef.density = masa/areaCirculo;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	fixtureDef.userData = this;
	this->getBody()->CreateFixture(&fixtureDef);
}

Granada::~Granada(void)
{
}

void Granada::disparar(bool sentido, float potencia, float angulo){

	ExplosivaPorTiempo::disparar(sentido,potencia,angulo);
	Reproductor::getReproductor()->reproducirSonido(SOLTARGRANADA);

}

void Granada::BeginContact(){
	Reproductor::getReproductor()->reproducirSonido(IMPACTOGRANADA);
}