#include "JuegoCliente.h"

Cliente* JuegoCliente::cliente = NULL;

JuegoCliente::JuegoCliente(string nombreCliente, string ip){
	
	cliente = new Cliente(nombreCliente, ip);
	//EscenarioParseado* e = cliente->getEscenarioActual();

	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	this->simulando = false;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	
	this->vista = new Vista(e);

	//TIENE Q TENERLO EL CLIENTE?? //////////
	this->escenario = new Escenario(e->altoU ,e->anchoU, e->nivelAgua, relacionPPU, relacionPPU);
	this->terreno = new Terreno(this->escenario->getWorld());
	this->terreno->generarTerreno(e->imagenTierra);
	this->escenario->setTerreno(this->terreno);
	/////////////////////////////////////////


	agregarTexturas(e);
	agregarAgua(e);
	this->dibujablesBase = new list<Dibujable*>(this->vista->getListaDibujables()->size());
	copy(this->vista->getListaDibujables()->begin(),this->vista->getListaDibujables()->end(),this->dibujablesBase->begin());
	
}

void JuegoCliente::ejecutar(){
	Logger::getLogger()->guardarEstado();
	list<Dibujable*> *lista = new list<Dibujable*>(this->dibujablesBase->size());
	//game loop
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->leerEvento();
		this->cliente->actualizar();
		string vistaSerializada = this->cliente->vistaSerializada;
		//this->crearLista(vistaSerializada, lista);

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
		int accion = this->vista->getAccion();
		switch(accion){

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

		if(accion == CLICK || accion == IZQUIERDA || accion == DERECHA || accion == ARRIBA || accion == SOLTARARRIBA || accion == SOLTARIZQUIERDA || accion == SOLTARDERECHA){
			//Para estos eventos tengo que notificar al servidor
			Evento* e = new Evento();
			int x,y;
			SDL_GetMouseState(&x,&y);

			e->accion = accion;
			e->x = x;
			e->y = y;
			this->cliente->enviarEvento(e->serializar());
			delete e;
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

//le paso la lista como parametro para no estar haciendo new cada vez que entro 
/*list<Dibujable*>* */void JuegoCliente::crearLista(string vistaSerializada, list<Dibujable*>* lista){
	
	copy(this->dibujablesBase->begin(),this->dibujablesBase->end(),lista->begin());
	//Saco el agua
	lista->pop_back();

	vector<string> entidadesSerializadas = StringUtil::split(vistaSerializada,separadorEntidades);
	for (int i = 0; i < entidadesSerializadas.size(); i++) {
		string entidadSerializada = entidadesSerializadas.at(i);
		vector<string> des = StringUtil::split(entidadSerializada,separadorCampoTipoEntidades);
		switch (StringUtil::str2int(des.at(0))){
			case serializadoGusanoDibujable: {
				GusanoDibujable* gusano = new GusanoDibujable();
				gusano->deserealizar(entidadSerializada);

				gusano = new GusanoDibujable(this->vista->renderer, gusano->getRect(), rutaGusano, rutaGusanoDEF);
				lista->push_back(gusano);
				break;
			}
			case serializadoGusanoSprite: {
				GusanoSprite* gusano = new GusanoSprite();
				gusano->deserealizar(entidadSerializada);
				int frame = gusano->getFrame();
				bool cambIzq = gusano->hayCambioImgIzq();
				bool cambDer = gusano->hayCambioImgDer();

				gusano = new GusanoSprite(this->vista->renderer, gusano->getRect(),spriteWormIzq, 1, 10, 60, 600);
				gusano->setFrame(frame);
				gusano->setCambiarImgIzq(cambIzq);
				gusano->setCambiarImgDer(cambDer);
				lista->push_back(gusano);
				break;
			}
			case serializadoCirculoDibujable:{
				CirculoDibujable* circulo = new CirculoDibujable();
				circulo->deserealizar(entidadSerializada);
				lista->push_back(circulo);
				break;
			}
			case serializadoPoligonoDibujable:{
				PoligonoDibujable* poligono = new PoligonoDibujable();
				poligono->deserealizar(entidadSerializada);
				lista->push_back(poligono);
				break;
			}
			case serializadoRectanguloDibujable:{
				RectanguloDibujable* rectangulo = new RectanguloDibujable();
				rectangulo->deserealizar(entidadSerializada);
				lista->push_back(rectangulo);
				break;
			}
		default:
			{
				break;
			}
		}

		this->vista->setListaDibujables(lista);
	}
	//Agrego a lo ultimo el agua
	lista->push_back(this->dibujablesBase->back());

	//return lista;
}

JuegoCliente::~JuegoCliente(){

	//delete this->escenario;
	//delete this->evento;
	//delete Logger::getLogger();
}
