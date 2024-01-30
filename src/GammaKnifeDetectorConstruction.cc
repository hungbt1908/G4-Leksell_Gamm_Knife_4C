#include "GammaKnifeDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ThreadLocal
G4GlobalMagFieldMessenger* GammaKnifeDetectorConstruction::fMagFieldMessenger = 0;


GammaKnifeDetectorConstruction::GammaKnifeDetectorConstruction()
	: G4VUserDetectorConstruction()
	, helmetSize(4)
{
	detectorMessenger = new GammaKnifeDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GammaKnifeDetectorConstruction::~GammaKnifeDetectorConstruction()
{
	delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* GammaKnifeDetectorConstruction::Construct()
{
	// Define materials 
	DefineMaterials();
	//ReadFile("MachineAngle.in");
	// Define volumes
	return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GammaKnifeDetectorConstruction::DefineMaterials()
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* GammaKnifeDetectorConstruction::DefineVolumes()
{
	// ------------------------------------------------------------------------------------------
	// Mother world volume
	// ------------------------------------------------------------------------------------------
	solidWorld = new G4Box("solidWorld", 500 * cm, 500 * cm, 500 * cm);

	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");

	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, check_overlap);

	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

	// ------------------------------------------------------------------------------------------
	// Sphere shielding No.1 contain source block
	// ------------------------------------------------------------------------------------------

	// Activate core
	solid_activate_core = new G4Tubs("solid_activate_core", 0. * deg, 0.05 * cm, 1.0 * cm, 0 * deg, 360 * deg);
	logic_activate_core = new G4LogicalVolume(solid_activate_core, Cobalt, "logic_activate_core");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 40.1 * cm), logic_activate_core, "physic_activate_core", logicWorld, false, 0, check_overlap);

	// Airgap under activate core
	solid_cone_airgap = new G4Cons("solid_cone_airgap", 0., 0., 0., 0.05 * cm, 0.03 / 2 * cm, 0 * deg, 360 * deg);
	logic_cone_airgap = new G4LogicalVolume(solid_cone_airgap, Air, "logic_cone_airgap");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 39.085 * cm), logic_cone_airgap, "physic_cone_airgap", logicWorld, false, 0, check_overlap);

	// Capsule inside
	solid_capsule_inside_part_1 = new G4Tubs("solid_capsule_inside_part_1", 0., 0.24 * cm, 2.42 / 2 * cm, 0 * deg, 360 * deg);
	solid_capsule_inside_part_2 = new G4Cons("solid_capsule_inside_part_2", 0., 0.05 * cm, 0., 0.24 * cm, 0.18 / 2 * cm, 0 * deg, 360 * deg);

	// Union (solid_capsule_inside_part_1 + solid_capsule_inside_part_2)
	G4RotationMatrix rotm = G4RotationMatrix();
	G4ThreeVector position1 = G4ThreeVector(0., 0., 0. * cm);
	G4ThreeVector position2 = G4ThreeVector(0., 0., -1.3 * cm);
	G4Transform3D tr1 = G4Transform3D(rotm, position1);
	G4Transform3D tr2 = G4Transform3D(rotm, position2);

	G4MultiUnion* solid_capsule_inside_union = new G4MultiUnion("solid_capsule_inside_union");
	solid_capsule_inside_union->AddNode(*solid_capsule_inside_part_1, tr1);
	solid_capsule_inside_union->AddNode(*solid_capsule_inside_part_2, tr2);
	solid_capsule_inside_union->Voxelize();

	// solid_capsule_inside = (solid_capsule_inside_part_1 + solid_capsule_inside_part_2) #solid_activate_core #solid_cone_airgap
	// (solid_capsule_inside_part_1 + solid_capsule_inside_part_2) #solid_activate_core
	G4ThreeVector position3 = G4ThreeVector(0., 0., -0.28 * cm);
	G4Transform3D tr3 = G4Transform3D(rotm, position3);
	G4SubtractionSolid* solid_capsule_inside_sub_1 = new G4SubtractionSolid("solid_capsule_inside_sub_1", solid_capsule_inside_union, solid_activate_core, tr3);
	// (solid_capsule_inside_part_1 + solid_capsule_inside_part_2) #solid_activate_core #solid_cone_airgap
	G4ThreeVector position4 = G4ThreeVector(0., 0., -1.295 * cm);
	G4Transform3D tr4 = G4Transform3D(rotm, position4);
	G4SubtractionSolid* solid_capsule_inside = new G4SubtractionSolid("solid_capsule_inside", solid_capsule_inside_sub_1, solid_cone_airgap, tr4);
	logic_capsule_inside = new G4LogicalVolume(solid_capsule_inside, Stainless_Steel, "logic_capsule_inside");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 40.38 * cm), logic_capsule_inside, "physic_capsule_inside", logicWorld, false, 0, check_overlap);

	// Airgap 2 under the Capsule inside
	//solid_tubs_outside_airgap = new G4Tubs("solid_tubs_outside_airgap", 0., 0.24 * cm, 0.18 / 2 * cm, 0 * deg, 360 * deg);
	//solid_airgap_2 = solid_tubs_outside_airgap # solid_capsule_inside_part_2
	//G4SubtractionSolid* solid_airgap_2 = new G4SubtractionSolid("solid_airgap_2", solid_tubs_outside_airgap, solid_capsule_inside_part_2);
	//logic_airgap_2 = new G4LogicalVolume(solid_airgap_2, Air, "logic_airgap_2");
	//new G4PVPlacement(0, G4ThreeVector(0., 0., 39.08 * cm), logic_airgap_2, "physic_airgap_2", logicWorld, false, 0, check_overlap);

	// SS slab under Airgap 2
	solid_SS_slab = new G4Tubs("solid_SS_slab", 0., 0.24 * cm, 0.04 / 2 * cm, 0 * deg, 360 * deg);
	logic_SS_slab = new G4LogicalVolume(solid_SS_slab, Stainless_Steel, "logic_SS_slab");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 38.97 * cm), logic_SS_slab, "physic_SS_slab", logicWorld, false, 0, check_overlap);

	// Cylinder airgap 3 under SS slab
	//solid_airgap_3 = new G4Tubs("solid_airgap_3", 0., 0.24 * cm, 0.08 / 2 * cm, 0 * deg, 360 * deg);
	//logic_airgap_3 = new G4LogicalVolume(solid_airgap_3, Air, "logic_airgap_3");
	//new G4PVPlacement(0, G4ThreeVector(0., 0., 38.91 * cm), logic_airgap_3, "physic_airgap_3", logicWorld, false, 0, check_overlap);

	// Capsule outside
	solid_capsule_outside_part_1 = new G4Tubs("solid_capsule_outside_part_1", 0., 0.4 * cm, 2.57 / 2 * cm, 0 * deg, 360 * deg);
	solid_capsule_outside_part_2 = new G4Cons("solid_capsule_outside_part_2", 0., 0.38 * cm, 0., 0.4 * cm, 0.32 / 2 * cm, 0 * deg, 360 * deg);

	G4ThreeVector position5 = G4ThreeVector(0., 0., 0. * cm);
	G4ThreeVector position6 = G4ThreeVector(0., 0., -1.285 * cm);
	G4Transform3D tr5 = G4Transform3D(rotm, position5);
	G4Transform3D tr6 = G4Transform3D(rotm, position6);

	G4MultiUnion* solid_capsule_outside_union = new G4MultiUnion("solid_capsule_outside_union");
	solid_capsule_outside_union->AddNode(*solid_capsule_outside_part_1, tr5);
	solid_capsule_outside_union->AddNode(*solid_capsule_outside_part_2, tr6);
	solid_capsule_outside_union->Voxelize();

	solid_tub_hole = new G4Tubs("solid_tub_hole", 0., 0.24 * cm, 2.89 / 2 * cm, 0 * deg, 360 * deg);
	// Capsule outside = solid_capsule_outside_union #solid_tub_hole
	G4ThreeVector position7 = G4ThreeVector(0., 0., -0.16 * cm);
	G4Transform3D tr7 = G4Transform3D(rotm, position7);
	G4SubtractionSolid* solid_capsule_outside = new G4SubtractionSolid("solid_capsule_outside", solid_capsule_outside_union, solid_tub_hole, tr7);

	logic_capsule_outside = new G4LogicalVolume(solid_capsule_outside, Stainless_Steel, "logic_capsule_outside");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 40.305 * cm), logic_capsule_outside, "physic_capsule_outside", logicWorld, false, 0, check_overlap);

	// Bushing system
	solid_bushing_part_1 = new G4Tubs("solid_bushing_part_1", 0., 1.1 * cm, 1.1 / 2 * cm, 0. * deg, 360 * deg);
	solid_bushing_part_2 = new G4Tubs("solid_bushing_part_2", 0., 0.8 * cm, 0.5 / 2 * cm, 0. * deg, 360 * deg);
	solid_bushing_part_3 = new G4Tubs("solid_bushing_part_3", 0., 1.4 * cm, 0.25 / 2 * cm, 0. * deg, 360 * deg);
	solid_bushing_part_4 = new G4Tubs("solid_bushing_part_4", 0., 1.2 * cm, 1.7 / 2 * cm, 0. * deg, 360 * deg);
	// solid_bushing_part_1 + solid_bushing_part_2 + solid_bushing_part_3 + solid_bushing_part_4
	G4ThreeVector position8 = G4ThreeVector(0., 0., -0.8 * cm);
	G4Transform3D tr8 = G4Transform3D(rotm, position8);

	G4ThreeVector position9 = G4ThreeVector(0., 0., -1.175 * cm);
	G4Transform3D tr9 = G4Transform3D(rotm, position9);

	G4ThreeVector position10 = G4ThreeVector(0., 0., -2.15 * cm);
	G4Transform3D tr10 = G4Transform3D(rotm, position10);

	G4MultiUnion* solid_bushing_union = new G4MultiUnion("solid_bushing_union");
	solid_bushing_union->AddNode(*solid_bushing_part_1, tr1);
	solid_bushing_union->AddNode(*solid_bushing_part_2, tr8);
	solid_bushing_union->AddNode(*solid_bushing_part_3, tr9);
	solid_bushing_union->AddNode(*solid_bushing_part_4, tr10);
	solid_bushing_union->Voxelize();
	// (solid_bushing_part_1 + solid_bushing_part_2 + solid_bushing_part_3 + solid_bushing_part_4) #solid_capsule_outside_union
	G4ThreeVector position11 = G4ThreeVector(0., 0., -1.145 * cm);
	G4Transform3D tr11 = G4Transform3D(rotm, position11);
	G4SubtractionSolid* solid_bushing_subtraction_1 = new G4SubtractionSolid("solid_bushing_subtraction_1", solid_bushing_union, solid_capsule_outside_union, tr11);
	// Bottom airgap 4 under Bushing
	solid_bottom_airgap_4 = new G4Tubs("solid_bottom_airgap_4", 0., 0.25 * cm, 0.42 / 2 * cm, 0 * deg, 360 * deg);
	G4ThreeVector position12 = G4ThreeVector(0., 0., -2.79 * cm);
	G4Transform3D tr12 = G4Transform3D(rotm, position12);
	G4SubtractionSolid* solid_bushing_subtraction_2 = new G4SubtractionSolid("solid_bushing_subtraction_2", solid_bushing_subtraction_1, solid_bottom_airgap_4, tr12);
	logic_bushing = new G4LogicalVolume(solid_bushing_subtraction_2, Aluminum, "logic_bushing");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 41.45 * cm), logic_bushing, "physic_bushing", logicWorld, false, 0, check_overlap);

	// Shielding around

	/*
	// Sphere shielding
	solid_shielding1 = new G4Sphere("solid_shielding1", 38.47 * cm, 42 * cm, 0 * deg, 360 * deg, 0 * deg, 90 * deg);

	G4ThreeVector position13 = G4ThreeVector(0., 0., 41.45 * cm);
	G4Transform3D tr13 = G4Transform3D(rotm, position13);

	G4SubtractionSolid* logic_shielding1_subtraction = new G4SubtractionSolid("logic_shielding1_subtraction", solid_shielding1, solid_bushing_union, tr13);
	logic_shielding1 = new G4LogicalVolume(logic_shielding1_subtraction, Iron, "logic_shielding1");
	new G4PVPlacement(0, G4ThreeVector(), logic_shielding1, "physic_shielding1", logicWorld, false, 0, check_overlap);
	//logic_shielding1->SetVisAttributes(G4VisAttributes::GetInvisible());*/


	// ------------------------------------------------------------------------------------------
	// Sphere shielding No.2 contain primary collimator
	// ------------------------------------------------------------------------------------------
	//solid_shielding2 = new G4Sphere("solid_shielding2", 22.60 * cm, 38.35 * cm, 0 * deg, 360 * deg, 0 * deg, 90 * deg);
	//logic_shielding2 = new G4LogicalVolume(solid_shielding2, Iron, "logic_shielding2");
	//new G4PVPlacement(0, G4ThreeVector(), logic_shielding2, "physic_shielding2", logicWorld, false, 0, check_overlap);
	//logic_shielding2->SetVisAttributes(G4VisAttributes::GetInvisible());

	// Primary Collimator 
	// Part 1
	solid_primary_collimator_part_1 = new G4Tubs("solid_primary_collimator_part_1", 0.25 * cm, 1.2 * cm, 6.5 / 2 * cm, 0 * deg, 360 * deg);
	logic_primary_collimator_part_1 = new G4LogicalVolume(solid_primary_collimator_part_1, Tungsten, "logic_primary_collimator_part_1");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 35.1 * cm), logic_primary_collimator_part_1, "physic_primary_collimator_part_1", logicWorld, false, 0, check_overlap);

	//Part 2
	solid_primary_collimator_part_2 = new G4Tubs("solid_primary_collimator_part_2", 0., 1.2 * cm, 9.25 / 2 * cm, 0 * deg, 360 * deg);
	solid_primary_collimator_part_2_hole = new G4Cons("solid_primary_collimator_part_2_hole", 0., 0.425 * cm, 0., 0.25 * cm, 9.26 / 2 * cm, 0 * deg, 360 * deg);

	G4SubtractionSolid* solid_primary_collimator_part_2_sub = new G4SubtractionSolid("solid_primary_collimator_part_2_sub", solid_primary_collimator_part_2, solid_primary_collimator_part_2_hole);
	logic_primary_collimator_part_2 = new G4LogicalVolume(solid_primary_collimator_part_2_sub, Lead, "solid_primary_collimator_part_2");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 27.225 * cm), logic_primary_collimator_part_2, "physic_primary_collimator_part_2", logicWorld, false, 0, check_overlap);

	/*
	// ------------------------------------------------------------------------------------------
	// Sphere shielding No.3 contain secondary collimator
	// ------------------------------------------------------------------------------------------
	solid_shielding3 = new G4Sphere("solid_shielding3", 16.51 * cm, 22.50 * cm, 0 * deg, 360 * deg, 0 * deg, 90 * deg);
	logic_shielding3 = new G4LogicalVolume(solid_shielding3, Iron, "logic_shielding3");
	new G4PVPlacement(0, G4ThreeVector(), logic_shielding3, "physic_shielding3", logicWorld, false, 0, check_overlap);
	logic_shielding3->SetVisAttributes(G4VisAttributes::GetInvisible());*/
	solid_secondary_collimator = new G4Cons("solid_secondary_collimator", 0. /*will be set later*/, 0.9 * cm, 0. /*will be set later*/, 0.9 * cm, 3 * cm, 0 * deg, 360 * deg);
	logic_secondary_collimator = new G4LogicalVolume(solid_secondary_collimator, Tungsten, "logic_secondary_collimator");
	new G4PVPlacement(0, G4ThreeVector(0., 0., 19.5 * cm), logic_secondary_collimator, "physic_secondary_collimator", logicWorld, false, 0, check_overlap);

	// ------------------------------------------------------------------------------------------
	// Water phantom
	// ------------------------------------------------------------------------------------------	
	solid_phantom = new G4Orb("solid_phantom", 8.0 * cm);
	logic_phantom = new G4LogicalVolume(solid_phantom, Water, "logic_phantom");
	new G4PVPlacement(0, G4ThreeVector(), logic_phantom, "physic_phantom", logicWorld, false, 0, check_overlap);

	return physWorld;

}

