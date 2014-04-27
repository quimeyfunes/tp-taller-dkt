#include "Box2D/Box2D.h"
#include "../constantes.h"
#include "Escenario\Escenario.h"



class ResolverContacto : public b2ContactListener
  {
  private:
	  Escenario* escenario;
	  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
  
  public:
	  ResolverContacto();
	  void setEscenario(Escenario* escenario);
  };