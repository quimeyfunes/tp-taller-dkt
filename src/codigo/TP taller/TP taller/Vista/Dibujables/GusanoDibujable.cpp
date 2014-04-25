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

void GusanoDibujable::dibujar(SDL_Renderer* renderer, int corrimiento , float escalaZoom, int posZoomX, int posZoomY){
	
	SDL_Rect rect = this->rect;
	rect.x += corrimiento;

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax) && (escalaZoom >= zoomMin)) {
		rect = realizarZoom(rect, posZoomX, posZoomY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->imagen,NULL, &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
	}
}