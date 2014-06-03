#ifndef __TERRENO_H__
#define __TERRENO_H__

#include <iostream>
#include "Box2D/Box2D.h"
#include "../../constantesFisicas.h"
#include "../../tablas.h"
#include <boost\geometry.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/foreach.hpp>

#include "../../Parser/mascara/LectorTerreno.h"
#include <list>

using namespace std;

namespace bg = boost::geometry;
typedef bg::model::point<float,2,bg::cs::cartesian> Punto;
typedef bg::model::polygon<Punto> PoligonoBoost;

class Terreno
{
private:
	b2Body* body;
	list<PoligonoBoost>* terreno;
	LectorTerreno* lectorTerreno;
public:
	Terreno(b2World* world);
	void generarTerreno(string nombre);
	b2ChainShape* generarBorde(b2Vec2 borde); 
	void destruirTerreno(float x, float y,int radio);
	~Terreno();
	b2Body* getBody();
	LectorTerreno* getLector();
	LectorTerreno* getLectorTerreno();
};

#endif