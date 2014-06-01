	#include "Juego.h"

Servidor* Juego::servidor = NULL;
string Juego::jugadorActual = "";

Juego::Juego(){
}

Juego::Juego(string texto){
	this->turno = new Turno();
	this->simulando = true;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();
	this->vista = new Vista(e);
	//SDL_HideWindow(this->vista->window);
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


	this->NUMCLICKDERECHOS=0;
}


void Juego::ejecutar(){
	Logger::getLogger()->guardarEstado();

	Reproductor::getReproductor()->activar();
	//inicio un thread para el reproductor de audio
	_beginthread(Reproductor::getReproductor()->actualizar, 0, (void*)0);
	Reproductor::getReproductor()->reproducirSonido(MUSICAFONDO);
	servidor = new Servidor();
	int tamanio;

    const int SKIP_TICKS = 1000 / FPS;
	int sleepTime =0;
    DWORD next_game_tick = GetTickCount();

	//cout << "esperando a 2 jugadores..." << endl;
	//	while( Servidor::getCantidadDeClientes()<2 ){
	//		this->chequearNuevosJugadores();
	//	}
	//
	
	while(this->estadoActual != SALIDA && (evento->type != SDL_QUIT)){
		
		/*this->turno->actualizar();
		Servidor::tiempo= this->turno->getTiempoActual();
		if( this->turno->estaTerminado() ){
			Juego::cambiarJugador(Servidor::siguienteJugador());
			cout << "Turno de: " <<Juego::getJugadorActual() << endl;
			this->turno->comenzar();
		}*/

		//this->chequearNuevosJugadores();
		this->leerEvento();
		
		

		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
			}
		}
		escenario->notificar();
		b2Vec3 explosion;
		do {
			explosion = this->escenario->hayExplosion();
		
			if ( explosion.z >= 0){
				if (this->escenario->getGusanoActivo()->armaActual.armaTipo == BANANA){
					for (int i = 0; i <5; i++) {
						this->escenario->getGusanoActivo()->setArma(new Banana(explosion.x + i, explosion.y + i, 0, this->escenario->getWorld(), false, radioExplosionBanana, radioBanana, masaBanana, tiempoExplosionMiniBanana )); 
						this->escenario->agregarArma(this->escenario->getGusanoActivo()->getArmaSeleccionada());
						this->escenario->getGusanoActivo()->getArmaSeleccionada()->agregarObservador(this->vista->crearArmaTiempoDibujable(explosion.x + i, explosion.y + i,  relacionPPU * 2*radioBanana,relacionPPU * 2*radioBanana,rutaBanana,rutaBanana));
						this->escenario->getGusanoActivo()->getArmaSeleccionada()->getBody()->ApplyForceToCenter(b2Vec2(10,80),true);
						this->escenario->getGusanoActivo()->disparar();
						this->escenario->getGusanoActivo()->armaActual.potenciaDisparo = 0;
					}
					this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA;
				}
				explosion *= relacionPPU;
				this->vista->destruir((explosion.x ),(explosion.y ),explosion.z,this->terreno->getLector());
				//aviso al servidor q se modifico el terreno
				Reproductor::getReproductor()->reproducirSonido(EXPLOSION);
				Servidor::setTerrenoModificado(true);
			}
		} while (explosion.z >= 0);
		this->servidor->dibujablesSerializados = this->crearLista(tamanio);
		this->vista->Dibujar();

        next_game_tick += SKIP_TICKS;
        sleepTime = next_game_tick - GetTickCount();
        if( sleepTime >= 0 ) {
            Sleep( sleepTime );
        }
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
						GusanoSprite* gusano = vista->crearGusanoSprite( worm->getPosicion().x * escalaAncho, worm->getPosicion().y * escalaAlto , anchoGusano * relacionPPU*2.5, altoGusano * relacionPPU*2.5, spriteWormIzq, 1, 10, 60, 600,nombreGusano,this->escenario->getMaximosClientes());
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

	if(this->escenario->getGusanoActivo() != NULL)
		if( this->escenario->getGusanoActivo()->armaActual.potenciaDisparo >= POTENCIA_MAXIMA_DISPARO) dispararArma();

	 if (this->vista->leerEvento(evento)){
		 int x, y;
				
			switch(this->vista->getAccion()){

                case SALIR:				salir();                                break;
                case JUGAR:             reiniciar();                            break;
                case PAUSAR:            alternarPausa();                        break;
				case ARRIBA:            this->escenario->arriba(true);			break;
				case ABAJO:				this->escenario->abajo(true);			break;
                case IZQUIERDA:         this->escenario->izquierda(true);       break;
                case DERECHA:           this->escenario->derecha(true);         break; 
				case ENTER:				this->escenario->enter(true);			break;
				case SOLTARENTER:		this->escenario->enter(false);			break;
                case SOLTARARRIBA:		this->escenario->arriba(false);         break;
				case SOLTARABAJO:		this->escenario->abajo(false);			break;
                case SOLTARIZQUIERDA:   this->escenario->izquierda(false);	
                                        this->escenario->reiniciarTeclas();		break;
				case SOLTARDERECHA:		this->escenario->derecha(false);
										this->escenario->reiniciarTeclas();		break; 

				case ESPACIO: 			this->escenario->espacio(true); 		
										break;

				case SOLTARESPACIO:
										this->dispararArma();
										Reproductor::getReproductor()->detenerSonido(CARGANDODISPARO);
										//Reproductor::getReproductor()->reproducirSonido(SOLTARDISPARO);
										this->escenario->espacio(false);
										break;

				case CLICK:     
						 SDL_GetMouseState(&x,&y);
						 this->escenario->click((x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom()) ,  (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom()));
						// if (!(this->escenario->click((x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom()) ,  (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom())))) {
							//this->vista->destruir((x + this->vista->getCorrimientoX()) / (this->vista->getZoom()),(y + this->vista->getCorrimientoY()) / (this->vista->getZoom()),5 * relacionPPU,this->terreno->getLector());
							//aviso al servidor q se modifico el terreno
							//Servidor::setTerrenoModificado(true);
						//}
                        break;

				case CLICKDERECHO:
						if(this->escenario->getGusanoActivo() != NULL){
							this->NUMCLICKDERECHOS++;
						//	cout<<"tengo un arma"<<endl;

							switch(NUMCLICKDERECHOS % 6){

							case 0: this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA;
									this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
									break;
							case 1: this->escenario->getGusanoActivo()->armaActual.armaTipo = BAZOOKA;
									this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
									break;
							case 2: this->escenario->getGusanoActivo()->armaActual.armaTipo = GRANADA;			
									this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
									break;
							case 3: this->escenario->getGusanoActivo()->armaActual.armaTipo = ALELUYA;
									this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
									break;
							case 4: this->escenario->getGusanoActivo()->armaActual.armaTipo = DINAMITA;
									this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
									break;
							case 5: this->escenario->getGusanoActivo()->armaActual.armaTipo = BANANA;
									this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
									break;
							}

						}
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
					case CLICK:	{
						list<Gusano*> figurasOtrosClientes;
						for(int j=0; j< this->escenario->getMaximosClientes(); j++){
							if(i != j){
								figurasOtrosClientes.insert(figurasOtrosClientes.end(), this->servidor->clientes[j].figuras.begin(), this->servidor->clientes[j].figuras.end());
							}
						}
						this->escenario->clickCliente(i,this->servidor->clientes[i].figuras,figurasOtrosClientes, evento->x, evento->y);
						break;
					}
					case CLICKARMA:	{
						switch(evento->x){
							/*case 0: this->escenario->getFigurasActivas()[i]->armaActual.armaTipo = BAZOOKA; break;
							case 1: this->escenario->getFigurasActivas()[i]->armaActual.armaTipo = GRANADA; break;
							case 2: this->escenario->getFigurasActivas()[i]->armaActual.armaTipo = ALELUYA; break;
							case 3: this->escenario->getFigurasActivas()[i]->armaActual.armaTipo = DINAMITA; break;
							default: this->escenario->getFigurasActivas()[i]->armaActual.armaTipo = NINGUNA; break;*/
						}
					}

				}

				delete evento;
				this->servidor->clientes[i].ultimoEventoSerializado = "";
			}
		}
	}

}

