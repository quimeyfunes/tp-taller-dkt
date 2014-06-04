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
	
	if (angulo < 0) {
		angulo += 360;
	} else {
		if (angulo > 360) {
			angulo -= 360;
		}
	}
	cout<<angulo<<endl;
	float vX = potencia*coseno[(int)angulo];//os(angulo*DEGTORAD);
	float vY = -potencia*seno[(int)angulo];//in(angulo*DEGTORAD);

	if(sentido) vX *= -1;

	this->getBody()->SetLinearVelocity(b2Vec2(vX, vY));

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