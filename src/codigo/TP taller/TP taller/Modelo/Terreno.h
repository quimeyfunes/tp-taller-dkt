#ifndef __TERRENO_H__
#define __TERRENO_H__

#include <iostream>
#include "Box2D/Box2D.h"
#include "../Parser/mascara/LectorTerreno.h"
#include <list>

using namespace std;


class Terreno
{
	b2Body* body;
public:
	Terreno(b2World* world);
	void generarTerreno(char* nombre);
	b2ChainShape* generarBorde(b2Vec2 borde); 
	~Terreno();
};

#endif