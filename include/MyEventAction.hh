#ifndef MYEVENTACTION_HH
#define MYEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "MyRunAction.hh"
#include "G4RunManager.hh"

class MyEventAction : public G4UserEventAction
{
public:
	MyEventAction(MyRunAction* runAction);
	~MyEventAction();

	void BeginOfEventAction(const G4Event* event);
	void EndOfEventAction(const G4Event* event);
};

#endif // !MYEVENTACTION_HH
