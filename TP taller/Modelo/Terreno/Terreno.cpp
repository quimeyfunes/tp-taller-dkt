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
	this->terreno.clear();
	for (int i = 0 ; i < posVec ; i++) {
		b2Vec2 vec = vecBorde[i];
		bg::append(this->terreno,Punto (vec.x,vec.y));
	}
	//Por si hubo aguas profundas en 1000 agrego un punto por abajo
	/*bg::append(this->terreno,Punto(vecBorde[posVec-1].x,1001)); 
	//Cierro poligono
	bg::append(this->terreno,Punto (vecBorde[0].x,1001)); 
	bg::append(this->terreno,Punto (vecBorde[0].x,vecBorde[0].y));*/
	//cout<<this->terreno.outer().size()<<endl;
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


	int n = 200; //Aproximo lados = radio para circulo
	PoligonoBoost circulo;
	float angulo = (2 * b2_pi / n);
	for (int i = 0; i < n; i++) {
		bg::append(circulo,Punto (radio * cos(angulo*i) + x, radio * sin(angulo*i) + y)); 
	}
	bg::append(circulo,Punto (radio * cos(angulo) + x, radio * sin(angulo) + y)); 
	list<PoligonoBoost> output;

	try {
		bg::difference(circulo,this->terreno,output);
	} catch (exception &e) {
		cout<<"FFF"<<endl;
	}
	
	cout<<this->terreno.outer().size()<<endl;
	if (output.size() != 0) {
		//for (b2Fixture* f = this->body->GetFixtureList(); f; f = f->GetNext()) {
			//if ( f != NULL)
				this->body->DestroyFixture(this->body->GetFixtureList());
		//}//Destruyo la fixture y el poligonoBoost
		cout<<"(Y) "<<endl;
		this->terreno.clear();
		cout<<this->terreno.outer().size()<<endl;
	} else {
		list<PoligonoBoost> output2;
		try {
			bg::difference(this->terreno,circulo,output2);
		}catch (exception &e) {
		}
		cout<<"CantOutput: "<<output2.size()<<"  ";
		BOOST_FOREACH(PoligonoBoost const& p, output2) {
			//vector<Punto> const& vertices = p.inners();
			cout<<p.outer().size()<<endl;
		}
		/*cout<<output.size()<<endl;
		this->body->DestroyFixture(this->body->GetFixtureList());
		this->terreno.clear();*/
	}
	circulo.clear();
	
	b2FixtureDef fixtureDef;
	
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	
	BOOST_FOREACH(PoligonoBoost const& p, output)
    {
		
		vector<Punto> const& vertices = p.outer();
		b2Vec2* vecBorde = new b2Vec2[vertices.size()];
		//Paso puntos de poligono a vector para crear chain
		Punto puntoAux(0,0);
		int puntos = 0;
		for (int j = 0; j < vertices.size() - 1; j++) 
		{ 
			Punto punto = vertices[j];
			//float dist = bg::distance(punto,puntoAux);
			if (bg::distance(punto,puntoAux) > (0.5)) {
				vecBorde[puntos].Set(punto.get<0>(),punto.get<1>());
				puntoAux = punto;
				puntos++;
			}
		}

		if ((puntos > 1) && (vecBorde != NULL)) {
			cout << "Puntos: " << puntos << "VecBorde: " << vecBorde << endl;
			b2ChainShape chain;
			chain.CreateChain(vecBorde,puntos);
			fixtureDef.shape = &chain;
			this->body->CreateFixture(&fixtureDef);

			for (int i = 0 ; i < puntos - 1; i++) {
				b2Vec2 vec = vecBorde[puntos - 1 - i];
				bg::append(this->terreno,Punto (vec.x,vec.y));
			}
		}
		delete vecBorde;

    }
	
}