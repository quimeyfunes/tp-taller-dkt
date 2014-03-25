#include "Rectangulo.h"


Rectangulo::Rectangulo()
{
	//DETERMINAR VALORES POR DEFECTO
}

Rectangulo::Rectangulo(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa)
	: Figura(x , y , rotacion, world, estatico)
{
	b2PolygonShape rectanguloShape;
	//Divido a la mitad el ancho y alto para que las medidas sean correctas
	rectanguloShape.SetAsBox(ancho/2, alto/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rectanguloShape;
	float areaRec = ancho * alto;
	fixtureDef.density = masa/areaRec;
	this->getBody()->CreateFixture(&fixtureDef);

}


Rectangulo::~Rectangulo(void)
{
}
