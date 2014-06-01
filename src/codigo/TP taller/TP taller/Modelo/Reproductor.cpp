#include "Reproductor.h"

Reproductor* Reproductor::repInstancia = NULL;
Mix_Chunk** Reproductor::efectosDeSonido;
sonido Reproductor::aReproducir = NINGUNO;

Reproductor::Reproductor(){

	Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 1024 );
	//cargo todos los audios

	//Mix_Volume(-1, 50);
	Mix_AllocateChannels(13);
	efectosDeSonido = new Mix_Chunk*[14];
	efectosDeSonido[MUSICAFONDO] = Mix_LoadWAV(rutaMusicaFondo);
	efectosDeSonido[EXPLOSION] = Mix_LoadWAV(rutaSonidoExplosion);
	efectosDeSonido[AGUA] = Mix_LoadWAV(rutaSonidoAgua);
	efectosDeSonido[SONIDOALELUYA] = Mix_LoadWAV(rutaSonidoAleluya);
	efectosDeSonido[IMPACTOALELUYA] = Mix_LoadWAV(rutaSonidoImpactoAleluya);
	efectosDeSonido[IMPACTOGRANADA] = Mix_LoadWAV(rutaSonidoImpactoGranada);
	efectosDeSonido[CARGANDODISPARO] = Mix_LoadWAV(rutaSonidoCargandoDisparo);
	efectosDeSonido[SOLTARDISPARO] = Mix_LoadWAV(rutaSonidoSoltarDisparo);
	efectosDeSonido[SOLTARGRANADA] = Mix_LoadWAV(rutaSonidoSoltarGranada);
	efectosDeSonido[CAMINANDO] = Mix_LoadWAV(rutaSonidoWalkC);
	efectosDeSonido[RISA] = Mix_LoadWAV(rutaSonidoRisa);
	efectosDeSonido[MECHA] = Mix_LoadWAV(rutaSonidoMecha);
	efectosDeSonido[ACUBIERTO] = Mix_LoadWAV(rutaSonidoPonteACubierto);
	efectosDeSonido[13] = Mix_LoadWAV(rutaSonidoWalkE);
	efectosDeSonido[IMPACTOBANANA] = Mix_LoadWAV(rutaSonidoImpactoBanana);
}


Reproductor::~Reproductor(){

	for(int i=0; i<14; i++)
		Mix_FreeChunk(efectosDeSonido[i]);
	delete repInstancia;
	Mix_Quit();
}

Reproductor* Reproductor::getReproductor(){

	if(repInstancia == NULL) repInstancia = new Reproductor();

	return repInstancia;
}

void Reproductor::reproducirSonido(sonido son){
	aReproducir = son;
}

void Reproductor::detenerSonido(sonido son){
	if(estaReproduciendo(son)) Mix_HaltChannel(son);
}

void Reproductor::pausarSonido(sonido son){
	if(estaReproduciendo(son)) Mix_Pause(son);
}

bool Reproductor::estaReproduciendo(sonido s){
	return (Mix_Playing(s) != 0);
}

void Reproductor::actualizar(void* arg){
	while(true){
		
		if(aReproducir == MUSICAFONDO){
		
			Mix_PlayChannel(MUSICAFONDO, efectosDeSonido[MUSICAFONDO], -1);
			Mix_Volume(MUSICAFONDO, 128);
			aReproducir=NINGUNO;
		}

		if(aReproducir == MECHA){
			Mix_PlayChannel(aReproducir, efectosDeSonido[aReproducir], -1);
			Mix_Volume(aReproducir, 20);
			aReproducir=NINGUNO;
		}

		if(aReproducir != NINGUNO){
			
			Mix_PlayChannel(aReproducir, efectosDeSonido[aReproducir], 0);	
			Mix_Volume(aReproducir, 40);
			aReproducir=NINGUNO;
			
		}


	}
}

sonido Reproductor::getAReproducir(){
	return aReproducir;
}