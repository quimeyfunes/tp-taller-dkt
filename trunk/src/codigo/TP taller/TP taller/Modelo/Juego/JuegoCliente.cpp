#include "JuegoCliente.h"
#include "../MotorParticulas/MotorParticulas.h"

Cliente* JuegoCliente::cliente = NULL;

JuegoCliente::JuegoCliente(string nombreCliente, string ip,SDL_Window* window, SDL_Renderer* renderer,Menu* menu){
	this->menu = menu; 
	string error="";
	cliente = new Cliente(nombreCliente, ip, error);
	if(error != ""){
		menu->agregarMensaje(error, 30, 0 , 255, 0); 
		menu->dibujar();
		Sleep(3000);
		exit(1);
	} 

	this->simulando = false;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	this->cartelInfo = NULL;

	while(this->cliente->recibirDeServidor(error));	//recibe todas las cosas del servidor hasta que le llega el paqueteDescargaLista
	if(error != ""){
		menu->agregarMensaje(error, 30, 0 , 255, 0); 
		menu->dibujar();
		Sleep(3000);
		exit(1);
	} 
	Sleep(200);
	//this->armas = cliente->getArmasActual();
	this->esc = cliente->getEscenarioActual();
	//cout<<"cliente "<<cliente->getId()<<endl;
	this->lector = new LectorTerreno(this->esc, this->esc->imagenTierra, this->cliente->getId());
	
	this->vista = new Vista(esc,window,renderer);
	
	agregarTexturas(esc);
	agregarAgua(esc);
	this->dibujablesBase = new list<Dibujable*>(this->vista->getListaDibujables()->size());
	copy(this->vista->getListaDibujables()->begin(),this->vista->getListaDibujables()->end(),this->dibujablesBase->begin());

	for (int i = 0; i < teclas; i++) {
		eventos[i] = false;
	}
}

