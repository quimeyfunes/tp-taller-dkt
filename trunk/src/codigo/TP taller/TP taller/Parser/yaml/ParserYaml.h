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

using namespace std;

class ParserYaml
{
private:
	string nombreArchivo;
	YAML::Node documento;
	bool ParserYaml::esNumero(const std::string& s);
	string getNodoInfo(const YAML::Node & nodo);
	int getValorEscalar(const YAML::Node & nodo, string clave, const int valorPorDefecto);
	bool ParserYaml::validarEscalar(const YAML::Node & nodo, string clave, int &valor);
	bool ParserYaml::ValidarSecuencia(const YAML::Node &nodo, string clave);
	bool ParserYaml::ValidarCadena(const YAML::Node &nodo, string clave, string cadenaValida);
public:
	ParserYaml(std::string pathArchivo);
	void parsear();
	//Tiene que devolver el escenario
	Escenario getEscenario();
};


#endif
