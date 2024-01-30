#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class G4VisAttributes;
class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class PhantomSD;
class PhantomROGeometry;
class DetectorMessenger;
class G4Box;
class G4Cons;

class DetectorConstruction : public G4VUserDetectorConstruction 
{
public:
    // methods
    DetectorConstruction();
    ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ConstructSDandField();
    void DefineMaterial();
    void DefineAttribute();
    G4VPhysicalVolume* ConstructVolume();
    void ConstructActivateCore();
    void ConstructPrimaryCollimator();
    void ConstructSecondaryCollimator();
 
public:
    // attributes

    G4bool checkOverlap = false;

    // materials 
    G4Material* Air;
    G4Material* Iron;
    G4Material* Tungsten;
    G4Material* Water;
    G4Material* Stainless_Steel;
    G4Material* Lead;
    G4Material* Aluminum;
    G4Material* Cobalt;

    // visualization
    G4VisAttributes* worldAtt;
    G4VisAttributes* Steel_Color;
    G4VisAttributes* Iron_Color;
    G4VisAttributes* Tungsten_Color;

    // volumes
    G4Box* solidWorld;
    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;
    G4Cons* solidSecondaryCollimator;

    G4int helmetSize; // helmet size (4, 8, 14, 18)
    G4int NS; // source number

public:
    // set methods
    void ConstructLGK();
    void ConstructMTLGK();
    void ConstructPhantom();
    void UpdateHelmet();
    void SetHelmetSize(G4int);
    void PrintInformation();

public:
    DetectorMessenger* pDetectorMessenger;

};

#endif
