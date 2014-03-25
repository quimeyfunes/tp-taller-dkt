#include "Circulo.h"


Circulo::Circulo(void)
{
	//DETERMINAR VALORS POR DEFECTO
}

Circulo::Circulo(float x, float y, short int rotacion, b2World* world, bool estatico, float radio, float masa)
	: Figura(x, y, rotacion, world, estatico) //Llamo al constructor del padre
{
	b2CircleShape circleShape;
	circleShape.m_radius = radio;

	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &circleShape; 
	float areaCirculo = b2_pi * radio * radio;
	myFixtureDef.density = masa/areaCirculo;
	this->getBody()->CreateFixture(&myFixtureDef);
}

Circulo::~Circulo(void)
{
}
