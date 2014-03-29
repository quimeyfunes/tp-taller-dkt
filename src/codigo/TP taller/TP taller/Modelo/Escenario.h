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
	void crearPoligono(ObjetoParseado objeto, b2World* world);
	void crearCirculo(ObjetoParseado objeto, b2World* world);
 	void crearRectangulo(ObjetoParseado objeto, b2World* world);
	void agregarFigura(Figura* figura);
	void notificar();

};


#endif