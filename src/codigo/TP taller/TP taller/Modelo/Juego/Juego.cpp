#include "Juego.h"

Juego::Juego(){
	this->simulando = false;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();

	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();
	
	this->vista = new Vista(e);
	this->escenario = new Escenario(e->altoU,e->anchoU,e->nivelAgua,e->anchoPx * 1.0 / e->anchoU,e->altoPx * 1.0 / e->altoU);
	this->terreno = new Terreno(this->escenario->getWorld());
	this->terreno->generarTerreno(e->imagenTierra);
	this->escenario->setTerreno(this->terreno);
	this->mundo = escenario->getWorld();

	agregarTexturas(e);
	agregarObjetos();

}

void Juego::ejecutar(){
	Logger::getLogger()->guardarEstado();
	//game loop
	while(this->estadoActual != SALIR && (evento->type != SDL_QUIT)){
		
		this->leerEvento();

		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}

		escenario->notificar();	
		vista->Dibujar();
		
	}
}

void Juego::leerEvento(){

	if (SDL_PollEvent(evento) != 0) {

		if(evento->type == SDL_QUIT){
			this->salir();
			return;
		}
		
		if(evento->type == SDL_KEYDOWN){

			switch(evento->key.keysym.sym){

			case SDLK_ESCAPE:	salir();			break;
			case SDLK_s:		reiniciar();		break;
			case SDLK_p:		alternarPausa();	break;
		
			}
		}
	}
}

void Juego::jugar(){
	mundo->Step(stepTiempo, iteracionesVelocidad, iteracionesPosicion);
	escenario->simularAgua(); 
}

void Juego::salir(){
	this->estadoActual = SALIR;
}

void Juego::reiniciar(){
	this->simulando = !simulando;
	this->escenario->reiniciar();

	if(this->estadoActual == PAUSADO) this->estadoActual = JUGANDO;
}

void Juego::alternarPausa(){
	if(this->estadoActual != PAUSADO) this->estadoActual = PAUSADO;	else this->estadoActual = JUGANDO;
}

void Juego::esperar(){}

void Juego::agregarTexturas(EscenarioParseado* e){

	vista->crearDibujableTextura(0, 0,e->anchoPx, e->altoPx, texturaFondo, "");
	vista->crearDibujableTextura(0, 0,e->anchoPx, e->altoPx, e->imagenCielo, texturaCieloDEF);
	vista->crearScrollingSprite(0, 10, e->anchoPx / 5, e->altoPx/10, rutaNube1);
	vista->crearScrollingSprite(e->anchoPx/2, 30, e->anchoPx / 5, e->altoPx / 10, rutaNube2);
	vista->crearDibujableTextura(0, e->nivelAgua * this->escenario->getRelacionAlto(), e->anchoPx, e->altoPx - e->nivelAgua, texturaAgua, texturaAguaDEF);
	for(int i=0;i<4;i++){
		vista->crearSprite( (i*e->anchoPx)/4, e->nivelAgua* this->escenario->getRelacionAlto() - 15, e->anchoPx/4, 15, spriteOlas, 2, 6, 256, 144);
	}
	Dibujable* dibTierra = vista->crearDibujableTextura(0, 0, terreno->getLector()->getAnchoMatriz(),terreno->getLector()->getAltoMatriz(),terreno->getLector()->getRutaTexturaActualizada(), "");
	dibTierra->setColor(ParserDeHexARgb::parsearDeHexARgb("804000"));
}

void Juego::agregarObjetos(){
	
	vector<ObjetoParseado>* objetos = ParserYaml::getParser()->getObjetos();	
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();

	float escalaAncho = this->escenario->getRelacionAncho();
	float escalaAlto = this->escenario->getRelacionAlto();
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
				break;
			}
		case 2:
			{
				Rectangulo* rec = escenario->crearRectangulo(*it);
				if(rec){
					cout<<(*it).ancho<<endl;
					RectanguloDibujable* rectangulo = vista->crearRectanguloDibujable((*it).ancho * escalaAncho, (*it).alto * escalaAlto);
					rectangulo->setColor(ParserDeHexARgb::parsearDeHexARgb((*it).color));
					rec->agregarObservador(rectangulo);
				}
				break;
			}
		default:
			{
				Poligono* pol = escenario->crearPoligono(*it);
				if (pol) {
					PoligonoDibujable* poligono = vista->crearPoligonoDibujable((*it).tipo,(*it).escala * escalaAncho, (*it).escala * escalaAlto);
					poligono->setColor(ParserDeHexARgb::parsearDeHexARgb((*it).color));
					pol->agregarObservador(poligono);
				}
				break;
			}
		}
	}
}

Juego::~Juego(){

	delete this->escenario;
	delete this->terreno;
	delete this->evento;
	delete Logger::getLogger();
}