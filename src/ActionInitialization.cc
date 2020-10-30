#include "ActionInitialization.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

#ifdef MATSCAN
#include "SteppingAction.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const {
#ifndef WITHROOT
  // No ntuple merging if input is read from file (flag WITHROOT was set by
  // CMake that found ROOT package and thus allowed to change primary generator
  // from particle gun to input from ROOT file)
  EventAction *eventAction = new EventAction();
  SetUserAction(new RunAction(eventAction));
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);

  EventAction *eventAction = new EventAction();
  SetUserAction(eventAction);

  RunAction *runAction = new RunAction(eventAction);
  SetUserAction(runAction);

#ifdef MATSCAN
  SetUserAction(new SteppingAction(eventAction));
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
