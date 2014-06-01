#include "Reproductor.h"

Reproductor* Reproductor::repInstancia = NULL;
Mix_Chunk** Reproductor::efectosDeSonido;
vector<audio> Reproductor::listaDeReproduccion;
bool Reproductor::activo = false;
audio Reproductor::agregar1;
audio Reproductor::agregar2;

Reproductor::Reproductor(){

	Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 2048 );
	//cargo todos los audios

	//Mix_Volume(-1, 50);
	agregar1.efecto = NINGUNO;
	agregar2.efecto = NINGUNO;
	Mix_AllocateChannels(16);
	efectosDeSonido = new Mix_Chunk*[16];
	efectosDeSonido[MUSICAFONDO] = Mix_LoadWAV(rutaMusicaFondo);
	efectosDeSonido[EXPLOSION] = Mix_LoadWAV(rutaSonidoExplosion);
	efectosDeSonido[AGUA] = Mix_LoadWAV(rutaSonidoAgua);
	efectosDeSonido[SONIDOALELUYA] = Mix_LoadWAV(rutaSonidoAleluya);
	efectosDeSonido[IMPACTOALELUYA] = Mix_LoadWAV(rutaSonidoImpactoAleluya);
	efectosDeSonido[IMPACTOGRANADA] = Mix_LoadWAV(rutaSonidoImpactoGranada);
	efectosDeSonido[IMPACTOBANANA] = Mix_LoadWAV(rutaSonidoImpactoBanana);
	efectosDeSonido[ENEMIGOALAS12] = Mix_LoadWAV(rutaSonidoEnemigoALas12);
	efectosDeSonido[CARGANDODISPARO] = Mix_LoadWAV(rutaSonidoCargandoDisparo);
	efectosDeSonido[SOLTARDISPARO] = Mix_LoadWAV(rutaSonidoSoltarDisparo);
	efectosDeSonido[SOLTARGRANADA] = Mix_LoadWAV(rutaSonidoSoltarGranada);
	efectosDeSonido[RISA] = Mix_LoadWAV(rutaSonidoRisa);
	efectosDeSonido[MECHA] = Mix_LoadWAV(rutaSonidoMecha);
	efectosDeSonido[ACUBIERTO] = Mix_LoadWAV(rutaSonidoPonteACubierto);
	efectosDeSonido[CAMINANDO] = Mix_LoadWAV(rutaSonidoWalkC);
	efectosDeSonido[15] = Mix_LoadWAV(rutaSonidoWalkE);
	
}


Reproductor::~Reproductor(){

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

		audio nuevo;
		nuevo.activo= true;
		nuevo.efecto = son;
		nuevo.canal = son;
		nuevo.porPrimeraVez = true;

		for(int i=0; i < listaDeReproduccion.size(); i++){
			if(!listaDeReproduccion.at(i).activo){
				listaDeReproduccion.at(i) = nuevo;
				return;
			}
		}
		
		if(agregar1.efecto == NINGUNO) agregar1 = nuevo;
		else if(agregar2.efecto == NINGUNO) agregar2 = nuevo;
	}
}

int Reproductor::getPosicion(sonido s){
	for(int i=0; i < listaDeReproduccion.size(); i++){
		if(listaDeReproduccion.at(i).efecto == s)
			return i;
	}
	return -1;
}

void Reproductor::detenerSonido(sonido son){
		int pos = getPosicion(son);		
		if(pos != -1){
			Mix_HaltChannel(listaDeReproduccion.at(pos).canal);
			listaDeReproduccion.at(pos).activo = false;
		}
}

bool Reproductor::estaReproduciendo(sonido s){
	int pos = getPosicion(s);

	if(pos == -1) return false;
	else return listaDeReproduccion.at(pos).activo;
}

void Reproductor::actualizar(void* arg){
	int loops;
	while(activo){
		if(activo){

			if(agregar1.efecto != NINGUNO){
				listaDeReproduccion.push_back(agregar1);
				agregar1.efecto = NINGUNO;
			}

			if(agregar2.efecto != NINGUNO){
				listaDeReproduccion.push_back(agregar2);
				agregar2.efecto = NINGUNO;
			}

			for(int i = 0; i< listaDeReproduccion.size(); i++){
				if(listaDeReproduccion.at(i).activo){
					if((listaDeReproduccion.at(i).efecto == MECHA) || (listaDeReproduccion.at(i).efecto == MUSICAFONDO) ){
						loops = -1;
					}else{
						loops = 0;
					}
					if(listaDeReproduccion.at(i).porPrimeraVez){
						Mix_PlayChannel(listaDeReproduccion.at(i).canal, efectosDeSonido[listaDeReproduccion.at(i).efecto], loops);
						listaDeReproduccion.at(i).porPrimeraVez = false;
					}

					if(!Mix_Playing(listaDeReproduccion.at(i).canal)){
						listaDeReproduccion.at(i).activo = false;
					}
				}
			}
		}
	}
}

void Reproductor::activar(){
	activo=true;
}

void Reproductor::apagar(){
	activo=false;
}