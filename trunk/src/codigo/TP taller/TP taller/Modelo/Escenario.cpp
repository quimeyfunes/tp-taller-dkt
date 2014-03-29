#include "Escenario.h"

Escenario::Escenario(){
}

Escenario::Escenario(int altoU,int anchoU,int nivelAgua){
	this->altoU = altoU;
	this->anchoU = anchoU;
	this->nivelAgua = nivelAgua;
	this->listaFiguras = new list<Figura*>();
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

void Escenario::agregarFigura(Figura* figura) {
	this->listaFiguras->push_back(figura);
}

void Escenario::notificar() {
	for (list<Figura*>::iterator it = this->listaFiguras->begin(); it != this->listaFiguras->end(); it++) {
		(*it)->notificar();
	}


}

void Escenario::crearPoligono(ObjetoParseado objeto, b2World* world){
	Poligono* poligono = new Poligono(objeto.x,objeto.y,objeto.rotacion,world,objeto.estatico,objeto.escala,objeto.masa,objeto.tipo);
	this->agregarFigura(poligono);
}

void Escenario::crearCirculo(ObjetoParseado objeto, b2World* world){
	Circulo* circulo = new Circulo(objeto.x,objeto.y,objeto.rotacion,world,objeto.estatico,objeto.escala,objeto.masa);
	this->agregarFigura(circulo);
}

void Escenario::crearRectangulo(ObjetoParseado objeto, b2World* world){
	Rectangulo* rectangulo = new Rectangulo(objeto.x,objeto.y,objeto.rotacion,world,objeto.estatico,objeto.ancho,objeto.alto,objeto.masa);
	this->agregarFigura(rectangulo);
}
