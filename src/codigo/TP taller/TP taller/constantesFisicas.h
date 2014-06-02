#ifndef CONSTANTES_FISICAS_H_
#define CONSTANTES_FISICAS_H_

	const int gravedadX = 0;
	const float gravedadY = 9.8f;

	const float stepTiempo = 1/40.0; //cuanto mas alto el stepTiempo, mas rapido funciona el mundo
	const int iteracionesVelocidad = 6;
	const int iteracionesPosicion = 2;

	const float friccion = 0.6f;
	const float restitucion = 0.1;

	const int velocidadAgua = 4;
	const float desaceleracionAgua = 0.973;

	const int anchoGusano = 2;
	const int altoGusano = 2.5;
	const int velocidadGusano = 3;	//Para moverse hacia la derecha/izquierda
	const int saltoGusano = -8;	//Cuanto salta
	const int vidaGusano = 100;
	const float pxPorVida = 0.5;
	const int potenciaPorUnidad = 3000; //Para explosion
	const int impulsoCaida = 100; //La fuerza con la q tiene q caer para q le saque vida. Tambien le saca vida si choca fuerte contra algo. 

	typedef enum categoriaFigura {
		NORMAL = 0x0001,
		GUSANO = 0x0002,
		PROYECTIL = 0x0004,
	};
	
#endif