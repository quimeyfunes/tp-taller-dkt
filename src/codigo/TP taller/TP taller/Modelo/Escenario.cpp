#include "Escenario.h"

Escenario::Escenario(){
}

Escenario::Escenario(int altoPx,int anchoPx,int altoU,int anchoU,int nivelAgua,string imagenTierra,string imagenCielo){
	this->altoPx = altoPx;
	this->anchoPx = anchoPx;
	this->altoU = altoU;
	this->anchoU = anchoU;
	this->nivelAgua = nivelAgua;
	this->imagenTierra = imagenTierra;
	this->imagenCielo = imagenCielo;
	this->listaFiguras = new list<Figura*>();
}

int Escenario::getAltoPx(){
	return altoPx;
}

int Escenario::getAnchoPx(){
	return anchoPx;
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

string Escenario::getImagenTierra(){
	return imagenTierra;
}

string Escenario::getImagenCielo(){
	return imagenCielo;
}

void Escenario::agregarFigura(Figura* figura) {
	this->listaFiguras->push_back(figura);
}