void GammaKnifeDetectorConstruction::ConstructSDandField() {


	G4ThreeVector fieldValue;
	fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
	fMagFieldMessenger->SetVerboseLevel(1);

	// Register the field messenger for deleting
	G4AutoDelete::Register(fMagFieldMessenger);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void GammaKnifeDetectorConstruction::UpdateHelmet()
{
	if (solid_secondary_collimator)
	{
		switch (helmetSize)
		{
		case 18:
			solid_secondary_collimator->SetInnerRadiusMinusZ(5.3 * mm);
			solid_secondary_collimator->SetInnerRadiusPlusZ(4.15 * mm);
			break;

		case 14:
			solid_secondary_collimator->SetInnerRadiusMinusZ(4.25 * mm);
			solid_secondary_collimator->SetInnerRadiusPlusZ(3.15 * mm);
			break;

		case 8:
			solid_secondary_collimator->SetInnerRadiusMinusZ(2.5 * mm);
			solid_secondary_collimator->SetInnerRadiusPlusZ(1.9 * mm);
			break;

		case 4:
			solid_secondary_collimator->SetInnerRadiusMinusZ(1.25 * mm);
			solid_secondary_collimator->SetInnerRadiusPlusZ(1 * mm);
			break;
		}
		// Inform the run manager about change in the geometry
		G4RunManager::GetRunManager()->GeometryHasBeenModified();
	}
}

void GammaKnifeDetectorConstruction::SetHelmetSize(G4int size)
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
