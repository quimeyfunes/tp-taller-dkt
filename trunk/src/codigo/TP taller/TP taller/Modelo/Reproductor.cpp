#include "Reproductor.h"

Reproductor* Reproductor::repInstancia = NULL;
audio* Reproductor::listaDeReproduccion;
bool Reproductor::activo = false;

Reproductor::Reproductor(){

	Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 2048 );

	//cargo todos los audios
	listaDeReproduccion = new audio[18];
	listaDeReproduccion[MUSICAFONDO].efecto = Mix_LoadWAV(rutaMusicaFondo);
	listaDeReproduccion[EXPLOSION].efecto = Mix_LoadWAV(rutaSonidoExplosion);
	listaDeReproduccion[AGUA].efecto = Mix_LoadWAV(rutaSonidoAgua);
	listaDeReproduccion[SONIDOALELUYA].efecto = Mix_LoadWAV(rutaSonidoAleluya);
	listaDeReproduccion[IMPACTOALELUYA].efecto = Mix_LoadWAV(rutaSonidoImpactoAleluya);
	listaDeReproduccion[IMPACTOGRANADA].efecto = Mix_LoadWAV(rutaSonidoImpactoGranada);
	listaDeReproduccion[IMPACTOBANANA].efecto = Mix_LoadWAV(rutaSonidoImpactoBanana);
	listaDeReproduccion[ENEMIGOALAS12].efecto = Mix_LoadWAV(rutaSonidoEnemigoALas12);
	listaDeReproduccion[CARGANDODISPARO].efecto = Mix_LoadWAV(rutaSonidoCargandoDisparo);
	listaDeReproduccion[SOLTARDISPARO].efecto = Mix_LoadWAV(rutaSonidoSoltarDisparo);
	listaDeReproduccion[SOLTARGRANADA].efecto = Mix_LoadWAV(rutaSonidoSoltarGranada);
	listaDeReproduccion[RISA].efecto = Mix_LoadWAV(rutaSonidoRisa);
	listaDeReproduccion[MECHA].efecto = Mix_LoadWAV(rutaSonidoMecha);
	listaDeReproduccion[ACUBIERTO].efecto = Mix_LoadWAV(rutaSonidoPonteACubierto);
	listaDeReproduccion[CAMINANDO].efecto = Mix_LoadWAV(rutaSonidoWalk);
	listaDeReproduccion[SALTO].efecto = Mix_LoadWAV(rutaSonidoSalto);
	listaDeReproduccion[OUCH].efecto = Mix_LoadWAV(rutaSonidoOuch);
	listaDeReproduccion[TIMERTICK].efecto = Mix_LoadWAV(rutaSonidoTimerTick);

	for(int i=0; i < 18; i++){

		listaDeReproduccion[i].canal = -1;
		listaDeReproduccion[i].activo = false;

		if( i == MUSICAFONDO || i == MECHA ) //musica de fondo y la mecha de dinamita se reproducen infinitamente hasta ser detenidas manualmente
			listaDeReproduccion[i].loops = -1;
		else
			listaDeReproduccion[i].loops = 0; //reproduce una sola vez y corta.
	}	
}


Reproductor::~Reproductor(){
	//Mix_HaltChannel(-1);
	//for(int i=0; i<16; i++)
	//	Mix_FreeChunk(efectosDeSonido[i]);

	//delete repInstancia;
	//Mix_Quit();
}

Reproductor* Reproductor::getReproductor(){

	if(repInstancia == NULL) repInstancia = new Reproductor();

	return repInstancia;
}

void Reproductor::reproducirSonido(sonido son){

	if(activo){
		if(listaDeReproduccion[son].loops == -1) detenerSonido(son); //si se quieren reproducir al mismo tiempo 2 sonidos con loops infinitos, hay q detener uno para q no se quede colgado
		listaDeReproduccion[son].canal = Mix_PlayChannel(-1, listaDeReproduccion[son].efecto, listaDeReproduccion[son].loops);
		listaDeReproduccion[son].activo = true;
	}
}

void Reproductor::detenerSonido(sonido son){

	if(listaDeReproduccion[son].activo){
		if(Mix_Playing(listaDeReproduccion[son].canal) != 0){
			Mix_HaltChannel(listaDeReproduccion[son].canal);
		}
		listaDeReproduccion[son].activo = false;
	}
	
}

bool Reproductor::estaReproduciendo(sonido s){
	
	if(listaDeReproduccion[s].activo)
		listaDeReproduccion[s].activo = (Mix_Playing(listaDeReproduccion[s].canal) != 0)? true:false;

	return listaDeReproduccion[s].activo;
}

void Reproductor::activar(){
	activo=true;
}

void Reproductor::apagar(){
	activo=false;
}