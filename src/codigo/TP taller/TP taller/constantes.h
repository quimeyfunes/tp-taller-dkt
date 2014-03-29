#ifndef CONSTANTES_H_
#define CONSTANTES_H_

	#define archivoLog "logErrors.txt"
	#define configPath "config/config.yaml"
	#define propiedadesPNG "config/propiedadesPNG.png"
	#define mascaraTerrenoDEF "imagenes/mascaras/mascaraDefault.png"
	#define texturaCieloDEF "imagenes/texturas/cieloDefault.png"
	#define texturaTerreno "imagenes/texturas/terrenoActual.png"
	#define texturaAgua "imagenes/texturas/agua.png"
	#define texturaCielo "imagenes/texturas/cielo.png"


	const int maxPixelesInvalidosMascara = 10;
	const int maxColumnasInvalidasMascara = 10;

	//Escenario
	const unsigned anchoPxDEF=1366;
	const unsigned altoPxDEF=768;
	const unsigned anchoUDEF=3000;
	const unsigned altoUDEF=1000;
	const unsigned nivelAguaDEF=20;

	//Objetos
	const unsigned tipoObjDEF=4;//Por defecto son cuadrados
	const unsigned xDEF=5;
	const unsigned yDEF=90;
	const unsigned anchoObjDEF=2;
	const unsigned altoObjDEF=2;
	#define colorDEF "#111111"
	const unsigned rotacionDEF=45;
	const unsigned masaDEF=3;
	const bool estaticoDEF=false;
	const bool escalaDef=2;

	//cuando la imagen de terreno es invalida, se genera una con una f(x) aleatoria
	//estos parametros ajustan el valor minimo y maximo de la funcion
	const int fMinDEFAULT = 10;
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
#endif
