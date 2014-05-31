#include "Reproductor.h"

Reproductor* Reproductor::repInstancia = NULL;
bool Reproductor::fondo=false;
bool Reproductor::explosion=false;
bool Reproductor::agua=false;
bool Reproductor::aleluya=false;
bool Reproductor::impactoAleluya=false;
bool Reproductor::impactoGranada=false;
bool Reproductor::cargandoDisparo=false;
bool Reproductor::soltarDisparo=false;
bool Reproductor::caminando=false;

Mix_Music* Reproductor::musicaFondo; 
Mix_Chunk** Reproductor::efectosDeSonido;

Reproductor::Reproductor(){

	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

	//cargo todos los audios
	musicaFondo = Mix_LoadMUS(rutaMusicaFondo);
	efectosDeSonido = new Mix_Chunk*[9];
	efectosDeSonido[0] = Mix_LoadWAV(rutaSonidoExplosion);
	efectosDeSonido[1] = Mix_LoadWAV(rutaSonidoAgua);
	efectosDeSonido[2] = Mix_LoadWAV(rutaSonidoAleluya);
	efectosDeSonido[3] = Mix_LoadWAV(rutaSonidoImpactoAleluya);
	efectosDeSonido[4] = Mix_LoadWAV(rutaSonidoImpactoGranada);
	efectosDeSonido[5] = Mix_LoadWAV(rutaSonidoCargandoDisparo);
	efectosDeSonido[6] = Mix_LoadWAV(rutaSonidoSoltarDisparo);
	efectosDeSonido[7] = Mix_LoadWAV(rutaSonidoWalkC);
	efectosDeSonido[8] = Mix_LoadWAV(rutaSonidoWalkE);
}


Reproductor::~Reproductor(){
	Mix_FreeMusic(musicaFondo);
	for(int i=0; i<9; i++)
		Mix_FreeChunk(efectosDeSonido[i]);
	delete repInstancia;
	Mix_Quit();
}

Reproductor* Reproductor::getReproductor(){

	if(repInstancia == NULL) repInstancia = new Reproductor();

	return repInstancia;
}

void Reproductor::reproducirExplosion(){
	explosion=true;
}

void Reproductor::reproducirMusicaFondo(){
	fondo=true;
}

void Reproductor::reproducirAgua(){
	agua=true;
}
void Reproductor::reproducirAleluya(){
	aleluya=true;
}

void Reproductor::reproducirImpactoAleluya(){
	impactoAleluya=true;
}

void Reproductor::reproducirImpactoGranada(){
	impactoGranada=true;
}

void Reproductor::reproducirCargandoDisparo(){
	cargandoDisparo=true;
}

void Reproductor::reproducirSoltarDisparo(){
	soltarDisparo=true;
}

void Reproductor::reproducirCaminando(){
	caminando=true;
}

void Reproductor::actualizar(void* arg){
	while(true){
	
		if(fondo) Mix_PlayMusic(musicaFondo, -1);

		if(explosion){
			Mix_PlayChannel(-1, efectosDeSonido[0], 0);
			explosion = false;
		}

		if(agua){
			Mix_PlayChannel(-1, efectosDeSonido[1], 0);
			agua=false;
		}

		if(aleluya){
			Mix_PlayChannel(-1, efectosDeSonido[2], 0);
			aleluya=false;
		}

		if(impactoAleluya){
			Mix_PlayChannel(-1, efectosDeSonido[3], 0);
			impactoAleluya=false;
		}

		if(impactoGranada){
			Mix_PlayChannel(-1, efectosDeSonido[4], 0);
			impactoGranada=false;
		}

		if(cargandoDisparo){
			Mix_PlayChannel(-1, efectosDeSonido[5], 0);
			cargandoDisparo=false;
		}

		if(soltarDisparo){
			Mix_PlayChannel(-1, efectosDeSonido[6], 0);
			soltarDisparo=false;
		}

		if(caminando){
			Mix_PlayChannel(-1, efectosDeSonido[7], 0);
			caminando=false;
		}
	}
}