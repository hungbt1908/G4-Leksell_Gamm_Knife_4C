#include "MyEventAction.hh"

MyEventAction::MyEventAction(MyRunAction* runAction)
{}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
	auto eventID = event->GetEventID();
	G4cout << "---> End of event: " << eventID << G4endl;
}