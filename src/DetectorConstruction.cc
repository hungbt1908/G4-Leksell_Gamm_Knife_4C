#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
{
  	pDetectorMessenger = new DetectorMessenger(this); 
}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// clear old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	DefineMaterial();
	DefineAttribute();
	return ConstructVolume();
}

void DetectorConstruction::DefineMaterial()
{
	// NIST
	G4NistManager* nist = G4NistManager::Instance();

	// Air
	Air = nist->FindOrBuildMaterial("G4_AIR");

	// Iron
	Iron = nist->FindOrBuildMaterial("G4_Fe");
	Iron_Color = new G4VisAttributes(G4Colour(0.5, 0.1, 0.1, 0.1));
	Iron_Color->SetVisibility(true);
	Iron_Color->SetForceSolid(true);
	//Iron_Color->SetForceAuxEdgeVisible(true);

	// Tungsten
	Tungsten = nist->FindOrBuildMaterial("G4_W");
	Tungsten_Color = new G4VisAttributes(G4Colour(1., 0.3, 0.3));
	Tungsten_Color->SetVisibility(true);
	Tungsten_Color->SetForceWireframe(true);
	Tungsten_Color->SetForceAuxEdgeVisible(true);

	// Water
	Water = nist->FindOrBuildMaterial("G4_WATER");
	G4VisAttributes* Water_Color = new G4VisAttributes(G4Colour(0.1, 0.2, 0.3));
	Water_Color->SetVisibility(true);
	Water_Color->SetForceSolid(true);

	// Stainless Steel
	G4Element* elC = G4NistManager::Instance()->FindOrBuildElement("C");
	G4Element* elSi = G4NistManager::Instance()->FindOrBuildElement("Si");
	G4Element* elP = G4NistManager::Instance()->FindOrBuildElement("P");
	G4Element* elS = G4NistManager::Instance()->FindOrBuildElement("S");
	G4Element* elCr = G4NistManager::Instance()->FindOrBuildElement("Cr");
	G4Element* elMn = G4NistManager::Instance()->FindOrBuildElement("Mn");
	G4Element* elFe = G4NistManager::Instance()->FindOrBuildElement("Fe");
	G4Element* elNi = G4NistManager::Instance()->FindOrBuildElement("Ni");
	G4Element* elMo = G4NistManager::Instance()->FindOrBuildElement("Mo");

	Stainless_Steel = new G4Material("Stainless Steel", 7.80 * g / cm3, 9 /* components */);
	Stainless_Steel->AddElement(elC, 0.026 * perCent);
	Stainless_Steel->AddElement(elSi, 0.42 * perCent);
	Stainless_Steel->AddElement(elP, 0.019 * perCent);
	Stainless_Steel->AddElement(elS, 0.003 * perCent);
	Stainless_Steel->AddElement(elCr, 16.8 * perCent);
	Stainless_Steel->AddElement(elMn, 1.4 * perCent);
	Stainless_Steel->AddElement(elFe, 68.212 * perCent);
	Stainless_Steel->AddElement(elNi, 11.01 * perCent);
	Stainless_Steel->AddElement(elMo, 2.11 * perCent);

	Steel_Color = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9));
	Steel_Color->SetVisibility(true);
	Steel_Color->SetForceWireframe(true);
	Steel_Color->SetForceAuxEdgeVisible(true);
	//Steel_Color->SetForceSolid(true);

	// Lead
	Lead = nist->FindOrBuildMaterial("G4_Pb");

	// Alumium
	Aluminum = nist->FindOrBuildMaterial("G4_Al");

	// Cobalt
	Cobalt = nist->FindOrBuildMaterial("G4_Co");

	G4VisAttributes* Cobalt_Color = new G4VisAttributes(G4Colour(0., 0., 0.7));
	Cobalt_Color->SetVisibility(true);
	Cobalt_Color->SetForceSolid(true);

	// Print materials
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void DetectorConstruction::DefineAttribute()
{}

G4VPhysicalVolume* DetectorConstruction::ConstructVolume()
{

	// world volume
	solidWorld = new G4Box("solidWorld", 500 * cm, 500 * cm, 500 * cm);
	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlap);
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	
	// PrintInformation();
	ConstructActivateCore();
	ConstructPrimaryCollimator();
	ConstructSecondaryCollimator();
	// ConstructPhantom();

	return physWorld;
}

