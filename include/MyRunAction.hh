#ifndef MYRUNACTION_HH
#define MYRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4Timer.hh"
#include "G4RunManager.hh"

class MyRunAction : public G4UserRunAction
{
public:
	MyRunAction();
	~MyRunAction();

public:
	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run* );

private:
	G4Timer* timer;
	G4double sum_real;
	G4double sum_user;
};
#endif



