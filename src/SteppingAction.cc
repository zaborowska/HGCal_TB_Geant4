#ifdef MATSCAN
#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4UnitsTable.hh"

SteppingAction::SteppingAction(EventAction* aEventAction)
  : G4UserSteppingAction(),
  fEventAction(aEventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step *aStep)
{
  if (aStep->GetPostStepPoint()->GetStepStatus() == G4StepStatus::fWorldBoundary) return;
  // get material of the current volume
  G4Material* material = aStep->GetPreStepPoint()->GetMaterial();

  // G4cout << "step in\t" << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() 
  // << "\t"  <<aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << "\t" << material->GetName() << "\t"
  // << G4BestUnit(aStep->GetStepLength(), "Length") << "\t"
  // << G4BestUnit(material->GetRadlen(), "Length") << "\t"
  // << G4BestUnit(material->GetNuclearInterLength(), "Length") << G4endl;

  fEventAction->AddStep( aStep->GetStepLength(), material);
}
#endif