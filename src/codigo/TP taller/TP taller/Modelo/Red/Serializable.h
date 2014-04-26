#ifndef __SERILIZABLE_H__
#define __SERILIZABLE_H__


#include <string>

using namespace std;

class Serializable
{
public:
	Serializable();
	~Serializable();
	virtual string serializar();
	virtual void deserealizar(string aDeserealizar);
};

#endif