#ifndef REPRODUCTOR_H_
#define REPRODUCTOR_H_
#include <SDL/SDL_Mixer.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "../constantesAudio.h"
#include <vector>
using namespace std;

class Reproductor{
private:
	static bool activo;
	static Reproductor* repInstancia;
	Reproductor();
	static audio* listaDeReproduccion;

public:
	static Reproductor* getReproductor();
	~Reproductor();
	static void activar();
	static void apagar();
	static void reproducirSonido(sonido aRep);
	static void detenerSonido(sonido aRep);
	static bool estaReproduciendo(sonido aRep);

};

#endif
