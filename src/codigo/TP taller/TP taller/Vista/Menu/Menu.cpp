#include "Menu.h"


Menu::Menu(SDL_Window* window,SDL_Renderer* renderer){
	
	this->menuPrincipal = true;
	this->evento = new SDL_Event();
	this->window = window;
	this->renderer = renderer;
	this->listaDibujables = new list<Boton*>;
	this->menuCliente = false;
	SDL_GetWindowSize(window,&this->ancho,&this->alto);
	Boton* dib;
	SDL_Rect rect;	
	
	rect.x = 0;
	rect.y = 0;
	rect.w = this->ancho;
	rect.h = this->alto;
	this->fondo = new DibujableTextura(this->renderer,rect,texturaFondoMenu,texturaFondoMenu);
		
	rect.x = this->ancho / 2;
	rect.y = this->alto * 1 / 3;
	rect.w = 100;
	rect.h = 50;
	dib = new BotonServidor(this->renderer,rect,"Servidor",50,255,0,0);
	rect.x = this->ancho / 4;
	this->textInputNombre = new TextInput(this->renderer, rect,"Nombre: ",50,0,255,0,true);

	this->listaDibujables->push_back(dib);
	
	rect.x = this->ancho / 2;
	rect.y = this->alto * 2 / 3;
	rect.w = 100;
	rect.h = 50;
	dib = new BotonCliente(this->renderer,rect,"Cliente",50,0,255,0);
	rect.x = this->ancho / 4;
	this->textInputIP = new TextInput(renderer, rect, "IP: ", 50,0,255,0,true);
	this->listaDibujables->push_back(dib);
}

void Menu::dibujar(){
	SDL_RenderClear(this->renderer);
	this->fondo->dibujar(renderer,0,0,1,this->ancho,this->alto);
	if (this->menuPrincipal || (!this->menuCliente && !this->menuPrincipal)) {
		list<Boton*>::iterator it = this->listaDibujables->begin();
		while (it != this->listaDibujables->end())
		{
			(*it)->dibujar(this->renderer, 0, 0, 1, this->ancho, this->alto);
			++it;
		}
	} else {
		if (this->menuCliente) {
			this->textInputNombre->dibujar(this->renderer, 0, 0, 1, this->ancho, this->alto);
			this->textInputIP->dibujar(this->renderer, 0, 0, 1, this->ancho, this->alto);
		} 
	} 
	SDL_RenderPresent(this->renderer);
	SDL_Delay(10);
}

int Menu::leerEvento(){
	while(SDL_PollEvent(this->evento)) {
		
		if (evento->type == SDL_QUIT){
			return nameMenu::SALIR;
		}

		if (evento->type == SDL_KEYDOWN) {
			switch (evento->key.keysym.sym) {
			case SDLK_RETURN: 	
				if (this->menuCliente && this->textInputNombre->getTexto().size() != 0 && this->textInputIP->getTexto().size() != 0) {
					this->menuCliente = false;
					this->listaDibujables->clear();
					return nameMenu::CLIENTE;
				}
				break;
			case SDLK_TAB: 
				this->textInputNombre->setActivo(!this->textInputNombre->getActivo());
				this->textInputIP->setActivo(!this->textInputIP->getActivo());
				break;
			}
		}

		if (evento->type == SDL_MOUSEBUTTONDOWN && evento->button.button == SDL_BUTTON_LEFT) {
			int x,y;
			SDL_GetMouseState(&x,&y);
			if (!this->menuCliente) {
				for (list<Boton*>::iterator it = this->listaDibujables->begin(); it != this->listaDibujables->end();it++) {
					int retorno = (*it)->click(x,y);
					if (retorno != 0) {
						switch (retorno){
						case nameMenu::SERVIDOR:
							this->listaDibujables->clear();
							return retorno;
							break;
						case nameMenu::CLIENTE:
							this->textInputNombre->setActivo(true);
							this->menuCliente = true;
							this->menuPrincipal = false;
							return 0;
							break;
						}
					}
				}
			}
			if (this->menuCliente) {
				if (this->textInputIP->click(x,y)) {
					this->textInputIP->setActivo(true);
					this->textInputNombre->setActivo(false);
				} else {
					if (this->textInputNombre->click(x,y)) {
						this->textInputIP->setActivo(false);
						this->textInputNombre->setActivo(true);
					}
				}
			}
		}

		this->escribir(evento);
	}
	return 0;
}

void Menu::escribir(SDL_Event* evento){

	TextInput* textInput;
	if (this->textInputNombre->getActivo()) {
		textInput = this->textInputNombre;
	} else if (this->textInputIP->getActivo()) {
		textInput = this->textInputIP;
	} else {
		return;
	}
	std::string inputText = textInput->getTexto();

	bool renderText = false;

	if( evento->type == SDL_KEYDOWN )
	{
		//Para borrar
		if( evento->key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
		{
			inputText.pop_back();
			renderText = true;
		}
		//Ctrl-c
		else if( evento->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
		{
			inputText = SDL_SetClipboardText( inputText.c_str() );
		}
		//ctrl-V
		else if( evento->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
		{
			inputText = SDL_GetClipboardText();
			renderText = true;
		}
	}
	else if( evento->type == SDL_TEXTINPUT )
	{
		if( !( ( evento->text.text[ 0 ] == 'c' || evento->text.text[ 0 ] == 'C' ) && ( evento->text.text[ 0 ] == 'v' || evento->text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
		{
			inputText += evento->text.text;
			renderText = true;
		}
	}
	if(renderText)
		textInput->loadFromRenderedText(this->renderer, inputText.c_str());
}

string Menu::getNombre() {
	return this->textInputNombre->getTexto();
}

string Menu::getIP() {
	return this->textInputIP->getTexto();
}

void Menu::setIP(string ip) {
	this->textInputIP->loadFromRenderedText(this->renderer,ip);
}

void Menu::agregarMensaje(string nombre, int tamanioTexto, int r, int g, int b){
	SDL_Rect rect;
	rect.x = 20;
	rect.y = 10;
	for (list<Boton*>::iterator it = this->listaDibujables->begin(); it != this->listaDibujables->end();it++){
		rect.y += (*it)->getRect().h + 5;
	}
	
	Boton* boton = new Boton(this->renderer,rect,nombre,tamanioTexto,r,g,b,false);
	this->listaDibujables->push_back(boton);
}

void Menu::limpiar(){
	SDL_DestroyTexture(this->fondo->getImagen());
	SDL_DestroyTexture(this->textInputNombre->getImagen());
	SDL_DestroyTexture(this->textInputIP->getImagen());
	this->listaDibujables->clear();
}