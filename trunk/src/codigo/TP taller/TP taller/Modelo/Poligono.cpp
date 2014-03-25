#include "Poligono.h"


Poligono::Poligono(void)
{
}

Poligono::Poligono(float x, float y, short int rotacion, b2World* world, bool estatico, float escala, float masa, short int n)
	: Figura(x, y, rotacion, world, estatico)
{
	b2PolygonShape shape;
	b2Vec2* vertices = new b2Vec2[n];
	
	for (int i = 0; i < n; i++) {
		float angulo = (2 * b2_pi / n);
		float corrimiento = ((360-90*(n-2))/n) * 2 * b2_pi / 360; 
		vertices[i].Set(escala * cos(angulo*(i) + corrimiento), escala * sin(angulo*(i) + corrimiento)); 
	}

	shape.Set(vertices, n);
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &shape; 
	double numero = 360/n;
	float areaPoligono = 0.5 * n * escala * escala * sin(numero * DEGTORAD);
	myFixtureDef.density = masa/areaPoligono;
	this->getBody()->CreateFixture(&myFixtureDef);
	this->n = n;
}


Poligono::~Poligono(void)
{
}
