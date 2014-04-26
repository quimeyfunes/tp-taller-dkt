#include "GusanoDibujable.h"


GusanoDibujable::GusanoDibujable(SDL_Renderer* renderer, SDL_Rect rect)
{
	this->rect = rect;
	this->imagen = IMG_LoadTexture(renderer, rutaGusano);
	string pathImagen = "imagenes/texturas/worm.png";
	//this->superficie = IMG_Load(pathImagen.c_str());
	if(!imagen){
		Logger::getLogger()->escribir("No se encontró la textura " + pathImagen + ". Se usará la textura por defecto.");
		this->imagen = IMG_LoadTexture(renderer, rutaGusanoDEF);
	}
	this->anguloRotacion = 0;
}


GusanoDibujable::~GusanoDibujable()
{
	delete this->imagen;
}

void GusanoDibujable::actualizar(Observable* observable) {

	Figura* fig = (Figura*)observable;
	SDL_Rect rect = this->rect;
	rect.x = ((fig->getPosicion().x * Escenario::getRelacionAncho()) - rect.w /2);
	rect.y = ((fig->getPosicion().y * Escenario::getRelacionAlto()) - rect.h /2);
	this->rect = rect;
	
}

void GusanoDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int posZoomX, int posZoomY){
	
	SDL_Rect rect = this->rect;
	rect.x += corrimientoX;
	rect.y += corrimientoY;

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax) && (escalaZoom >= zoomMin)) {
		rect = realizarZoom(rect, posZoomX, posZoomY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->imagen,NULL, &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
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
	this->anguloRotacion = StringUtil::str2float(atributos.at(0).c_str());
    this->rect.x = StringUtil::str2int(atributos.at(1));
	this->rect.y = StringUtil::str2int(atributos.at(2));
	this->rect.w = StringUtil::str2int(atributos.at(3));
	this->rect.h = StringUtil::str2int(atributos.at(4));
}