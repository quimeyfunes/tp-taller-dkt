#ifndef __VISTA_H__
#define __VISTA_H__

#include "Dibujables/DibujableTextura.h"
#include "Dibujables/CirculoDibujable.h"
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
	DibujableTextura* crearDibujableTextura(int x , int y ,int ancho,int alto, string pathImagen);
	void crearSprite(int x, int y, int anchoFrame, int altoFrame, string path, int col, int fil, int anchoTex, int altoTex);
	CirculoDibujable* crearCirculoDibujable(int x , int y ,int radioHorizontal,int radioVertical);
	void agregarDibujable(Dibujable* dibujable);
	list<Dibujable*>* getListaDibujables();
	void Dibujar();
	void AgregarCieloAguaTierra(string pathCielo, string pathAgua, string pathTierra);
	
};

#endif