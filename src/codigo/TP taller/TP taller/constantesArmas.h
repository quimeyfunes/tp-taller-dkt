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

//RADIO DE ARMA CIRCULARES
const float radioGranada = 1;
const float radioBazooka = 4;

//PARA EXPLOSIONES
const float radioExplosionBazooka = 5;
const float radioExplosionGranada = 5;

////////// TIPOS DE ARMAS ////////////
typedef enum{
	NINGUNA,
	BAZOOKA,
	GRANADA,
}tipoArma;
//////////////////////////////////////

#endif