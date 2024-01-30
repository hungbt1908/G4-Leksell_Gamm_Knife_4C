// This is the program to simulate Gamma ART-6000TM
// Version: 2.0
// Date: 12/14/2021
// Author: Hung Bui Tien
// Email: hungbuitien19081997@gmail.com
// Organization: Hanoi University of Science and Technology 


#include <iostream>

#include "G4Types.hh"
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4RunManager.hh"

#include "MyDetectorConstruction.hh"
#include "MyPhysicList.hh"
#include "MyActionInitialization.hh"

#include "G4ScoringManager.hh"

#include <ctime>

namespace {
	void PrintUsage() {
		G4cerr << " Usage: " << G4endl;
		G4cerr << " ./sim [-m macro ] [-u UIsession] [-t nThreads] [-vDefault]"
			<< G4endl;
		G4cerr << "   note: -t option is available only for multi-threaded mode."
			<< G4endl;
	}
}

int main(int argc, char** argv)
{
	// Evaluate arguments
	//
	if (argc > 7) {
		PrintUsage();
		return 1;
	}

	G4String macro;
	G4String session;
	G4bool verboseBestUnits = true;
#ifdef G4MULTITHREADED
	G4int nThreads = 0;
#endif
	for (G4int i = 1; i < argc; i = i + 2) {
		if (G4String(argv[i]) == "-m") macro = argv[i + 1];
		else if (G4String(argv[i]) == "-u") session = argv[i + 1];
#ifdef G4MULTITHREADED
		else if (G4String(argv[i]) == "-t") {
			nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
		}
#endif
		else if (G4String(argv[i]) == "-vDefault") {
			verboseBestUnits = false;
			--i;  // this option is not followed with a parameter
		}
		else {
			PrintUsage();
			return 1;
		}
	}

	// Detect interactive mode (if no macro provided) and define UI session
	//
	G4UIExecutive* ui = nullptr;
	if (!macro.size()) {
		ui = new G4UIExecutive(argc, argv, session);
	}

	G4ScoringManager::GetScoringManager(); // This enables scoring
	
	// Optionally: choose a different Random engine...
	//
	G4Random::setTheEngine(new CLHEP::MTwistEngine);


	// Use G4SteppingVerboseWithUnits
	if (verboseBestUnits) {
		G4int precision = 4;
		G4SteppingVerbose::UseBestUnit(precision);
	}

	// Construct the default run manager
	//
	auto* runManager =
		G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
	if (nThreads > 0) {
		runManager->SetNumberOfThreads(nThreads);
	}
#endif

	// Set mandatory initialization classes
	//
	auto detConstruction = new MyDetectorConstruction();
	runManager->SetUserInitialization(detConstruction);

	runManager->SetUserInitialization(new MyPhysicList());

	auto actionInitialization = new MyActionInitialization();
	runManager->SetUserInitialization(actionInitialization);

	// Initialize G4 kernel
	//
	//runManager->Initialize();

	// Initialize visualization
	//
	auto visManager = new G4VisExecutive;
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	auto UImanager = G4UImanager::GetUIpointer();


	// Process macro or start UI session
	//
	if (macro.size()) {
		// batch mode
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command + macro);
	}
	else {
		// interactive mode : define UI session
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		UImanager->ApplyCommand("/control/execute defaultMacro.mac");
		if (ui->IsGUI()) {
			UImanager->ApplyCommand("/control/execute gui.mac");
		}
		ui->SessionStart();
		delete ui;
	}
	delete visManager;
	delete runManager;

	return 0;
}