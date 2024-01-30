#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1


#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"

class G4GeneralParticleSource;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGeneratorAction();
	~PrimaryGeneratorAction();
	void GeneratePrimaries(G4Event* anEvent);
	G4GeneralParticleSource* fParticleGun;
};

#endif
