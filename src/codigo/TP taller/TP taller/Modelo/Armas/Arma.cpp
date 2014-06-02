#include "Arma.h"


Arma::Arma(void)
{
}

Arma::Arma(float x, float y, short int rotacion, b2World* world, bool estatico, bool rotacionFija, float radio) : Figura(x,y,rotacion,world,estatico,rotacionFija)
{
	this->enElAgua = false;
	this->radio = radio;
	this->explota = false;
}

void Arma::disparar(bool sentido, float potencia, float angulo){
}

void Arma::explotar(bool explotar){
	this->explota = explotar;
}

bool Arma::getExplotar(){
	return this->explota;
}

float Arma::getRadio(){
	return this->radio;
}

tipoArma Arma::getArmaTipo(){
	return this->armaTipo;
}

void Arma::simularAgua(int nivelAgua){

	if (this->getPosicion().y > nivelAgua){
		if(!enElAgua) Reproductor::getReproductor()->reproducirSonido(AGUA);
		enElAgua=true;
		b2Vec2 velocidad = this->getBody()->GetLinearVelocity();
		float velocidadY = velocidad.y;
		if (velocidadY > velocidadAgua) {
			if (velocidadY * desaceleracionAgua < velocidadAgua)  {
				velocidadY = velocidadAgua;
			} else {
				velocidadY = velocidadY * desaceleracionAgua;
			}
		}
		this->getBody()->SetLinearVelocity(b2Vec2(0 ,velocidadY));
	}
}

Arma::~Arma(void)
{
}