#ifndef __GUSANO_H__
#define __GUSANO_H__

#include <iostream>
#include <string>
#include <vector>
#include "Box2D/Box2D.h"
#include "../../Observador/Observable.h"
#include "../../constantes.h"
#include "../../constantesArmas.h"
#include "../Armas/Arma.h"
#include "../Armas/Bazooka.h"
#include "Figura.h"
#include "time.h"

class Gusano: public Figura
{
private:
	int numContactos;
	bool meClick;
	vector<bool> meClickearon;
	bool congelado;
	int maximosCLientes;
	int vida;
	bool choqueConArma;
	b2Fixture* fixtureCuerpo;
	int tiempoInicial;

	typedef struct{
		tipoArma armaTipo;
		Arma* armaSeleccionada;
		float anguloDisparo;
		float potenciaDisparo;
		bool sentidoDisparo;
		bool puedeCargarse;
	}armaStruct;


public:
	Gusano();
	Gusano(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa,int maximosClientes);
	~Gusano(void);
	armaStruct armaActual;
	bool meClickeo(float x,float y);
	void agregarContacto();
	void sacarContacto();
	bool puedeSaltar();
	void simularAgua(int nivelAgua);
	void reiniciar();
	bool getMeClickearon(int cliente);
	void setMeClickearon(bool d);
	void setMeClickearon(bool meClickearon, int cliente);
	bool getCongelado();
	void setCongelado(bool congelado);
	Arma* getArmaSeleccionada();
	void setArma(Arma* nuevaArma);
	bool tieneUnArma();
	tipoArma getTipoArma();
	void BeginContact();
	void EndContact();
	void explotar(float fuerza);
	int getVida();
	void disparar();
	void chequearChoqueConArma();
};

#endif