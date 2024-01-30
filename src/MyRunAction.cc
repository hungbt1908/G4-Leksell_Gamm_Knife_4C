#include "MyRunAction.hh"

MyRunAction::MyRunAction()
{
	sum_real = 0;
	sum_user = 0;
	timer = new G4Timer;
}

MyRunAction::~MyRunAction()
{
	delete timer;
}

void MyRunAction::BeginOfRunAction(const G4Run* aRun)
{
	G4RunManager::GetRunManager()->SetRandomNumberStore(true);
	G4cout << "Run " << aRun->GetRunID() << " starts ..." << G4endl;

	timer->Start();
}

void MyRunAction::EndOfRunAction(const G4Run* aRun)
{
	timer->Stop();

	G4double time_real = timer->GetRealElapsed();
	G4double time_user = timer->GetUserElapsed();

	sum_real = sum_real + time_real;
	sum_user = sum_user + time_user;
	G4cout << "\t  User TOT = " << sum_user <<
		"\t  Real TOT = " << sum_real << G4endl;
	G4cout << " Summary of Run " << aRun->GetRunID() << " :" << G4endl;
}