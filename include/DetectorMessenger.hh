#ifndef DetecorMessenger_h
#define DetecorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class DetectorMessenger: public G4UImessenger
{
public:
    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
private:
    DetectorConstruction* fDetectorConstruction;

    G4UIdirectory* lgkDir;
    G4UIcmdWithAnInteger* setHelmetSizeCmd;  
};



#endif