void DetectorConstruction::ConstructActivateCore()
{
	// Activate core
	G4Tubs* solidActivateCore = new G4Tubs("solidActivateCore", 0.*deg, 0.05*cm, 1.0*cm, 0*deg, 360*deg);
	G4LogicalVolume* logicActivateCore = new G4LogicalVolume(solidActivateCore, Cobalt, "logicActivateCore");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 40.1*cm), logicActivateCore, "physActivateCore", logicWorld, false, 0, checkOverlap);

	// Airgap under activate core
	G4Cons* solidConeAirgap = new G4Cons("solidConeAirgap", 0., 0., 0., 0.05*cm, 0.03/2*cm, 0*deg, 360*deg);
	G4LogicalVolume* logicConeAirgap = new G4LogicalVolume(solidConeAirgap, Air, "logicConeAirgap");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 39.085*cm), logicConeAirgap, "physicConeAirgap", logicWorld, false, 0, checkOverlap);

	// Capsule inside
	G4Tubs* solidCapsuleInsidePart_1 = new G4Tubs("solidCapsuleInsidePart_1", 0., 0.24*cm, 2.42/2*cm, 0*deg, 360*deg);
	G4Cons* solidCapsuleInsidePart_2 = new G4Cons("solidCapsuleInsidePart_2", 0., 0.05*cm, 0., 0.24*cm, 0.18/2*cm, 0*deg, 360*deg);
	//
	// Union (solidCapsuleInsidePart_1 + solidCapsuleInsidePart_2)
	G4RotationMatrix rotm = G4RotationMatrix();
	G4ThreeVector position1 = G4ThreeVector(0., 0., 0.*cm);
	G4ThreeVector position2 = G4ThreeVector(0., 0., -1.3*cm);
	G4Transform3D tr1 = G4Transform3D(rotm, position1);
	G4Transform3D tr2 = G4Transform3D(rotm, position2);
	//
	G4MultiUnion* solidCapsuleInsideUnion = new G4MultiUnion("solidCapsuleInsideUnion");
	solidCapsuleInsideUnion->AddNode(*solidCapsuleInsidePart_1, tr1);
	solidCapsuleInsideUnion->AddNode(*solidCapsuleInsidePart_2, tr2);
	solidCapsuleInsideUnion->Voxelize();
	// solid_capsule_inside = solidCapsuleInsideUnion #solidActivateCore #solidConeAirgap
	G4ThreeVector position3 = G4ThreeVector(0., 0., -0.28*cm);
	G4Transform3D tr3 = G4Transform3D(rotm, position3);
	G4SubtractionSolid* solidCapsuleInsideSub_1 = new G4SubtractionSolid("solidCapsuleInsideSub_1", solidCapsuleInsideUnion, solidActivateCore, tr3);
	G4ThreeVector position4 = G4ThreeVector(0., 0., -1.295*cm);
	G4Transform3D tr4 = G4Transform3D(rotm, position4);
	G4SubtractionSolid* solidCapsuleInside = new G4SubtractionSolid("solidCapsuleInside", solidCapsuleInsideSub_1, solidConeAirgap, tr4);
	G4LogicalVolume* logicCapsuleInside = new G4LogicalVolume(solidCapsuleInside, Stainless_Steel, "logicCapsuleInside");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 40.38*cm), logicCapsuleInside, "physicCapsuleInside", logicWorld, false, 0, checkOverlap);

	// SS slab under Airgap 2
	G4Tubs* solidSSSlab = new G4Tubs("solidSSSlab", 0., 0.24 * cm, 0.04 / 2 * cm, 0 * deg, 360 * deg);
	G4LogicalVolume* logicSSSlab = new G4LogicalVolume(solidSSSlab, Stainless_Steel, "logicSSSlab");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 38.97*cm), logicSSSlab, "physicSSSlab", logicWorld, false, 0, checkOverlap);

	// Capsule outside
	G4Tubs* solidCapsuleOutsidePart_1 = new G4Tubs("solidCapsuleOutsidePart_1", 0., 0.4*cm, 2.57/2*cm, 0*deg, 360*deg);
	G4Cons* solidCapsuleOutsidePart_2 = new G4Cons("solidCapsuleOutsidePart_2", 0., 0.38*cm, 0., 0.4*cm, 0.32/2*cm, 0*deg, 360*deg);
	G4ThreeVector position5 = G4ThreeVector(0., 0., 0.*cm);
	G4ThreeVector position6 = G4ThreeVector(0., 0., -1.285*cm);
	G4Transform3D tr5 = G4Transform3D(rotm, position5);
	G4Transform3D tr6 = G4Transform3D(rotm, position6);
	G4MultiUnion* solidCapsuleOutsideUnion = new G4MultiUnion("solidCapsuleOutsideUnion");
	solidCapsuleOutsideUnion->AddNode(*solidCapsuleOutsidePart_1, tr5);
	solidCapsuleOutsideUnion->AddNode(*solidCapsuleOutsidePart_2, tr6);
	solidCapsuleOutsideUnion->Voxelize();
	//
	G4Tubs* solidTubHole = new G4Tubs("solidTubHole", 0., 0.24 * cm, 2.89 / 2 * cm, 0 * deg, 360 * deg);
	// Capsule outside = solidCapsuleOutsideUnion #solidTubHole
	G4ThreeVector position7 = G4ThreeVector(0., 0., -0.16 * cm);
	G4Transform3D tr7 = G4Transform3D(rotm, position7);
	G4SubtractionSolid* solidCapsuleOutside = new G4SubtractionSolid("solidCapsuleOutside", solidCapsuleOutsideUnion, solidTubHole, tr7);
	G4LogicalVolume* logicCapsuleOutside = new G4LogicalVolume(solidCapsuleOutside, Stainless_Steel, "logicCapsuleOutside");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 40.305*cm), logicCapsuleOutside, "physCapsuleOutside", logicWorld, false, 0, checkOverlap);

	// Bushing system
	G4Tubs* solidBushingPart_1 = new G4Tubs("solidBushingPart_1", 0., 1.1*cm, 1.1/2*cm, 0.*deg, 360*deg);
	G4Tubs* solidBushingPart_2 = new G4Tubs("solidBushingPart_2", 0., 0.8*cm, 0.5/2*cm, 0.*deg, 360*deg);
	G4Tubs* solidBushingPart_3 = new G4Tubs("solidBushingPart_3", 0., 1.4*cm, 0.25/2*cm, 0.*deg, 360*deg);
	G4Tubs* solidBushingPart_4 = new G4Tubs("solidBushingPart_4", 0., 1.2*cm, 1.7/2*cm, 0.*deg, 360*deg);
	// solidBushingPart_1 + solidBushingPart_2 + solidBushingPart_3 + solidBushingPart_4
	G4ThreeVector position8 = G4ThreeVector(0., 0., -0.8*cm);
	G4Transform3D tr8 = G4Transform3D(rotm, position8);
	//
	G4ThreeVector position9 = G4ThreeVector(0., 0., -1.175*cm);
	G4Transform3D tr9 = G4Transform3D(rotm, position9);
	G4ThreeVector position10 = G4ThreeVector(0., 0., -2.15*cm);
	G4Transform3D tr10 = G4Transform3D(rotm, position10);
	G4MultiUnion* solidBushingUnion = new G4MultiUnion("solidBushingUnion");
	solidBushingUnion->AddNode(*solidBushingPart_1, tr1);
	solidBushingUnion->AddNode(*solidBushingPart_2, tr8);
	solidBushingUnion->AddNode(*solidBushingPart_3, tr9);
	solidBushingUnion->AddNode(*solidBushingPart_4, tr10);
	solidBushingUnion->Voxelize();
	// solidBushingUnion #solidCapsuleOutsideUnion
	G4ThreeVector position11 = G4ThreeVector(0., 0., -1.145*cm);
	G4Transform3D tr11 = G4Transform3D(rotm, position11);
	G4SubtractionSolid* solidBushingSubtraction_1 = new G4SubtractionSolid("solidBushingSubtraction_1", solidBushingUnion, solidCapsuleOutsideUnion, tr11);
	// Bottom airgap 4 under Bushing
	G4Tubs* solidBottomAirgap_4 = new G4Tubs("solidBottomAirgap_4", 0., 0.25*cm, 0.42/2*cm, 0*deg, 360*deg);
	G4ThreeVector position12 = G4ThreeVector(0., 0., -2.79*cm);
	G4Transform3D tr12 = G4Transform3D(rotm, position12);
	G4SubtractionSolid* solidBushingSubtraction_2 = new G4SubtractionSolid("solidBushingSubtraction_2", solidBushingSubtraction_1, solidBottomAirgap_4, tr12);
	G4LogicalVolume* logicBushing = new G4LogicalVolume(solidBushingSubtraction_2, Aluminum, "logicBushing");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 41.45*cm), logicBushing, "physBushing", logicWorld, false, 0, checkOverlap);
}

