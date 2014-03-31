#include "Modelo\Juego.h"
#include <iostream>
#include <vector>

int main( int argc,  char** argv )
{	

	//ParserYaml* parser = ParserYaml::getParser();
	////EscenarioParseado* e = parser->getEscenario();
	////EscenarioParseado* e2 = parser->getEscenario();
	//vector<ObjetoParseado>* objectos = parser->getObjetos();
	/*
	LectorTerreno* lector = new LectorTerreno("imagenes/mascaras/mascara.png");
	bool** matriz = lector->getMatrizTerreno();
	char* tex = lector->getRutaTexturaActualizada();

	

	/*
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;
    
	//The surface contained by the window
	SDL_Surface* gScreenSurface = NULL;

	//The image we will load and show on the screen
	SDL_Surface* gHelloWorld = NULL;

    //Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );

	//Starts up SDL and creates window
	bool init();

	//Loads media
	bool loadMedia();

	//Frees media and shuts down SDL
	void close();*/
	/*
	SDL_Window* window = NULL;
	window = SDL_CreateWindow("Worms!", 50, 50, 400, 400,  SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP );

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_Event* evento = new SDL_Event();
	*/
	////PRUEBA BOX2D

	//B2_NOT_USED(argc);
	//B2_NOT_USED(argv);

	//// Define the gravity vector.
	//b2Vec2 gravity(0.0f, 9.8f);

	//// Construct a world object, which will hold and simulate the rigid bodies.
	//b2World world(gravity);

	//// Define the ground body.
	//b2BodyDef groundBodyDef;
	//groundBodyDef.position.Set(0.0f, 300.0f);
	//groundBodyDef.type = b2_staticBody;
	//// Call the body factory which allocates memory for the ground body
	//// from a pool and creates the ground box shape (also from a pool).
	//// The body is also added to the world.
	//b2Body* groundBody = world.CreateBody(&groundBodyDef);

	//// Define the ground box shape.
	//b2PolygonShape groundBox;

	//// The extents are the half-widths of the box.
	//groundBox.SetAsBox(5000000.0f, 10.0f);

	//// Add the ground fixture to the ground body.
	//groundBody->CreateFixture(&groundBox, 1.0f);

	//// Define the dynamic body. We set its position and call the body factory.
	//b2BodyDef bodyDef;
	//bodyDef.type = b2_dynamicBody;
	//bodyDef.position.Set(0.0f, 4.0f);
	//b2Body* body = world.CreateBody(&bodyDef);

	//// Define another box shape for our dynamic body.
	//b2PolygonShape dynamicBox;
	//dynamicBox.SetAsBox(1.0f, 1.0f);

	//// Define the dynamic body fixture.
	//b2FixtureDef fixtureDef;
	//fixtureDef.shape = &dynamicBox;

	//// Set the box density to be non-zero, so it will be dynamic.
	//fixtureDef.density = 1.0f;

	//// Override the default friction.
	//fixtureDef.friction = 0.3f;

	//fixtureDef.restitution = 1.0f;

	//// Add the shape to the body.
	//body->CreateFixture(&fixtureDef);

	//// Prepare for simulation. Typically we use a time step of 1/60 of a
	//// second (60Hz) and 10 iterations. This provides a high quality simulation
	//// in most game scenarios.
	//float32 timeStep = 1.0f / 2000.0f;
	//int32 velocityIterations = 6;
	//int32 positionIterations = 2;

	//Vista* vista = new Vista();
	//SDL_Rect recImg;

	
	//recImg.h= lector->getAltoMatriz();
	//recImg.w = lector->getAnchoMatriz();
	//recImg.x = 0;
	//recImg.y = 0;
	
	//FiguraDibujable* dib2 = vista->crearFiguraDibujable(recImg,"imagenes/imagen.jpg");
	//vista->crearDibujable(recImg,"imagenes/r4.png");
	//string hex = "#F4ACC3";
	//dib2->setColor(parsearDeHexARgb(hex));
	//Rectangulo* rec = new Rectangulo (20,20,0,&world,false,40,40,10);
	//rec->agregarObservador(dib2);
	/*Dibujable* dib = new Dibujable(vista->getRenderer(), recImg,  "imagenes/imagen.jpg");
	SDL_Texture* tierra = IMG_LoadTexture(renderer, tex);
	SDL_Texture* agua = IMG_LoadTexture(renderer, "imagenes/texturas/agua.png");
	SDL_Texture* cielo = IMG_LoadTexture(renderer, "imagenes/texturas/cielo.png");
	SDL_Rect recAgua;
	recAgua.h = 100;
	recAgua.w = lector->getAnchoMatriz();
	recAgua.x=0;
	recAgua.y=lector->getAltoMatriz()-recAgua.h;

	SDL_Rect recCielo;
	recCielo.x=0;
	recCielo.y=0;
	recCielo.w = lector->getAnchoMatriz();
	recCielo.h= lector->getAltoMatriz();
	*/

	//vista->agregarDibujable(dib);
/*		bool jugar = true;

	
	while((evento->type != SDL_QUIT)&&(jugar)){

		SDL_PollEvent(evento);

		if(evento->key.keysym.sym == SDLK_ESCAPE)
			jugar = false;
		SDL_RenderClear(renderer);

		//for(int i=0; i<lector->getAltoMatriz(); i++){
		//	for(int j=0; j<lector->getAnchoMatriz(); j++){

		//		if(matriz[j][i]){
		//			recImg.x = j;
		//			recImg.y = i;
		SDL_RenderCopy(renderer, cielo, NULL, &recCielo);
		SDL_RenderCopy(renderer, agua, NULL, &recAgua);
		SDL_RenderCopy(renderer, tierra, NULL, &recImg);
		//		}
		//	}
		//}
		SDL_RenderPresent(renderer);
			
		// Instruct the world to perform a single step of simulation.
		//// It is generally best to keep the time step and iterations fixed.
		//world.Step(timeStep, velocityIterations, positionIterations);

		//rec->notificar();
		// Now print the position and angle of the body.
		/*b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();
		


	recImg.x = body->GetPosition().x;
		recImg.y = body->GetPosition().y;*/

		//vista->Dibujar();
		/*SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, img, NULL, &recImg); 

		SDL_RenderPresent(renderer);
		
	}
    //Quit SDL
    SDL_Quit();*/



	Juego* juego = new Juego();
	juego->comenzar();

    return 0;    
}