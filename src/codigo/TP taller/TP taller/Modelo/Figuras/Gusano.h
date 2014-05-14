#ifndef __GUSANO_H__
#define __GUSANO_H__

#include <iostream>
#include <string>
#include <vector>
#include "Box2D/Box2D.h"
#include "../../Observador/Observable.h"
#include "../../constantes.h"
#include "Figura.h"

class Gusano: public Figura
{
private:
	int numContactos;
	vector<bool> meClickearon;
	bool congelado;
	int maximosCLientes;
public:
	Gusano();
	Gusano(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa,int maximosClientes);
	~Gusano(void);
	bool meClickeo(float x,float y);
	void agregarContacto();
	void sacarContacto();
	bool puedeSaltar();
	void simularAgua(int nivelAgua);
	void reiniciar();
	bool getMeClickearon(int cliente);
	void setMeClickearon(bool meClickearon, int cliente);
	bool getCongelado();
	void setCongelado(bool congelado);
	
};

#endif