void DetectorConstruction::ConstructPrimaryCollimator()
{
	// Part 1
	G4Tubs* solidPrimaryCollimatorPart_1 = new G4Tubs("solidPrimaryCollimatorPart_1", 0.25*cm, 1.2*cm, 6.5/2*cm, 0*deg, 360*deg);
	G4LogicalVolume* logicPrimaryCollimatorPart_1 = new G4LogicalVolume(solidPrimaryCollimatorPart_1, Tungsten, "logicPrimaryCollimatorPart_1");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 35.1*cm), logicPrimaryCollimatorPart_1, "physPrimaryCollimatorPart_1", logicWorld, false, 0, checkOverlap);
	
	// Part 2
	G4Tubs* solidPrimaryCollimatorPart_2 = new G4Tubs("solidPrimaryCollimatorPart_2", 0., 1.2*cm, 9.25/2*cm, 0*deg, 360*deg);
	G4Cons* solidPrimaryCollimatorPart_2_hole = new G4Cons("solidPrimaryCollimatorPart_2_hole", 0., 0.425*cm, 0., 0.25*cm, 9.26/2*cm, 0*deg, 360*deg);

	G4SubtractionSolid* solidPrimaryCollimatorPart_2_sub = new G4SubtractionSolid("solidPrimaryCollimatorPart_2_sub", solidPrimaryCollimatorPart_2, solidPrimaryCollimatorPart_2_hole);
	G4LogicalVolume* logicPrimaryCollimatorPart_2 = new G4LogicalVolume(solidPrimaryCollimatorPart_2_sub, Lead, "logicPrimaryCollimatorPart_2");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 27.225*cm), logicPrimaryCollimatorPart_2, "physPrimaryCollimatorPart_2", logicWorld, false, 0, checkOverlap);
}

