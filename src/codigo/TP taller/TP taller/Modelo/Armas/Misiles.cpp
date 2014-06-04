#include "Misiles.h"


Misiles::Misiles(void)
{
}

Misiles::Misiles(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, float radio)
	: ExplosivaPorContacto(x,y,rotacion,world,estatico,ancho,alto,masa, radio)
{
	this->pedidoDeLanzamiento = false;
	this->armaTipo = MISILES;
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

void Misiles::disparar(bool sentido, float potencia, float angulo){
	//si hay tiempo agrego lo de elegir el sentido
	float vX = 0;
	float vY = 30;

	this->getBody()->SetLinearVelocity(b2Vec2(vX, vY));
	
}




Misiles::~Misiles(void)
{
}

void Misiles::setPedidoDeLanzamiento(bool pedido){
	this->pedidoDeLanzamiento = pedido;

}
bool Misiles::getPedidoDeLanzamiento(){
	return this->pedidoDeLanzamiento;
}