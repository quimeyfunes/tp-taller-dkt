#include "Evento.h"

Evento::Evento(void){
}

Evento::~Evento(void){
}
string Evento::serializar(){
	string serializado = "";
	serializado += StringUtil::int2string(this->accion);
	if(this->accion == CLICK){
		//Si hubo click tengo que decir las posiciones
		serializado += separadorCamposEntidades;
		serializado += StringUtil::int2string(this->x);
		serializado += separadorCamposEntidades;
		serializado += StringUtil::int2string(this->y);
	}
	return serializado;
}

void Evento::deserealizar(string aDeserealizar){
	vector<string> atributos = StringUtil::split(aDeserealizar,separadorCamposEntidades);
	this->accion = StringUtil::str2int(atributos.at(0));
	if(this->accion == CLICK){
		this->x = StringUtil::str2int(atributos.at(1));
		this->y = StringUtil::str2int(atributos.at(2));
	}
}