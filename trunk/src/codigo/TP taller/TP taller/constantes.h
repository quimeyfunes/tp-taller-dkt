#ifndef CONSTANTES_H_
#define CONSTANTES_H_

	#define archivoLog "logErrors.txt"
	#define imagenTerrenoDEF "imagenes/terrenoDefault.png"
	#define imagenCieloDEF "imagenes/cieloDefault.txt"
	#define configPath "config/config.yaml"

	const double PI = 3.1415926535;

	const unsigned anchoPxDEF=3000;
	const unsigned altoPxDEF=768;
	const unsigned anchoUDEF=3000;
	const unsigned altoUDEF=1000;
	const unsigned nivelAguaDEF=20;

	//cuando la imagen de terreno es invalida, se genera una con una f(x) aleatoria
	//estos parametros ajustan el valor minimo y maximo de la funcion
	const int fMinDEFAULT = 10;
	const int fMaxDEFAULT = 500;

	#define DEGTORAD 0.0174532925199432957f
	#define RADTODEG 57.295779513082320876f


	/*
	TIPOS DE ERRORES: (ir completando a medida que surjan)

	Error 001: no se encuentra la imagen de terreno.
	Error 002: la imagen de terreno no es PNG o esta corrupta.
	Error 003: la imagen de terreno tiene un pixel de color invalido.
	Error 004: la imagen de terreno tiene una columna con tierra-cielo-tierra.
	*/

#endif
