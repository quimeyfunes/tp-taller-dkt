#ifndef __JUEGO_H__
#define __JUEGO_H__

#include "SDL/SDL.h"
#include "Box2D/Box2D.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL2_gfxPrimitives.h"
#include "yaml-cpp/yaml.h"
#include "../Red/Servidor.h"
#include "../../Vista/Vista.h"
#include "../Escenario/Escenario.h"
#include "../../Parser/ParserDeHexARgb.h"
#include "../Terreno/Terreno.h"
#include <iostream>
#include <vector>
#include "../StringUtil.h"
#include "../ResolverContacto.h"
#include "../Red/Evento.h"
#include "../Armas/Granada.h"
#include "../Armas/Aleluya.h"
#include "../Armas/Dinamita.h"
#include "Turno.h"


using namespace std;

typedef enum{
	PAUSADO,
	JUGANDO,
	SALIDA,
}ESTADO_JUEGO;

class Juego{

private:
	static Servidor* servidor;
	//list<DibujableSerializado> Juego::crearLista(int &tamanio);
	b2World* mundo;

	Turno* turno;
	b2Vec2 getPosicionInicialDisparo(b2Vec2 posGusano, int angulo, bool sentido, double separacion);
	void agregarObjetos();
	static void servidorLoop(void * arg);
	static void clienteLoop(void * arg);
	static string jugadorActual;

	void dispararArma();
	//para setear el arma mas facil mientras se prueba
	int NUMCLICKDERECHOS;
public:
	string Juego::crearLista(int &tamanio);
	Vista* vista;
	Escenario* escenario;
	SDL_Event* evento;
	Terreno* terreno;
	void reiniciar();
	void salir();
	void jugar();
	void alternarPausa();
	void esperar();
	static string getJugadorActual();
	static void cambiarJugador(string jugador);
	void agregarTexturas(EscenarioParseado* e);
	void agregarAgua(EscenarioParseado* e);
	void leerEvento();
	void chequearNuevosJugadores();
	bool simulando;
	ESTADO_JUEGO estadoActual;
	Juego();
	Juego(string texto);
	void ejecutar();
	~Juego();
};


#endif