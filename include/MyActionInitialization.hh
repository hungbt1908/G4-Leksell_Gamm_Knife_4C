#ifndef MYACTIONINITIALIZATION_HH
#define MYACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include "MyPrimaryGenerator.hh"
#include "MyRunAction.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public:
	MyActionInitialization();
	~MyActionInitialization();
	
	virtual void Build() const;
	virtual void BuildForMaster() const;
};

#endif