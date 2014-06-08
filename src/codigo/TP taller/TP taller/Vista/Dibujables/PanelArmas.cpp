#include "PanelArmas.h"

PanelArmas::PanelArmas(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF):DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
	this->visible = false;
	this->armaSeleccionada = -1;

	int tamanoCuadradoX = (this->getRect().w/3 - 20)/3;
	int tamanoCuadradoY = (this->getRect().h/3 - 20)/3;

	this->recCuadroProyectiles = new SDL_Rect[11];
	for(int i=0; i<11; i++){
			recCuadroProyectiles[i].h =  46;
			recCuadroProyectiles[i].w = 46;
			recCuadroProyectiles[i].x = 0;
			recCuadroProyectiles[i].y = 46*i;
	}
}


PanelArmas::~PanelArmas(){
}

void PanelArmas::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{
	if(this->visible){
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &this->rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);

		for(int i=0; i<this->armasTexturas.size(); i++){
			if(i==this->armaSeleccionada){
				//Uso la imagen del arma seleccionada
				this->armasTexturas[i]->setImagen(renderer,this->nombresArmasSeleccionada[i]);
			}else{
				this->armasTexturas[i]->setImagen(renderer,this->nombresArmasDeseleccionada[i]);
			}
			SDL_RenderCopyEx(renderer,this->armasTexturas[i]->getImagen(),NULL, &this->armasTexturas[i]->getRect(),this->armasTexturas[i]->getAngulo(),NULL,SDL_FLIP_NONE);
		}

		for(int i=0; i<this->cantidadProyectilesTexturas.size(); i++){
			SDL_RenderCopy(renderer,this->cantidadProyectilesTexturas[i]->getImagen(),&this->recCuadroProyectiles[this->cantidadProyectiles[i]],&this->cantidadProyectilesTexturas[i]->getRect());
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


void PanelArmas::agregarArma(SDL_Renderer* renderer, string nombreArmaSeleccionada, string nombreArmaDeseleccionada, int cantidadProyectiles, bool seDisparaConClick){
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

	SDL_Rect rectCantidadProyectiles;
	rectCantidadProyectiles.x = rect.x + tamanoCuadradoX - 20;
	rectCantidadProyectiles.y = rect.y + tamanoCuadradoY - 20;
	rectCantidadProyectiles.w = rect.w/3;
	rectCantidadProyectiles.h = (rect.h/3);
	

	DibujableTextura* cantidadProyectilesTextura = new DibujableTextura(renderer, rectCantidadProyectiles, "imagenes/texturas/cantidadProyectilesBlanco.png", "");
	this->cantidadProyectilesTexturas.push_back(cantidadProyectilesTextura);

	this->nombresArmasSeleccionada.push_back(nombreArmaSeleccionada);
	this->nombresArmasDeseleccionada.push_back(nombreArmaDeseleccionada);

	if(cantidadProyectiles == -1){
		cantidadProyectiles = 10;
	}
	this->cantidadProyectiles.push_back(cantidadProyectiles);
	this->seDisparaConClick.push_back(seDisparaConClick);
}

void PanelArmas::seleccionarArma(int numeroArma){
	this->armaSeleccionada = numeroArma;
}

int PanelArmas::getArmaSeleccionada(){
	return this->armaSeleccionada;
}

void PanelArmas::descontarArma(int numeroArma){
	if(numeroArma >= 0){
		if(this->cantidadProyectiles[numeroArma] > 0 && this->cantidadProyectiles[numeroArma] < 10){
			this->cantidadProyectiles[numeroArma]--;
			this->armaSeleccionada = -1;
		}
	}
}

void PanelArmas::descontarArmaEspacio(int numeroArma){
	if(numeroArma >= 0 && !this->seDisparaConClick[numeroArma]){
		this->descontarArma(numeroArma);
	}
}

void PanelArmas::descontarArmaClick(int numeroArma){
	if(numeroArma >= 0 && this->seDisparaConClick[numeroArma]){
		this->descontarArma(numeroArma);
	}
}

bool PanelArmas::proyectilRestante(int numeroArma){
	if(numeroArma >= cantidadProyectiles.size()) return false;
	if(numeroArma == -1) return false;

	return (this->cantidadProyectiles[numeroArma] > 0 || this->cantidadProyectiles[numeroArma] == 10);
}