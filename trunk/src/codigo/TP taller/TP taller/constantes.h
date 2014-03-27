#ifndef CONSTANTES_H_
#define CONSTANTES_H_

	#define archivoLog "logErrors.txt"
	#define propiedadesPNG "config/propiedadesPNG.png"
	#define imagenTerrenoDEF "imagenes/mascaras/mascaraDefault.png"
	#define imagenCieloDEF "imagenes/cieloDefault.png"
	#define configPath "config/config.yaml"

	const int maxPixelesInvalidosMascara = 10;
	const int maxColumnasInvalidasMascara = 10;

	const unsigned anchoPxDEF=1024;
	const unsigned altoPxDEF=768;
	const unsigned anchoUDEF=3000;
	const unsigned altoUDEF=1000;
	const unsigned nivelAguaDEF=20;

	//cuando la imagen de terreno es invalida, se genera una con una f(x) aleatoria
	//estos parametros ajustan el valor minimo y maximo de la funcion
	const int fMinDEFAULT = 10;
	const int fMaxDEFAULT = 500;

	const double PI = 3.1415926535;
	#define DEGTORAD 0.0174532925199432957f
	#define RADTODEG 57.295779513082320876f
#endif
