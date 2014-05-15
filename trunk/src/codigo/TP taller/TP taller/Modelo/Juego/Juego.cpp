	#include "Juego.h"

Servidor* Juego::servidor = NULL;

Juego::Juego(){
}

Juego::Juego(string texto){
	this->simulando = true;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();
	this->vista = new Vista(e);
	SDL_HideWindow(this->vista->window);
	this->escenario = new Escenario(e->altoU ,e->anchoU, e->nivelAgua, relacionPPU, relacionPPU, e->maximosClientes);
	this->terreno = new Terreno(this->escenario->getWorld());
	this->terreno->generarTerreno(e->imagenTierra);
	this->escenario->setTerreno(this->terreno);
	this->mundo = escenario->getWorld();
	ResolverContacto* resolverContacto = new ResolverContacto();
	this->mundo->SetContactListener(resolverContacto);
	agregarTexturas(e);
	agregarObjetos();
	agregarAgua(e);
}


void Juego::ejecutar(){
	Logger::getLogger()->guardarEstado();

	servidor = new Servidor();
	 int tamanio;

	//game loop
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		this->chequearNuevosJugadores();
		this->leerEvento();

		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}
		escenario->notificar();
		this->servidor->dibujablesSerializados = this->crearLista(tamanio);
		SDL_Delay(3);

	}
}

string Juego::crearLista(int &tamanio){
	//list<DibujableSerializado> lista;
	string lista = "";
	string serializado = "";
	tamanio=0;
	bool first = true;
	for (list<Dibujable*>::iterator it = vista->getListaDibujables()->begin(); it != vista->getListaDibujables()->end(); it++) {
		serializado = (*it)->serializar();
		if(serializado != ""){
			if(!first){
				lista += separadorEntidades;
			}
			lista += serializado;
			first = false;
		}
	}

	return lista;
}

void Juego::chequearNuevosJugadores(){
	for(int i=0; i< this->escenario->getMaximosClientes(); i++){
		if(this->servidor->clientes[i].activo){
			if(this->servidor->clientes[i].figuras.size() == 0){
				//Si el cliente esta activo y no tiene figuras es porque acaba de conectarse. Le asigno gusanos
				for(int j=0; j< gusanosPorPersonaje; j++){
					float escalaAncho = relacionPPU;
					float escalaAlto = relacionPPU;
					Gusano* worm = escenario->crearGusanoParaJugador();
					if (worm){
						string nombreGusano = this->servidor->clientes[i].username + " "+StringUtil::int2string(j);
						GusanoSprite* gusano = vista->crearGusanoSprite( worm->getPosicion().x * escalaAncho, worm->getPosicion().y * escalaAlto , anchoGusano * 5, altoGusano * 5, spriteWormIzq, 1, 10, 60, 600,nombreGusano,this->escenario->getMaximosClientes());
						worm->agregarObservador(gusano);
						this->servidor->clientes[i].figuras.push_back(worm);
					} 
					this->escenario->inicializarCliente(i);
				}
			}
		}
	}
}

