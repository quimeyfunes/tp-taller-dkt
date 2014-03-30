#include "Juego.h"

Juego::Juego(){
}

void Juego::comenzar(){
	//Meto todo lo que estaba en el main. Hay que sacar la mayoria de aca
	ParserYaml* parser = ParserYaml::getParser();
	EscenarioParseado* e = parser->getEscenario();
	//EscenarioParseado* e2 = parser->getEscenario();
	vector<ObjetoParseado>* objetos = parser->getObjetos();
	Escenario* escenario = new Escenario(e->altoU,e->anchoU,e->nivelAgua);
	
	//Muy feo, lector recibe un char* y imagenTierra es string 
	char* imagenTierra = new char[e->imagenTierra.size()+1];
	imagenTierra[e->imagenTierra.size()] = 0;
	memcpy(imagenTierra,e->imagenTierra.c_str(),e->imagenTierra.size());
	LectorTerreno* lector = new LectorTerreno(imagenTierra);

	bool** matriz = lector->getMatrizTerreno();
	char* tex = lector->getRutaTexturaActualizada();

	Vista* vista = new Vista(e);

	SDL_Event* evento = new SDL_Event();
	
	////PRUEBA BOX2D

	//B2_NOT_USED(argc);
	//B2_NOT_USED(argv);

	//// Define the gravity vector.
	b2Vec2 gravity(0.0f, 9.8f);

	//// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

	//// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 300.0f);
	groundBodyDef.type = b2_staticBody;
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	//// Define the ground box shape.
	b2PolygonShape groundBox;

	//// The extents are the half-widths of the box.
	groundBox.SetAsBox(5000000.0f, 10.0f);

	//// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 1.0f);


	//// Prepare for simulation. Typically we use a time step of 1/60 of a
	//// second (60Hz) and 10 iterations. This provides a high quality simulation
	//// in most game scenarios.
	float32 timeStep = 1.0f / 2000.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	Dibujable* dibTierra = vista->crearDibujable( 0, 0 , lector->getAnchoMatriz(),lector->getAltoMatriz(),lector->getRutaTexturaActualizada());
	
	//Color tierra para joder
	string hex = "#FF0000";
	dibTierra->setColor(ParserDeHexARgb::parsearDeHexARgb(hex));
	
	//Esto supongo q lo haria la clase Juego?
	/*float escalaAncho = e->anchoPx / e->anchoU;
	float escalaAlto = e->altoPx / e->altoU;
	for (std::vector<ObjetoParseado>::iterator it = objetos->begin(); it != objetos->end(); ++it) {
		switch ((*it).tipo) {
		case 1: 
			escenario->crearCirculo(*it,&world);
			//vista->crearDibujable((*it).x * escalaAncho, (*it).y * escalaAlto,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, pathCirculo );
			break;
		case 2:
			escenario->crearRectangulo(*it,&world);
			//vista->crearDibujable((*it).x * escalaAncho, (*it).y * escalaAlto,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, pathRectangulo );
		default:
			escenario->crearPoligono(*it,&world);
			//vista->crearDibujable((*it).x * escalaAncho, (*it).y * escalaAlto,(*it).ancho * escalaAncho,(*it).alto * escalaAlto, pathPoligono q venga );
		}
	}

	*/
	bool jugar = true;

	
	while((evento->type != SDL_QUIT)&&(jugar)){

		SDL_PollEvent(evento);

		if(evento->key.keysym.sym == SDLK_ESCAPE)
			jugar = false;
		
			
		// Instruct the world to perform a single step of simulation.
		//// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		//escenario->notificar();

		vista->Dibujar();
		
	}
    //Quit SDL
    SDL_Quit();
}

void Juego::pausar(){
	this->enPausa = !this->enPausa;
}
bool Juego::getPausa(){
	return this->enPausa;
}