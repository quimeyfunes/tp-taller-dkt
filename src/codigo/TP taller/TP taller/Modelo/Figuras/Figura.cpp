#include "Figura.h"


Figura::Figura()
{
	//DEFINIR LOS VALORES POR DEFECTO
}

Figura::Figura(float x, float y, short int rotacion, b2World* world, bool estatico, bool rotacionFija)
{
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
	this->anguloInicial = rotacion * DEGTORAD;
	//Angulo en radianes
	myBodyDef.angle = this->anguloInicial; 

	this->body = world->CreateBody(&myBodyDef);
	this->movimientoDer = false;
	this->movimientoIzq = false;
	this->muerto = false;
}

void Figura::quieto(){

	this->setMovimientoIzq(false);
	this->setMovimientoDer(false);
}

b2Body* Figura::getBody()
{
	return this->body;
}


void Figura::setPosicion(float x, float y, float angulo)
{
	this->body->SetTransform( b2Vec2 (x, y), angulo);
}

b2Vec2 Figura::getPosicion()
{
	return this->body->GetPosition();
}

float Figura::getAngulo()
{
	return this->body->GetAngle();
}

void Figura::reiniciar(){
	//this->setPosicion(this->posicionInicial.x,this->posicionInicial.y,this->anguloInicial);
	//this->body->SetAngularVelocity(0);
	//this->body->SetLinearVelocity(b2Vec2(0,0));
	this->body->SetAwake(true);
	this->setMovimientoIzq(false);
	this->setMovimientoDer(false);
}

Figura::~Figura(void)
{
}

bool Figura::seMueveALaDer(){
	return this->movimientoDer;
}

void Figura::setMovimientoDer(bool movDer){
	this->movimientoDer = movDer;
}

bool Figura::seMueveALaIzq(){
	return this->movimientoIzq;
}

void Figura::setMovimientoIzq(bool movIzq){
	this->movimientoIzq = movIzq;
}

bool Figura::meClickeo(float x,float y) {
	return false;
}

void Figura::simularAgua(int nivelAgua){

	if (this->getPosicion().y > nivelAgua){
		b2Vec2 velocidad = this->getBody()->GetLinearVelocity();
		float velocidadY = velocidad.y;
		if (velocidadY > velocidadAgua) {
			if (velocidadY * desaceleracionAgua < velocidadAgua)  {
				velocidadY = velocidadAgua;
			} else {
				velocidadY = velocidadY * desaceleracionAgua;
			}
		}
		this->getBody()->SetLinearVelocity(b2Vec2(velocidad.x * desaceleracionAgua,velocidadY));
	}
}

bool Figura::estaMuerto(){
	return this->muerto;
}

void Figura::setMuerto(bool estado){
	this->muerto = estado;
}

void Figura::BeginContact() {
}

void Figura::EndContact(){
}

void Figura::PostSolve(float impulso){
}

void Figura::explotar(float fuerza) {
}