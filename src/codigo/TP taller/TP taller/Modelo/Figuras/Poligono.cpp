#include "Poligono.h"


Poligono::Poligono(void)
{
}

Poligono::Poligono(float x, float y, short int rotacion, b2World* world, bool estatico, float escala, float masa, short int n)
	: Figura(x, y, rotacion, world, estatico, false)
{
	b2PolygonShape shape;
	b2Vec2* vertices = new b2Vec2[n];
	this->escala = escala;
	for (int i = 0; i < n; i++) {
		float angulo = 360 / n;
		float corrimiento = ((360-90*(n-2))/n);// * 2 * b2_pi / 360; 
		vertices[i].Set(escala * coseno[(int) (angulo * i + corrimiento)], escala * seno[(int)(angulo * i + corrimiento)]); 
		//vertices[i].Set(escala * os(angulo*(i) + corrimiento), escala * in(angulo*(i) + corrimiento)); 
	}

	shape.Set(vertices, n);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape; 
	float areaPoligono = 0.5 * n * escala * escala * seno[(int) 360/n];//in(360/n * DEGTORAD);
	fixtureDef.density = masa/areaPoligono;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	this->getBody()->CreateFixture(&fixtureDef);
	this->n = n;
	
}


short int Poligono::getCantidadDeLados(){
	return this->n;
}

Poligono::~Poligono(void)
{
}
