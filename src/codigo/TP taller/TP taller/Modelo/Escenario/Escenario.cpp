#include "Escenario.h"

Escenario::Escenario(){
}

Escenario::Escenario(int altoU,int anchoU,int nivelAgua){
	this->altoU = altoU;
	this->anchoU = anchoU;
	this->nivelAgua = nivelAgua;
	this->listaFiguras = new list<Figura*>();
	
	b2Vec2* gravity = new b2Vec2(gravedadX, gravedadY);
	this->world = new b2World(*gravity);
}



int Escenario::getAltoU(){
	return altoU;
}

int Escenario::getAnchoU(){
	return anchoU;
}

int Escenario::getNivelAgua(){
	return nivelAgua;
}

list<Figura*>* Escenario::getFiguras() {
	return this->listaFiguras;
}

b2World* Escenario::getWorld() {
	return this->world;
}

void Escenario::agregarFigura(Figura* figura) {
	this->listaFiguras->push_back(figura);
}

void Escenario::notificar() {
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->notificar();
	}


}

Poligono* Escenario::crearPoligono(ObjetoParseado objeto){
	Poligono* poligono = new Poligono(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.escala,objeto.masa,objeto.tipo);
	if (this->haySuperposicion(poligono)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(poligono->getBody());
		std::stringstream info;
		info << "Error al agregar figura: la figura de la linea " << objeto.linea << " se superpone con una agregada con anterioridad.";
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else if (this->haySuperposicionConTerreno(poligono)) {
		//Remuevo figura del world
		this->getWorld()->DestroyBody(poligono->getBody());
		std::stringstream info;
		info << "Error al agregar figura: la figura de la linea " << objeto.linea << " se superpone con el terreno.";
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else {
		this->agregarFigura(poligono);
		return poligono;
	}
}

Circulo* Escenario::crearCirculo(ObjetoParseado objeto){
	Circulo* circulo = new Circulo(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.escala,objeto.masa);
	if (this->haySuperposicion(circulo)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(circulo->getBody());
		std::stringstream info;
		info << "Error al agregar figura: la figura de la linea " << objeto.linea << " se superpone con una agregada con anterioridad.";
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else if (this->haySuperposicionConTerreno(circulo)) {
		//Remuevo figura del world
		this->getWorld()->DestroyBody(circulo->getBody());
		std::stringstream info;
		info << "Error al agregar figura: la figura de la linea " << objeto.linea << " se superpone con el terreno.";
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else {
		this->agregarFigura(circulo);
		return circulo;
	}
}

Rectangulo* Escenario::crearRectangulo(ObjetoParseado objeto){
	Rectangulo* rectangulo = new Rectangulo(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.ancho,objeto.alto,objeto.masa);
	if (this->haySuperposicion(rectangulo)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(rectangulo->getBody());
		std::stringstream info;
		info << "Error al agregar figura: la figura de la linea " << objeto.linea << " se superpone con una agregada con anterioridad.";
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else if(this->haySuperposicionConTerreno(rectangulo)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(rectangulo->getBody());
		std::stringstream info;
		info << "Error al agregar figura: la figura de la linea " << objeto.linea << " se superpone con el terreno.";
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else {
		this->agregarFigura(rectangulo);
		return rectangulo;
	}
}

void Escenario::simularAgua () {
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		b2Body* cuerpo = (*it)->getBody();
		if ((*it)->getPosicion().y > this->nivelAgua) {
			b2Vec2 velocidad = cuerpo->GetLinearVelocity();
			float velocidadY = velocidad.y;
			if (velocidadY > velocidadAgua) {
				if (velocidadY * 0.975 < velocidadAgua)  {
					velocidadY = velocidadAgua;
				} else {
					velocidadY = velocidadY * 0.975;
				}
			}
			cuerpo->SetLinearVelocity(b2Vec2(velocidad.x * 0.975,velocidadY));
		}
	}
}

void Escenario::reiniciar(){
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->reiniciar();
	}
}

bool Escenario::haySuperposicion(Figura* figura){
	bool chocan = false;
	for (std::list<Figura*>::const_iterator iterator = this->listaFiguras->begin(); iterator != this->listaFiguras->end(); ++iterator) {
		Figura* figuraActual = *iterator;
		chocan = b2TestOverlap(figura->getBody()->GetFixtureList()->GetShape(),0,figuraActual->getBody()->GetFixtureList()->GetShape(),0,figura->getBody()->GetTransform(),figuraActual->getBody()->GetTransform());
		if(chocan){
			break;
		}
	}

	return chocan;
}

bool Escenario::haySuperposicionConTerreno(Figura* figura){
	//Primero chequeo si la figura se superpone con la cadena
	Terreno* terreno = this->getTerreno();
	bool chocan = b2TestOverlap(figura->getBody()->GetFixtureList()->GetShape(),0,terreno->getBody()->GetFixtureList()->GetShape(),0,figura->getBody()->GetTransform(),terreno->getBody()->GetTransform());
	if(chocan){
		return true;
	}
	//Si no choca con los bordes del terreno tengo que chequear con la matriz
	bool** matrizTerreno = terreno->getLectorTerreno()->getMatrizTerreno();
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	b2Vec2 posicion = figura->getBody()->GetPosition();
	float x = (posicion.x * e->anchoPx)/this->anchoU;
	float y = (posicion.y * e->altoPx)/this->altoU;
	//Si x coincide con el ancho de la matriz, le resto uno para que no se vaya de rango
	if (x == terreno->getLectorTerreno()->getAnchoMatriz()) x--;
	//Si y coincide con el alto de la matriz, le resto uno para que no se vaya de rango
	if (y == terreno->getLectorTerreno()->getAltoMatriz()) y--;
	if(matrizTerreno[(int) floor(x)][(int) floor(y)]){
		//Como hay un 1 quiere decir que el centro esta dentro del terreno	
		return true;
	}
	return false;
}

Terreno* Escenario::getTerreno(){
	return this->terreno;
}

void Escenario::setTerreno(Terreno* terreno){
	this->terreno = terreno;
}
