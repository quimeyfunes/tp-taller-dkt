#ifndef CTTES_ARMAS_H_
#define CTTES_ARMAS_H_

#define rutaBaz "imagenes/texturas/bazIzq.png"
#define rutaGranada "imagenes/texturas/granada.png"
#define rutaAleluya "imagenes/texturas/aleluya.png"
#define rutaDinamita "imagenes/texturas/dinamita.png"
#define rutaWormBaz "imagenes/texturas/wormConBazookaIzq.png"
#define rutaWormGran "imagenes/texturas/wormGran.png"
#define rutaWormAle "imagenes/texturas/wormAleluya.png"
#define rutaWormDin "imagenes/texturas/wormConDinamita.png"


const int POTENCIA_MAXIMA_DISPARO = 40;
const float AUMENTO_POTENCIA = 0.5;

const float anchoBazooka = 1;
const float altoBazooka = 1;
const float radioBazooka = 4; // ???
const int masaBazooka = 80;

const float radioGranada = 1;
const float masaGranada = 10;
const int tiempoExplosionGranada = 3; //hay q poder variar este valor

const float radioAleluya = 1;
const int masaAleluya = 10;
const int tiempoExplosionAleluya = 3;

const float altoDinamita = 2;
const float anchoDinamita = 1;
const int masaDinamita = 5;
const int tiempoExplosionDinamita = 5;

//PARA EXPLOSIONES
const float radioExplosionGranada = 4;
const float radioExplosionBazooka = 5;
const float radioExplosionDinamita = 6;
const float radioExplosionAleluya = 7;

////////// TIPOS DE ARMAS ////////////
typedef enum{
	NINGUNA,
	BAZOOKA,
	GRANADA,
	ALELUYA,
	DINAMITA,
}tipoArma;
//////////////////////////////////////

#endif