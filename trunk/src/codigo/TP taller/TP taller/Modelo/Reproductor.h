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
	static Mix_Music* musicaFondo; 
	static Mix_Chunk** efectosDeSonido;
	


public:
	static Reproductor* getReproductor();
	~Reproductor();
		
	static void actualizar(void* arg);
	static void reproducirExplosion();
	static void reproducirMusicaFondo();

};

#endif
