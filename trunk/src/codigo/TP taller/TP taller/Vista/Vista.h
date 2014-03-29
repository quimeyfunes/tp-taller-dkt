#ifndef __VISTA_H__
#define __VISTA_H__

#include "FiguraDibujable.h"
#include "../Parser/yaml/ParserYaml.h"
#include <list>

using namespace std;

class Vista
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	list<Dibujable*>* listaDibujables;
	
public:

	Vista(EscenarioParseado* e);
	~Vista();
	SDL_Renderer* getRenderer();
	Dibujable* crearDibujable(int x , int y ,int ancho,int alto, string pathImagen);
	FiguraDibujable* crearFiguraDibujable(int x , int y ,int ancho,int alto, string pathImagen);
	void agregarDibujable(Dibujable* dibujable);
	list<Dibujable*>* getListaDibujables();
	void Dibujar();
	void AgregarCieloAguaTierra(string pathCielo, string pathAgua, string pathTierra);
	
};

#endif