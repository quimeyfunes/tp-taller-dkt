#include "Terreno.h"



Terreno::Terreno(b2World* world){

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0,0);
	bodyDef.angle = 0;
	this->body = world->CreateBody(&bodyDef);
	this->terreno = new list<PoligonoBoost>;
}


void Terreno::generarTerreno(string nombreArchivo){

	this->lectorTerreno = new LectorTerreno(nombreArchivo);
	pixel** matrizTerreno = lectorTerreno->getMatrizTerreno();
	int anchoMatriz = lectorTerreno->getAnchoMatriz();
	int altoMatriz = lectorTerreno->getAltoMatriz();
	bool huboTierra = false;
	bool aguasProfundas = false;
	int tamanioBorde = lectorTerreno->getTamanoBorde();
	bool hayTierra = false;
	int posVec = 0;
	int k = 0;
	int contCol = 0;


	b2Vec2* vecBorde = new b2Vec2[anchoMatriz*2];
	

	// Recorro la matriz hasta encontrar tierra
	for (int i = 0; i < anchoMatriz; i+=relacionPPU) { 
		hayTierra = false;
		int contFil = 0;
		for (int j = 0; ((j < altoMatriz) && !(hayTierra)); j++){
			//Encuentro el borde y genero un chain
			if (lectorTerreno->esTierra(matrizTerreno[i][j])){
				if (aguasProfundas){
					aguasProfundas = false;
					vecBorde[posVec].Set(i / relacionPPU,1000);
					posVec++;
				}

				hayTierra = true;
				huboTierra = true;
				//Seteo las cordenadas del borde en el vector
				if (posVec == 0) {
					vecBorde[posVec].Set(i /relacionPPU,1000);
					posVec++;
				}
				vecBorde[posVec].Set(i /relacionPPU,j /relacionPPU);
				posVec++;
				k++;
			} else {
				contFil++;
			}

		}

		if ((contFil == altoMatriz) && (huboTierra) && ( k != tamanioBorde)){
			vecBorde[posVec].Set((i- relacionPPU) / relacionPPU,1000);
			posVec++;
			aguasProfundas = true;
			huboTierra = false;
		}
	}

	b2ChainShape chain;
	chain.CreateChain(vecBorde, posVec);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &chain;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	this->body->CreateFixture(&fixtureDef);

	//Paso chain a poligono de boost
	PoligonoBoost poligono;
	for (int i = 0 ; i < posVec ; i++) {
		b2Vec2 vec = vecBorde[i];
		bg::append(poligono,Punto (vec.x,vec.y));
	}
	bg::correct(poligono);
	this->terreno->push_back(poligono);
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

void Terreno::destruirTerreno(float x, float y, int radio){
	//Destruyo fixtures 
	b2Fixture* fProxima;
	for (b2Fixture* f = this->body->GetFixtureList(); f; f = fProxima) {
		fProxima = f->GetNext();
		this->body->DestroyFixture(f);
	}
		
	//Creo circulo. Aproximo lados = 40 para circulo 
	int n = 40; 
	PoligonoBoost circulo;
	float angulo = (2 * b2_pi / n);
	for (int i = 0; i < n; i++) {
		bg::append(circulo,Punto (radio * cos(angulo*i) + x, radio * sin(angulo*i) + y)); 
	}
	//bg::append(circulo,Punto (radio * cos(angulo) + x, radio * sin(angulo) + y)); 
	bg::correct(circulo);

	int i = 0;
	int tamanio = this->terreno->size();
	for (list<PoligonoBoost>::iterator it = this->terreno->begin(); i < tamanio; it++) {
		i++;
		list<PoligonoBoost> output;
		bg::difference((*it),circulo,output);

		b2FixtureDef fixtureDef;
	
		fixtureDef.restitution = restitucion;
		fixtureDef.friction = friccion;
		
		//Itero sobre output y creo una chain por cada poligono de output. 
		BOOST_FOREACH(PoligonoBoost const& p, output)
		{
			//PoligonoBoost poligono;
			vector<Punto> const& vertices = p.outer();
			b2Vec2* vecBorde = new b2Vec2[vertices.size()];
			//Paso puntos de poligono a vector para crear chain
			Punto puntoAux(0,0);
			int puntos = 0;
			for (int j = 0; j < vertices.size() - 1; j++) 
			{ 
				Punto punto = vertices[j];
				if (bg::distance(punto,puntoAux) > 0.5) {
					vecBorde[puntos].Set(punto.get<0>(),punto.get<1>());
					puntoAux = punto;
					puntos++;
					//bg::append(poligono,punto);
				}
			}

			if ((puntos > 1) && (vecBorde != NULL)) {
				b2ChainShape chain;
				chain.CreateChain(vecBorde,puntos);
				fixtureDef.shape = &chain;
				this->body->CreateFixture(&fixtureDef);

				//bg::correct(poligono);
				this->terreno->push_back(p);
			}
			delete vecBorde;
		}
	}
	
	//Saco los primeros poligonos, que serian los viejos. 
	i = 0;
	for (int i = 0; i < tamanio; i++) {
		this->terreno->pop_front();
	}
}