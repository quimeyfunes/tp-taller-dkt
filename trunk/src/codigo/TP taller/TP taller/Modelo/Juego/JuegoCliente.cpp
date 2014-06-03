#include "JuegoCliente.h"

Cliente* JuegoCliente::cliente = NULL;

JuegoCliente::JuegoCliente(string nombreCliente, string ip){
	
	cliente = new Cliente(nombreCliente, ip);
	this->simulando = false;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	this->cartelInfo = NULL;
	while(this->cliente->recibirDeServidor());	//recibe todas las cosas del servidor hasta que le llega el paqueteDescargaLista
	
	bool a = true;
	//espero que el servidor me diga q arranque...
	while(this->cliente->arrancarJuego == false){
		if(a){
			cout <<"esperando a los demas jugadores..."<<endl;
			a = false;
		}
	};


	this->esc = cliente->getEscenarioActual();
	//this->armas = cliente->getArmasActual();
	this->vista = new Vista(esc);
	agregarTexturas(esc);
	agregarAgua(esc);
	this->dibujablesBase = new list<Dibujable*>(this->vista->getListaDibujables()->size());
	copy(this->vista->getListaDibujables()->begin(),this->vista->getListaDibujables()->end(),this->dibujablesBase->begin());

	for (int i = 0; i < teclas; i++) {
		eventos[i] = false;
	}
	
}

void JuegoCliente::ejecutar(){



	Reproductor::getReproductor()->activar();
	Logger::getLogger()->guardarEstado();
	//list<Dibujable*> *lista = new list<Dibujable*>(this->dibujablesBase->size());
	//game loop

	const int SKIP_TICKS = 1000 / FPS;
	int sleepTime =0;
    DWORD next_game_tick = GetTickCount();

	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->leerEvento();
		this->cliente->actualizar();
		this->reloj->setTiempoActual(this->cliente->getTiempoActualDeJuego());
		
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

	/*	next_game_tick += SKIP_TICKS;
        sleepTime = next_game_tick - GetTickCount();
        if( sleepTime >= 0 ) {
            Sleep( sleepTime );
        }*/
	}
}

