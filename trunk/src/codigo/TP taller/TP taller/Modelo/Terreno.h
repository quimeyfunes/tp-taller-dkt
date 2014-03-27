#ifndef __TERRENO_H__
#define __TERRENO_H__

#include <iostream>
#include "Box2D/Box2D.h"
#include "../Parser/mascara/LectorTerreno.h"
#include <list>

using namespace std;


class Terreno
{
private:
	list<b2Body*>* listaBodies;
	

public:
	Terreno();
	void generarTerreno(b2World* world, char* nombre);
	void agregarBody (b2Body* body);
	list<b2Body*>* getBodies();
	~Terreno();
};

#endif