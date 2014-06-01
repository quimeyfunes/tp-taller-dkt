#include "MiniBanana.h"


MiniBanana::MiniBanana(void)
{
}

MiniBanana::MiniBanana(float x, float y, short int rotacion, b2World* world, bool estatico,float radioExplosion, float radioArma, float masa, int tiempoExplosion)
	 : ExplosivaPorContacto(x,y,rotacion,world,estatico,2*radioArma,2*radioArma,masa, radioExplosion)
{
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

void MiniBanana::disparar(bool sentido, float potencia, float angulo){

	float vX = potencia*cos(angulo*DEGTORAD);
	float vY = -potencia*sin(angulo*DEGTORAD);

	if(sentido) vX *= -1;
	this->getBody()->SetLinearVelocity(b2Vec2(vX, vY));
	this->getBody()->SetAngularVelocity(2);
	//Reproductor::getReproductor()->reproducirSonido(SOLTARGRANADA);

}


MiniBanana::~MiniBanana(void)
{
}
