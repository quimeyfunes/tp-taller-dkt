#include "TextInput.h"


TextInput::TextInput(void)
{
}

TextInput::TextInput(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b)
: Boton(renderer, rect, nombre, tamanioTexto,r,g,b){
	this->activo = false;
	this->texto = texto;
	SDL_Rect rectInput = this->rect;
	rectInput.x += this->rect.w;
	rectInput.y += this->rect.h/2;
	this->input = new CartelDibujable(renderer,rectInput,"",tamanioTexto,r,g,b);
}

void TextInput::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx)
{
	SDL_RenderCopy(renderer,this->imagen, NULL, &rect);
	this->input->dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
}

bool TextInput::loadFromRenderedText(SDL_Renderer* renderer, string textureText )
{
	return this->input->loadFromRenderedText(renderer,textureText);
}

string TextInput::getTexto() {
	return this->input->texto;
}


TextInput::~TextInput(void)
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

void TextInput::setActivo(bool activo){
	this->activo = activo;
}

bool TextInput::getActivo(){
	return this->activo;
}