#ifndef GammaKnifeSteppingAction_h
#define GammaKnifeSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "GammaKnifeEventAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

class GammaKnifeSteppingAction : public G4UserSteppingAction
{
public:
    GammaKnifeSteppingAction(GammaKnifeEventAction* eventAction);
    ~GammaKnifeSteppingAction();

    // method from the base class
    void UserSteppingAction(const G4Step*) override;

};

#endif