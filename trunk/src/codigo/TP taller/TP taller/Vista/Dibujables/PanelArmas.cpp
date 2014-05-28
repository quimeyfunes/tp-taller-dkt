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
			if(i==this->armaSeleccionada){
				//Uso la imagen del arma seleccionada
				this->armasTexturas[i]->setImagen(renderer,this->nombresArmasSeleccionada[i]);
			}else{
				this->armasTexturas[i]->setImagen(renderer,this->nombresArmasDeseleccionada[i]);
			}
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


void PanelArmas::agregarArma(SDL_Renderer* renderer, string nombreArmaSeleccionada, string nombreArmaDeseleccionada){
	SDL_Rect rect;
	int tamanoCuadradoX = this->getRect().w/3;
	int tamanoCuadradoY = this->getRect().h/3;
	int numeroCuadrado = this->armasTexturas.size();
	int x = this->getRect().x + tamanoCuadradoX * (numeroCuadrado%3);
	int y = this->getRect().y + tamanoCuadradoY * (numeroCuadrado/3);
	rect.x = x + 4;
	rect.y = y + 4;
	rect.w = tamanoCuadradoX - 20;
	rect.h = tamanoCuadradoY - 20;

	DibujableTextura* arma = new DibujableTextura(renderer, rect, nombreArmaSeleccionada, "");
	this->armasTexturas.push_back(arma);
	this->nombresArmasSeleccionada.push_back(nombreArmaSeleccionada);
	this->nombresArmasDeseleccionada.push_back(nombreArmaDeseleccionada);
}

void PanelArmas::seleccionarArma(int numeroArma){
	this->armaSeleccionada = numeroArma;
}