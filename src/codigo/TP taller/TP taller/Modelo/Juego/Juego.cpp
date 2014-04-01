#include "Juego.h"

Juego::Juego(){

	this->estadoActual = PAUSADO;
	this->evento = new SDL_Event();

	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();

	this->vista = new Vista(e);
	this->escenario = new Escenario(e->altoU,e->anchoU,e->altoPx,e->anchoPx,e->nivelAgua);
	this->terreno = new Terreno(this->escenario->getWorld());
	this->terreno->generarTerreno(e->imagenTierra);
	this->escenario->setTerreno(this->terreno);
	this->mundo = escenario->getWorld();
}

void Juego::ejecutar(){

	//no se si esta bueno que esto esté aca
	vector<ObjetoParseado>* objetos = ParserYaml::getParser()->getObjetos();	
	Dibujable* dibTierra = vista->crearDibujableTextura(0, 0, terreno->getLector()->getAnchoMatriz(),terreno->getLector()->getAltoMatriz(),terreno->getLector()->getRutaTexturaActualizada());
	dibTierra->setColor(ParserDeHexARgb::parsearDeHexARgb("804000"));
	agregarObjetos(objetos);

	//game loop
	while(this->estadoActual != SALIR){
		
		this->leerEvento();

		switch(estadoActual){

			case JUGANDO:		jugar();	break;
			case PAUSADO:		esperar();	break;
		}
	}
}

void Juego::leerEvento(){

	if (SDL_PollEvent(evento) != 0) {

		if(evento->type == SDL_KEYDOWN){

			switch(evento->key.keysym.sym){

			case SDLK_ESCAPE:	salir();			break;
			case SDLK_s:		reiniciar();		break;
			case SDLK_p:		pausar();			break;
		
			}
		}
	}
	
	escenario->notificar();	
	vista->Dibujar();
	SDL_Delay(4);
}

void Juego::jugar(){
	mundo->Step(stepTiempo, iteracionesVelocidad, iteracionesPosicion);
	escenario->simularAgua(); 
}

void Juego::salir(){
	this->estadoActual = SALIR;
}

void Juego::reiniciar(){
	this->estadoActual = PAUSADO;
	this->escenario->reiniciar();
}

void Juego::pausar(){
	if(this->estadoActual != PAUSADO) this->estadoActual = PAUSADO;	else this->estadoActual = JUGANDO;
}

void Juego::esperar(){
}

//ni puta idea lo que hace esto -> lo dejé asi como estaba
void Juego::agregarObjetos(vector<ObjetoParseado>* objetos){

	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();

		//Parsea objetos del yaml y hace figuras/dibujables(dibujables todavia no)
	float escalaAncho = e->anchoPx / e->anchoU;
	float escalaAlto = e->altoPx / e->altoU;
	//Levanta cuadrados (de imagen) medio raro
	for (std::vector<ObjetoParseado>::iterator it = objetos->begin(); it != objetos->end(); ++it) {
		switch ((*it).tipo) {
		case 1: 
			{
				Circulo* cir = escenario->crearCirculo(*it);
				if (cir) {
				CirculoDibujable* circulo = vista->crearCirculoDibujable((*it).x * escalaAncho, (*it).y * escalaAlto,(*it).escala * escalaAncho,(*it).escala* escalaAlto);
				circulo->setColor(ParserDeHexARgb::parsearDeHexARgb((*it).color));
				cir->agregarObservador(circulo);
				}
			}
		case 2:
			{
				//Rectangulo* rec = escenario->crearRectangulo(*it);
				//if(rec){
				//	rec->agregarObservador(vista->crearFiguraDibujable((*it).x * escalaAncho - (*it).ancho * escalaAncho /2, (*it).y * escalaAlto - (*it).alto * escalaAlto/2,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, "imagenes/imagen.jpg" ));
				//}
			}
		default:
			{
			//Poligono* pol = escenario->crearPoligono(*it);
			//vista->crearDibujable((*it).x * escalaAncho, (*it).y * escalaAlto,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, pathPoligono q venga );
			}
		}
	}
}

Juego::~Juego(){
}