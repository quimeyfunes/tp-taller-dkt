#include "Juego.h"

Servidor* Juego::servidor = NULL;
//string Juego::jugadorActual = "";

Juego::Juego(){
}

Juego::Juego(string texto,SDL_Window* window, SDL_Renderer* renderer,Menu* menu){
	this->menu = menu;
	this->turno = new Turno();
	this->simulando = true;
	this->estadoActual = JUGANDO;
	this->evento = new SDL_Event();
	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();
	this->vista = new Vista(e,window,renderer);
	//SDL_HideWindow(this->vista->window);
	this->escenario = new Escenario(e->altoU ,e->anchoU, e->nivelAgua, relacionPPU, relacionPPU, e->maximosClientes);
	this->terreno = new Terreno(this->escenario->getWorld());

	this->mundo = escenario->getWorld();
	ResolverContacto* resolverContacto = new ResolverContacto();
	this->mundo->SetContactListener(resolverContacto);

	//agregarTexturas(e);
	////agregarObjetos();
	//agregarAgua(e);

	this->NUMCLICKDERECHOS=0;
	servidor = new Servidor();
}


void Juego::ejecutar(){
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	this->terreno->generarTerreno(e);
	this->escenario->setTerreno(this->terreno);

	agregarTexturas(e);
	//agregarObjetos();
	agregarAgua(e);

	this->estadoActual = JUGANDO;
	Logger::getLogger()->guardarEstado();
	Reproductor::getReproductor()->apagar();
	Reproductor::getReproductor()->enviar = true;	//setea si enviar o no los sonidos al cliente
	
	int tamanio;
	int vidaRestada = -1;
	explosion exp;
    const int SKIP_TICKS = 1000 / FPS;
	int sleepTime =0;
    DWORD next_game_tick = GetTickCount();

	bool empezo = true;

	this->menu->agregarMensaje("Esperando a 2 jugadores...",30,255,0,0);//cout << "esperando a 2 jugadores..." << endl;
	this->menu->dibujar();
	while(!Servidor::esperarA(ParserYaml::getParser()->getEscenario()->maximosClientes)){
			
		if (this->menu->leerEvento() == nameMenu::SALIR) return;

	}

	Servidor::darArranque();
	this->chequearNuevosJugadores();
	


	Reproductor::getReproductor()->reproducirSonido(MUSICAFONDO);

	//this->menu->dibujar();
	int vidaGusanoActivo=0;
	int contadorAhogado=-1;
	int contadorLastimado=-1;
	while(this->estadoActual != SALIDA && this->estadoActual != GANADO && (evento->type != SDL_QUIT)){
		this->turno->actualizar();
		Servidor::tiempo = this->turno->getTiempoActual();
		if( this->turno->estaTerminado() ){
			this->escenario->setBloquearTeclas(false);
			this->escenario->detenerMovimientos();
			cambiarJugador(Servidor::siguienteJugador());
			//cout << "Turno de: " <<getJugadorActual() << endl;
			reproducirSonidosFinTurno(vidaRestada);
			vidaRestada = 0;
			vidaGusanoActivo=this->escenario->getGusanoActivo()->getVida();
			contadorAhogado=0;
			contadorLastimado=0;
			this->turno->comenzar();
			this->accionRealizada = false;
		}
		
		this->leerEvento();
		//si el turno esta detenido es porque alguien disparo entonces le pregunto al escenario si ya terminaron los disparos.
		if(this->turno->estaDetenido() && escenario->getHuboDisparo() && !( escenario->hayExposionPendiente() ) ){
				this->turno->esperarDisparo();
				escenario->setHuboDisparo(false);
		}
		
		//this->comprobarGanador();
		vidaRestada += this->escenario->restarVidaGusanos();

		//si el gusano que esta jugando y se ahoga, pierde el turno
		if( (contadorAhogado == 0) &&  (escenario->getGusanoActivo()->estaAhogado()) ){
			this->escenario->setBloquearTeclas(true);
			this->escenario->detenerMovimientos();
			this->turno->esperarGolpe();
			contadorAhogado++;
		}


		//si se lastima y no hubo disparo, pierde el turno
		if( (contadorLastimado == 0) && (!this->escenario->getHuboDisparo())  &&  (vidaGusanoActivo != escenario->getGusanoActivo()->getVida())  ){
			this->escenario->setBloquearTeclas(true);
			this->escenario->detenerMovimientos();
			this->turno->esperarGolpe();
			contadorLastimado++;
		}


		//si se desconecta cuando esta jugando, se cambia de jugador
		if(this->escenario->getGusanoActivo()->getCongelado()) this->turno->terminar();
	
		if(simulando){
			switch(estadoActual){

				case JUGANDO:		jugar();	break;
				case PAUSADO:		esperar();	break;
				//case GANADO:		volverAjugarServidor();	break;
				
			}
		}
		escenario->notificar();
		b2Vec3 explosion;

		this->servidor->dibujablesSerializados = this->crearLista(tamanio);
		
		if (this->menu->leerEvento() == nameMenu::SALIR) return;
		//this->vista->Dibujar();
		do {
			explosion = this->escenario->hayExplosion();
		
			if ( explosion.z >= 0){
				this->escenario->setBloquearTeclas(true);
				if (this->escenario->getGusanoActivo()->armaActual.armaTipo == BANANA){
					for (int i = 0; i <5; i++) {
						//Tiempo random para la explosion de la mini banana entre 1 y 3 segundos
						int tiempoExplosionMiniBanana = rand() % 3 + 1;
						b2Vec2 fuerza;
						fuerza.x = rand() % 1500 + 500;
						fuerza.y = -(rand() % 1500 + 500);
						if (i % 2 == 0){
							fuerza.x = -fuerza.x;
						}
						this->escenario->getGusanoActivo()->setArma(new Banana(explosion.x , explosion.y , 0, this->escenario->getWorld(), false, radioExplosionMiniBanana, radioBanana, masaBanana, tiempoExplosionMiniBanana )); 
						this->escenario->agregarArma(this->escenario->getGusanoActivo()->getArmaSeleccionada());
						this->escenario->getGusanoActivo()->getArmaSeleccionada()->agregarObservador(this->vista->crearArmaTiempoDibujable(explosion.x, explosion.y,  relacionPPU * 2*radioBanana,relacionPPU * 2*radioBanana,rutaBanana,rutaBanana));
						this->escenario->getGusanoActivo()->disparar();
						this->escenario->getGusanoActivo()->getArmaSeleccionada()->getBody()->ApplyForceToCenter(fuerza, true);
						this->escenario->getGusanoActivo()->armaActual.potenciaDisparo = 0;
					}
					this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA;
				}
				explosion *= relacionPPU;
				this->vista->destruir((explosion.x ),(explosion.y ),explosion.z,this->terreno->getLector());
				//aviso al servidor q se modifico el terreno
				exp.radio = explosion.z;
				exp.x = explosion.x;
				exp.y = explosion.y;
				Reproductor::getReproductor()->reproducirSonido(EXPLOSION);
				Servidor::setTerrenoModificado(exp);
				
				
			}
		} while (explosion.z >= 0);
		

        //next_game_tick += SKIP_TICKS;
        //sleepTime = next_game_tick - GetTickCount();
        //if( sleepTime >= 0 ) {
            Sleep(12);
        //}
	}

	if(this->estadoActual == GANADO) this->volverAjugarServidor();
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
		if(this->servidor->clientes[i].puedeJugar){
			if(this->servidor->clientes[i].figuras.size() == 0){
				//Si el cliente esta activo y no tiene figuras es porque acaba de conectarse. Le asigno gusanos
				for(int j=0; j< gusanosPorPersonaje; j++){
					float escalaAncho = relacionPPU;
					float escalaAlto = relacionPPU;
					Gusano* worm = NULL;
					worm = escenario->crearGusanoParaJugador();
					if (worm){
						string nombreGusano = this->servidor->clientes[i].username + " " + StringUtil::int2string(j);
						GusanoSprite* gusano = vista->crearGusanoSprite( worm->getPosicion().x * escalaAncho, worm->getPosicion().y * escalaAlto , anchoGusano * relacionPPU*2.5, altoGusano * relacionPPU*2.5, spriteWormIzq, 1, 10, 60, 600,nombreGusano,this->escenario->getMaximosClientes());
						worm->agregarObservador(gusano);
						this->servidor->clientes[i].figuras.push_back(worm);
					} 
					//this->escenario->inicializarCliente(i);
				}
			}
		}
	}
}

