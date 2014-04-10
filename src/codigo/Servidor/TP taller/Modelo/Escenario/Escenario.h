#ifndef __ESCENARIO_H__
#define __ESCENARIO_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <valarray>
#include <list>
#include "../../Parser/yaml/ParserYaml.h"
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
	void agregarFigura(Figura* figura);
	void notificar();
	void simularAgua();
	void reiniciar();
	bool haySuperposicion(Figura* figura);
	bool haySuperposicionConTerreno(Figura* figura);
	Terreno* getTerreno();
	void setTerreno(Terreno* terreno);
	static float getRelacionAncho();
	static float getRelacionAlto();

};


#endif