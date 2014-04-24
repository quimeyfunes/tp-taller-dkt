#include "escenario.h"

float Escenario::relacionAncho = 0;
float Escenario::relacionAlto = 0;

Escenario::Escenario(){
}

Escenario::Escenario(int altoU,int anchoU,int nivelAgua, float relacionAncho, float relacionAlto){
	this->altoU = altoU;
	this->anchoU = anchoU;
	this->nivelAgua = nivelAgua;
	this->relacionAncho = relacionAncho;
	this->relacionAlto = relacionAlto;
	this->listaFiguras = new list<Figura*>();
	
	b2Vec2* gravity = new b2Vec2(gravedadX, gravedadY);
	this->world = new b2World(*gravity);

	this->figuraActiva = NULL;
	this->puedeMoverseArriba = false;
	this->puedeMoverseDerecha = false;
	this->puedeMoverseIzquierda = false;
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
	this->saltar();
	this->moverDerecha();
	this->moverIzquierda();
}

Gusano* Escenario::crearGusano(ObjetoParseado objeto){
	Gusano* gusano = new Gusano(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.ancho,objeto.alto,objeto.masa);
	if (this->haySuperposicion(gusano)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(gusano->getBody());
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else if(this->haySuperposicionConTerreno(gusano)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(gusano->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else {
		this->agregarFigura(gusano);
		return gusano;
	}
}

Poligono* Escenario::crearPoligono(ObjetoParseado objeto){
	Poligono* poligono = new Poligono(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.escala,objeto.masa,objeto.tipo);
	if (this->haySuperposicion(poligono)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(poligono->getBody());
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else if (this->haySuperposicionConTerreno(poligono)) {
		//Remuevo figura del world
		this->getWorld()->DestroyBody(poligono->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
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
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else if (this->haySuperposicionConTerreno(circulo)) {
		//Remuevo figura del world
		this->getWorld()->DestroyBody(circulo->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
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
		std::stringstream info = this->getMensajeSuperposicionObjeto(objeto.linea);
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	}
	else if(this->haySuperposicionConTerreno(rectangulo)){
		//Remuevo figura del world
		this->getWorld()->DestroyBody(rectangulo->getBody());
		std::stringstream info = this->getMensajeSuperposicionTerreno(objeto.linea);
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
		if ((*it)->getPosicion().y > this->nivelAgua){
			b2Vec2 velocidad = cuerpo->GetLinearVelocity();
			float velocidadY = velocidad.y;
			if (velocidadY > velocidadAgua) {
				if (velocidadY * desaceleracionAgua < velocidadAgua)  {
					velocidadY = velocidadAgua;
				} else {
					velocidadY = velocidadY * desaceleracionAgua;
				}
			}
			cuerpo->SetLinearVelocity(b2Vec2(velocidad.x * desaceleracionAgua,velocidadY));
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
	b2ChainShape* shapeTerreno = (b2ChainShape*) terreno->getBody()->GetFixtureList()->GetShape();
	for (int i = 0; i < shapeTerreno->GetChildCount();i++) {
		bool chocan = b2TestOverlap(figura->getBody()->GetFixtureList()->GetShape(),0,shapeTerreno,i,figura->getBody()->GetTransform(),terreno->getBody()->GetTransform());
		if(chocan){
			return true;
		}
	}
	//Si no choca con los bordes del terreno tengo que chequear con la matriz
	bool** matrizTerreno = terreno->getLectorTerreno()->getMatrizTerreno();
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	b2Vec2 posicion = figura->getBody()->GetPosition();
	float x = posicion.x * e->anchoPx / this->anchoU;
	float y = posicion.y * e->altoPx / this->altoU;
	//Si x coincide con el ancho de la matriz, le resto uno para que no se vaya de rango
	if (x == terreno->getLectorTerreno()->getAnchoMatriz()) x--;
	//Si y coincide con el alto de la matriz, le resto uno para que no se vaya de rango
	if (y == terreno->getLectorTerreno()->getAltoMatriz()) y--;
	if(y >= terreno->getLectorTerreno()->getAltoMatriz() || x >= terreno->getLectorTerreno()->getAnchoMatriz() ||  y < 0 || x < 0){
		return false;
	}
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

float Escenario::getRelacionAncho(){
	return relacionAncho;
}

float Escenario::getRelacionAlto(){
	return relacionAlto;
}

std::stringstream Escenario::getMensajeSuperposicionObjeto(int linea){
	std::stringstream info;
	if(linea > 0){
		info << "Error al agregar figura: la figura de la linea " << linea << " se superpone con una agregada con anterioridad.";
	}else{
		info << "Error al agregar figura: la figura default se superpone con una agregada con anterioridad.";
	}

	return info;
}

std::stringstream Escenario::getMensajeSuperposicionTerreno(int linea){
	std::stringstream info;
	if(linea > 0){
		info << "Error al agregar figura: la figura de la linea " << linea << " se superpone con el terreno.";
	}else{
		info << "Error al agregar figura: la figura default se superpone con el terreno.";
	}

	return info;
}

void Escenario::click(float x, float y){
	for (std::list<Figura*>::const_iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		if ((*it)->getBody()->GetFixtureList()->GetShape()->TestPoint((*it)->getBody()->GetTransform(),b2Vec2(x,y))) {
			this->figuraActiva = (*it);
			return;
		}
	}
}

void Escenario::arriba(bool arriba){
	this->puedeMoverseArriba = arriba;
}

void Escenario::izquierda(bool izquierda){
	this->puedeMoverseIzquierda = izquierda;
}

void Escenario::derecha(bool derecha){
	this->puedeMoverseDerecha = derecha;
}

void Escenario::saltar(){
	if ((this->figuraActiva != NULL) && (this->puedeMoverseArriba)) {
		b2Body* cuerpo = this->figuraActiva->getBody();
		cuerpo->ApplyLinearImpulse(b2Vec2(0,-3),this->figuraActiva->getPosicion(),true);
	}
}

void Escenario::moverIzquierda(){
	if ((this->figuraActiva != NULL) && (this->puedeMoverseIzquierda)) {
		b2Body* cuerpo = this->figuraActiva->getBody();
		cuerpo->SetLinearVelocity(b2Vec2(-10,cuerpo->GetLinearVelocity().y));
	}
}

void Escenario::moverDerecha(){
	if ((this->figuraActiva != NULL) && (this->puedeMoverseDerecha)) {
		b2Body* cuerpo = this->figuraActiva->getBody();
		cuerpo->SetLinearVelocity(b2Vec2(10,cuerpo->GetLinearVelocity().y));
	}
}

