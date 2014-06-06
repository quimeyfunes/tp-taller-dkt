#include "Menu.h"


Menu::Menu(SDL_Window* window,SDL_Renderer* renderer){
	this->window = window;
	this->renderer = renderer;
	this->listaDibujables = new list<DibujableTextura*>;
	SDL_GetWindowSize(window,&this->ancho,&this->alto);
	DibujableTextura* dib;
	SDL_Rect rect;	
	rect.x = this->ancho / 2;
	rect.y = this->alto * 1 / 3;
	rect.w = 100;
	rect.h = 50;
	this->textInput = new TextInput(renderer, rect, "..", "..", "Servidor");
	dib = new CartelDibujable(this->renderer,rect,"Servidor",50,255,0,0);
	this->listaDibujables->push_back(dib);
}


void Menu::dibujar(){
	SDL_Event* e = NULL;
	SDL_PollEvent(e);	
	SDL_RenderClear(this->renderer);
	list<DibujableTextura*>::iterator it = this->listaDibujables->begin();
	
	while (it != this->listaDibujables->end())
	{
		(*it)->dibujar(this->renderer, 0, 0, 1, this->ancho, this->alto);
		++it;
		}
	SDL_RenderPresent(this->renderer);
}

void Menu::escribir(SDL_Renderer* renderer){

	bool quit = false;

			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 0xFF };

			//The current input text.
			std::string inputText = "Some Text";
			//gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
			this->textInput->loadFromRenderedText(renderer, inputText.c_str(), textColor);

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
						this->textInput->loadFromRenderedText(renderer, inputText.c_str(), textColor );
					}
					//Text is empty
					else
					{
						//Render space texture
						//gInputTextTexture.loadFromRenderedText( " ", textColor );
						this->textInput->loadFromRenderedText(renderer, inputText.c_str(), textColor);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( this->renderer );

				//ACA VAN LAS POSICIONES DE DONDE SE VA A ESCRIBIR
				//gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
				//gInputTextTexture.render( ( 500 - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );

				//Update screen
				SDL_RenderPresent( this->renderer);
			}
			
			//Disable text input
			SDL_StopTextInput();
}

