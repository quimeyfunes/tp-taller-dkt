#ifndef __JUEGO_H__
#define __JUEGO_H__

#include "SDL/SDL.h"
#include "Box2D/Box2D.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL2_gfxPrimitives.h"
#include "yaml-cpp/yaml.h"
#include "../Red/Servidor.h"
#include "../Red/Cliente.h"
#include "../../Vista/Vista.h"
#include "../Escenario/Escenario.h"
#include "../../Parser/ParserDeHexARgb.h"
#include "../Terreno/Terreno.h"
#include <iostream>
#include <vector>
#include "../StringUtil.h"


using namespace std;

typedef enum{
	PAUSADO,
	JUGANDO,
	SALIDA,
}ESTADO_JUEGO;

class Juego{

private:
	static Servidor* servidor;
	static Cliente* cliente1; //despues sacar!
	static Cliente* cliente2;
	//list<DibujableSerializado> Juego::crearLista(int &tamanio);
	string Juego::crearLista(int &tamanio);
	Vista* vista;
	Escenario* escenario;
	Terreno* terreno;
	SDL_Event* evento;
	b2World* mundo;


	bool simulando;
	ESTADO_JUEGO estadoActual;

	void reiniciar();
	void salir();
	void jugar();
	void alternarPausa();
	void esperar();
	void agregarObjetos();
	void agregarTexturas(EscenarioParseado* e);
	void leerEvento();
	static void servidorLoop(void * arg);
	static void clienteLoop(void * arg);
public:
	Juego();
	void ejecutar();
	~Juego();
};


#endif