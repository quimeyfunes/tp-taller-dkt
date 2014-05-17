#include "Circulo.h"


Circulo::Circulo(void)
{
	//DETERMINAR VALORS POR DEFECTO
}

Circulo::Circulo(float x, float y, short int rotacion, b2World* world, bool estatico, float radio, float masa)
	: Figura(x, y, rotacion, world, estatico, false) //Llamo al constructor del padre
{
	this->escala;
	b2CircleShape circleShape;
	circleShape.m_radius = radio;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape; 
	float areaCirculo = b2_pi * radio * radio;
	fixtureDef.density = masa/areaCirculo;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	this->getBody()->CreateFixture(&fixtureDef);
}

Circulo::~Circulo(void)
{
}
