#ifndef __ESCENARIO_H__
#define __ESCENARIO_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <valarray>
#include <list>
#include "../Parser/yaml/ParserYaml.h"
#include "Poligono.h"
#include "Circulo.h"
#include "Rectangulo.h"
#include "../constantes.h"

using namespace std;

class Escenario
{
private:
	b2World* world;  
	int altoU;
    int anchoU;
    int nivelAgua;
	list<Figura*>* listaFiguras;
public:
	Escenario();
	Escenario(int altoU,int anchoU,int nivelAgua);
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
	bool haySuperposicion(Figura* figura);

};


#endif