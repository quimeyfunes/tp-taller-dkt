#include "ResolverContacto.h"


ResolverContacto::ResolverContacto() {
}

void ResolverContacto::BeginContact(b2Contact* contact) {
	  
	// Para ver si gusano toca algo por abajo y volver a saltar
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	if ( fixtureUserData ) {
		//Solo el gusano tiene userData por ahora
		Figura* fig = (Figura*) fixtureUserData;
		fig->BeginContact();
	}
	  
	fixtureUserData = contact->GetFixtureB()->GetUserData();
      
	if ( fixtureUserData ) {
		//Solo el gusano tiene userData por ahora
		Figura* fig = (Figura*) fixtureUserData;
		fig->BeginContact();
	}
}

void ResolverContacto::EndContact(b2Contact* contact) {

	// Para ver si gusano toca algo por abajo y volver a saltar
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	if ( fixtureUserData ) {
		//Solo el gusano tiene userData por ahora
		Figura* fig = (Figura*) fixtureUserData;
		fig->EndContact();
	}
      
	if ( fixtureUserData ) {
		Figura* fig = (Figura*) fixtureUserData;
		fig->EndContact();
	}
}

