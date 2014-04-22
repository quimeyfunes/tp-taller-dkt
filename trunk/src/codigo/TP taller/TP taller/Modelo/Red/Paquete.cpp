#include "Paquete.h"

Paquete::Paquete(){
}

void Paquete::serializar(char * data){
	memcpy(data, this, sizeof(Paquete));
}

void Paquete::deserializar(char * data){
	memcpy(this, data, sizeof(Paquete));
}

int Paquete::getTipo(){
	return this->tipo;
}

void Paquete::setTipo(int tipo){
	this->tipo = tipo;
}

char* Paquete::getMensaje(){
	return this->mensaje;
}

void Paquete::setMensaje(char* msg){
	this->mensaje = msg;
}