#include "Reproductor.h"

Reproductor* Reproductor::repInstancia = NULL;
bool Reproductor::fondo=false;
bool Reproductor::explosion=false;
Mix_Music* Reproductor::musicaFondo; 
Mix_Chunk** Reproductor::efectosDeSonido;

Reproductor::Reproductor(){

	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

	//cargo todos los audios
	musicaFondo = Mix_LoadMUS(rutaMusicaFondo);
	efectosDeSonido = new Mix_Chunk*[1];
	efectosDeSonido[0] = Mix_LoadWAV(rutaSonidoExplosion);
}


Reproductor::~Reproductor(){
	Mix_FreeMusic(musicaFondo);
	Mix_FreeChunk(efectosDeSonido[0]);
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


void Reproductor::actualizar(void* arg){
	while(true){
	
		if(fondo) Mix_PlayMusic(musicaFondo, -1);

		if(explosion){
			Mix_PlayChannel(-1, efectosDeSonido[0], 0);
			explosion = false;
		}
	}
}