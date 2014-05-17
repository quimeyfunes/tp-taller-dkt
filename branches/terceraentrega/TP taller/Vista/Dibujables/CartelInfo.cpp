#include "CartelInfo.h"
#include <time.h>

CartelInfo::CartelInfo(SDL_Renderer* renderer, SDL_Rect rect) : CartelDibujable(renderer, rect, "", rutaCartelDEF, "")
{
	this->renderer = renderer;
	TTF_CloseFont(this->font);
	this->font = TTF_OpenFont(rutaFuente , 40);
	this->texto = "";
	this->tiempoDibujando = 0;
	this->tiempoMaximo = TIEMPO_CARTEL_INFO;
	this->dibujado=false;
}


CartelInfo::~CartelInfo(){}

void CartelInfo::setColor(int R, int G, int B, int A){

	this->textColor.a = A;
	this->textColor.b = B;
	this->textColor.g = G;
	this->textColor.r = R;

	this->surf =  TTF_RenderText_Solid(this->font, this->texto.c_str() , this->textColor);
	SDL_DestroyTexture(this->imagen);
	this->imagen = SDL_CreateTextureFromSurface( renderer, this->surf );
	SDL_FreeSurface(this->surf);

}

void CartelInfo::setInfo(string info){

	this->texto = info;
	this->surf =  TTF_RenderText_Solid(this->font, this->texto.c_str() , this->textColor);
	SDL_DestroyTexture(this->imagen);
	this->imagen = SDL_CreateTextureFromSurface(this->renderer , this->surf );
	SDL_FreeSurface(this->surf);

	this->rect.w = this->texto.length() * 10;
}

void CartelInfo::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{
	
	if(this->texto != ""){
		if(!dibujado) this->tiempoDibujando = time(NULL);
		dibujado=true;
		SDL_RenderCopy(renderer,this->imagen, NULL , &this->rect);

		if(time(NULL) - this->tiempoDibujando > this->tiempoMaximo){
			this->texto = "";
			dibujado = false;
		}
	}
}