void DetectorConstruction::ConstructSecondaryCollimator()
{
	solidSecondaryCollimator = new G4Cons("solidSecondaryCollimator", 0. /*will be set later*/, 0.9*cm, 0. /*will be set later*/, 0.9*cm, 3*cm, 0*deg, 360*deg);
	UpdateHelmet();
	G4LogicalVolume* logicSecondaryCollimator = new G4LogicalVolume(solidSecondaryCollimator, Tungsten, "logicSecondaryCollimator");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 19.5*cm), logicSecondaryCollimator, "physSecondaryCollimator", logicWorld, false, 0, checkOverlap);
}

void DetectorConstruction::ConstructLGK()
{

}

void DetectorConstruction::ConstructMTLGK()
{

}

void DetectorConstruction::ConstructPhantom()
{
	G4Orb* solid_phantom = new G4Orb("solid_phantom", 8.0 * cm);
	G4LogicalVolume* logic_phantom = new G4LogicalVolume(solid_phantom, Water, "logic_phantom");
	new G4PVPlacement(0, G4ThreeVector(), logic_phantom, "physic_phantom", logicWorld, false, 0, checkOverlap);
}

void DetectorConstruction::SetHelmetSize(G4int size)
{
	if (size != helmetSize) // Only if the size changes
	{
		// Allow only valid numbers
		switch (size)
		{
		case 18:
		case 14:
		case 8:
		case 4:
			helmetSize = size;
			G4cout << "Helmet size set to " << helmetSize << std::endl;
			UpdateHelmet();
			break;
		default:
			G4Exception("GammaKnifeDetectorConstruction::SetHelmetSize()",
				"GammaKnife001", FatalException,
				"Error: Invalid helmet size.");

		}
	}
}

void DetectorConstruction::UpdateHelmet()
{
	if(solidSecondaryCollimator)
	{
		switch (helmetSize)
		{
		case 18:
			solidSecondaryCollimator->SetInnerRadiusMinusZ(5.3*mm);
			solidSecondaryCollimator->SetInnerRadiusPlusZ(4.15*mm);
			break;

		case 14:
			solidSecondaryCollimator->SetInnerRadiusMinusZ(4.25*mm);
			solidSecondaryCollimator->SetInnerRadiusPlusZ(3.15*mm);
			break;

		case 8:
			solidSecondaryCollimator->SetInnerRadiusMinusZ(2.5*mm);
			solidSecondaryCollimator->SetInnerRadiusPlusZ(1.9*mm);
			break;

		case 4:
			solidSecondaryCollimator->SetInnerRadiusMinusZ(1.25*mm);
			solidSecondaryCollimator->SetInnerRadiusPlusZ(1*mm);
			break;
		}
		// Inform the run manager about change in the geometry
		G4RunManager::GetRunManager()->GeometryHasBeenModified();
	}
}

void DetectorConstruction::PrintInformation()
{}

void DetectorConstruction::ConstructSDandField()
{

}