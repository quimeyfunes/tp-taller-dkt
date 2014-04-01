#include "Juego.h"

Juego::Juego(){
	this->enPausa = false;
	this->enSimulacion = false;
}

void Juego::comenzar(){
	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();
	vector<ObjetoParseado>* objetos = parser->getObjetos();
	Escenario* escenario = new Escenario(e->altoU,e->anchoU,e->altoPx,e->anchoPx,e->nivelAgua);
	b2World* world = escenario->getWorld();
	//Muy feo, lector recibe un char* y imagenTierra es string 
	char* imagenTierra = new char[e->imagenTierra.size()+1];
	imagenTierra[e->imagenTierra.size()] = 0;
	memcpy(imagenTierra,e->imagenTierra.c_str(),e->imagenTierra.size());
	LectorTerreno* lector = new LectorTerreno(imagenTierra);
	
	
	Terreno* terr = new Terreno(escenario->getWorld());
	terr->generarTerreno(imagenTierra);
	escenario->setTerreno(terr);

	Vista* vista = new Vista(e);
	

	SDL_Event* evento = new SDL_Event();
	
	Dibujable* dibTierra = vista->crearDibujable( 0, 0 , lector->getAnchoMatriz(),lector->getAltoMatriz(),lector->getRutaTexturaActualizada());
	
	//Color tierra
	string hex = "#00FF00";
	dibTierra->setColor(ParserDeHexARgb::parsearDeHexARgb(hex));
	
	//Parsea objetos del yaml y hace figuras/dibujables(dibujables todavia no)
	float escalaAncho = e->anchoPx / e->anchoU;
	float escalaAlto = e->altoPx / e->altoU;
	//Levanta cuadrados (de imagen) medio raro
	for (std::vector<ObjetoParseado>::iterator it = objetos->begin(); it != objetos->end(); ++it) {
		switch ((*it).tipo) {
		case 1: 
			{
			//Circulo* cir = escenario->crearCirculo(*it);
			//cir->agregarObservador(vista->crearFiguraDibujable((*it).x * escalaAncho, (*it).y * escalaAlto,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, "imagenes/r4.png" ));
			}
		case 2:
			{
				Rectangulo* rec = escenario->crearRectangulo(*it);
				if(rec){
					rec->agregarObservador(vista->crearFiguraDibujable((*it).x * escalaAncho - (*it).ancho * escalaAncho /2, (*it).y * escalaAlto - (*it).alto * escalaAlto/2,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, "imagenes/imagen.jpg" ));
				}
			}
		default:
			{
			//escenario->crearPoligono(*it);
			//vista->crearDibujable((*it).x * escalaAncho, (*it).y * escalaAlto,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, pathPoligono q venga );
			}
		}
	}
	
	bool jugar = true;

	while((evento->type != SDL_QUIT)&&(jugar)){

		if (SDL_PollEvent(evento) != 0) {
			if(evento->key.keysym.sym == SDLK_ESCAPE) { 
				jugar = false;
			} 
			else if (evento->key.keysym.sym == SDLK_s && evento->type == SDL_KEYDOWN) {
				if (this->enSimulacion) {
					escenario->reiniciar();
				}
				this->enPausa = false;
				this->simular();
			} else if (evento->key.keysym.sym == SDLK_p && evento->type == SDL_KEYDOWN) {
				this->pausar();
			}
		}
		if (this->enSimulacion && !this->enPausa) { 
			world->Step(stepTiempo, iteracionesVelocidad, iteracionesPosicion);
			
			escenario->simularAgua(); 
		}
		escenario->notificar();
			
		vista->Dibujar();
		
		SDL_Delay(10);
		
	}
    //Quit SDL
    SDL_Quit();
}

void Juego::pausar(){
	this->enPausa = !this->enPausa;
}
bool Juego::getPausa(){
	return this->enPausa;
}

void Juego::simular(){
	this->enSimulacion = !this->enSimulacion; 
}
