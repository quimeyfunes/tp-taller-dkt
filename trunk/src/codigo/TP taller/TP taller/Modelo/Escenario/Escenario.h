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
	int maximosClientes;
	static float relacionAncho;
	static float relacionAlto;
	list<Figura*>* listaFiguras;
	Terreno* terreno;
	//Es la del servidor
	Gusano* gusanoActivo;
	vector<Gusano*> figurasActivas;
	bool puedeMoverseArriba; 
	bool puedeMoverseIzquierda;
	bool puedeMoverseDerecha;

	//Para clientes
	vector<bool> puedeMoverseArribaClientes;
	vector<bool> puedeSaltarClientes;
	vector<bool> puedeMoverseIzquierdaClientes;
	vector<bool> puedeMoverseDerechaClientes;

	std::stringstream getMensajeSuperposicionObjeto(int linea);
	std::stringstream getMensajeSuperposicionTerreno(int linea);
public:
	Escenario();
	Escenario(int altoU,int anchoU,int nivelAgua, float relacionAncho, float relacionAlto,int maximosClientes);
    int getAltoU();
    int getAnchoU();
    int getNivelAgua();
	int getMaximosClientes();
	list<Figura*>* getFiguras();
	b2World* getWorld();
	Poligono* crearPoligono(ObjetoParseado objeto);
	Circulo* crearCirculo(ObjetoParseado objeto);
 	Rectangulo* crearRectangulo(ObjetoParseado objeto);
	Gusano* crearGusano(ObjetoParseado objeto);
	Gusano* crearGusanoParaJugador();
	void agregarFigura(Figura* figura);
	void notificar();
	void simularAgua();
	void reiniciar();
	void reiniciarTeclas();
	bool haySuperposicion(Figura* figura);
	bool haySuperposicionConTerreno(Figura* figura);
	Terreno* getTerreno();
	void setTerreno(Terreno* terreno);
	void click(float x, float y);
	void clickCliente(int cliente, list<Gusano*> figurasCliente, list<Gusano*> figurasOtrosCliente,float x, float y);
	void arriba(bool arriba);
	void arribaCliente(int cliente, bool arriba);
	void setPuedeSaltarCliente(int cliente,bool puedeSaltar);
	void izquierda(bool izquierda);
	void izquierdaCliente(int cliente,bool izquierda);
	void derecha(bool derecha);	
	void derechaCliente(int cliente, bool derecha);	
	void moverse();
	void moverseClientes();
	void saltar();
	void moverIzquierda();
	void moverDerecha();
	void saltarClientes();
	void moverIzquierdaClientes();
	void moverDerechaClientes();
	vector<Gusano*> getFigurasActivas();
	//Es la del servidor
	Figura* getFiguraActiva();
	void inicializarCliente(int cliente);
};


#endif