#ifndef __SERILIZABLE_H__
#define __SERILIZABLE_H__


#include <string>
#include "../StringUtil.h"
#include "../../constantes.h"

using namespace std;

class Serializable
{
public:
	Serializable();
	~Serializable();
	virtual string serializar();
	virtual int deserealizar(string aDeserealizar);
};

#endif