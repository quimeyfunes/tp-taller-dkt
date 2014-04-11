#include <winsock2.h>
#include <Windows.h>
#include "ClienteRed.h"

class Cliente
{

public:

    Cliente();
    ~Cliente(void);

    ClienteRed* red; 
};
