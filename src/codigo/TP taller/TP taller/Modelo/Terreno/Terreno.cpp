#include "Terreno.h"



Terreno::Terreno(b2World* world){

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0,0);
	bodyDef.angle = 0;
	this->body = world->CreateBody(&bodyDef);
}


void Terreno::generarTerreno(string nombreArchivo){

	this->lectorTerreno = new LectorTerreno(nombreArchivo);
	bool** matrizTerreno = lectorTerreno->getMatrizTerreno();
	int anchoMatriz = lectorTerreno->getAnchoMatriz();
	int altoMatriz = lectorTerreno->getAltoMatriz();
	bool tierra = true;
	bool huboTierra = false;
	bool aguasProfundas = false;
	int tamanioBorde = lectorTerreno->getTamanoBorde();
	bool hayTierra = false;
	int posVec = 0;
	int k = 0;
	int contCol = 0;


	b2Vec2* vecBorde = new b2Vec2[anchoMatriz*2];
	
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	// Recorro la matriz hasta encontrar tierra
	float relacionAncho = e->anchoPx / e->anchoU;
	float relacionAlto = e->altoPx / e->altoU;
	for (int i = 0; i < anchoMatriz; i+=relacionAncho){
		hayTierra = false;
		int contFil = 0;
		for (int j = 0; ((j < altoMatriz) && !(hayTierra)); j++){
			//Encuentro el borde y genero un chain
			if (matrizTerreno[i][j] == tierra){
				if (aguasProfundas){
					aguasProfundas = false;
					vecBorde[posVec].Set(i / relacionAncho,1000);
					posVec++;
				}
				hayTierra = true;
				huboTierra = true;
				//Seteo las cordenadas del borde en el vector
				vecBorde[posVec].Set(i /relacionAncho,j /relacionAlto);
				posVec++;
				k++;
			} else {
				contFil++;
			}

		}

		if ((contFil == altoMatriz) && (huboTierra) && ( k != tamanioBorde)){
			vecBorde[posVec].Set((i- relacionAncho) / relacionAncho,1000);
			posVec++;
			aguasProfundas = true;
			huboTierra = false;
		}
	}

	b2ChainShape chain;
	chain.CreateChain(vecBorde, posVec);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &chain;
	fixtureDef.restitution = 0;
	this->body->CreateFixture(&fixtureDef);


	delete vecBorde;
}

b2Body* Terreno::getBody(){
	return this->body;
}

LectorTerreno* Terreno::getLector(){

	return this->lectorTerreno;
}

LectorTerreno* Terreno::getLectorTerreno(){
	return this->lectorTerreno;
}

Terreno::~Terreno(void)
{
	delete this->lectorTerreno;
}
