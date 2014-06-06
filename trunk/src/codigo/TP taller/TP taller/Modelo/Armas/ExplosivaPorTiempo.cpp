#include "ExplosivaPorTiempo.h"


ExplosivaPorTiempo::ExplosivaPorTiempo(void)
{
}

ExplosivaPorTiempo::ExplosivaPorTiempo(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float masa, int tiempoExplosion)	: Arma(x,y, rotacion, world, estatico, false, radioExplosion)
{
	this->tiempoExplosion = tiempoExplosion;
	this->tiempoInicial = time(NULL);

}

void ExplosivaPorTiempo::actualizar(int anchoU, int altoU){
	if (time(NULL) - this->tiempoInicial >= this->tiempoExplosion){
		this->explotar(true);
	}else this->explotar(false);
}

ExplosivaPorTiempo::~ExplosivaPorTiempo(void)
{
}

void ExplosivaPorTiempo::BeginContact(){
}

void ExplosivaPorTiempo::EndContact(){}

void ExplosivaPorTiempo::disparar(bool sentido, float potencia, float angulo) {
	Arma::disparar(sentido,potencia,angulo);
	this->getBody()->SetAngularVelocity(2);
}

int ExplosivaPorTiempo::getTiempoRestante(){
	return (this->tiempoExplosion - (time(NULL) - this->tiempoInicial));
}