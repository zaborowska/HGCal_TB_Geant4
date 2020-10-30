#include "SiliconPixelHit.hh"

#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "HGCalTBMaterials.hh"

#include <cstdlib>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiliconPixelHit::SiliconPixelHit(G4String aVolumeName, G4int aCopyNumSensor,
                                 G4int aCopyNumCell)
    : fVolumeName(aVolumeName), fCopyNumCell(aCopyNumCell),
      fCopyNumSensor(aCopyNumSensor) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiliconPixelHit::Digitise(const G4double aTimeWindow,
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
    if (fEdepDigi > aToaThreshold) {
      if (fTimeOfArrival == -1)
        fTimeOfArrival = fEdep[i].second;
      fTimeOfArrivalLast = fEdep[i].second;
    }
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiliconPixelHit::Draw() {
  G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance();
  if (!(fEdepDigi > 0))
    return;
  if (pVVisManager) {
    if (!pVVisManager->FilterHit(*this))
      return;
    G4Transform3D trans(G4RotationMatrix(),
                        G4ThreeVector(fPosX * cm, fPosY * cm, fPosZ * cm));
    G4VisAttributes attribs;
    auto solid = HexagonSolid("dummy", 0.3 * mm, 0.6496345 * cm);
    G4Colour colour(1, 0, 0);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*solid, attribs, trans);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......