void JuegoCliente::leerEvento(){

	if (this->vista->leerEvento(evento)){
		int accion = this->vista->getAccion();
		switch(accion){

		case SALIR:			salir();						break;
		//case JUGAR:			reiniciar();					break;
		case CLICKDERECHO:		this->alternarPanelArmas();				break;
		}

		bool enviar = false;
		if ( accion == IZQUIERDA || accion == DERECHA || accion == ABAJO || accion == ARRIBA || accion == ENTER || accion == ESPACIO) {
			if (!this->eventos[accion]) {
				this->eventos[accion] = true;
				enviar = true;
			}		
		} else {
			if (accion == SOLTARIZQUIERDA || accion == SOLTARABAJO || accion == SOLTARESPACIO || accion == SOLTARENTER || accion == SOLTARARRIBA || accion == SOLTARDERECHA) {
				this->eventos[accion - teclas] = false;
				enviar = true;
			}
		}
		if((accion == CLICK && !this->panelArmas->visible) || enviar){
			//Para estos eventos tengo que notificar al servidor
			Evento* e = new Evento();
			int x,y;
			SDL_GetMouseState(&x,&y);

			e->accion = accion;
			 
			e->x = (x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom());
			e->y = (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom());
			
			this->cliente->enviarEvento(e->serializar());
			delete e;
		}else if(accion == CLICK && this->panelArmas->visible){
			//Manejo el click aparte para verificar el click sobre el panel de armas
			Evento* e = new Evento();
			int x,y;
			SDL_GetMouseState(&x,&y);
			int armaSeleccionada = this->getArmaSeleccionada(x,y);
			if(armaSeleccionada >= 0){
				this->panelArmas->seleccionarArma(armaSeleccionada);

				//Notifico al servidor el arma seleccionada
				e->accion = CLICKARMA;
				e->x = armaSeleccionada;
				e->y = armaSeleccionada;
			}else{
				//Envio el click al servidor para que lo procese
				e->accion = accion;
				e->x = (x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom());
				e->y = (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom());
				
			}
			
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

void JuegoCliente::alternarPanelArmas(){
	this->panelArmas->alternarVisibilidad();
}

void JuegoCliente::esperar(){}

void JuegoCliente::agregarTexturas(EscenarioParseado* e){

	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, texturaFondo, "");
	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, e->imagenCielo, texturaCieloDEF);
	vista->crearScrollingSprite(0, 10,  e->anchoPx/ 5, e->altoPx /10, rutaNube1);
	vista->crearScrollingSprite( e->anchoU*relacionPPU/2, 30, e->anchoPx / 5, e->altoPx / 10, rutaNube2);

	//le seteo el terreno q se va a ir actualizando
	vista->crearDibujableTerreno(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, e->imagenTierra, "");
	
	//agrego el reloj
	this->reloj = vista->crearRelojSprite(0,0,3*relacionPPU,4*relacionPPU,spriteReloj,1,10,42,560);
	this->reloj->setTiempoActual(tiempoTurno);
	this->cartelInfo = vista->crearCartelInfo(10, 10, 0, 20);
	this->cartelInfo->setColor(255,0,255,100);
	//Las armas vienen en el paquete inicial
	vector<int> armas;
	this->panelArmas = vista->crearPanelArmas(e->anchoPx - e->altoPx/4, 10, e->altoPx/4, e->altoPx/4, armas);
}

void JuegoCliente::agregarAgua(EscenarioParseado* e){
	Juego::agregarAgua(e);
}

void JuegoCliente::crearLista(string vistaSerializada){
	list<Dibujable*> *lista = new list<Dibujable*>(this->vista->getListaDibujables()->size());
	int cantidadGusanosTotales = this->esc->maximosClientes*gusanosPorPersonaje + 1;

	//elimino elementos de la vista que son proyectiles, porque los creo en cada ciclo
	int index = 0;
	for (list<Dibujable*>::iterator it = this->vista->getListaDibujables()->begin(); it != this->vista->getListaDibujables()->end(); it++) {
		if(index > dibujablesBase->size() - 6 + cantidadGusanosTotales && index < this->vista->getListaDibujables()->size() - 5){
			(*it)->setDestruido(true);
		}
		index++;
	}

	copy(this->vista->getListaDibujables()->begin(),this->vista->getListaDibujables()->end(),lista->begin());
	//Saco el agua
	
	for (int index = 0; index < 5;index ++) {
		lista->pop_back();
	}
	
	//Paso la lista a vector para trabajar con los indices
	std::vector<Dibujable*> vecAux(lista->begin(), lista->end() );

	vector<string> entidadesSerializadas = StringUtil::split(vistaSerializada,separadorEntidades);
	for(int i=0; i<entidadesSerializadas.size(); i++){
		
		string entidadSerializada = entidadesSerializadas.at(i);
		vector<string> des = StringUtil::split(entidadSerializada,separadorCampoTipoEntidades);
		switch (StringUtil::str2int(des.at(0))){				
			case serializadoGusanoSprite: {
				GusanoSprite* gusano = new GusanoSprite();
				gusano->deserealizar(entidadSerializada);				
				GusanoSprite* gusano2;
				if(vecAux.size() + 5 - this->dibujablesBase->size() < cantidadGusanosTotales){
					//Si no estaba lo creo
					gusano2 = new GusanoSprite(this->vista->renderer, gusano->getRect(),spriteWormIzq, 1, 10, 60, 600, gusano->getNombre(),this->esc->maximosClientes);
					//gusano2->deserealizar(entidadSerializada);	
				}else{
					gusano2 = (GusanoSprite*)vecAux[this->dibujablesBase->size() - 5 + i];
					//gusano2->deserealizar(entidadSerializada);	
					gusano2->setRect(gusano->getRect());
                    SDL_Rect rectCart = gusano->getRect();
                    rectCart.h = gusano->getRect().h / 4;
                    rectCart.x = gusano->getRect().x + gusano->getRect().w/2;
                    rectCart.w = gusano2->getCartel()->texto.length() * 7;
                    gusano2->getCartel()->setRect(rectCart);

					SDL_Rect rectVida = gusano->getRect();
					rectVida.h = 5;
					rectVida.w = pxPorVida * vidaGusano;
					gusano2->getVida()->setRect(rectVida);
				}

				gusano2->setFrame(gusano->getFrame());
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
				gusano2->armaTipo = gusano->armaTipo;

				if(gusano2->armaTipo != NINGUNA){
					gusano2->enUso = gusano2->rectApuntando;
				}else{
					gusano2->enUso = gusano2->recCuadro;
				}

				gusano2->contArma = gusano->contArma;
				gusano2->anguloDisparo = gusano->anguloDisparo;
				gusano2->anguloRotacion = gusano->anguloRotacion;
				gusano2->mostrarCrosshair = gusano->mostrarCrosshair;
				gusano2->frameCrosshair = gusano->frameCrosshair;
				gusano2->cliente = this->cliente->getId();
				if(vecAux.size() + 5 - this->dibujablesBase->size() < cantidadGusanosTotales){
					vecAux.push_back(gusano2);
				}
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
			case serializadoArmaDibujable:{
				ArmaDibujable* arma = new ArmaDibujable();
				arma->deserealizar(entidadSerializada);

				ArmaDibujable* arma2 = new ArmaDibujable(this->vista->renderer, arma->getRect(),arma->rutaImagen,"");
				arma2->anguloRotacion = arma->anguloRotacion;
				arma2->setDestruido(arma->getDestruido());
				lista->push_back(arma2);
				delete arma;
				break;
			}
		default:
			{
				break;
			}
		}
	}

	
	//list<Dibujable*> *listaAnterior = this->vista->getListaDibujables();
	//borro la lista anterior(no los elementos)
	
	for(int j=0; j<vecAux.size(); j++){
		if(j >= this->vista->getListaDibujables()->size() - 5){
			lista->push_back(vecAux[j]);
		}
	}

	//delete listaAnterior;
	
	this->vista->setListaDibujables(lista);
	index = 0;
	for (list<Dibujable*>::iterator it = this->dibujablesBase->begin(); it != this->dibujablesBase->end(); it++) {
		if (index > this->dibujablesBase->size() - 6) {
			Dibujable* dib = *it;
			lista->push_back(dib);
		}
		index++;
	}

}

int JuegoCliente::getArmaSeleccionada(int x, int y){
	if(x > this->panelArmas->getRect().x && x < (this->panelArmas->getRect().x + this->panelArmas->getRect().w) && y > this->panelArmas->getRect().y && y < (this->panelArmas->getRect().y + this->panelArmas->getRect().h)){
		//Se clickeo dentro del panel
		int tamanoCuadradoX = this->panelArmas->getRect().w/3;
		int tamanoCuadradoY = this->panelArmas->getRect().h/3;
		int cuadradoSeleccionado = (x - this->panelArmas->getRect().x)/tamanoCuadradoX + ((y - this->panelArmas->getRect().y)/tamanoCuadradoY) * 3;
		return cuadradoSeleccionado;
	}
	return -1;
}

JuegoCliente::~JuegoCliente(){

	delete this->esc;
	delete this->reloj;
	//delete this->evento;
	delete Logger::getLogger();
}
