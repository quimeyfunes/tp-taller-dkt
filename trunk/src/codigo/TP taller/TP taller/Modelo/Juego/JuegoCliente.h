#ifndef __JUEGOCLIENTE_H__
#define __JUEGOCLIENTE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL2_gfxPrimitives.h"
#include "yaml-cpp/yaml.h"
#include "Juego.h"
#include <iostream>
#include <vector>
#include "../Red/Cliente.h"
#include "../StringUtil.h"
#include "../../Vista/Dibujables/CartelInfo.h"


using namespace std;

const int teclas = 6;	//Arriba,Abajo,Derecha,Izquierda,Enter,Space

class JuegoCliente: public Juego{

private:
	//list<DibujableSerializado> Juego::crearLista(int &tamanio);
	//string Juego::crearLista(int &tamanio);
	/*Vista* vista;
	Escenario* escenario;
	SDL_Event* evento;*/

	EscenarioParseado* esc;
	CartelInfo* cartelInfo;
	PanelArmas* panelArmas;
	static Cliente* cliente;
	list<Dibujable*>* dibujablesBase;
	bool eventos[teclas];		
	void reiniciar();
	void salir();
	void jugar();
	void alternarPausa();
	void alternarPanelArmas();
	void esperar();
	void agregarTexturas(EscenarioParseado* e);
	void agregarAgua(EscenarioParseado* e);
	void leerEvento();
	/*list<Dibujable*>* */void crearLista(string vistaSerializada);
	int getArmaSeleccionada(int x, int y);
	RelojSprite* reloj;


public:
	JuegoCliente();
	JuegoCliente(string nombreCliente, string ip);
	void ejecutar();
	~JuegoCliente();
	void actualizarDibujables(Paquete* paqueteVista);
	
};


#endif