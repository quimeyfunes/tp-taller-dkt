#include "GusanoDibujable.h"


GusanoDibujable::GusanoDibujable(SDL_Renderer* renderer, SDL_Rect rect,string pathImagen, string pathDEF) 
	: DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
}

GusanoDibujable::GusanoDibujable()
{
}


GusanoDibujable::~GusanoDibujable()
{
}

void GusanoDibujable::actualizar(Observable* observable) {

	Figura* fig = (Figura*)observable;
	SDL_Rect rect = this->rect;
	rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
	rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
	this->setRect(rect);
	
}

void GusanoDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	
	SDL_Rect rect = this->rect;
	//cout<<"X"<<rect.x + rect.w/2<<"  Y:  "<<rect.y + rect.h/2<<endl;
	rect.x -= corrimientoX;
	rect.y -= corrimientoY;

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax) && (escalaZoom >= zoomMin)) {
		rect = realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->imagen, NULL, &rect, this->anguloRotacion,NULL,SDL_FLIP_NONE);
	}
}

string GusanoDibujable::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoGusanoDibujable);
	serializado += separadorCampoTipoEntidades;    
	serializado += StringUtil::float2string(this->anguloRotacion);
	serializado += separadorCamposEntidades;    
	serializado += StringUtil::int2string(this->rect.x);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->rect.y);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.w);
	serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.h);
	return serializado;
}

void GusanoDibujable::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);
	this->setAngulo(StringUtil::str2float(atributos.at(0).c_str()));
    SDL_Rect rectAux;
	rectAux.x = StringUtil::str2int(atributos.at(1));
	rectAux.y = StringUtil::str2int(atributos.at(2));
	rectAux.w = StringUtil::str2int(atributos.at(3));
	rectAux.h = StringUtil::str2int(atributos.at(4));
	this->setRect(rectAux);
}