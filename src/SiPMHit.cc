#include "SiPMHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiPMHit::SiPMHit(G4String aVolumeName, G4int aCopyNumSensor,
                 G4int aCopyNumCell) {
  fVolumeName = aVolumeName;
  fCopyNumCell = aCopyNumCell;
  fCopyNumSensor = aCopyNumSensor;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiPMHit::Digitise(const G4double aTimeWindow,
                       const G4double aToaThreshold) {

  // process energy deposits
  if (fEdep.size() == 0) {
    fIsValidHit = false;
    return;
  }

  std::sort(fEdep.begin(), fEdep.end(),
            [](const std::pair<G4double, G4double> &left,
               const std::pair<G4double, G4double> &right) {
              return left.second < right.second; // second = time
            });

  G4double firstHitTime = fEdep[0].second;
  fEdepDigi = 0;
  for (size_t i = 0; i < fEdep.size(); i++) {
    if (aTimeWindow < 0 || fEdep[i].second < firstHitTime + aTimeWindow)
      fEdepDigi += fEdep[i].first;
#ifdef DEBUG
    else
      std::cout << "Rejecting hit (" << fEdep[i].first << " keV)  at time "
                << fEdep[i].second << "(start: " << firstHitTime << ")"
                << std::endl;
#endif
    if ((fTimeOfArrival == -1) && (fEdepDigi > aToaThreshold))
      fTimeOfArrival = fEdep[i].second;
  }
  fIsValidHit = (fEdepDigi > 0);

  // non ionizing part, does not contribute to TOAs
  if (fEdepNonIonizing.size() == 0)
    return;

  std::sort(fEdepNonIonizing.begin(), fEdepNonIonizing.end(),
            [](const std::pair<G4double, G4double> &left,
               const std::pair<G4double, G4double> &right) {
              return left.second < right.second; // second = time
            });

  fEdepNonIonizingDigi = 0;
  for (size_t i = 0; i < fEdep.size(); i++) {
    if (aTimeWindow == -1 ||
        fEdepNonIonizing[i].second < firstHitTime + aTimeWindow)
      fEdepNonIonizingDigi += fEdepNonIonizing[i].first;
  }
}
