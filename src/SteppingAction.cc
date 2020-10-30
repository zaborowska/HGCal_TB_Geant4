#ifdef MATSCAN
#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4UnitsTable.hh"

SteppingAction::SteppingAction(EventAction *aEventAction)
    : G4UserSteppingAction(), fEventAction(aEventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *aStep) {
  if (aStep->GetPostStepPoint()->GetStepStatus() ==
      G4StepStatus::fWorldBoundary)
    return;
  // get material of the current volume
  G4Material *material = aStep->GetPreStepPoint()->GetMaterial();

  fEventAction->AddStep(aStep->GetStepLength(), material);
}
#endif