#include "Dinamita.h"


Dinamita::Dinamita(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float ancho, float alto, float masa, int tiempoExplosion): ExplosivaPorTiempo(x,y,rotacion,world,estatico,radioExplosion, masa, tiempoExplosion)
{
	this->armaTipo = DINAMITA;
	b2PolygonShape rectanguloShape;
	//Divido a la mitad el ancho y alto para que las medidas sean correctas
	rectanguloShape.SetAsBox(ancho/2, alto/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rectanguloShape;
	float areaRec = ancho * alto;
	fixtureDef.density = masa/areaRec;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	fixtureDef.userData = this;
	this->getBody()->CreateFixture(&fixtureDef);
}

void Dinamita::disparar(bool sentido, float potencia, float angulo){

	//float vX = 1;
	//if(sentido) vX *= -1;
	//this->getBody()->SetLinearVelocity(b2Vec2(vX, 0));
}

Dinamita::~Dinamita(void)
{
}
