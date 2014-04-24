#include "Paquete.h"

Paquete::Paquete(){
}

void Paquete::serializar(char * data){
	memcpy(data, this, 500);
}

void Paquete::deserializar(char * data){
	memcpy(this, data, 500);
}

int Paquete::getTipo(){
	return this->tipo;
}

void Paquete::setTipo(int tipo){
	this->tipo = tipo;
}

string Paquete::getMensaje(){
	return this->mensaje;
}

void Paquete::setMensaje(string msg){
	this->mensaje = msg;
}

int Paquete::getTamanio(){
	return this->tamanio;
}

void Paquete::setTamanio(int num){
	this->tamanio = num;
}