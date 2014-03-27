#include "Terreno.h"



Terreno::Terreno()
{
	this->listaBodies = new list<b2Body*>;
}


void Terreno::generarTerreno(b2World* world, char* nombreArchivo)
{
	LectorTerreno* lectorT = new LectorTerreno(nombreArchivo);
	bool** matrizTerreno = lectorT->getMatrizTerreno();
	int anchoMatriz = lectorT->getAnchoMatriz();
	int altoMatriz = lectorT->getAltoMatriz();
	bool tierra = true;
	int tamanioBorde = 100; //CAMBIAR ESTO POR METODO DE MARIAN
	bool hayTierra = false;
	int k = 0;
	b2Vec2* vecBorde = new b2Vec2[tamanioBorde];
	b2Body* body;

	// Recorro la matriz hasta encontrar tierra
	for (int i = 0; i < anchoMatriz; i++){
		for (int j = 0; ((j < altoMatriz) && !(hayTierra)); j++){
			//Encuentro el borde y genero un chain
			if (matrizTerreno[i][j] == tierra){

				hayTierra = true;
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position.Set(i,j);
				bodyDef.angle = 0;

				body = world->CreateBody(&bodyDef);
				this->agregarBody(body);
				//Seteo las cordenadas del borde en el vector
				vecBorde[k].Set(i,j);
				k++;
			}

		}

		hayTierra = false;
	}

	b2ChainShape chain;
	chain.CreateChain(vecBorde, tamanioBorde);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &chain;
	body->CreateFixture(&fixtureDef);

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
