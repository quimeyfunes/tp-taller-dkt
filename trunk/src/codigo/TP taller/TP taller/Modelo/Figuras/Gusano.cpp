#include "Gusano.h"



Gusano::Gusano()
{
}


Gusano::Gusano(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa)
	: Figura(x , y , rotacion, world, estatico, true)
{
	
	b2PolygonShape rectanguloShape;
	//Divido a la mitad el ancho y alto para que las medidas sean correctas
	rectanguloShape.SetAsBox(ancho/2, alto/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rectanguloShape;
	float areaRec = ancho * alto;
	fixtureDef.density = masa/areaRec;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	this->getBody()->CreateFixture(&fixtureDef);
	rectanguloShape.SetAsBox(ancho/2,2,b2Vec2(0,alto/2),0);
	b2FixtureDef fixtureDefSensor;
	fixtureDefSensor.isSensor = true;
	fixtureDefSensor.shape = &rectanguloShape;
	fixtureDefSensor.userData = this;
	this->getBody()->CreateFixture(&fixtureDefSensor);
	this->numContactos = 0;

	//this->meClickearon = false;
	for(int i=0; i < MAXIMOS_CLIENTES; i++){
		this->meClickearon.push_back(false);
	}
	
	this->congelado = false;
}

Gusano::~Gusano(void)
{
}

bool Gusano::meClickeo(float x,float y) {
	for (b2Fixture* f = this->getBody()->GetFixtureList(); f; f = f->GetNext())
	{
		  if (f->GetShape()->TestPoint(this->getBody()->GetTransform(),b2Vec2(x,y))) {
			  return true;
		  }
	}
	return false;
}

void Gusano::agregarContacto(){
	this->numContactos++;
}

void Gusano::sacarContacto(){
	this->numContactos--;
}

bool Gusano::puedeSaltar(){
	if (this->numContactos > 4) return true;
	return false;
}

void Gusano::simularAgua(int nivelAgua){
	if (this->getPosicion().y - altoGusano / 2 > nivelAgua){
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
		this->setMuerto(true);
		//this->getBody()->SetType(b2_staticBody);
	}
}

void Gusano::reiniciar(){
	Figura::reiniciar();
	this->getBody()->SetType(b2_dynamicBody);
	this->setMuerto(false);
}

bool Gusano::getMeClickearon(int cliente) {
	return this->meClickearon[cliente];
}

void Gusano::setMeClickearon(bool meClickearon, int cliente){
	this->meClickearon[cliente] = meClickearon;
}

bool Gusano::getCongelado() {
	return this->congelado;
}

void Gusano::setCongelado(bool congelado){
	this->congelado = congelado;
}