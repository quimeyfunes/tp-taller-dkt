#include "Menu.h"


Menu::Menu(SDL_Window* window,SDL_Renderer* renderer){
	
	this->evento = new SDL_Event();
	this->window = window;
	this->renderer = renderer;
	this->listaDibujables = new list<Boton*>;
	SDL_GetWindowSize(window,&this->ancho,&this->alto);
	Boton* dib;
	SDL_Rect rect;	
	
	/*rect.x = 0;
	rect.y = 0;
	rect.w = this->ancho;
	rect.h = this->alto;
	dib = new DibujableTextura(this->renderer,rect,texturaCielo,texturaCielo);
	this->listaDibujables->push_back(dib);*/
	
	rect.x = this->ancho / 2 - 50;
	rect.y = this->alto * 1 / 3 - 25;
	rect.w = 100;
	rect.h = 50;
	dib = new BotonServidor(this->renderer,rect,"Servidor",50,255,0,0);
	this->textInput = new TextInput(renderer, rect, "..", "..", "Servidor");
	this->textInput = new TextInput(renderer, rect, rutaCartel, rutaCartel, "Servidor");
	this->listaDibujables->push_back(dib);

	rect.x = this->ancho / 2 - 50;
	rect.y = this->alto * 2 / 3 - 25;
	rect.w = 100;
	rect.h = 50;
	dib = new BotonCliente(this->renderer,rect,"Cliente",50,0,255,0);
	this->listaDibujables->push_back(dib);
}


void Menu::dibujar(){
	SDL_RenderClear(this->renderer);
	list<Boton*>::iterator it = this->listaDibujables->begin();
	
	while (it != this->listaDibujables->end())
	{
		(*it)->dibujar(this->renderer, 0, 0, 1, this->ancho, this->alto);
		++it;
	}
	SDL_RenderPresent(this->renderer);
}


int Menu::leerEvento(){
	if (SDL_PollEvent(this->evento) != 0) {
		
		if(evento->type == SDL_QUIT){
			return nameMenu::SALIR;
		}

		if (evento->type == SDL_MOUSEBUTTONDOWN) {
			int x,y;
			SDL_GetMouseState(&x,&y);
			switch(evento->button.button){
			case SDL_BUTTON_LEFT:		
				for (list<Boton*>::iterator it = this->listaDibujables->begin(); it != this->listaDibujables->end();it++) {
					int retorno = (*it)->click(x,y);
					if (retorno != 0) {
						return retorno;	
					};
				}
			}
		}
	}
	return 0;
}

int Menu::clickServidor() {
	return nameMenu::SERVIDOR;
}

int Menu::clickCliente() {
	return nameMenu::CLIENTE;
}

void Menu::escribir(){


	bool quit = false;

			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 0xFF };

			//The current input text.
			std::string inputText = "Some Text";
			//gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
			this->textInput->loadFromRenderedText(this->renderer, inputText.c_str(), textColor);

			//Enable text input
			SDL_StartTextInput();

			//While application is running
			while( !quit )
			{
				//The rerender text flag
				bool renderText = false;

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					//Special key input
					else if( e.type == SDL_KEYDOWN )
					{
						//Handle backspace
						if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
						{
							//lop off character
							inputText.pop_back();
							renderText = true;
						}
						//Handle copy
						else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
						{
							inputText = SDL_SetClipboardText( inputText.c_str() );
						}
						//Handle paste
						else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
						{
							inputText = SDL_GetClipboardText();
							renderText = true;
						}
					}
					//Special text input event
					else if( e.type == SDL_TEXTINPUT )
					{
						//Not copy or pasting
						if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
						{
							//Append character
							inputText += e.text.text;
							renderText = true;
						}
					}
				}

				//Rerender text if needed
				if( renderText )
				{
					//Text is not empty
					if( inputText != "" )
					{
						//Render new text
						//gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
						this->textInput->loadFromRenderedText(this->renderer, inputText.c_str(), textColor );
					}
					//Text is empty
					else
					{
						//Render space texture
						//gInputTextTexture.loadFromRenderedText( " ", textColor );
						this->textInput->loadFromRenderedText(this->renderer, inputText.c_str(), textColor);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( this->renderer );

				//ACA VAN LAS POSICIONES DE DONDE SE VA A ESCRIBIR
				this->textInput->dibujar(this->renderer, 50, 50, 0, 100, 100);

				//Update screen
				SDL_RenderPresent( this->renderer);
			}
			
			//Disable text input
			SDL_StopTextInput();
}

