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
	list<Arma*>* listaArmas;
	Terreno* terreno;
	//Es la del servidor
	Gusano* gusanoActivo;
	vector<Gusano*> figurasActivas;
	bool bloquearTeclas;
	
	bool puedeMoverseArriba; 
	bool puedeMoverseAbajo;
	bool puedeMoverseIzquierda;
	bool puedeMoverseDerecha;
	bool puedeDisparar;
	bool puedeSaltar;

	//Para clientes
	vector<bool> puedeMoverseArribaClientes;
	vector<bool> puedeSaltarClientes;
	vector<bool> puedeMoverseIzquierdaClientes;
	vector<bool> puedeMoverseDerechaClientes;
	vector<bool> puedeDispararClientes;
	vector<bool> puedeMoverseAbajoClientes;

	std::stringstream getMensajeSuperposicionObjeto(int linea);
	std::stringstream getMensajeSuperposicionTerreno(int linea);
public:
	Escenario();
	Escenario(int altoU,int anchoU,int nivelAgua, float relacionAncho, float relacionAlto,int maximosClientes);
    int getAltoU();
    int getAnchoU();
    int getNivelAgua();
	int getMaximosClientes();
	void detenerMovimientos();
	Gusano* getGusanoActivo();
	void setGusanoActivo(Gusano* gusano);
	list<Figura*>* getFiguras();
	b2World* getWorld();
	Poligono* crearPoligono(ObjetoParseado objeto);
	Circulo* crearCirculo(ObjetoParseado objeto);
 	Rectangulo* crearRectangulo(ObjetoParseado objeto);
	Gusano* crearGusano(ObjetoParseado objeto);
	Gusano* crearGusanoParaJugador();
	void agregarFigura(Figura* figura);
	void agregarArma(Arma* arma);
	void notificar();
	void simularAgua();
	void reiniciar();
	void reiniciarTeclas();
	bool haySuperposicion(Figura* figura);
	bool haySuperposicionConTerreno(Figura* figura);
	Terreno* getTerreno();
	void setTerreno(Terreno* terreno);
	bool click(float x, float y);
	void clickCliente(int cliente, list<Gusano*> figurasCliente, list<Gusano*> figurasOtrosCliente,float x, float y);
	void arriba(bool arriba);
	void abajo(bool abajo);
	void espacio(bool esp);
	void espacioCliente(int cliente, bool esp);
	void enter(bool enter);
	void enterCliente(int cliente, bool enter);
	void arribaCliente(int cliente, bool arriba);
	void abajoCliente(int cliente, bool arriba);
	void setPuedeSaltarCliente(int cliente,bool puedeSaltar);
	void izquierda(bool izquierda);
	void izquierdaCliente(int cliente,bool izquierda);
	void derecha(bool derecha);	
	void derechaCliente(int cliente, bool derecha);	
	void moverse();
	void moverseClientes();
	void saltar();
	void subir();
	void bajar();
	void cargarDisparo();
	void moverIzquierda();
	void moverDerecha();
	void saltarClientes();
	void moverIzquierdaClientes();
	void moverDerechaClientes();
	void bajarClientes();
	void subirClientes();
	void cargarDisparoClientes();
	vector<Gusano*> getFigurasActivas();
	//Es la del servidor
	Figura* getFiguraActiva();
	void inicializarCliente(int cliente);
	b2Vec3 hayExplosion();
	void explotar(float x, float y, int radio);
	void disparar(bool disparo);
	void realizarDisparo();
	bool hayExposionPendiente();
	bool huboDisparo;
	bool getHuboDisparo();
	void setHuboDisparo(bool b);
	bool getPuedeDisparar();

	bool getBloquearTeclas();
	void setBloquearTeclas(bool bloquear);

	int restarVidaGusanos();
};


#endif