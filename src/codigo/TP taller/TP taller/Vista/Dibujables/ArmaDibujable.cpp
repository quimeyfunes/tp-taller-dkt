#include "ArmaDibujable.h"


ArmaDibujable::ArmaDibujable(void)
{
	this->imagen = NULL;
}

ArmaDibujable::ArmaDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
	this->rutaImagen = pathImagen;
}

void ArmaDibujable::actualizar(Observable* observable){
}

void ArmaDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	
	SDL_Rect rect = this->rect;
	rect.x -= corrimientoX;
	rect.y -= corrimientoY;

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->imagen, NULL, &rect, this->anguloRotacion,NULL,SDL_FLIP_NONE);
	}
}

ArmaDibujable::~ArmaDibujable(void)
{
}

string ArmaDibujable::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoArmaDibujable);
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
	serializado += separadorCamposEntidades;
	serializado += this->rutaImagen;
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->destruido);

	return serializado;
}

void ArmaDibujable::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);
	this->anguloRotacion = StringUtil::str2float(atributos.at(0).c_str());
    SDL_Rect rectAux;
	rectAux.x = StringUtil::str2int(atributos.at(1));
	rectAux.y = StringUtil::str2int(atributos.at(2));
	rectAux.w = StringUtil::str2int(atributos.at(3));
	rectAux.h = StringUtil::str2int(atributos.at(4));
	this->setRect(rectAux);

	this->rutaImagen = atributos.at(5);
	this->destruido = StringUtil::str2int(atributos.at(6));
}