void JuegoCliente::ejecutar(){

	this->cliente->enviarQuieroJugar();

	Reproductor::getReproductor()->activar();
	Logger::getLogger()->guardarEstado();
	//list<Dibujable*> *lista = new list<Dibujable*>(this->dibujablesBase->size());
	//game loop
	//espero que el servidor me diga q arranque...
	this->menu->agregarMensaje(string("Esperando a los demas jugadores..."),30,0,255,0);
	this->menu->dibujar();
	while(this->cliente->arrancarJuego == false){
		
		if (this->menu->leerEvento() == nameMenu::SALIR) return;
		this->cliente->actualizar();
	};
	
	const int SKIP_TICKS = 1000 / FPS;
	int sleepTime =0;
    DWORD next_game_tick = GetTickCount();


	while(!this->cliente->partidaTerminada && this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->leerEvento();
		this->cliente->actualizar();
		
		this->reloj->setTiempoActual(this->cliente->getTiempoActualDeJuego());
		
		this->crearLista(this->cliente->vistaSerializada);

		if(this->cliente->nuevoMensaje){
			this->cartelInfo->setInfo(this->cliente->mensajeInfo);
			this->cliente->nuevoMensaje = false;
		}

		for(int i=0; i< maxExplosionesPorTurno; i++){
			if(this->cliente->exp[i].radio >= 0){
				this->vista->destruir(cliente->exp[i].x, cliente->exp[i].y, cliente->exp[i].radio, this->lector);
				this->vista->motor->generarExplosion(cliente->exp[i].x, cliente->exp[i].y);
				this->cliente->exp[i].radio = -1;
			}
		}

		this->vista->motor->actualizar();

		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}
		
		vista->Dibujar();

		next_game_tick += SKIP_TICKS;
        sleepTime = next_game_tick - GetTickCount();
        if( sleepTime >= 0 ) {
            Sleep( sleepTime );
        }
	}

	if(this->cliente->partidaTerminada) this->volverAjugarCliente();
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
				if(accion == ESPACIO && !this->panelArmas->proyectilRestante(this->panelArmas->getArmaSeleccionada())){
					//Si no me quedan mas proyectiles no en7vio el evento
					enviar = false;
				}
			}		
		} else {
			if (accion == SOLTARIZQUIERDA || accion == SOLTARABAJO || accion == SOLTARESPACIO || accion == SOLTARENTER || accion == SOLTARARRIBA || accion == SOLTARDERECHA) {
				this->eventos[accion - teclas] = false;
				enviar = true;
				if(accion == SOLTARESPACIO){
					//Descuento un proyectil
					this->panelArmas->descontarArmaEspacio(this->panelArmas->getArmaSeleccionada());
				}
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
			
			if(armaSeleccionada >= 0 && this->panelArmas->proyectilRestante(armaSeleccionada)){
				this->panelArmas->seleccionarArma(armaSeleccionada);
				
				//Notifico al servidor el arma seleccionada
				e->accion = CLICKARMA;
				e->x = armaSeleccionada;
				e->y = armaSeleccionada;
			}else{
				//Envio el click al servidor para que lo procese
				this->panelArmas->descontarArmaClick(this->panelArmas->getArmaSeleccionada());
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
	vista->crearScrollingSprite(0, 10,  160, 60, rutaNube1);
	vista->crearScrollingSprite( e->anchoU*relacionPPU/2, 30, 160, 60, rutaNube2);

	//le seteo el terreno q se va a ir actualizando
	vista->crearDibujableTerreno(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, this->lector->getRutaTexturaActualizada(), "");
	
	//agrego el reloj
	this->reloj = vista->crearRelojSprite(0,0,3*relacionPPU,4*relacionPPU,spriteReloj,1,10,42,560);
	this->reloj->setTiempoActual(tiempoTurno);
	this->cartelInfo = vista->crearCartelInfo(10*relacionPPU, 10, 0, 20);
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
	int cantidadGusanosTotales = this->esc->maximosClientes*gusanosPorPersonaje;

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
				bool nuevo = false;
				if(vecAux.size() + 5 - this->dibujablesBase->size() < cantidadGusanosTotales){
					//Si no estaba lo creo
					gusano2 = new GusanoSprite(this->vista->renderer, gusano->getRect(),spriteWormIzq, 1, 10, 60, 600, gusano->getNombre(),this->esc->maximosClientes);
					nuevo = true;
				}else{
					gusano2 = (GusanoSprite*)vecAux[this->dibujablesBase->size() - 5 + i];
				}

				gusano->copiarGusano(gusano2, nuevo);
				gusano2->cliente = this->cliente->getId();

				if(vecAux.size() + 5 - this->dibujablesBase->size() < cantidadGusanosTotales){
					vecAux.push_back(gusano2);
				}

				//printf("mostrar: %d\n", gusano2->mostrarCrosshair);
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
			case serializadoATiempoDibujable:{
				ATiempoDibujable* timer = new ATiempoDibujable();
				timer->deserealizar(entidadSerializada);

				ATiempoDibujable* timer2 = new ATiempoDibujable(this->vista->renderer, timer->getRect(),timer->rutaImagen,"");
				timer2->anguloRotacion = timer->anguloRotacion;
				timer2->setDestruido(timer->getDestruido());
				timer2->reloj->frame = timer->reloj->frame;
				timer2->reloj->numCuadros = timer->reloj->numCuadros;
				timer2->reloj->tiempoActual = timer->reloj->tiempoActual;

				SDL_Rect rectReloj = timer->reloj->getRect();
				rectReloj.x = timer->getRect().x;
				rectReloj.y = timer->getRect().y - 5;
				timer2->reloj->setRect(rectReloj);

				lista->push_back(timer2);

				delete timer;
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

	//delete this->esc;
	delete this->reloj;
	//delete this->evento;
	delete Logger::getLogger();
}

void JuegoCliente::volverAjugarCliente(){

	menu->menuJugarDeNuevo();

	int accion = 0;
	while(accion == 0 ){
		accion = menu->leerEvento();
		cliente->enviarEstado();
		Sleep(20);
	}

	if (accion == nameMenu::NO || nameMenu::SALIR )	this->salir();

	if (accion == nameMenu::SI){
		
		this->cliente->partidaTerminada = false;
		this->cliente->arrancarJuego = false; //hay q esperar q el server de arranque.
		this->ejecutar();
	}

	
	

}