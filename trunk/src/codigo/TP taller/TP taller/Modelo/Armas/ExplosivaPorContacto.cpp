#include "ExplosivaPorContacto.h"


ExplosivaPorContacto::ExplosivaPorContacto(void)
{
}


ExplosivaPorContacto::ExplosivaPorContacto(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, float radio)
	: Arma(x , y , rotacion, world, estatico, false, radio)
{
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

void ExplosivaPorContacto::BeginContact() {
	this->explotar(true);
}

ExplosivaPorContacto::~ExplosivaPorContacto(void)
{
}
