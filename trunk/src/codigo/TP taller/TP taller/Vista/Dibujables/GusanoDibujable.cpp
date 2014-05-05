#include "GusanoDibujable.h"


GusanoDibujable::GusanoDibujable(SDL_Renderer* renderer, SDL_Rect rect,string pathImagen, string pathDEF) 
	: DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
	this->cambiarImgDer = false;
	this->cambiarImgIzq = false;
	this->contIzq = 0;
	this->contDer = 0;
	this->contFrent = 0;

	SDL_Rect rectCart = rect;
	rectCart.w = rect.w;
	rectCart.h = rect.h / 4;

	this->cartel = new CartelDibujable(renderer, rectCart, rutaCartel, rutaCartelDEF);
	
}

GusanoDibujable::GusanoDibujable()
{
}


GusanoDibujable::~GusanoDibujable()
{
	delete this->cartel;
}

void GusanoDibujable::actualizar(Observable* observable) {

	Figura* fig = (Figura*)observable;

	if ( !(fig->seMueveALaDer() ) && !(fig->seMueveALaIzq()) ) {
		this->contFrent++;
		this->contIzq = 0;
		this->contDer = 0;
		this->setCambiarImgDer(false);
		this->setCambiarImgIzq(false);
	} 
	if ((fig->seMueveALaDer())){
			this->contIzq = 0;
			this->contFrent = 0;
			this->contDer++;
			this->setCambiarImgDer(true);
			this->setCambiarImgIzq(false);
	}
	if ((fig->seMueveALaIzq())){
			this->contDer = 0;
			this->contFrent = 0;
			this->contIzq++;
			this->setCambiarImgDer(false);
			this->setCambiarImgIzq(true);
	}

	SDL_Rect rect = this->rect;
	rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
	rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
	this->setRect(rect);
	
	SDL_Rect rectCartel = this->cartel->getRect();
	rectCartel.x = (fig->getPosicion().x * relacionPPU) - rect.w / 2;
	rectCartel.y = ((fig->getPosicion().y * relacionPPU) - rect.h / 2) - alturaCartel;
	this->cartel->setRect(rectCartel);


}

void GusanoDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	
	SDL_Rect rect = this->rect;
	rect.x -= corrimientoX;
	rect.y -= corrimientoY;

	if ( !(this->hayCambioImgDer()) && !(this->hayCambioImgIzq()) && (this->contFrent == 1) )
		this->setImagen(renderer, rutaGusano);
	if ( (this->hayCambioImgDer()) && (this->contDer == 1 ) )
		this->setImagen(renderer, rutaGusanoDer);
	if ( (this->hayCambioImgIzq())  && (this->contIzq == 1) )
		this->setImagen(renderer, rutaGusanoIzq);
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
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

void GusanoDibujable::setCambiarImgDer(bool cambio){
	this->cambiarImgDer = cambio;
}

bool GusanoDibujable::hayCambioImgDer(){
	return this->cambiarImgDer;
}

void GusanoDibujable::setCambiarImgIzq(bool cambio){
	this->cambiarImgIzq = cambio;
}

bool GusanoDibujable::hayCambioImgIzq(){
	return this->cambiarImgIzq;
}

CartelDibujable* GusanoDibujable::getCartel(){
	return this->cartel;
}