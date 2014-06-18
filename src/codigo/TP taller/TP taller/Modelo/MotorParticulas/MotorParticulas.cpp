#include "MotorParticulas.h"
#include "../../constantesArmas.h"


MotorParticulas::MotorParticulas(SDL_Renderer* renderer){

	this->renderer = renderer;
	//levanto las posibles texturas de particula
	this->texturas.push_back(rutaCirculo);
	this->texturas.push_back(rutaEstrella);
	this->texturas.push_back(rutaDiamante);
	this->bufferParticulas.numPartic = 0;
	this->particulas = new vector<Particula*>;
}


MotorParticulas::~MotorParticulas()
{
}

void MotorParticulas::generarExplosion(int x, int y){

	this->bufferParticulas.numPartic = 50;
	this->bufferParticulas.x = x;
	this->bufferParticulas.y = y;

}

Particula* MotorParticulas::generarParticula(int x, int y){

	string textura = this->texturas.at(rand()%this->texturas.size());

    int vX = rand()%20 -10;
    int vY = rand()%20 -10;
    float angle = 0;
    float angularVelocity = 0.5f + rand()%2;

    int ttl = 6 + rand()%20;
	this->pos.x=x;
	this->pos.y=y;
	this->pos.h=2 + rand()%5;
	this->pos.w=2 + rand()%5;
	
	return new Particula(renderer, this->pos, vX, vY, angle, angularVelocity, ttl, textura);
}

void MotorParticulas::actualizar(){


	if(this->bufferParticulas.numPartic > 0){
		for(int i=0; i< 5; i++ ) {
			this->particulas->push_back(generarParticula(bufferParticulas.x, bufferParticulas.y));
			this->bufferParticulas.numPartic--;
		}
	}


	vector<Particula*>::iterator it = this->particulas->begin();
	while(it != particulas->end()){
		(*it)->actualizar();
		if ((*it)->tiempoDeVida <= 0){
			delete (*it);
			it = this->particulas->erase(it);
		}else it++;
	}
}

void MotorParticulas::dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, float escalaZoom, int anchoPx, int altoPx){
	for (int index = 0; index < particulas->size(); index++){
		particulas->at(index)->dibujar(renderer, corrimientoX, corrimientoY, escalaZoom, anchoPx, altoPx);
    }
}
