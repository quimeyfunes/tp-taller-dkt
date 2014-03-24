#ifndef CONSTANTES_H_
#define CONSTANTES_H_

	#define archivoLog "logErrors.txt"

	const unsigned anchoPxDEF=3000;
	const unsigned altoPxDEF=1000;
	const unsigned anchoUDEF=3000;
	const unsigned altoUDEF=1000;
	const unsigned nivelAguaDEF=20;

	#define imagenTerrenoDEF "imagenes/terrenoDefault.txt"
	#define imagenCieloDEF "imagenes/cieloDefault.txt"

	/*
	TIPOS DE ERRORES: (ir completando a medida que surjan)

	Error 001: no se encuentra la imagen de terreno.
	Error 002: la imagen de terreno no es PNG o esta corrupta.
	Error 003: la imagen de terreno tiene un pixel de color invalido.
	Error 004: la imagen de terreno tiene una columna con tierra-cielo-tierra.
	*/

#endif
