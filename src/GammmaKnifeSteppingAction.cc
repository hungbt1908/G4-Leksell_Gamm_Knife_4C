#include "GammmaKnifeSteppingAction.hh"

GammaKnifeSteppingAction::GammaKnifeSteppingAction(GammaKnifeEventAction* eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GammaKnifeSteppingAction::~GammaKnifeSteppingAction()
{}

void GammaKnifeSteppingAction::UserSteppingAction(const G4Step* step)
{
	G4Track* track = step->GetTrack();
	G4double Ekin = track->GetKineticEnergy();

	const G4double Emin_gamma = 0.01 * MeV; // Set energy threshold to kill gamma
	const G4double Emin_electron = 0.521 * MeV; // Set energy threshold to kill electron and positron

    const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();

    if (particleDefinition == G4Gamma::Gamma())
    {
        if (Ekin <= Emin_gamma) track->SetTrackStatus(fStopAndKill);
    }

    else if (particleDefinition == G4Electron::Electron())
    {
        if (Ekin <= Emin_electron) track->SetTrackStatus(fStopAndKill);
    }

    else if (particleDefinition == G4Positron::Positron())
    {
        if (Ekin <= Emin_electron) track->SetTrackStatus(fStopAndKill);
    }
    else
    { }
}