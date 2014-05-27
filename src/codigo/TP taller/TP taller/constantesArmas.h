#ifndef CTTES_ARMAS_H_
#define CTTES_ARMAS_H_

#define rutaBaz "imagenes/texturas/bazIzq.png"
#define rutaGranada "imagenes/texturas/granada.png"
#define rutaAleluya "imagenes/texturas/aleluya.png"
#define rutaWormBaz "imagenes/texturas/wormConBazookaIzq.png"
#define rutaWormGran "imagenes/texturas/wormGran.png"
#define rutaWormAle "imagenes/texturas/wormAleluya.png"


const int POTENCIA_MAXIMA_DISPARO = 40;
const float AUMENTO_POTENCIA = 0.5;

const float anchoBazooka = 1;
const float altoBazooka = 1;
const int masaBazooka = 80;
//RADIO DE ARMA CIRCULARES
const float radioGranada = 1;
const float radioBazooka = 4;
const float radioAleluya = 1;

//PARA EXPLOSIONES
const float radioExplosionBazooka = 5;
const float radioExplosionGranada = 5;
const float radioExplosionAleluya = 10;

////////// TIPOS DE ARMAS ////////////
typedef enum{
	NINGUNA,
	BAZOOKA,
	GRANADA,
	ALELUYA,
}tipoArma;
//////////////////////////////////////

#endif