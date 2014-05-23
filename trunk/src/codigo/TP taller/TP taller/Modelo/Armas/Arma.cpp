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
	this->anguloInicial = rotacion;
	//AnguloInicial en GRADOS!!
	//myBodyDef.angle en RADIANES
	myBodyDef.angle = this->anguloInicial*DEGTORAD; 

	this->disparada=false;
	this->potencia=0;
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

void Arma::disparar(){
}

void Arma::explotar(){
}

void Arma::aumentarPotencia(){
	if(this->potencia < POTENCIA_MAXIMA_DISPARO) this->potencia+=0.5f;
}

float Arma::getPotencia(){
	return this->potencia;
}

void Arma::aumentarAnguloDisparo(){
	if(this->anguloInicial < 90) this->anguloInicial+=1.0f;
}

void Arma::disminuirAnguloDisparo(){
	if(this->anguloInicial > -90 ) this->anguloInicial-=1.0f;
}

float Arma::getAnguloDisparo(){
	return this->anguloInicial;
}

bool Arma::fueDisparada(){
	return this->disparada;
}

Arma::~Arma(void)
{
}
