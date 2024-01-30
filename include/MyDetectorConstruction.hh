#ifndef MYDETECTORCONSTRUCTION_HH
#define	MYDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Orb.hh"

#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"

#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"

#include "G4RunManager.hh"

#include "MyDetectorMessenger.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();
	
	virtual G4VPhysicalVolume *Construct();

private:
	// methods
	//
	void DefineMaterials();
	G4VPhysicalVolume* DefineVolumes();

	G4bool check_overlap = true;

public:
	// Solid volume
	G4Box* solidWorld;
	G4Sphere* solid_shielding1, * solid_shielding2, * solid_shielding3;
	G4Orb* solid_phantom;
	G4Tubs* solid_activate_core, * solid_capsule_inside_part_1, * solid_tubs_outside_airgap, * solid_SS_slab,
		* solid_airgap_3, * solid_capsule_outside_part_1, * solid_tub_hole, * solid_bottom_airgap_4, * solid_bushing_part_1,
		* solid_bushing_part_2, * solid_bushing_part_3, *solid_bushing_part_4, * solid_shielding_outer_part_1, * solid_shielding_outer_part_2,
		* solid_primary_collimator_part_1, * solid_primary_collimator_part_2;
	G4Cons* solid_cone_airgap, * solid_capsule_inside_part_2, * solid_capsule_outside_part_2, * solid_primary_collimator_part_2_hole,
		* solid_secondary_collimator;

	// Logical volume
	G4LogicalVolume* logicWorld, * logic_shielding1, * logic_shielding2, * logic_shielding3, * logic_phantom,
		* logic_activate_core, * logic_cone_airgap, * logic_capsule_inside, * logic_airgap_2, * logic_SS_slab,
		* logic_airgap_3, * logic_capsule_outside, * logic_bottom_airgap_4, * logic_bushing, * logic_primary_collimator_part_1,
		* logic_primary_collimator_part_2, * logic_secondary_collimator;

	// Physical volume
	G4VPhysicalVolume* physWorld;

	// Material
	G4Material* Air, * Iron,* Tungsten, * Water, * Stainless_Steel, * Lead, * Aluminum, * Cobalt;

	G4VisAttributes* Steel_Color, * Iron_Color, * Tungsten_Color;

private:
	void UpdateHelmet();
	G4int helmetSize; // helmet size (4, 8, 14, 18)

public:
	void SetHelmetSize(G4int);
	
	MyDetectorMessenger* detectorMessenger;
};


#endif