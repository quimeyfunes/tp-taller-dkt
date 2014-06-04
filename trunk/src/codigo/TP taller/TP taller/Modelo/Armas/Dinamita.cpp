#include "Dinamita.h"


Dinamita::Dinamita(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float ancho, float alto, float masa, int tiempoExplosion): ExplosivaPorTiempo(x,y,rotacion,world,estatico,radioExplosion, masa, tiempoExplosion)
{
	this->aCubierto = false;
	this->armaTipo = DINAMITA;
	b2PolygonShape rectanguloShape;
	//Divido a la mitad el ancho y alto para que las medidas sean correctas
	rectanguloShape.SetAsBox(ancho/2, alto/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rectanguloShape;
	float areaRec = ancho * alto;
	fixtureDef.density = masa/areaRec;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	fixtureDef.userData = this;
	this->getBody()->CreateFixture(&fixtureDef);
}

void Dinamita::disparar(bool sentido, float potencia, float angulo){

	Reproductor::getReproductor()->reproducirSonido(RISA);
	Reproductor::getReproductor()->reproducirSonido(MECHA);
}

bool Dinamita::getExplotar(){
	this->explota = false;

	if(!aCubierto){
		if(time(NULL) - this->tiempoInicial == this->tiempoExplosion - 1){
			Reproductor::getReproductor()->reproducirSonido(ACUBIERTO);
			aCubierto=true;
		}
	}else{	
		if(time(NULL) - this->tiempoInicial == this->tiempoExplosion){
			this->explota=true;
			Reproductor::getReproductor()->detenerSonido(MECHA);
		}
		else
			this->explota=false;
	}
	return this->explota;
}

Dinamita::~Dinamita(void)
{
}
