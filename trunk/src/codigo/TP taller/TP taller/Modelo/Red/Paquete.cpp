#include "Paquete.h"

Paquete::Paquete(){
}

void Paquete::serializar(char * data){

	int offset = 0;
	memcpy(data, &this->tipo, sizeof(this->tipo));
	offset += sizeof(this->tipo);
	memcpy(data + offset, &this->tamanio, sizeof(this->tamanio));
	offset += sizeof(this->tamanio);
	strcpy(data+offset, this->mensaje.c_str());
}

void Paquete::deserializar(char * data){

	int offset = 0;
	memcpy(&this->tipo, data, sizeof(this->tipo));
	offset += sizeof(this->tipo);
	memcpy(&this->tamanio, data + offset, sizeof(this->tamanio));
	offset += sizeof(this->tamanio);
	string msg(data+offset);
	this->mensaje=msg;
}

int Paquete::getPesoPaquete(){
	return ( sizeof(int) + sizeof(int) + tamanio);
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