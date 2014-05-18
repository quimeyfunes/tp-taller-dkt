#ifndef CONSTANTES_FISICAS_H_
#define CONSTANTES_FISICAS_H_

	const int gravedadX = 0;
	const float gravedadY = 9.8f;

	const float stepTiempo = 3/50.0; //cuanto mas alto el stepTiempo, mas rapido funciona el mundo
	const int iteracionesVelocidad = 6;
	const int iteracionesPosicion = 2;

	const float friccion = 0.6f;
	const float restitucion = 0;

	const int velocidadAgua = 8;
	const float desaceleracionAgua = 0.973;

	const int anchoGusano = 15;
	const int altoGusano = 18;
	const int velocidadGusano = 5;	//Para moverse hacia la derecha/izquierda
	const int saltoGusano = -25;	//Cuanto salta

#endif