void Juego::dispararArma(){

	b2Vec2 posGusano;
	b2Vec2 posD;
	ArmaDibujable* arma;

	if((this->escenario->getGusanoActivo() != NULL)&&(this->escenario->getGusanoActivo()->armaActual.armaTipo != NINGUNA ) && !(this->escenario->getGusanoActivo()->estaMuerto())){
		posGusano=this->escenario->getFiguraActiva()->getBody()->GetWorldCenter();
		posD = posGusano; //getPosicionInicialDisparo(posGusano, this->escenario->getGusanoActivo()->armaActual.anguloDisparo, this->escenario->getGusanoActivo()->armaActual.sentidoDisparo, altoGusano/2 + 1.5);
					

		switch (this->escenario->getGusanoActivo()->armaActual.armaTipo){
		case BAZOOKA:
			this->escenario->getGusanoActivo()->setArma(new Bazooka(posD.x, posD.y, 0, this->escenario->getWorld(), false, anchoBazooka, altoBazooka, masaBazooka, radioExplosionBazooka ));
				arma = this->vista->crearArmaContactoDibujable(posD.x, posD.y, anchoBazooka*relacionPPU,altoBazooka*relacionPPU,rutaBaz,rutaBaz);
							
			break;
				
		case GRANADA:
			this->escenario->getGusanoActivo()->setArma(new Granada(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionGranada, radioGranada, masaGranada , tiempoExplosionGranada));
			arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y,  relacionPPU * 2*radioGranada,relacionPPU * 2*radioGranada,rutaGranada,rutaGranada); 
			break;

		case ALELUYA:
			this->escenario->getGusanoActivo()->setArma(new Aleluya(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionAleluya, radioAleluya, masaAleluya ));
			arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y,  relacionPPU * 2*radioAleluya,relacionPPU * 2*radioAleluya,rutaAleluya,rutaAleluya); 
			break;

		case DINAMITA:
			this->escenario->getGusanoActivo()->setArma(new Dinamita(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionDinamita, anchoDinamita, altoDinamita, masaDinamita, tiempoExplosionDinamita));
			arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y, relacionPPU * anchoDinamita,relacionPPU * altoDinamita,rutaDinamita,rutaDinamita);
			break;
		case BANANA:
			this->escenario->getGusanoActivo()->setArma(new Banana(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionBanana, radioBanana, masaBanana, tiempoExplosionBanana ));
			arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y,  relacionPPU * 2*radioBanana,relacionPPU * 2*radioBanana,rutaBanana,rutaBanana); 
			break;		
			}
			this->escenario->agregarArma(this->escenario->getGusanoActivo()->getArmaSeleccionada());
			this->escenario->getGusanoActivo()->getArmaSeleccionada()->agregarObservador(arma);
			this->escenario->getGusanoActivo()->disparar();
			this->escenario->getGusanoActivo()->armaActual.potenciaDisparo = 0;
	}
}

