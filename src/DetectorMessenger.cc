#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* pDetectorConstruction)
    : G4UImessenger(),fDetectorConstruction(pDetectorConstruction)
{
    //
    lgkDir = new G4UIdirectory("/LGK/");
    lgkDir -> SetGuidance("Command to change LGK setups");

    //
    setHelmetSizeCmd = new G4UIcmdWithAnInteger("/LGK/SetHelmetSize", this);
    setHelmetSizeCmd->SetGuidance("Set helmet size(4, 8, 14, 18)");
    setHelmetSizeCmd->SetParameterName("Size", false);
    setHelmetSizeCmd->AvailableForStates(G4State_Idle, G4State_PreInit);
}

DetectorMessenger::~DetectorMessenger()
{
    delete lgkDir;
    delete setHelmetSizeCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{       
    if(command == setHelmetSizeCmd)
    {
        fDetectorConstruction->SetHelmetSize(setHelmetSizeCmd->GetNewIntValue(newValue));
    }
}