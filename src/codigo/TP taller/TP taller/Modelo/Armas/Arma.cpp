#include "Arma.h"


Arma::Arma(void)
{
}

Arma::Arma(float x, float y, short int rotacion, b2World* world, bool estatico, bool rotacionFija){

	b2BodyDef myBodyDef;
	if (estatico){
	myBodyDef.type = b2_staticBody;
	} else {
		myBodyDef.type = b2_dynamicBody;
	}
	myBodyDef.allowSleep = false;
	myBodyDef.fixedRotation = rotacionFija;
	myBodyDef.position.Set(x, y);
	this->posicionInicial = b2Vec2(x,y);

	//AnguloInicial en GRADOS!!
	//myBodyDef.angle en RADIANES
	myBodyDef.angle = rotacion*DEGTORAD; 

	this->body = world->CreateBody(&myBodyDef);
}

b2Body* Arma::getBody()
{
	return this->body;
}


void Arma::setPosicion(float x, float y, float angulo)
{
	this->body->SetTransform( b2Vec2 (x, y), angulo);
}

b2Vec2 Arma::getPosicion()
{
	return this->body->GetPosition();
}

void Arma::disparar(bool sentido, int potencia, float angulo){
}

void Arma::explotar(){
}

Arma::~Arma(void)
{
}
