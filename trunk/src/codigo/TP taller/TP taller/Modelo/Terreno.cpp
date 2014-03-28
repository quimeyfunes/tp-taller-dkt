#include "Terreno.h"



Terreno::Terreno()
{
}


void Terreno::generarTerreno(b2World* world, char* nombreArchivo)
{
	LectorTerreno* lectorT = new LectorTerreno(nombreArchivo);
	bool** matrizTerreno = lectorT->getMatrizTerreno();
	int anchoMatriz = lectorT->getAnchoMatriz();
	int altoMatriz = lectorT->getAltoMatriz();
	bool tierra = true;
	bool huboTierra = false;
	bool aguasProfundas = false;
	int tamanioBorde = lectorT->getTamanoBorde();
	bool hayTierra = false;
	int posVec = 0;
	int k = 0;
	int contCol = 0;

	b2Vec2* vecBorde = new b2Vec2[anchoMatriz*2];
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0,0);
	bodyDef.angle = 0;
	b2Body*	body = world->CreateBody(&bodyDef);

	// Recorro la matriz hasta encontrar tierra
	for (int i = 0; i < anchoMatriz; i++){
		hayTierra = false;
		int contCol = 0;
		for (int j = 0; ((j < altoMatriz) && !(hayTierra)); j++){
			//Encuentro el borde y genero un chain
			if (matrizTerreno[i][j] == tierra){
				if (aguasProfundas){
					aguasProfundas = false;
					vecBorde[posVec].Set(i,-100);
					posVec++;
				}
				hayTierra = true;
				//Seteo las cordenadas del borde en el vector
				vecBorde[posVec].Set(i,j);
				posVec++;
				k++;
			} else {
				contCol++;
			}

		}

		if ((contCol == altoMatriz) && (huboTierra) && ( k != tamanioBorde)){
			vecBorde[posVec].Set(i-1,-100);
			posVec++;
			aguasProfundas = true;
			huboTierra = false;
		}
	}

	b2ChainShape chain;
	chain.CreateChain(vecBorde, posVec);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &chain;
	body->CreateFixture(&fixtureDef);

	delete lectorT;
	delete vecBorde;
}

Terreno::~Terreno(void)
{
}
