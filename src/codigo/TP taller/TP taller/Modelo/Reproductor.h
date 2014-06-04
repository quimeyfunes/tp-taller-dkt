#ifndef REPRODUCTOR_H_
#define REPRODUCTOR_H_
#include <SDL/SDL_Mixer.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "../constantesAudio.h"
#include "../Parser/yaml/ParserYaml.h"
#include <vector>
#include <queue>

using namespace std;

class Reproductor{
private:
	static bool activo;
	static Reproductor* repInstancia;
	Reproductor();
	static audio* listaDeReproduccion;
	static int numClientes;
	static queue<audioEnCola>* colaDeEspera;
	

public:
	static bool enviar;
	static Reproductor* getReproductor();
	~Reproductor();
	static void activar();
	static void apagar();
	static void reproducirSonido(sonido aRep);
	static void detenerSonido(sonido aRep);
	static bool estaReproduciendo(sonido aRep);
	static queue<audioEnCola>* getColaDeEspera();

};

#endif
