#include "JuegoCliente.h"

Cliente* JuegoCliente::cliente = NULL;

JuegoCliente::JuegoCliente(string nombreCliente, string ip){
	
	cliente = new Cliente(nombreCliente, ip);

	this->simulando = false;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	this->cartelInfo = NULL;
	while(this->cliente->recibirDeServidor());	//recibe todas las cosas del servidor hasta que le llega el paqueteDescargaLista
	
	this->esc = cliente->getEscenarioActual();
	this->vista = new Vista(esc);
	agregarTexturas(esc);
	agregarAgua(esc);
	this->dibujablesBase = new list<Dibujable*>(this->vista->getListaDibujables()->size());
	copy(this->vista->getListaDibujables()->begin(),this->vista->getListaDibujables()->end(),this->dibujablesBase->begin());
	
}

void JuegoCliente::ejecutar(){
	Logger::getLogger()->guardarEstado();
	//list<Dibujable*> *lista = new list<Dibujable*>(this->dibujablesBase->size());
	//game loop
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->leerEvento();
		this->cliente->actualizar();
		this->crearLista(this->cliente->vistaSerializada);

		if(this->cliente->nuevoMensaje){
			this->cartelInfo->setInfo(this->cliente->mensajeInfo);
			this->cliente->nuevoMensaje = false;
		}

		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}
		vista->Dibujar();
		//SDL_Delay(3);
	}
}

void JuegoCliente::leerEvento(){

	if (this->vista->leerEvento(evento)){
		int accion = this->vista->getAccion();
		switch(accion){

		case SALIR:			salir();						break;
		//case JUGAR:			reiniciar();					break;
		//case PAUSAR:		alternarPausa();				break;
		}

		if(accion == CLICK || accion == IZQUIERDA || accion == DERECHA || accion == ARRIBA || accion == SOLTARARRIBA || accion == SOLTARIZQUIERDA || accion == SOLTARDERECHA){
			//Para estos eventos tengo que notificar al servidor
			Evento* e = new Evento();
			int x,y;
			SDL_GetMouseState(&x,&y);

			e->accion = accion;
			 
			e->x = (x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom());
			e->y = (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom());
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

	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, texturaFondo, "");
	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, e->imagenCielo, texturaCieloDEF);
	vista->crearScrollingSprite(0, 10,  e->anchoPx/ 5, e->altoPx /10, rutaNube1);
	vista->crearScrollingSprite( e->anchoU*relacionPPU/2, 30, e->anchoPx / 5, e->altoPx / 10, rutaNube2);
	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, e->imagenTierra, "");
	this->cartelInfo = vista->crearCartelInfo(10, 10, 0, 20);
	this->cartelInfo->setColor(255,0,255,100);
}

void JuegoCliente::agregarAgua(EscenarioParseado* e){
	Juego::agregarAgua(e);
}

//le paso la lista como parametro para no estar haciendo new cada vez que entro 
/*list<Dibujable*>* */void JuegoCliente::crearLista(string vistaSerializada){
	//elimino elementos de la vista que son figuras
	int index = 0;
	for (list<Dibujable*>::iterator it = this->vista->getListaDibujables()->begin(); it != this->vista->getListaDibujables()->end(); it++) {
		if(index > this->dibujablesBase->size() - 6 && index < this->vista->getListaDibujables()->size() - 5){
			delete *it;
		}
		index++;
	}
	
	list<Dibujable*> *lista = new list<Dibujable*>(this->dibujablesBase->size());


	copy(this->dibujablesBase->begin(),this->dibujablesBase->end(),lista->begin());
	//Saco el agua
	
	for (index = 0; index < 5;index ++) {
		lista->pop_back();
	}
	
	vector<string> entidadesSerializadas = StringUtil::split(vistaSerializada,separadorEntidades);
	for (int i = 0; i < entidadesSerializadas.size(); i++) {
		string entidadSerializada = entidadesSerializadas.at(i);
		vector<string> des = StringUtil::split(entidadSerializada,separadorCampoTipoEntidades);
		switch (StringUtil::str2int(des.at(0))){
			case serializadoGusanoDibujable: {
				GusanoDibujable* gusano = new GusanoDibujable();
				gusano->deserealizar(entidadSerializada);

				GusanoDibujable* gusano2 = new GusanoDibujable(this->vista->renderer, gusano->getRect(), rutaGusano, rutaGusanoDEF);
				lista->push_back(gusano2);
				//delete gusano;
				break;
			}
			case serializadoGusanoSprite: {
				GusanoSprite* gusano = new GusanoSprite();
				gusano->deserealizar(entidadSerializada);
				int frame = gusano->getFrame();
				GusanoSprite* gusano2 = new GusanoSprite(this->vista->renderer, gusano->getRect(),spriteWormIzq, 1, 10, 60, 600, gusano->getNombre(),this->esc->maximosClientes);
				gusano2->setFrame(frame);
				gusano2->setCambiarImgIzq(gusano->hayCambioImgIzq());
				gusano2->setCambiarImgDer(gusano->hayCambioImgDer());
				gusano2->contDer = gusano->contDer;
				gusano2->contIzq = gusano->contIzq;
				gusano2->contFrent = gusano->contFrent;
				gusano2->contMuerte = gusano->contMuerte;
				gusano2->contador = gusano->contador;
				gusano2->estado = gusano->estado;
				gusano2->mostrarCartel = gusano->mostrarCartel;
				gusano2->velocidadRefresco = gusano->velocidadRefresco;
				gusano2->numCuadros = gusano->numCuadros;
				gusano2->congelado = gusano->congelado;
				gusano2->cliente = this->cliente->getId();
				lista->push_back(gusano2);

				delete gusano;
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

	}

	list<Dibujable*> *listaAnterior = this->vista->getListaDibujables();
	//borro la lista anterior(no los elementos)
	delete listaAnterior;
	this->vista->setListaDibujables(lista);
	index = 0;
	for (list<Dibujable*>::iterator it = this->dibujablesBase->begin(); it != this->dibujablesBase->end(); it++) {
		if (index > this->dibujablesBase->size() - 6) {
			Dibujable* dib = *it;
			lista->push_back(dib);
		}
		index++;
	}
	//Agrego a lo ultimo el agua
	//lista->push_back(this->dibujablesBase->back());

	//return lista;
}

JuegoCliente::~JuegoCliente(){

	delete this->esc;
	//delete this->evento;
	delete Logger::getLogger();
}
