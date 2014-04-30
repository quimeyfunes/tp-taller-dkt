#include "Paquete.h"

Paquete::Paquete(){
}

void Paquete::serializar(char * data){

	int offset = 0;
	memcpy(data, &this->tipo, sizeof(this->tipo));
	offset += sizeof(this->tipo);
	memcpy(data + offset, &this->tamanio, sizeof(this->tamanio));
	offset += sizeof(this->tamanio);
	//strcpy(data+offset, this->mensaje);
	memcpy(data + offset, this->mensaje, this->tamanio);


}

void Paquete::deserializar(char * data){
	//
	int offset = 0;
	memcpy(&this->tipo, data, sizeof(this->tipo));
	offset += sizeof(this->tipo);
	memcpy(&this->tamanio, data + offset, sizeof(this->tamanio));
	offset += sizeof(this->tamanio);
	this->mensaje= data+offset;
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



void Paquete::setMensaje(string msg){
	this->mensaje = (char*)StringUtil::stringToChar(msg);
}

void Paquete::setMensaje(char* msg){
	this->mensaje = msg;
}

char* Paquete::getMensajeChar(){
	return this->mensaje;
}

string Paquete::getMensaje(){
	string m(this->mensaje);
	return m;
}

int Paquete::getTamanio(){
	return this->tamanio;
}







void Paquete::setTamanio(int num){
	this->tamanio = num;
}