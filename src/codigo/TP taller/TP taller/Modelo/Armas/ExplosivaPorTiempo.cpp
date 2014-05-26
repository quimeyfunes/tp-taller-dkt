#include "ExplosivaPorTiempo.h"


ExplosivaPorTiempo::ExplosivaPorTiempo(void)
{
}

ExplosivaPorTiempo::ExplosivaPorTiempo(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa)
	: Arma(x,y, rotacion, world, estatico, false, radioExplosion)
{
	b2CircleShape circleShape;
	circleShape.m_radius = radioArma;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape; 
	float areaCirculo = b2_pi * radioArma * radioArma;
	fixtureDef.density = masa/areaCirculo;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	this->getBody()->CreateFixture(&fixtureDef);
	this->timer = time(NULL);

}

ExplosivaPorTiempo::~ExplosivaPorTiempo(void)
{
}

void ExplosivaPorTiempo::setTimer(int timer){
	this->timer = timer;
}

int ExplosivaPorTiempo::getTimer(){
	return this->timer;
}