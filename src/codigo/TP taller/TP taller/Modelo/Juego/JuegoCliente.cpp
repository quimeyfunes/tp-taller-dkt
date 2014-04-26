#include "JuegoCliente.h"

Cliente* JuegoCliente::cliente = NULL;

JuegoCliente::JuegoCliente(string nombreCliente){
	this->simulando = false;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();
	this->vista = new Vista(e);
	this->escenario = new Escenario(e->altoU ,e->anchoU, e->nivelAgua, relacionPPU, relacionPPU);
	this->terreno = new Terreno(this->escenario->getWorld());
	this->terreno->generarTerreno(e->imagenTierra);
	this->escenario->setTerreno(this->terreno);
	agregarTexturas(e);
	agregarAgua(e);
	this->dibujablesBase = this->vista->getListaDibujables();
	cliente = new Cliente(nombreCliente);
}

void JuegoCliente::ejecutar(){
	Logger::getLogger()->guardarEstado();

	//game loop
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->leerEvento();
		this->cliente->actualizar();
		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}
		vista->Dibujar();
		SDL_Delay(1);
	}
}

void JuegoCliente::leerEvento(){

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
			this->escenario->click((x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom()) ,  (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom()));
			break;
		}
	}
}

void JuegoCliente::jugar(){
	escenario->simularAgua();
}

void JuegoCliente::salir(){
	Juego::salir();
}

void JuegoCliente::reiniciar(){
	Juego::reiniciar();
}

void JuegoCliente::alternarPausa(){
	Juego::alternarPausa();
}

void JuegoCliente::esperar(){}

void JuegoCliente::agregarTexturas(EscenarioParseado* e){
	Juego::agregarTexturas(e);
}

void JuegoCliente::agregarAgua(EscenarioParseado* e){
	Juego::agregarAgua(e);
}

JuegoCliente::~JuegoCliente(){

	delete this->escenario;
	delete this->evento;
	delete Logger::getLogger();
}
