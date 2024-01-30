#include "GammaKnifePhysicsList.hh"


GammaKnifePhysicsList::GammaKnifePhysicsList()
{
	RegisterPhysics(new G4EmStandardPhysics_option3());
}

GammaKnifePhysicsList::~GammaKnifePhysicsList()
{}