void Juego::leerEvento(){
	 if (this->vista->leerEvento(evento)){

                switch(this->vista->getAccion()){

                case SALIR:                     salir();                                                break;
                case JUGAR:                     reiniciar();                                    break;
                case PAUSAR:            alternarPausa();                                break;
                case ARRIBA:            this->escenario->arriba(true);          break;
                case IZQUIERDA:         this->escenario->izquierda(true);       break;
                case DERECHA:           this->escenario->derecha(true);         break; 
                case SOLTARARRIBA:              this->escenario->arriba(false);         break;
                case SOLTARIZQUIERDA:   {this->escenario->izquierda(false);
                                                                 this->escenario->reiniciarTeclas();}   break;
                case SOLTARDERECHA:             {this->escenario->derecha(false);
                                                                 this->escenario->reiniciarTeclas();}   break; 
                case CLICK:     
                        int x,y;
                        SDL_GetMouseState(&x,&y);
                        this->escenario->click((x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom()) ,  (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom()));
                        break;
                }
       }
		
	
	
	//Lector de eventos de los clientes. Lo anterior lo dejo para que siga funcionando mover en el servidor
	for(int i=0; i< this->escenario->getMaximosClientes(); i++){
		if(this->servidor->clientes[i].activo){
			//Si el cliente esta activo chequeo eventos
			string ultimoEvento = this->servidor->clientes[i].ultimoEventoSerializado;
			if(ultimoEvento != ""){
				Evento* evento = new Evento();
				evento->deserealizar(ultimoEvento);
				switch(evento->accion){

					case SALIR:			salir();						break;
					case JUGAR:			reiniciar();					break;
					case PAUSAR:		alternarPausa();				break;
					case ARRIBA:		this->escenario->arribaCliente(i,true);		break;
					case IZQUIERDA:		this->escenario->izquierdaCliente(i,true);	break;
					case DERECHA:		this->escenario->derechaCliente(i,true);		break; 
					case SOLTARARRIBA:		this->escenario->arribaCliente(i,false);		break;
					case SOLTARIZQUIERDA:	{this->escenario->izquierdaCliente(i,false);
											 this->escenario->reiniciarTeclas();}	break;
					case SOLTARDERECHA:		{this->escenario->derechaCliente(i,false);
											 this->escenario->reiniciarTeclas();}	break; 
					case CLICK:	
						list<Gusano*> figurasOtrosClientes;
						for(int j=0; j< this->escenario->getMaximosClientes(); j++){
							if(i != j){
								figurasOtrosClientes.insert(figurasOtrosClientes.end(), this->servidor->clientes[j].figuras.begin(), this->servidor->clientes[j].figuras.end());
							}
						}
						this->escenario->clickCliente(i,this->servidor->clientes[i].figuras,figurasOtrosClientes, evento->x, evento->y);
						break;
				}

				delete evento;
				this->servidor->clientes[i].ultimoEventoSerializado = "";
			}
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

	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, texturaFondo, "");
	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU, e->imagenCielo, texturaCieloDEF);
	vista->crearScrollingSprite(0, 10,  e->anchoPx/ 5, e->altoPx /10, rutaNube1);
	vista->crearScrollingSprite( e->anchoU*relacionPPU/2, 30, e->anchoPx / 5, e->altoPx / 10, rutaNube2);
	vista->crearDibujableTextura(0, 0, e->anchoU*relacionPPU, e->altoU*relacionPPU,terreno->getLector()->getRutaTexturaActualizada(), "");
}

void Juego::agregarAgua(EscenarioParseado* e){
	//vista->crearDibujableTextura(0, e->nivelAgua*this->escenario->getRelacionAlto(), terreno->getLector()->getAnchoMatriz(), terreno->getLector()->getAltoMatriz() - e->nivelAgua*this->escenario->getRelacionAlto(), texturaAgua, texturaAguaDEF);
	
	for(int i=0;i<4;i++){
		//vista->crearSprite( (i* terreno->getLector()->getAnchoMatriz())/4, e->nivelAgua* this->escenario->getRelacionAlto() - 15,  terreno->getLector()->getAnchoMatriz()/4, 15, spriteOlas, 2, 6, 256, 144);
		Sprite* sprite = vista->crearSprite( (i* e->anchoU*relacionPPU)/4, e->nivelAgua*relacionPPU - 24,  e->anchoU*relacionPPU/4, 24, spriteOlas, 2, 6, 256, 144);
		sprite->setTransparencia(230);
	}
	DibujableTextura* agua = vista->crearDibujableTextura(0, e->nivelAgua*relacionPPU, e->anchoU*relacionPPU, (e->altoU - e->nivelAgua) *relacionPPU, texturaAgua, texturaAguaDEF);
	agua->setTransparencia(230);
}

void Juego::agregarObjetos(){
	Gusano* worm;
	vector<ObjetoParseado>* objetos = ParserYaml::getParser()->getObjetos();	
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();

	float escalaAncho = relacionPPU;
	float escalaAlto = relacionPPU;
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
					//cout<<(*it).ancho<<endl;
					RectanguloDibujable* rectangulo = vista->crearRectanguloDibujable((*it).ancho * escalaAncho, (*it).alto * escalaAlto);
					rectangulo->setColor(ParserDeHexARgb::parsearDeHexARgb((*it).color));
					rec->agregarObservador(rectangulo);
				}
				break;
			}
			/*worm = escenario->crearGusano(*it);
			if (worm){
				//GusanoDibujable* gusano = vista->crearGusanoDibujable((*it).x * escalaAncho, (*it).y * escalaAlto , (*it).ancho * escalaAncho, (*it).alto * escalaAlto, rutaGusano, rutaGusanoDEF);
				GusanoSprite* gusano = vista->crearGusanoSprite( (*it).x * escalaAncho, (*it).y * escalaAlto , anchoGusano * 5, altoGusano * 5, spriteWormIzq, 1, 10, 60, 600,"Rasta");
				worm->agregarObservador(gusano);
			} */
			break;
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

	//delete this->evento;
	//delete Logger::getLogger();
}
