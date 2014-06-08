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
	bool activo;
	vector<bool> meClickearon;
	
	bool congelado;
	int maximosCLientes;
	
	bool choqueConArma;
	b2Fixture* fixtureCuerpo;
	int tiempoInicial;
	bool explota;
	bool ahogado;

	typedef struct{
		tipoArma armaTipo;
		Arma* armaSeleccionada;
		float anguloDisparo;
		float potenciaDisparo;
		bool sentidoDisparo;
		bool puedeCargarse;
	}armaStruct;


public:
	bool meMuevo; //para el sonido de caminar
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
	void quedateQuietoWachin();
	bool getActivo();
	void setActivo(bool activo);
	void setMeClickearon(bool meClickearon, int cliente);
	bool getCongelado();
	void setCongelado(bool congelado);
	Arma* getArmaSeleccionada();
	void setArma(Arma* nuevaArma);
	bool tieneUnArma();
	tipoArma getTipoArma();
	void setTipoArma(tipoArma tipo);
	void BeginContact();
	void EndContact();
	void PostSolve(float impulso);
	void explotar(float fuerza);
	int getVida();
	void disparar();
	void chequearChoqueConArma();
	bool getExplota();
	void setExplota(bool explota);
	void setVida(int vida);
	int vida;
	int bufferVida;
};

#endif