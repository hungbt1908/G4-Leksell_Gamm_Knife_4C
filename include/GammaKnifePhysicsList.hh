#ifndef GammaKnifePhysicsList_h
#define GammaKnifePhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics_option3.hh"


class GammaKnifePhysicsList: public G4VModularPhysicsList
{
public:
  GammaKnifePhysicsList();
  virtual ~GammaKnifePhysicsList();
};

#endif



