#ifndef REPRODUCTOR_H_
#define REPRODUCTOR_H_
#include <SDL/SDL_Mixer.h>
#include <iostream>
#include "../constantesAudio.h"

class Reproductor{
private:

	static Reproductor* repInstancia;
	Reproductor();
	static bool fondo;
	static bool explosion;
	static bool agua;
	static bool aleluya;
	static bool impactoAleluya;
	static bool impactoGranada;
	static bool cargandoDisparo;
	static bool soltarDisparo;
	static bool caminando;
	
	static Mix_Music* musicaFondo; 
	static Mix_Chunk** efectosDeSonido;
	


public:
	static Reproductor* getReproductor();
	~Reproductor();
		
	static void actualizar(void* arg);
	static void reproducirExplosion();
	static void reproducirAgua();
	static void reproducirAleluya();
	static void reproducirImpactoAleluya();
	static void reproducirImpactoGranada();
	static void reproducirCargandoDisparo();
	static void reproducirSoltarDisparo();
	static void reproducirCaminando();
	static void reproducirMusicaFondo();

};

#endif
