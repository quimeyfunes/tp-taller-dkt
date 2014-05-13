#ifndef CONSTANTES_H_
#define CONSTANTES_H_
	#define archivoLog "logErrores.txt"
	#define propiedadesPNG "config/propiedadesPNG.png"
	#define mascaraTerrenoDEF "imagenes/mascaras/mascaraDefault.png"
	#define texturaCielo "imagenes/texturas/cielo.png"
	#define texturaCieloDEF "imagenes/texturas/cieloDefault.png"
	#define texturaAguaDEF "imagenes/texturas/aguaDefault.png"
	#define texturaTerreno "imagenes/texturas/terrenoActual.png"
	#define texturaFondo "imagenes/fondo.png"
	#define texturaAgua "imagenes/texturas/agua.png"
	#define spriteOlas "imagenes/texturas/olas.png"
	#define spriteWormIzq "imagenes/texturas/wormFrentIzq.png"
	#define spriteWormDer "imagenes/texturas/wormFrentDer.png"
	#define rutaNube1 "imagenes/texturas/nube1.png"
	#define rutaNube2 "imagenes/texturas/nube2.png"
	#define rutaIcono "imagenes/texturas/icono.ico"
	#define rutaGusano "imagenes/texturas/worm.png"
	#define rutaGusanoDer "imagenes/texturas/wormDer.png"
	#define rutaGusanoIzq "imagenes/texturas/wormIzq.png"
	#define rutaGusanoDEF "imagenes/texturas/wormDefault.png"
	#define rutaCartel "imagenes/texturas/cartel.png"
	#define rutaCartelDEF "imagenes/texturas/cartelDefault.png"
	#define rutaFuente "fuentes/acme.ttf"
	#define rutaGrave "imagenes/texturas/grave.png"
	#define rutaWormGrisIzq "imagenes/texturas/wormGrisFrentIzq.png"
	#define rutaWormGrisDer "imagenes/texturas/wormGrisFrentDer.png"
	
	const int timeStepOlas = 8;
	const int timeStepNubes = 10;
	const int maxColumnasInvalidasMascara = 10;

	const int alturaCartel = 8;

	//Para gusanos
	const int timeGusanoQuieto = 100;
	const int timeGusanoMovil = 20;
	const int timeGrave = 40;
	const int gusanosPorPersonaje = 3;

	//Relacion Pixel por Unidad
	const float relacionPPU = 2;

	//Escenario
	const unsigned anchoPxDEF=1200;
	const unsigned altoPxDEF=600;
	const unsigned anchoUDEF=400;
	const unsigned altoUDEF=300;
	const unsigned nivelAguaDEF=100;

	const unsigned altoUMIN=100;
	const unsigned anchoUMIN=100;
	const unsigned altoPXMIN=200;
	const unsigned anchoPXMIN=200;


	//Objetos
	const unsigned tipoObjDEF=4;//Por defecto son cuadrados
	const unsigned xDEF=200;
	const unsigned yDEF=250;
	const unsigned anchoObjDEF=80;
	const unsigned altoObjDEF=80;
	#define colorDEF "FF0000"
	const unsigned rotacionDEF=45;
	const unsigned masaDEF=3;
	const bool estaticoDEF=false;
	const float escalaDEF=6;

	//cuando la imagen de terreno es invalida, se genera una con una f(x) aleatoria
	//estos parametros ajustan el valor minimo y maximo de la funcion
	const int fMinDEFAULT = 0;
	const int fMaxDEFAULT = 350;

	const float PI = 3.1415926535;
	const float DEGTORAD = 0.0174532925199432957f;
	const float RADTODEG = 57.295779513082320876f;

	//Figuras
	#define trianguloString "tri"
  	#define rectanguloString "rect"
	#define cuadradoString "cuad"
	#define circuloString "circ"
	#define pentagonoString "pent"
	#define hexagonoString "hex"
	const unsigned circuloTipo=1;
	const unsigned rectanguloTipo=2;
	const unsigned trianguloTipo=3;
	const unsigned cuadradoTipo=4;
	const unsigned pentagonoTipo=5;
	const unsigned hexagonoTipo=6;
	const unsigned gusanoTipo = 7;

	//el tiempo q tarda en desaparecer el cartel de info 
	const int TIEMPO_CARTEL_INFO = 5;

	const float porcentajeScroll = 0.075;
	const float velocidadScroll = 1/600.0;

	//Para Zoom
	const int escalaZoomDefault = 1;
	const int zoomMax = 8;
	//const int zoomMin = 1;

	//Constantes fisicas
	const int gravedadX = 0;
	const float gravedadY = 9.8f;
	const float stepTiempo = 2/60.0;
	const int iteracionesVelocidad = 6;
	const int iteracionesPosicion = 2;
	const float friccion = 0.4f;
	const float restitucion = 0.3f;
	const int velocidadAgua = 8;
	const float desaceleracionAgua = 0.973;
	const int anchoGusano = 15;
	const int altoGusano = 18;
	//Tipos de paquete
	const unsigned paqueteInicial=1;
	const unsigned paqueteEvento=2;
	const unsigned paqueteMensaje=3;
	const unsigned paqueteEstado=4;
	const unsigned paqueteVista=5;
	const unsigned paqueteFinal=6;
	const unsigned paqueteDescargaLista=7;
	const unsigned paqueteEscenario=8;
	const unsigned paqueteTextura=9;
	const unsigned paqueteMensajeInfo=10;

	#define MAX_PACKET_SIZE 1000000
	#define MAXIMOS_CLIENTES 4

	//Serializacion
	const char separadorCamposEntidades = '#';
	const char separadorCamposArreglo = ',';
	const char separadorCampoTipoEntidades = '&';
	const char separadorEntidades = ';';

	#define serializadoCirculoDibujable 1
	#define serializadoDibujableTextura 2
	#define serializadoGusanoDibujable 3
	#define serializadoPoligonoDibujable 4
	#define serializadoRectanguloDibujable 5
	#define serializadoGusanoSprite 6

#endif
