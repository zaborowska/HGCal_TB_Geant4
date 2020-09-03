#ifdef MATSCAN
#ifndef SteppingAction_h
#define SteppingAction_h

#include "G4UserSteppingAction.hh"
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(EventAction*);
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:
  EventAction* fEventAction;
};

#endif
#endif