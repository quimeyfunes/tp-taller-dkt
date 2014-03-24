#include "Figura.h"


Figura::Figura()
{
	//DEFINIR LOS VALORES POR DEFECTO
}

Figura::Figura(float x, float y, short int rotacion, b2World world, bool estatico)
{
	b2BodyDef myBodyDef;
	if (estatico){
	myBodyDef.type = b2_staticBody;
	} else {
		myBodyDef.type = b2_dynamicBody;
	}
	myBodyDef.position.Set(x, y); 
	//Angulo en radianes
	myBodyDef.angle = (rotacion * DEGTORAD); 

	this->body = world.CreateBody(&myBodyDef);
	
}


b2Body* Figura::getBody()
{
	return this->body;
}


void Figura::setPosicion(float x, float y, float angulo)
{
	float radianes = (angulo * DEGTORAD);
	this->body->SetTransform( b2Vec2 (x, y), radianes);
}

b2Vec2 Figura::getPosicion()
{
	return this->body->GetPosition();
}

float Figura::getAngulo()
{
	return this->body->GetAngle();
}

Figura::~Figura(void)
{
}
