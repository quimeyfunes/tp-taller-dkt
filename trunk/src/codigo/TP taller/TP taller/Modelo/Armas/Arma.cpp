#include "Arma.h"


Arma::Arma(void)
{
}

Arma::Arma(float x, float y, short int rotacion, b2World* world, bool estatico, bool rotacionFija, float radio) : Figura(x,y,rotacion,world,estatico,rotacionFija)
{
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
Arma::~Arma(void)
{
}
