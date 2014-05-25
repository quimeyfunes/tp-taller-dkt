#include "Gusano.h"



Gusano::Gusano()
{
}


Gusano::Gusano(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, int maximosCLientes)
	: Figura(x , y , rotacion, world, estatico, true)
{
	this->armaActual.potenciaDisparo = 0;
	this->armaActual.anguloDisparo=0;
	this->armaActual.armaSeleccionada = NULL;
	this->armaActual.armaTipo = NINGUNA;
	this->armaActual.sentidoDisparo = true;	//true = izq
	

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
	rectanguloShape.SetAsBox(ancho/2,alto/8,b2Vec2(0,alto/2),0);
	b2FixtureDef fixtureDefSensor;
	fixtureDefSensor.isSensor = true;
	fixtureDefSensor.shape = &rectanguloShape;
	fixtureDefSensor.userData = this;
	this->getBody()->CreateFixture(&fixtureDefSensor);
	this->numContactos = 0;
	this->maximosCLientes = maximosCLientes;

	this->meClick = false;
	for(int i=0; i < this->maximosCLientes; i++){
		this->meClickearon.push_back(false);
	}
	this->congelado = false;
	//this->armaSeleccionada = new Bazooka(x - (ancho/2), y, rotacion, world, estatico, ancho /4 ,alto /4, masa /2 );
}

Gusano::~Gusano(void)
{
	//delete this->armaSeleccionada;
}

bool Gusano::meClickeo(float x,float y) {
	for (b2Fixture* f = this->getBody()->GetFixtureList(); f; f = f->GetNext())
	{
		if ((f->GetShape()->TestPoint(this->getBody()->GetTransform(),b2Vec2(x,y))) && !(this->estaMuerto())) {
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
	if ((this->numContactos > 0) && !(this->estaMuerto())) return true;
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

void Gusano::setMeClickearon(bool d){
	this->meClick = d;
}

Arma* Gusano::getArmaSeleccionada(){
	return this->armaActual.armaSeleccionada;
}

void Gusano::setArma(Arma* nueva){
	if(this->armaActual.armaSeleccionada != NULL) delete this->armaActual.armaSeleccionada;
	this->armaActual.armaSeleccionada = nueva;
	this->armaActual.armaTipo = nueva->armaTipo;
}

bool Gusano::tieneUnArma(){
	return (this->armaActual.armaTipo == NINGUNA)? false:true;
}

tipoArma Gusano::getTipoArma(){
	return this->armaActual.armaTipo;
}