#include "Juego.h"

Servidor* Juego::servidor = NULL;

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

	//En el jugar pongo el loop del server
	 servidor = new Servidor();
	//Creo el trhead con el loop del servidor: en el loop se van a escuchar los clientes y a recibir los mensajes
	 _beginthread( Juego::servidorLoop, 0, (void*)12);

	 //Puse el cliente aca para probar que se conecte pero obviamente esto se hacen en el cliente
	 Cliente* cliente = new Cliente();

	//game loop
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->leerEvento();

		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}

		escenario->notificar();	
		vista->Dibujar();
		SDL_Delay(1);
		
	}
}

void Juego::leerEvento(){

	if (this->vista->leerEvento(evento)){
	
		switch(this->vista->getAccion()){

		case SALIR:			salir();						break;
		case JUGAR:			reiniciar();					break;
		case PAUSAR:		alternarPausa();				break;
		case ARRIBA:		this->escenario->saltar();		break;
		case IZQUIERDA:		this->escenario->izquierda();	break;
		case DERECHA:		this->escenario->derecha();		break; 
		case CLICK:	
			int x,y;
			SDL_GetMouseState(&x,&y);
			this->escenario->click( (x - this->vista->getCorrimiento())/ this->escenario->getRelacionAncho(), y / this->escenario->getRelacionAlto());
			break;
		}
	}
}

void Juego::jugar(){
	mundo->Step(stepTiempo, iteracionesVelocidad, iteracionesPosicion);
	escenario->simularAgua();
}

void Juego::salir(){
	this->estadoActual = SALIDA;
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
	dibTierra->setColor(ParserDeHexARgb::parsearDeHexARgb("804000"),255);
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

void Juego::servidorLoop(void * arg) 
{ 
    while(true) 
    {
        servidor->actualizar();
    }
}

Juego::~Juego(){

	delete this->escenario;
	delete this->terreno;
	delete this->evento;
	delete Logger::getLogger();
}