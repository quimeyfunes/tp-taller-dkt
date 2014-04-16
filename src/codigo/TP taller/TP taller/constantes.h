#ifndef CONSTANTES_H_
#define CONSTANTES_H_

	#define archivoLog "logErrores.txt"
	#define propiedadesPNG "config/propiedadesPNG.png"
	#define mascaraTerrenoDEF "imagenes/mascaras/mascaraDefault.png"
	#define texturaCieloDEF "imagenes/texturas/cieloDefault.png"
	#define texturaAguaDEF "imagenes/texturas/aguaDefault.png"
	#define texturaTerreno "imagenes/texturas/terrenoActual.png"
	#define texturaFondo "imagenes/fondo.png"
	#define texturaAgua "imagenes/texturas/agua.png"
	#define spriteOlas "imagenes/texturas/olas.png"
	#define rutaNube1 "imagenes/texturas/nube1.png"
	#define rutaNube2 "imagenes/texturas/nube2.png"
	#define rutaIcono "imagenes/texturas/icono.ico"

	const int timeStepOlas = 30;
	const int timeStepNubes = 25;
	const int maxPixelesInvalidosMascara = 10;
	const int maxColumnasInvalidasMascara = 10;

	//Escenario
	const unsigned anchoPxDEF=800;
	const unsigned altoPxDEF=600;
	const unsigned anchoUDEF=400;
	const unsigned altoUDEF=300;
	const unsigned nivelAguaDEF=200;

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

	const double PI = 3.1415926535;
	#define DEGTORAD 0.0174532925199432957f
	#define RADTODEG 57.295779513082320876f

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

	//Constantes fisicas
	#define gravedadX 0
	#define gravedadY 9.8f
	#define stepTiempo (float)(1/60.0)
	#define iteracionesVelocidad 6
	#define iteracionesPosicion 2
	#define friccion 0.3f
	#define restitucion 0.3f
	#define velocidadAgua 8
	#define desaceleracionAgua 0.973

#endif