void Juego::leerEvento(){

	if(this->escenario->getGusanoActivo() != NULL)
		if( this->escenario->getGusanoActivo()->armaActual.potenciaDisparo >= POTENCIA_MAXIMA_DISPARO) dispararArma();

	 //if (this->vista->leerEvento(evento)){
		// int x, y;
		//		
		//	switch(this->vista->getAccion()){

  //              case SALIR:				salir();                                break;
  //              case JUGAR:             reiniciar();                            break;
  //              case PAUSAR:            alternarPausa();                        break;
		//		case ARRIBA:            this->escenario->arriba(true);			break;
		//		case ABAJO:				this->escenario->abajo(true);			break;
  //              case IZQUIERDA:         this->escenario->izquierda(true);       break;
  //              case DERECHA:           this->escenario->derecha(true);         break; 
		//		case ENTER:				this->escenario->enter(true);			break;
		//		case SOLTARENTER:		this->escenario->enter(false);			break;
  //              case SOLTARARRIBA:		this->escenario->arriba(false);         break;
		//		case SOLTARABAJO:		this->escenario->abajo(false);			break;
  //              case SOLTARIZQUIERDA:   this->escenario->izquierda(false);	
  //                                      this->escenario->reiniciarTeclas();		break;
		//		case SOLTARDERECHA:		this->escenario->derecha(false);
		//								this->escenario->reiniciarTeclas();		break; 

		//		case ESPACIO: 			this->escenario->espacio(true); 		
		//								break;

		//		case SOLTARESPACIO:
		//								//if( (this->escenario->getGusanoActivo()->armaActual.armaTipo) != MISILES){
		//									
		//									this->dispararArma();
		//									Reproductor::getReproductor()->detenerSonido(CARGANDODISPARO);
		//									//Reproductor::getReproductor()->reproducirSonido(SOLTARDISPARO);
		//								//}
		//								this->escenario->espacio(false);
		//								break;

		//		case CLICK:     
		//				 SDL_GetMouseState(&x,&y);
		//				 this->escenario->click((x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom()) ,  (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom()));
		//				// if (!(this->escenario->click((x + this->vista->getCorrimientoX()) / (relacionPPU * this->vista->getZoom()) ,  (y + this->vista->getCorrimientoY()) / (relacionPPU * this->vista->getZoom())))) {
		//					//this->vista->destruir((x + this->vista->getCorrimientoX()) / (this->vista->getZoom()),(y + this->vista->getCorrimientoY()) / (this->vista->getZoom()),5 * relacionPPU,this->terreno->getLector());
		//					//aviso al servidor q se modifico el terreno
		//					//Servidor::setTerrenoModificado(true);
		//				//}
		//				 if(this->escenario->getGusanoActivo() != NULL){
		//					 if( (this->escenario->getGusanoActivo()->armaActual.armaTipo) == MISILES  ){

		//						 x= ( x + this->vista->getCorrimientoX() ) / (relacionPPU * this->vista->getZoom())  ;
		//						 x -=12;
		//						 Reproductor::getReproductor()->reproducirSonido(INCOMING);
		//						 Reproductor::getReproductor()->reproducirSonido(AVION);
		//						 for(int i=0;i<4;i++){

		//							 x+=5;
		//							this->escenario->getGusanoActivo()->setArma(new Misiles(x, -60, 0, this->escenario->getWorld(), false, anchoMisiles, altoMisiles, masaMisiles, radioExplosionMisiles ) );
		//							this->escenario->agregarArma(this->escenario->getGusanoActivo()->getArmaSeleccionada());
		//							this->escenario->getGusanoActivo()->getArmaSeleccionada()->agregarObservador( this->vista->crearArmaContactoDibujable(x, 0, anchoMisiles*relacionPPU,altoMisiles*relacionPPU,rutaMisil,rutaMisil) );
		//							this->escenario->getGusanoActivo()->disparar();
		//				
		//						}
		//						this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA;
		//					}
		//				 }

  //                      break;

		//		case CLICKDERECHO:
		//				if(this->escenario->getGusanoActivo() != NULL){
		//					this->NUMCLICKDERECHOS++;
		//				//	cout<<"tengo un arma"<<endl;

		//					switch(NUMCLICKDERECHOS % 8){

		//					case 0: this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA;
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
		//							break;
		//					case 1: this->escenario->getGusanoActivo()->armaActual.armaTipo = BAZOOKA;
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
		//							break;
		//					case 2: this->escenario->getGusanoActivo()->armaActual.armaTipo = GRANADA;			
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
		//							break;
		//					case 3: this->escenario->getGusanoActivo()->armaActual.armaTipo = ALELUYA;
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
		//							break;
		//					case 4: this->escenario->getGusanoActivo()->armaActual.armaTipo = BANANA;
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
		//							break;
		//					case 5: this->escenario->getGusanoActivo()->armaActual.armaTipo = DINAMITA;
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
		//							break;
		//					case 6:	this->escenario->getGusanoActivo()->armaActual.armaTipo = MISILES;
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
		//							break;
		//					case 7: this->escenario->getGusanoActivo()->armaActual.armaTipo = SUICIDA;
		//							this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
		//							break;

		//					}

		//				}
		//					break;
  //              }
  //     }
		
	
	
	//Lector de eventos de los clientes. Lo anterior lo dejo para que siga funcionando mover en el servidor
	for(int i=0; i< this->escenario->getMaximosClientes(); i++){
		if(this->servidor->clientes[i].activo){
			//Si el cliente esta activo chequeo eventos
			string ultimoEvento = this->servidor->clientes[i].ultimoEventoSerializado;
			if(ultimoEvento != ""){
				Evento* evento = new Evento();
				evento->deserealizar(ultimoEvento);
				switch(evento->accion){

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

				case ESPACIO: 			if(this->escenario->getGusanoActivo()->armaActual.armaTipo != NINGUNA){
											this->escenario->espacio(true); 
											//detengo el turno hasta que explote todo
											this->turno->detener();
											
										}
										break;

				case SOLTARESPACIO:
		
					if( (this->escenario->getGusanoActivo()->armaActual.armaTipo) != MISILES && !this->escenario->getHuboDisparo()){
											this->accionRealizada = true;
											this->dispararArma();
											Reproductor::getReproductor()->detenerSonido(CARGANDODISPARO);
											this->escenario->setHuboDisparo(true);
										}

										this->escenario->espacio(false);
										break;


					case CLICK:	{
						list<Gusano*> figurasOtrosClientes;
						for(int j=0; j< this->escenario->getMaximosClientes(); j++){
							if(i != j){
								figurasOtrosClientes.insert(figurasOtrosClientes.end(), this->servidor->clientes[j].figuras.begin(), this->servidor->clientes[j].figuras.end());
							}
						}
						this->escenario->clickCliente(i,this->servidor->clientes[i].figuras,figurasOtrosClientes, evento->x, evento->y);


						if(this->escenario->getGusanoActivo() != NULL){
							 if( (this->escenario->getGusanoActivo()->armaActual.armaTipo) == MISILES  ){
	
								 //detengo el turno hasta que explote todo
								 this->turno->detener();
								 this->escenario->setHuboDisparo(true);
								 this->accionRealizada = true;
								 int x,y;
								 x = evento->x;
								 y = evento->y;
								 x -=12;
								 Reproductor::getReproductor()->reproducirSonido(INCOMING);
								 Reproductor::getReproductor()->reproducirSonido(AVION);
								 for(int i=0;i<4;i++){

									 x+=5;
									this->escenario->getGusanoActivo()->setArma(new Misiles(x, -60, 0, this->escenario->getWorld(), false, anchoMisiles, altoMisiles, masaMisiles, radioExplosionMisiles ) );
									this->escenario->agregarArma(this->escenario->getGusanoActivo()->getArmaSeleccionada());
									this->escenario->getGusanoActivo()->getArmaSeleccionada()->agregarObservador( this->vista->crearArmaContactoDibujable(x, 0, anchoMisiles*relacionPPU,altoMisiles*relacionPPU,rutaMisil,rutaMisil) );
									this->escenario->getGusanoActivo()->disparar();
						
								}
								this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA;
							}
						}


						break;
					}
					case CLICKARMA:	{
						if(!this->accionRealizada){
							switch(evento->x){
								case 0: this->escenario->getGusanoActivo()->armaActual.armaTipo = BAZOOKA;
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
										break;
								case 1: this->escenario->getGusanoActivo()->armaActual.armaTipo = GRANADA; 
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
										break;
								case 2: this->escenario->getGusanoActivo()->armaActual.armaTipo = ALELUYA; 
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
										break;
								case 3: this->escenario->getGusanoActivo()->armaActual.armaTipo = DINAMITA; 
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
										break;
								case 4: this->escenario->getGusanoActivo()->armaActual.armaTipo = BANANA; 
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = true;
										break;
								case 5: this->escenario->getGusanoActivo()->armaActual.armaTipo = MISILES;
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
										break;
								case 6: this->escenario->getGusanoActivo()->armaActual.armaTipo = SUICIDA;
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
										break;
								default: this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA; 
										this->escenario->getGusanoActivo()->armaActual.puedeCargarse = false;
										break;
							}
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
	

	if((this->escenario->getGusanoActivo() != NULL)&&(this->escenario->getGusanoActivo()->armaActual.armaTipo != NINGUNA ) && !(this->escenario->getGusanoActivo()->estaMuerto()) && !this->escenario->getBloquearTeclas()){
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
		case MISILES:
			int x,y;
			SDL_GetMouseState(&x,&y);
				this->escenario->getGusanoActivo()->setArma(new Misiles(posD.x, 0, 0, this->escenario->getWorld(), false, anchoMisiles, altoMisiles, masaMisiles, radioExplosionMisiles ) );
				arma = this->vista->crearArmaContactoDibujable(posD.x, 0, anchoMisiles*relacionPPU,altoMisiles*relacionPPU,rutaMisil,rutaMisil);		
				break;
		case SUICIDA:
				this->escenario->getGusanoActivo()->setVida(0);
				this->escenario->getGusanoActivo()->setMuerto(true);
				break;
			}
		if (!(this->escenario->getGusanoActivo()->armaActual.armaTipo == SUICIDA)){
			this->escenario->agregarArma(this->escenario->getGusanoActivo()->getArmaSeleccionada());
			this->escenario->getGusanoActivo()->getArmaSeleccionada()->agregarObservador(arma);
			this->escenario->getGusanoActivo()->disparar();
			this->escenario->getGusanoActivo()->armaActual.potenciaDisparo = 0;
		}

		if (!(this->escenario->getGusanoActivo()->armaActual.armaTipo == SUICIDA) && !(this->escenario->getGusanoActivo()->armaActual.armaTipo == BANANA)){
			this->escenario->getGusanoActivo()->armaActual.armaTipo = NINGUNA;
		}
	}
}

void Juego::dispararArmaCliente(int cliente){

	b2Vec2 posGusano;
	b2Vec2 posD;
	ArmaDibujable* arma;

	if((this->escenario->getFigurasActivas()[cliente] != NULL)&&(this->escenario->getFigurasActivas()[cliente]->armaActual.armaTipo != NINGUNA ) && !(this->escenario->getFigurasActivas()[cliente]->estaMuerto())){
		posGusano = this->escenario->getFigurasActivas()[cliente]->getBody()->GetWorldCenter();
		posD = getPosicionInicialDisparo(posGusano, this->escenario->getFigurasActivas()[cliente]->armaActual.anguloDisparo, this->escenario->getFigurasActivas()[cliente]->armaActual.sentidoDisparo, altoGusano/2 + 1.5);
					

		switch (this->escenario->getFigurasActivas()[cliente]->armaActual.armaTipo){
			case BAZOOKA:
				this->escenario->getFigurasActivas()[cliente]->setArma(new Bazooka(posD.x, posD.y, 0, this->escenario->getWorld(), false, anchoBazooka, altoBazooka, masaBazooka, radioExplosionBazooka ));
					arma = this->vista->crearArmaContactoDibujable(posD.x, posD.y, anchoBazooka*relacionPPU,altoBazooka*relacionPPU,rutaBaz,rutaBaz);
							
				break;
				
			case GRANADA:
				this->escenario->getFigurasActivas()[cliente]->setArma(new Granada(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionGranada, radioGranada, masaGranada , tiempoExplosionGranada));
				arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y,  relacionPPU * 2*radioGranada,relacionPPU * 2*radioGranada,rutaGranada,rutaGranada); 
				break;

			case ALELUYA:
				this->escenario->getFigurasActivas()[cliente]->setArma(new Aleluya(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionAleluya, radioAleluya, masaAleluya ));
				arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y,  relacionPPU * 2*radioAleluya,relacionPPU * 2*radioAleluya,rutaAleluya,rutaAleluya); 
				break;

			case DINAMITA:
				this->escenario->getFigurasActivas()[cliente]->setArma(new Dinamita(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionDinamita, anchoDinamita, altoDinamita, masaDinamita, tiempoExplosionDinamita));
				arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y, relacionPPU * anchoDinamita,relacionPPU * altoDinamita,rutaDinamita,rutaDinamita);
				break;
			case BANANA:
				this->escenario->getFigurasActivas()[cliente]->setArma(new Banana(posD.x, posD.y, 0, this->escenario->getWorld(), false, radioExplosionBanana, radioBanana, masaBanana, tiempoExplosionBanana ));
				arma = this->vista->crearArmaTiempoDibujable(posD.x, posD.y,  relacionPPU * 2*radioAleluya,relacionPPU * 2*radioAleluya,rutaBanana,rutaBanana); 
				break;		
		}
		this->escenario->agregarArma(this->escenario->getFigurasActivas()[cliente]->getArmaSeleccionada());
		this->escenario->getFigurasActivas()[cliente]->getArmaSeleccionada()->agregarObservador(arma);
		this->escenario->getFigurasActivas()[cliente]->getArmaSeleccionada()->disparar(this->escenario->getFigurasActivas()[cliente]->armaActual.sentidoDisparo, this->escenario->getFigurasActivas()[cliente]->armaActual.potenciaDisparo, this->escenario->getFigurasActivas()[cliente]->armaActual.anguloDisparo); 
		this->escenario->getFigurasActivas()[cliente]->armaActual.potenciaDisparo = 0;
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

void Juego::esperar(){
	while(true)	this->jugar();
}

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
					GusanoSprite* gusano = vista->crearGusanoSprite( (*it).x * escalaAncho, (*it).y * escalaAlto , anchoGusano * escalaAlto * 2.5, altoGusano * escalaAncho * 2.5, spriteWormIzq, 1, 10, 60, 600,"Rasta",this->escenario->getMaximosClientes());
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
	//delete Reproductor::getReproductor();
	//delete this->evento;
	//delete Logger::getLogger();
}

string Juego::getJugadorActual(){
	return this->jugadorActual;
}

void Juego::cambiarJugador(string jugador){
	
	this->jugadorActual = jugador;

	//selecciono aleatoriamente un worms del cliente que le toca el turno:
	int idCliente = Servidor::buscarCliente(jugador);
	list<Gusano*> gusanos = servidor->clientes[idCliente].figuras;
	list<Gusano*> gusanosVivos;
	list<Gusano*>::iterator it;
	for(it = gusanos.begin();it != gusanos.end();it++){
		if(!(*it)->estaMuerto()) gusanosVivos.push_back(*it);
	}


	if( (gusanosVivos.size() > 1) ){

		int gusanoRandom = 1+ rand()%(gusanosVivos.size());
		while(servidor->clientes[idCliente].ultimoGusanoActivo == gusanoRandom) gusanoRandom = 1+ rand()%(gusanosVivos.size());
		servidor->clientes[idCliente].ultimoGusanoActivo = gusanoRandom;
		int i= 1;
		//itero hasta el gusano Random, como es una lista es un acceso secuencial.
		for(it = gusanosVivos.begin();it != gusanosVivos.end();it++){

			if(i == gusanoRandom) {

				this->escenario->setGusanoActivo(*it);
				(*it)->setMeClickearon(true,idCliente);
				(*it)->setActivo(true);
				Reproductor::getReproductor()->reproducirSonido(SELECCIONARWORM);
			}
			i++;
		}
	}
	else if (gusanosVivos.size() == 1 ) {
		this->escenario->setGusanoActivo(gusanosVivos.front());
		gusanosVivos.front()->setMeClickearon(true,idCliente);
		gusanosVivos.front()->setActivo(true);
		Reproductor::getReproductor()->reproducirSonido(SELECCIONARWORM);
	}
	else{
		//se murieron todos los gusanitos de este cliente, lo pongo como muerto:
		servidor->clientes[idCliente].muerto = true;
		//cambio recursivamente de jugador: (Extreme Programing: ON)
		this->cambiarJugador(Servidor::siguienteJugador());
	}
	
	//cout << Servidor::getCantidadDeClientesConectados() <<endl;
	this->comprobarGanador();
}

void Juego::comprobarGanador(){
	//gana el que mata a todos los gusanos de los demas jugadores y vive para contarlo, si hay un 
	//jugador desconectado tiene los gusanos congelados pero igual hay q matarle los wachines para ganar.
	
	int ganador = -1;
	int contador = 0; //contador de clientes con gusanos vivos
	for(int i=0;i<Servidor::getCantidadDeClientesConectados();i++){

		if( servidor->clientes[i].muerto == false ){
			//si tiene gusanos vivos incremento, no importa sin estan congelados
			if(servidor->tieneGusanosVivos(i)){
				contador++;
				ganador = i;
			}
			else{
				servidor->clientes[i].muerto = true;
			}
		}
	}

	if(contador == 1){
		this->turno->detener();
		this->estadoActual = GANADO;
		cout << "Felicitaciones " << servidor->clientes[ganador].username << " sos el ganador" << endl;
	}

}

void Juego::reproducirSonidosFinTurno(int vidaRestada){

	if(vidaRestada != -1){
		if(vidaRestada == 0){
			Reproductor::getReproductor()->reproducirSonido(FALLASTE);
		}else{
		
			int reproducir = 24 + rand()%3;
			Reproductor::getReproductor()->reproducirSonido((sonido)reproducir);
		}
	}
}

void Juego::volverAjugarServidor(){

	this->servidor->avisarPartidaTerminada();
	this->escenario->reiniciarJuego();
	this->vista->Dibujar();

	for (int i = 0; i<this->escenario->getMaximosClientes();i++){
		//servidor->clientes[i].figuras.clear();
		servidor->clientes[i].figuras.empty();
		servidor->clientes[i].muerto = false;
		cout<<servidor->clientes[i].figuras.size();
	}
	this->vista->listaDibujables->clear();
	this->ejecutar();

}

