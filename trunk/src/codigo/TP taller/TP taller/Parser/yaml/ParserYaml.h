#ifndef __YAMLPARSER_H__
#define __YAMLPARSER_H__

#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <valarray>
#include <algorithm>
#include "../../constantes.h"
#include "../../Modelo/Escenario.h"
#include "../../Logger/Logger.h"

using namespace std;

struct EscenarioParseado{
	int altoPx;
    int anchoPx;
    int altoU;
    int anchoU;
    int nivelAgua;
    string imagenTierra;
    string imagenCielo;
};


struct ObjetoParseado{
	int x;
    int y;
    int ancho;
    int alto;
    string color;
    int rotacion;
    int masa;
	bool estatico;
};

class ParserYaml
{
private:
	static ParserYaml* parserInstancia;
	string nombreArchivo;
	YAML::Node documento;
	static EscenarioParseado* escenario;
	static vector<ObjetoParseado>* objetos;
	bool ParserYaml::esNumero(const std::string& s);
	string getNodoInfo(const YAML::Node & nodo);
	int getValorEscalar(const YAML::Node & nodo, string clave, const int valorPorDefecto);
	string getValorCadena(const YAML::Node & nodo, string clave, string valorPorDefecto);
	vector<ObjetoParseado>* getValorSecuencia(const YAML::Node & nodo, string clave);
	bool ParserYaml::validarEscalar(const YAML::Node & nodo, string clave, int &valor);
	bool ParserYaml::validarSecuencia(const YAML::Node &nodo, string clave);
	bool ParserYaml::validarCadena(const YAML::Node &nodo, string clave, string &cadena);
	bool ParserYaml::validarImagen(string path);
	EscenarioParseado* parsearEscenario();
	EscenarioParseado* getEscenarioDefault();
	vector<ObjetoParseado>* ParserYaml::parsearObjetos();
	vector<ObjetoParseado>* ParserYaml::getObjetosDefault();
public:
	ParserYaml(std::string pathArchivo);
	ParserYaml();
	void parsear();
	static ParserYaml* getParser();
	//Tiene que devolver el escenario
	EscenarioParseado* getEscenario();
	vector<ObjetoParseado>* getObjetos();
};


#endif
