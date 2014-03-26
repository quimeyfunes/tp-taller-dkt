#include "Terreno.h"



Terreno::Terreno()
{
	this->listaBodies = new list<b2Body*>;
}


void Terreno::generarTerreno(b2World* world, char* nombreArchivo)
{
	lectorTerreno* lectorT = new lectorTerreno(nombreArchivo);
	bool** matrizTerreno = lectorT->getMatrizTerreno();
	int anchoMatriz = lectorT->getAnchoMatriz();
	int altoMatriz = lectorT->getAltoMatriz();
	bool tierra = true;

	// Recorro la matriz hasta encontrar tierra
	for (int i = 0; i < anchoMatriz; i++){
		for (int j = 0; j < altoMatriz; j++){
			//Donde haya tierra genero una caja de 1x1
			if (matrizTerreno[i][j] == tierra){

				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position.Set(i,j);
				bodyDef.angle = 0;

				b2Body* body = world->CreateBody(&bodyDef);
				this->agregarBody(body);

				b2PolygonShape caja;
				caja.SetAsBox(0.5,0.5);
  
				b2FixtureDef cajaFixtureDef;
				cajaFixtureDef.shape = &caja;
				cajaFixtureDef.density = 1;
                body->CreateFixture(&cajaFixtureDef);

			}

		}
	}

	delete lectorT;

}

void Terreno::agregarBody(b2Body* body)
{
	this->listaBodies->push_back(body);
}

list<b2Body*>* Terreno::getBodies()
{
	return this->listaBodies;
}

Terreno::~Terreno(void)
{
	delete this->listaBodies;
}
