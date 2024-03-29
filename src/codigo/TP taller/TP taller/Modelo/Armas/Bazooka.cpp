#include "Bazooka.h"


Bazooka::Bazooka(void)
{
}

Bazooka::Bazooka(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, float radio)
	: ExplosivaPorContacto(x,y,rotacion,world,estatico,ancho,alto,masa, radio)
{
	this->armaTipo = BAZOOKA;
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

void Bazooka::disparar(bool sentido, float potencia, float angulo){

	Arma::disparar(sentido,potencia,angulo);
	Reproductor::getReproductor()->reproducirSonido(SOLTARDISPARO);
}

Bazooka::~Bazooka(void)
{
}
