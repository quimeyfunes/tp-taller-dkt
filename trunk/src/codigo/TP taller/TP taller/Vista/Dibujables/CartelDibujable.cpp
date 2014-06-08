#include "CartelDibujable.h"


CartelDibujable::CartelDibujable(void)
{
}

CartelDibujable::CartelDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF, string nombre)
	: DibujableTextura(renderer, rect, pathImagen, pathDEF)
{

	this->texto = nombre;
	this->font = TTF_OpenFont(rutaFuente , 25);
	this->textColor.b = 0;
	this->textColor.g = 0;
	this->textColor.r = 0;
	this->surf =  TTF_RenderText_Solid(this->font, this->texto.c_str() , this->textColor);
	SDL_DestroyTexture(this->imagen);
	this->imagen = SDL_CreateTextureFromSurface( renderer, this->surf );
	SDL_FreeSurface(this->surf);
	this->rect.w = this->texto.length() * 7;
	this->rect.x -= this->rect.w/2;

}

CartelDibujable::CartelDibujable(SDL_Renderer* renderer, SDL_Rect rect, string nombre,int tamanioTexto, int r, int g, int b, bool centrado)
{
	this->rect = rect;
	this->anguloRotacion = 0;
	this->destruido = false;
	this->texto = nombre;
	this->textColor.b = b;
	this->textColor.g = g;
	this->textColor.r = r;
	this->font = TTF_OpenFont(rutaFuente , tamanioTexto);
	if (nombre.size() == 0) {
		nombre = " ";
	}
	this->surf =  TTF_RenderText_Solid(this->font, nombre.c_str() , this->textColor);
	this->imagen = SDL_CreateTextureFromSurface( renderer, this->surf );
	this->rect.w = this->surf->w;
	this->rect.h = this->surf->h;
	if (centrado) {
		this->rect.x -= this->rect.w/2;
		this->rect.y -= this->rect.h/2;
	}
	SDL_FreeSurface(this->surf);

}



void CartelDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{

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

CartelDibujable::~CartelDibujable(void)
{
	if(this->font != NULL){
		TTF_CloseFont( this->font );
		this->font = NULL;
	}

	if(this->imagen != NULL){
		SDL_DestroyTexture(this->imagen);
		this->imagen = NULL;
	}
}

bool CartelDibujable::loadFromRenderedText(SDL_Renderer* renderer, string textureText){
	this->texto = textureText;
	if (textureText.size() == 0) {
		textureText = " ";
	}
	if( this->imagen != NULL )
	{
		SDL_DestroyTexture( this->imagen );
		this->imagen = NULL;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid( this->font, textureText.c_str(), this->textColor );
	
	this->imagen = SDL_CreateTextureFromSurface( renderer, textSurface );
	this->rect.w = textSurface->w;
	this->rect.h = textSurface->h;
	SDL_FreeSurface( textSurface );
	return this->imagen != NULL;
}