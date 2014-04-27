#ifndef __ESCENARIO_H__
#define __ESCENARIO_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <valarray>
#include <list>
#include "../../Parser/yaml/ParserYaml.h"
#include "../Figuras/Gusano.h"
#include "../Figuras/Poligono.h"
#include "../Figuras/Circulo.h"
#include "../Figuras/Rectangulo.h"
#include "../../constantes.h"
#include "../Terreno/Terreno.h"

using namespace std;

class Escenario
{
private:
	b2World* world;  
	int altoU;
    int anchoU;
    int nivelAgua;
	static float relacionAncho;
	static float relacionAlto;
	list<Figura*>* listaFiguras;
	Terreno* terreno;
	Figura* figuraActiva;
	bool puedeMoverseArriba; //Esta es true si se apreta arriba
	bool puedeSaltar;		//Esta es true si esta tocando algo por abajo
	bool puedeMoverseIzquierda;
	bool puedeMoverseDerecha;

	std::stringstream getMensajeSuperposicionObjeto(int linea);
	std::stringstream getMensajeSuperposicionTerreno(int linea);
public:
	Escenario();
	Escenario(int altoU,int anchoU,int nivelAgua, float relacionAncho, float relacionAlto);
    int getAltoU();
    int getAnchoU();
    int getNivelAgua();
	list<Figura*>* getFiguras();
	b2World* getWorld();
	Poligono* crearPoligono(ObjetoParseado objeto);
	Circulo* crearCirculo(ObjetoParseado objeto);
 	Rectangulo* crearRectangulo(ObjetoParseado objeto);
	Gusano* crearGusano(ObjetoParseado objeto);
	void agregarFigura(Figura* figura);
	void notificar();
	void simularAgua();
	void reiniciar();
	bool haySuperposicion(Figura* figura);
	bool haySuperposicionConTerreno(Figura* figura);
	Terreno* getTerreno();
	void setTerreno(Terreno* terreno);
	void click(float x, float y);
	void arriba(bool arriba);
	void setPuedeSaltar(bool puedeSaltar);
	void izquierda(bool izquierda);
	void derecha(bool derecha);	
	void saltar();
	void moverIzquierda();
	void moverDerecha();
	Figura* getFiguraActiva();
};


#endif