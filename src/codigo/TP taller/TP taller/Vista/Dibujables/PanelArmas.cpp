#include "PanelArmas.h"

PanelArmas::PanelArmas(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF):DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
	this->visible = false;
}


PanelArmas::~PanelArmas(){
}

void PanelArmas::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{
	if(this->visible){
		SDL_Rect rect = this->rect;
	
		if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
			//rect = this->realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
			SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
		} else {
			rect.x -= corrimientoX;
			rect.y -= corrimientoY;
			SDL_RenderCopyEx(renderer,this->imagen,NULL, &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
		}

		for(int i=0; i<this->armasTexturas.size(); i++){
			//this->armasTexturas[i]->dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
			SDL_RenderCopyEx(renderer,this->armasTexturas[i]->getImagen(),NULL, &this->armasTexturas[i]->getRect(),this->armasTexturas[i]->getAngulo(),NULL,SDL_FLIP_NONE);
		}
	}
}

void PanelArmas::alternarVisibilidad(){
	if(this->visible){
		this->visible = false;
	}else{
		this->visible = true;
	}
}


void PanelArmas::agregarArma(SDL_Renderer* renderer, string nombreArmaSeleccionada){
	SDL_Rect rect;
	int tamanoCuadradoX = this->getRect().w/4;
	int tamanoCuadradoY = this->getRect().h/4;
	int numeroCuadrado = this->armasTexturas.size();
	int x = this->getRect().x + tamanoCuadradoX * (numeroCuadrado%4);
	int y = this->getRect().y + tamanoCuadradoY * (numeroCuadrado/4);
	rect.x = x + 8;
	rect.y = y + 8;
	rect.w = tamanoCuadradoX - 16;
	rect.h = tamanoCuadradoY - 16;

	DibujableTextura* arma = new DibujableTextura(renderer, rect, nombreArmaSeleccionada, "");
	this->armasTexturas.push_back(arma);
}