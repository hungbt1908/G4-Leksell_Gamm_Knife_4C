// This is the program to simulate Gamma ART-6000TM
// Version: 2.0
// Date: 12/14/2021
// Author: Hung Bui Tien
// Email: hungbuitien19081997@gmail.com
// Organization: Hanoi University of Science and Technology 


#include <iostream>

#include "G4Types.hh"
#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4RunManager.hh"

#include "MyDetectorConstruction.hh"
#include "MyPhysicList.hh"
#include "MyActionInitialization.hh"

#include "G4ScoringManager.hh"

#include <ctime>

int main(int argc, char** argv)
{

	G4int seconds = time(NULL);
	G4Random::setTheSeed(seconds);

	auto* runManager = G4RunManagerFactory::CreateRunManager();
	G4int nThreads = 4;
	runManager->SetNumberOfThreads(nThreads);

	G4ScoringManager::GetScoringManager(); // This enables scoring
	
	// Optionally: choose a different Random engine...
	//
	G4Random::setTheEngine(new CLHEP::MTwistEngine);

	// Set mandatory initialization classes
	//
	auto detConstruction = new MyDetectorConstruction();
	runManager->SetUserInitialization(detConstruction);

	runManager->SetUserInitialization(new MyPhysicList());

	auto actionInitialization = new MyActionInitialization();
	runManager->SetUserInitialization(actionInitialization);

	// Initialize G4 kernel
	//
	runManager->Initialize();

	// Initialize visualization
	//
	auto visManager = new G4VisExecutive;
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	auto UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	//
	if (argc != 1) {
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}
	else {
		// interactive mode : define UI session

		G4UIExecutive* ui = new G4UIExecutive(argc, argv);
		UImanager->ApplyCommand("/control/execute defaultMacro.mac");
		ui->SessionStart();
		delete ui;

	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted 
	// in the main() program !

	delete visManager;
	delete runManager;

	return 0;
}