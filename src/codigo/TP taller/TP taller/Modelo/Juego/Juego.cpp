#include "Juego.h"

Servidor* Juego::servidor = NULL;
Cliente* Juego::cliente1 = NULL;
Cliente* Juego::cliente2 = NULL;

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
	 //cliente1 = new Cliente();
	 //cliente2 = new Cliente();
	//Creo el trhead con el loop del servidor: en el loop se van a escuchar los clientes y a recibir los mensajes
	 _beginthread( Juego::servidorLoop, 0, (void*)12);
	 //_beginthread(Juego::clienteLoop,0, (void*)12);
	 //Puse el cliente aca para probar que se conecte pero obviamente esto se hacen en el cliente

	//game loop
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->leerEvento();

		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}

		//HAY QUE CAMBIAR EL NOTIFICAR: ahora se llama al metodo notificar de observable, y ahi se avisa a los observadores del estado actual. Eso tiene
		//que hacerse a traves de un mensaje a los clientes
		escenario->notificar();
		
		Paquete paquete;
		paquete.setTipo(5);
		char dataVista[sizeof(Vista)];
		char dataPaquete[sizeof(Paquete)];
		memcpy(dataVista, vista, sizeof(Vista));
		paquete.setMensaje(dataVista);
		paquete.serializar(dataPaquete);
		/*Servicio::enviarMensaje(cliente1->red->socketCliente, dataPaquete, sizeof(Paquete));
		Servicio::enviarMensaje(cliente2->red->socketCliente, paquete_data, sizeof(Paquete));*/

		for(int i=0; i< this->servidor->red->sessions.size(); i++){
			int enviado = Servicio::enviarMensaje(this->servidor->red->sessions.at(i), dataPaquete, sizeof(Paquete));
		}	
		vista->Dibujar();
		SDL_Delay(1);
		
	}
}

void Juego::leerEvento(){

	if (this->vista->leerEvento(evento)){

		
	//le envio un evento al servidor
    /*char paquete_data[sizeof(Paquete)];
    Paquete paquete;
    paquete.setTipo(2);
    paquete.serializar(paquete_data);
	Servicio::enviarMensaje(cliente1->red->socketCliente, paquete_data, sizeof(Paquete));
	Servicio::enviarMensaje(cliente2->red->socketCliente, paquete_data, sizeof(Paquete));
	for(int i=0; i< this->servidor->red->sessions.size(); i++)
		Servicio::enviarMensaje(this->servidor->red->sessions.at(i), paquete_data, sizeof(Paquete));*/
	////////////////////////////////

		switch(this->vista->getAccion()){

		case SALIR:			salir();						break;
		case JUGAR:			reiniciar();					break;
		case PAUSAR:		alternarPausa();				break;
		case ARRIBA:		this->escenario->arriba(true);		break;
		case IZQUIERDA:		this->escenario->izquierda(true);	break;
		case DERECHA:		this->escenario->derecha(true);		break; 
		case SOLTARARRIBA:		this->escenario->arriba(false);		break;
		case SOLTARIZQUIERDA:	this->escenario->izquierda(false);	break;
		case SOLTARDERECHA:		this->escenario->derecha(false);	break; 
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
		//En servidor actualizar se reciben clientes y se escuchan mensajes
        servidor->actualizar();
		//If nuevo cliente -> tengo que agregar un nuevo juegador si no se supero el limite de jugadores

		//If mensaje -> proceso el mensaje: si un jugador apreta boton, tengo que reflejarlo en el juego
    }
}

Juego::~Juego(){

	delete this->escenario;
	delete this->terreno;
	delete this->evento;
	delete Logger::getLogger();
}


void Juego::clienteLoop(void * arg) 
{ 
    while(true) 
    {
		//para todos los clientes de una lista
        cliente1->actualizar();
		cliente2->actualizar();
		
		
    }
}