#include "CirculoDibujable.h"


CirculoDibujable::CirculoDibujable(int x,int y, int radioHorizontal,int radioVertical)
{
	this->posicion.x = x;
	this->posicion.y = y;
	this->radioHorizontal = radioHorizontal;
	this->radioVertical = radioVertical;
};

CirculoDibujable::CirculoDibujable() {
}

CirculoDibujable::~CirculoDibujable() {
}



void CirculoDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	this->posicion.x = fig->getPosicion().x * relacionPPU;//Escenario::getRelacionAncho();
	this->posicion.y = fig->getPosicion().y * relacionPPU;//Escenario::getRelacionAlto();
}

void CirculoDibujable::dibujar(SDL_Renderer* renderer,int corrimientoX, int corrimientoY, int escalaZoom, int posZoomX, int posZoomY) {
	filledEllipseRGBA(renderer,this->posicion.x + corrimientoX,this->posicion.y + corrimientoY,this->radioHorizontal,this->radioVertical,this->getColor()[0],this->getColor()[1],this->getColor()[2],255);
}

string CirculoDibujable::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoCirculoDibujable);
	serializado += separadorCampoTipoEntidades;    
	serializado += StringUtil::int2string(this->radioHorizontal);
	serializado += separadorCamposEntidades;    
	serializado += StringUtil::int2string(this->radioVertical);;
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->posicion.x);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->posicion.y);;
	return serializado;
}
void CirculoDibujable::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);
    this->radioHorizontal = StringUtil::str2int(atributos.at(0));
    this->radioVertical = StringUtil::str2int(atributos.at(1));
	this->posicion.x = StringUtil::str2int(atributos.at(2));
	this->posicion.y = StringUtil::str2int(atributos.at(3));
}