b2Vec2 Juego::getPosicionInicialDisparo(b2Vec2 posGusano, int angulo, bool sentido, double separacion){

	b2Vec2 ret;
	ret.x = separacion*cos(angulo*DEGTORAD);
	ret.y = posGusano.y - separacion*sin(angulo*DEGTORAD);
	
	if(sentido) ret.x*=-1;
	ret.x += posGusano.x;
	
	return ret;
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
				/*Rectangulo* rec = escenario->crearRectangulo(*it);
				if(rec){
					//cout<<(*it).ancho<<endl;
					RectanguloDibujable* rectangulo = vista->crearRectanguloDibujable((*it).ancho * escalaAncho, (*it).alto * escalaAlto);
					rectangulo->setColor(ParserDeHexARgb::parsearDeHexARgb((*it).color));
					rec->agregarObservador(rectangulo);
				}
				break;
			}*/
				worm = escenario->crearGusano(*it);
				if (worm){
					//GusanoDibujable* gusano = vista->crearGusanoDibujable((*it).x * escalaAncho, (*it).y * escalaAlto , (*it).ancho * escalaAncho, (*it).alto * escalaAlto, rutaGusano, rutaGusanoDEF);
					GusanoSprite* gusano = vista->crearGusanoSprite( (*it).x * escalaAncho, (*it).y * escalaAlto , anchoGusano * escalaAlto * 2.5, altoGusano * escalaAncho * 2.5, spriteWormIzq, 1, 10, 60, 600,"Rasta",4);
					worm->agregarObservador(gusano);
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
	Reproductor::getReproductor()->apagar();
	delete Reproductor::getReproductor();
	//delete this->evento;
	//delete Logger::getLogger();
}


string Juego::getJugadorActual(){
	return Juego::jugadorActual;
}

void Juego::cambiarJugador(string jugador){
	Juego::jugadorActual = jugador;
}
