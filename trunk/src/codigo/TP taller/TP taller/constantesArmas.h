#ifndef CTTES_ARMAS_H_
#define CTTES_ARMAS_H_

#define rutaBazIzq "imagenes/texturas/bazIzq.png"
#define rutaBazDer "imagenes/texturas/bazDer.png"
#define rutaGranada "imagenes/texturas/granada.png"
#define rutaWormBazIzq "imagenes/texturas/wormConBazookaIzq.png"
#define rutaWormBazDer "imagenes/texturas/wormConBazookaDer.png"
#define rutaWormGran "imagenes/texturas/wormGran.png"

const int POTENCIA_MAXIMA_DISPARO = 100;
const int AUMENTO_POTENCIA = 2;

const float radioBazooka = 5;
const float radioGranada = 5;
////////// TIPOS DE ARMAS ////////////
typedef enum{
	NINGUNA,
	BAZOOKA,
	GRANADA,
}tipoArma;
//////////////////////////////////////

#endif