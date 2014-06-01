#ifndef REPRODUCTOR_H_
#define REPRODUCTOR_H_
#include <SDL/SDL_Mixer.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "../constantesAudio.h"

class Reproductor{
private:
	static sonido aReproducir;
	static Reproductor* repInstancia;
	Reproductor();
	static Mix_Chunk** efectosDeSonido;
	
public:
	static Reproductor* getReproductor();
	~Reproductor();
		
	static void actualizar(void* arg);
	static void reproducirSonido(sonido aRep);
	static void detenerSonido(sonido aRep);
	static void pausarSonido(sonido aRep);
	static void reproducirMusicaFondo();
	static bool estaReproduciendo(sonido rep);
	static sonido getAReproducir();

};

#endif
