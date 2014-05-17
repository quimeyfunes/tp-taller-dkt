#include "JuegoCliente.h"

Cliente* JuegoCliente::cliente = NULL;
Vista* JuegoCliente::vistaCliente = NULL;
EscenarioParseado* JuegoCliente::esc;
list<Dibujable*>* JuegoCliente::dibujablesBase;
CRITICAL_SECTION JuegoCliente::criticalSection;

JuegoCliente::JuegoCliente(string nombreCliente, string ip){
	
	cliente = new Cliente(nombreCliente, ip);
	this->simulando = false;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	this->cartelInfo = NULL;
	while(this->cliente->recibirDeServidor());	//recibe todas las cosas del servidor hasta que le llega el paqueteDescargaLista
	
	esc = cliente->getEscenarioActual();
	vistaCliente = new Vista(esc);
	agregarTexturas(esc);
	agregarAgua(esc);
	this->dibujablesBase = new list<Dibujable*>(this->vistaCliente->getListaDibujables()->size());
	copy(vistaCliente->getListaDibujables()->begin(),vistaCliente->getListaDibujables()->end(),this->dibujablesBase->begin());
	InitializeCriticalSection(&this->criticalSection);
}

void JuegoCliente::ejecutar(){
	Logger::getLogger()->guardarEstado();
	//list<Dibujable*> *lista = new list<Dibujable*>(this->dibujablesBase->size());
	//game loop

	//_beginthread(creandoLista, 0, (void*)0);
	
	bool ok = false;
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		this->leerEvento();
		crearLista(cliente->vistaSerializada);
		this->cliente->actualizar();

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

		EnterCriticalSection(&criticalSection);
		vistaCliente->Dibujar();
		LeaveCriticalSection(&criticalSection);

		SDL_Delay(3);
	}
}

void JuegoCliente::creandoLista(void* arg){
	
	while(true){
			if(TryEnterCriticalSection(&criticalSection)){	
			crearLista(cliente->vistaSerializada);
			LeaveCriticalSection(&criticalSection);
			}

		}
}

void JuegoCliente::leerEvento(){

	if (this->vistaCliente->leerEvento(evento)){
		int accion = this->vistaCliente->getAccion();
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
			 
			e->x = (x + vistaCliente->getCorrimientoX()) / (relacionPPU * vistaCliente->getZoom());
			e->y = (y + vistaCliente->getCorrimientoY()) / (relacionPPU * vistaCliente->getZoom());
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

	vistaCliente->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, texturaFondo, "");
	vistaCliente->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, e->imagenCielo, texturaCieloDEF);
	vistaCliente->crearScrollingSprite(0, 10,  e->anchoPx/ 5, e->altoPx /10, rutaNube1);
	vistaCliente->crearScrollingSprite( e->anchoU*relacionPPU/2, 30, e->anchoPx / 5, e->altoPx / 10, rutaNube2);
	vistaCliente->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, e->imagenTierra, "");
	this->cartelInfo = vistaCliente->crearCartelInfo(10, 10, 0, 20);
	this->cartelInfo->setColor(255,0,255,100);
}

void JuegoCliente::agregarAgua(EscenarioParseado* esc){
	//vista->crearDibujableTextura(0, e->nivelAgua*this->escenario->getRelacionAlto(), terreno->getLector()->getAnchoMatriz(), terreno->getLector()->getAltoMatriz() - e->nivelAgua*this->escenario->getRelacionAlto(), texturaAgua, texturaAguaDEF);
	
	for(int i=0;i<4;i++){
		//vista->crearSprite( (i* terreno->getLector()->getAnchoMatriz())/4, e->nivelAgua* this->escenario->getRelacionAlto() - 15,  terreno->getLector()->getAnchoMatriz()/4, 15, spriteOlas, 2, 6, 256, 144);
		Sprite* sprite = vistaCliente->crearSprite( (i* esc->anchoU*relacionPPU)/4, esc->nivelAgua*relacionPPU - 24,  esc->anchoU*relacionPPU/4, 24, spriteOlas, 2, 6, 256, 144);
		sprite->setTransparencia(230);
	}
	DibujableTextura* agua = vistaCliente->crearDibujableTextura(0, esc->nivelAgua*relacionPPU, esc->anchoU*relacionPPU, (esc->altoU - esc->nivelAgua) *relacionPPU, texturaAgua, texturaAguaDEF);
	agua->setTransparencia(230);
}

//le paso la lista como parametro para no estar haciendo new cada vez que entro 
/*list<Dibujable*>* */void JuegoCliente::crearLista(string vistaSerializada){
	//elimino elementos de la vista que son figuras
	int index = 0;
	for (list<Dibujable*>::iterator it = vistaCliente->getListaDibujables()->begin(); it != vistaCliente->getListaDibujables()->end(); it++) {
		if(index > dibujablesBase->size() - 6 && index < vistaCliente->getListaDibujables()->size() - 5){
			delete *it;
		}
		index++;
	}
	
	list<Dibujable*> *lista = new list<Dibujable*>(dibujablesBase->size());


	copy(dibujablesBase->begin(),dibujablesBase->end(),lista->begin());
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

				GusanoDibujable* gusano2 = new GusanoDibujable(vistaCliente->renderer, gusano->getRect(), rutaGusano, rutaGusanoDEF);
				lista->push_back(gusano2);
				//delete gusano;
				break;
			}
			case serializadoGusanoSprite: {
				GusanoSprite* gusano = new GusanoSprite();
				gusano->deserealizar(entidadSerializada);
				int frame = gusano->getFrame();
				GusanoSprite* gusano2 = new GusanoSprite(vistaCliente->renderer, gusano->getRect(),spriteWormIzq, 1, 10, 60, 600, gusano->getNombre(),esc->maximosClientes);
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
				gusano2->cliente = cliente->getId();
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

	list<Dibujable*> *listaAnterior = vistaCliente->getListaDibujables();
	//borro la lista anterior(no los elementos)
	delete listaAnterior;
	vistaCliente->setListaDibujables(lista);
	index = 0;
	for (list<Dibujable*>::iterator it = dibujablesBase->begin(); it != dibujablesBase->end(); it++) {
		if (index > dibujablesBase->size() - 6) {
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
	DeleteCriticalSection(&criticalSection);
}
