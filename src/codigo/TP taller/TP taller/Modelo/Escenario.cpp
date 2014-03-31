#include "Escenario.h"

Escenario::Escenario(){
}

Escenario::Escenario(int altoU,int anchoU,int nivelAgua){
	this->altoU = altoU;
	this->anchoU = anchoU;
	this->nivelAgua = nivelAgua;
	this->listaFiguras = new list<Figura*>();
	
	b2Vec2* gravity = new b2Vec2(gravedadX, gravedadY);
	//// Construct a world object, which will hold and simulate the rigid bodies.
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
	this->agregarFigura(poligono);
	return poligono;
}

Circulo* Escenario::crearCirculo(ObjetoParseado objeto){
	Circulo* circulo = new Circulo(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.escala,objeto.masa);
	this->agregarFigura(circulo);
	return circulo;
}

Rectangulo* Escenario::crearRectangulo(ObjetoParseado objeto){
	Rectangulo* rectangulo = new Rectangulo(objeto.x,objeto.y,objeto.rotacion,this->world,objeto.estatico,objeto.ancho,objeto.alto,objeto.masa);
	if (this->haySuperposicion(rectangulo)){
		//Remuevo figura del world
		rectangulo->getBody()->GetWorld()->DestroyBody(rectangulo->getBody());
		std::stringstream info;
		info << "Error al agregar figura: la figura de la linea " << objeto.linea << " se superpone con una agregada con anterioridad.";
		Logger::getLogger()->escribir(info.str());
		Logger::getLogger()->guardarEstado();
		return NULL;
	} else {
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
			cuerpo->SetLinearVelocity(b2Vec2(velocidad.x * 0.95,velocidadY));
		}
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
