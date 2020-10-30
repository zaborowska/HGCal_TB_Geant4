#include "HGCalTBMaterials.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SubtractionSolid *HexagonSolid(G4String aName, G4double aCellThickness,
                                 G4double aCellSideLength) {
  G4double fullCcellX = (2.) * aCellSideLength;
  G4double fullCellY = sqrt(3.) * aCellSideLength;
  G4Box *solidFullcell = new G4Box(aName, // its aName
                                   0.5 * fullCcellX, 0.5 * fullCellY,
                                   0.5 * aCellThickness); // its size

  G4double deltaXDash = aCellSideLength;
  G4double deltaYDash = sqrt(3) / 4 * aCellSideLength;

  G4Box *solidCutcell = new G4Box(aName, // its aName
                                  0.5 * deltaXDash, 0.5 * (deltaYDash),
                                  1. * aCellThickness); // its size

  G4double deltaTheta[4] = {30. * deg, 150. * deg, 210. * deg, 330. * deg};
  G4double deltaThetaRot[4] = {60. * deg, 120. * deg, 240 * deg, 300 * deg};
  G4double delta = sqrt(3) / 2 * aCellSideLength + deltaYDash / 2;

  G4RotationMatrix *rot = new G4RotationMatrix;
  rot->rotateZ(deltaThetaRot[0]);
  std::vector<G4SubtractionSolid *> subtracted;
  subtracted.push_back(
      new G4SubtractionSolid("cellSubtracted", solidFullcell, solidCutcell, rot,
                             G4ThreeVector(cos(deltaTheta[0]) * delta,
                                           sin(deltaTheta[0]) * delta, 0.)));

  for (int i = 1; i < 4; i++) {
    rot->rotateZ(-deltaThetaRot[i - 1]);
    rot->rotateZ(deltaThetaRot[i]);
    subtracted.push_back(new G4SubtractionSolid(
        "cellSubtracted", subtracted[i - 1], solidCutcell, rot,
        G4ThreeVector(cos(deltaTheta[i]) * delta, sin(deltaTheta[i]) * delta,
                      0.)));
  }
  delete rot;

  return subtracted[3];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume *HexagonLogical(G4String aName, G4double aCellThickness,
                                G4double aCellSideLength,
                                G4Material *aMaterial) {
  return new G4LogicalVolume(
      HexagonSolid(aName, aCellThickness, aCellSideLength), // its solid
      aMaterial,                                            // its aMaterial
      aName);                                               // its aName
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineMaterials() {
  /***** Definition of all available materials *****/
  // Get nist aMaterial manager
  G4NistManager *nist = G4NistManager::Instance();
  fMatVacuum = nist->FindOrBuildMaterial("G4_Galactic");
  fMatAIR = nist->FindOrBuildMaterial("G4_AIR");
  fMatAr = nist->FindOrBuildMaterial("G4_Ar");
  fMatAl = nist->FindOrBuildMaterial("G4_Al");
  fMatFe = nist->FindOrBuildMaterial("G4_Fe");
  fMatGlass = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
  fMatPb = nist->FindOrBuildMaterial("G4_Pb");
  fMatCu = nist->FindOrBuildMaterial("G4_Cu");
  fMatW = nist->FindOrBuildMaterial("G4_W");
  fMatSi = nist->FindOrBuildMaterial("G4_Si");
  fMatAu = nist->FindOrBuildMaterial("G4_Au");
  fMatQuartz = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  fMatC = nist->FindOrBuildMaterial("G4_C");
  fMatH = nist->FindOrBuildMaterial("G4_H");
  fMatO = nist->FindOrBuildMaterial("G4_O");
  fMatMn = nist->FindOrBuildMaterial("G4_Mn");
  fMatCr = nist->FindOrBuildMaterial("G4_Cr");
  fMatNi = nist->FindOrBuildMaterial("G4_Ni");
  fMatPolyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  fMatCl = nist->FindOrBuildMaterial("G4_Cl");
  fMatF = nist->FindOrBuildMaterial("G4_F");

  // AHCAL SiPMs
  G4double a = 1.01 * g / mole;
  G4Element *elH = new G4Element("Hydrogen", "H2", 1., a);
  a = 12.01 * g / mole;
  G4Element *elC = new G4Element("Carbon", "C", 6., a);
  G4double density = 1.032 * g / cm3;
  fMatPolystyrene = new G4Material("Polystyrene", density, 2);
  fMatPolystyrene->AddElement(elC, 19);
  fMatPolystyrene->AddElement(elH, 21);

  // CuW alloy: 60% Cu, 40% W in mass
  G4double CuFracInCuW = 0.75;
  fMatCuW = new G4Material("CuW", 14.979 * g / cm3, 2);
  fMatCuW->AddMaterial(fMatCu, CuFracInCuW);
  fMatCuW->AddMaterial(fMatW, 1 - CuFracInCuW);

  // PCB aMaterial
  fMatPCB = new G4Material("PCB", 1.7 * g / cm3, 4);
  fMatPCB->AddMaterial(fMatC, 0.13232243);
  fMatPCB->AddMaterial(fMatH, 0.032572448);
  fMatPCB->AddMaterial(fMatO, 0.48316123);
  fMatPCB->AddMaterial(fMatSi, 0.35194389);

  // Kapton aMaterial
  fMatKAPTON = new G4Material("Kapton", 1.11 * g / cm3, 3);
  fMatKAPTON->AddMaterial(fMatC, 0.59985105);
  fMatKAPTON->AddMaterial(fMatH, 0.080541353);
  fMatKAPTON->AddMaterial(fMatO, 0.31960759);

  // steel
  fMatSteel = new G4Material("StainlessSteel", 8.02 * g / cm3, 5);
  fMatSteel->AddMaterial(fMatFe, 0.6996);
  fMatSteel->AddMaterial(fMatC, 0.0004);
  fMatSteel->AddMaterial(fMatMn, 0.01);
  fMatSteel->AddMaterial(fMatCr, 0.19);
  fMatSteel->AddMaterial(fMatNi, 0.1);

  // LYSO implementation from here:
  // https://github.com/cacunas/ParG4_cluster/blob/master/LYSO/src/LYSOGeometry.cc
  G4Element *Oxigen = nist->FindOrBuildElement("O");
  G4Element *Lutetium = nist->FindOrBuildElement("Lu");
  G4Element *Yttrium = nist->FindOrBuildElement("Y");
  // Lutetium Oxide Lu2O3
  G4Material *LutetiumOxide =
      new G4Material("LutetiumOxide", 9.41 * g / cm3, 2);
  LutetiumOxide->AddElement(Lutetium, 2);
  LutetiumOxide->AddElement(Oxigen, 3);
  // Yttrium Oxide Y2O3
  G4Material *YttriumOxide = new G4Material("YttriumOxide", 5.01 * g / cm3, 2);
  YttriumOxide->AddElement(Yttrium, 2);
  YttriumOxide->AddElement(Oxigen, 3);
  // Build LYSO aMaterial
  fMatLYSO = new G4Material("LYSO", 7.1 * g / cm3, 3);
  fMatLYSO->AddMaterial(LutetiumOxide, 81 * perCent);
  fMatLYSO->AddMaterial(fMatQuartz, 14 * perCent);
  fMatLYSO->AddMaterial(YttriumOxide, 5 * perCent);

  // Scintillator aMaterial
  fMatScintillator = new G4Material("Scintillator", 1.032 * g / cm3, 2);
  fMatScintillator->AddMaterial(fMatC, 0.91512109);
  fMatScintillator->AddMaterial(fMatH, 0.084878906);

  // DWC gas
  fMatArCO2 = new G4Material("ArCO2", 1.729 * mg / cm3, 3);
  fMatArCO2->AddMaterial(fMatAr, 0.475815);
  fMatArCO2->AddMaterial(fMatC, 0.14306133);
  fMatArCO2->AddMaterial(fMatO, 0.38112367);

  fMatFreon = new G4Material("Freon-12", 4.93 * mg / cm3, 3);
  fMatFreon->AddMaterial(fMatC, 0.099340816);
  fMatFreon->AddMaterial(fMatCl, 0.58640112);
  fMatFreon->AddMaterial(fMatF, 0.31425807);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::SetEventDisplayColorScheme() {
  G4VisAttributes *visAttributes;

  visAttributes = new G4VisAttributes(G4Colour(.0, 0.0, 0.0));
  visAttributes->SetVisibility(false);
  fSiWaferLogical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(.3, 0.3, 0.3, 0.2));
  visAttributes->SetVisibility(true);
  fSiPixelLogical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.01));
  visAttributes->SetVisibility(false);
  fCuWbaseplateLogical->SetVisAttributes(visAttributes);
  fCuWbaseplate550umLogical->SetVisAttributes(visAttributes);
  fCuWbaseplate610umLogical->SetVisAttributes(visAttributes);
  fCuWbaseplate710umLogical->SetVisAttributes(visAttributes);
  fCuBaseplateLogical->SetVisAttributes(visAttributes);
  fCuBaseplate25umLogical->SetVisAttributes(visAttributes);
  fCuBaseplate175umLogical->SetVisAttributes(visAttributes);
  fPCBbaseplateLogical->SetVisAttributes(visAttributes);
  fPCBbaseplateThinLogical->SetVisAttributes(visAttributes);
  fKaptonLayerLogical->SetVisAttributes(visAttributes);
  fAlCaseLogical->SetVisAttributes(visAttributes);
  fAlCaseThickLogical->SetVisAttributes(visAttributes);
  fSteelCaseLogical->SetVisAttributes(visAttributes);
  fSteelCaseThickLogical->SetVisAttributes(visAttributes);
  fPbAbsorberEElogical->SetVisAttributes(visAttributes);
  fFeAbsorberEElogical->SetVisAttributes(visAttributes);
  fCuAbsorberEElogical->SetVisAttributes(visAttributes);
  fWabsorberEElogical->SetVisAttributes(visAttributes);
  fW2mmAbsorberEEDESY2018Logical->SetVisAttributes(visAttributes);
  fW4mmAbsorberEEDESY2018Logical->SetVisAttributes(visAttributes);
  fCuAbsorberFHlogical->SetVisAttributes(visAttributes);
  fFeAbsorberFHlogical->SetVisAttributes(visAttributes);
  fAHCALSiPMlogical->SetVisAttributes(visAttributes);
  fAHCALSiPM2x2HUBlogical->SetVisAttributes(visAttributes);
  fAlAbsorberAHCALlogical->SetVisAttributes(visAttributes);
  fPCBAHCALlogical->SetVisAttributes(visAttributes);
  fFeAbsorberAHCALlogical->SetVisAttributes(visAttributes);
  fScintillatorLogical->SetVisAttributes(visAttributes);
  fScintillatorThinLogical->SetVisAttributes(visAttributes);
  fMCPlogical->SetVisAttributes(visAttributes);
  fDWClogical->SetVisAttributes(visAttributes);
  fDWCgasLogical->SetVisAttributes(visAttributes);
  fDATURAlogical->SetVisAttributes(visAttributes);
  fHERDcalorimeterLogical->SetVisAttributes(visAttributes);
  fHERDcalorimeterSlabLogical->SetVisAttributes(visAttributes);
  fAlChipLogical->SetVisAttributes(visAttributes);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HGCalTBMaterials::HGCalTBMaterials() {
  DefineMaterials();
  DefineSiWaferAndCells();
  DefineHGCalBaseplates();
  DefineHGCalCases();
  DefineHGCalEEAbsorbers();
  DefineHGCalFHAbsorbers();
  DefineAHCALSiPM();
  DefineAHCALAbsorbers();
  DefineBeamLineElements();
  DefineHERDCalorimeter();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineSiWaferAndCells() {
  /***** Definition of silicon (wafer) sensors *****/
  // 300 microns thickness only
  fSiPixelSideLength = 0.6496345 * cm;
  fSiWaferThickness = 0.3 * mm;
  fAlpha = 60. / 180. * CLHEP::pi;
  fSiWaferSideLength = 11 * fSiPixelSideLength;

  fSiWaferLogical = HexagonLogical("Si_wafer", fSiWaferThickness,
                                   fSiWaferSideLength, fMatAIR);

  // Silicon pixel setups
  double dx = 2 * sin(fAlpha) * fSiPixelSideLength;
  double dy = fSiPixelSideLength * (2. + 2 * cos(fAlpha));
  fSiPixelLogical =
      HexagonLogical("SiCell", fSiWaferThickness, fSiPixelSideLength, fMatSi);

  int index = 1;
  int nRows[11] = {7, 6, 7, 6, 5, 6, 5, 4, 5, 4, 3};
  for (int nC = 0; nC < 11; nC++) {
    for (int middle_index = 0; middle_index < nRows[nC]; middle_index++) {
      new G4PVPlacement(
          0,
          G4ThreeVector(dy * (middle_index - nRows[nC] / 2. + 0.5), nC * dx / 2,
                        0.),
          fSiPixelLogical, "SiCell", fSiWaferLogical, false, index++, true);
      if (nC <= 0)
        continue;
      new G4PVPlacement(
          0,
          G4ThreeVector(dy * (middle_index - nRows[nC] / 2. + 0.5),
                        -nC * dx / 2, 0.),
          fSiPixelLogical, "SiCell", fSiWaferLogical, false, index++, true);
    }
  }

  fThicknessMap["Si_wafer"] = fSiWaferThickness;
  fLogicalVolumeMap["Si_wafer"] = fSiWaferLogical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineHGCalBaseplates() {
  /***** Definition of all baseplates *****/
  // CuW
  G4double CuWbaseplateThickness = 1.2 * mm;
  G4double CuWbaseplatesideLength = 11 * fSiPixelSideLength;
  fCuWbaseplateLogical = HexagonLogical("CuW_baseplate", CuWbaseplateThickness,
                                        CuWbaseplatesideLength, fMatCuW);
  fThicknessMap["CuW_baseplate"] = CuWbaseplateThickness;
  fLogicalVolumeMap["CuW_baseplate"] = fCuWbaseplateLogical;
  G4double CuWbaseplate550umthickness = 0.55 * mm;
  fCuWbaseplate550umLogical =
      HexagonLogical("CuW_baseplate_550um", CuWbaseplate550umthickness,
                     CuWbaseplatesideLength, fMatCuW);
  fThicknessMap["CuW_baseplate_550um"] = CuWbaseplate550umthickness;
  fLogicalVolumeMap["CuW_baseplate_550um"] = fCuWbaseplate550umLogical;
  G4double CuWbaseplate610umThickness = 0.61 * mm;
  fCuWbaseplate610umLogical =
      HexagonLogical("CuW_baseplate_610um", CuWbaseplate610umThickness,
                     CuWbaseplatesideLength, fMatCuW);
  fThicknessMap["CuW_baseplate_610um"] = CuWbaseplate610umThickness;
  fLogicalVolumeMap["CuW_baseplate_610um"] = fCuWbaseplate610umLogical;
  G4double CuWbaseplate710umThickness = 0.71 * mm;
  fCuWbaseplate710umLogical =
      HexagonLogical("CuW_baseplate_710um", CuWbaseplate710umThickness,
                     CuWbaseplatesideLength, fMatCuW);
  fThicknessMap["CuW_baseplate_710um"] = CuWbaseplate710umThickness;
  fLogicalVolumeMap["CuW_baseplate_710um"] = fCuWbaseplate710umLogical;

  // Cu
  G4double CuBaseplateThickness = 1.2 * mm;
  G4double CuBaseplatesideLength = 11 * fSiPixelSideLength;
  fCuBaseplateLogical = HexagonLogical("Cu_baseplate", CuBaseplateThickness,
                                       CuBaseplatesideLength, fMatCu);
  fThicknessMap["Cu_baseplate"] = CuBaseplateThickness;
  fLogicalVolumeMap["Cu_baseplate"] = fCuBaseplateLogical;
  G4double CuBaseplate25umThickness = 0.025 * mm;
  fCuBaseplate25umLogical =
      HexagonLogical("Cu_baseplate_25um", CuBaseplate25umThickness,
                     CuBaseplatesideLength, fMatCu);
  fThicknessMap["Cu_baseplate_25um"] = CuBaseplate25umThickness;
  fLogicalVolumeMap["Cu_baseplate_25um"] = fCuBaseplate25umLogical;
  G4double CuBaseplate175umThickness = 0.175 * mm;
  fCuBaseplate175umLogical =
      HexagonLogical("Cu_baseplate_175um", CuBaseplate175umThickness,
                     CuBaseplatesideLength, fMatCu);
  fThicknessMap["Cu_baseplate_175um"] = CuBaseplate175umThickness;
  fLogicalVolumeMap["Cu_baseplate_175um"] = fCuBaseplate175umLogical;

  // PCB
  G4double PCBbaseplateThickness = 1.3 * mm;
  G4double PCBbaseplateSideLength = 11 * fSiPixelSideLength;
  fPCBbaseplateLogical = HexagonLogical("PCB", PCBbaseplateThickness,
                                        PCBbaseplateSideLength, fMatPCB);
  fThicknessMap["PCB"] = PCBbaseplateThickness;
  fLogicalVolumeMap["PCB"] = fPCBbaseplateLogical;

  G4double PCBbaseplateThinThickness = 1.2 * mm;
  fPCBbaseplateThinLogical = HexagonLogical(
      "PCB_thin", PCBbaseplateThinThickness, PCBbaseplateSideLength, fMatPCB);
  fThicknessMap["PCB_thin"] = PCBbaseplateThinThickness;
  fLogicalVolumeMap["PCB_thin"] = fPCBbaseplateThinLogical;

  // Kapton layer
  G4double KaptonLayerThickness = 0.075 * mm;
  G4double KaptonLayerSideLength = 11 * fSiPixelSideLength;
  fKaptonLayerLogical = HexagonLogical("Kapton_layer", KaptonLayerThickness,
                                       KaptonLayerSideLength, fMatKAPTON);
  fThicknessMap["Kapton_layer"] = KaptonLayerThickness;
  fLogicalVolumeMap["Kapton_layer"] = fKaptonLayerLogical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineHGCalCases() {
  G4double AlCaseThickness = 2.1 * mm;
  G4double AlCaseXY = 40 * cm;
  G4Box *AlCaseSolid = new G4Box("Al_case", 0.5 * AlCaseXY, 0.5 * AlCaseXY,
                                 0.5 * AlCaseThickness);
  fAlCaseLogical = new G4LogicalVolume(AlCaseSolid, fMatAl, "Al_case");
  fThicknessMap["Al_case"] = AlCaseThickness;
  fLogicalVolumeMap["Al_case"] = fAlCaseLogical;

  G4double AlCaseThickThickness = 5 * mm;
  G4Box *AlCaseThickSolid =
      new G4Box("Al_case_thick", 0.5 * AlCaseXY, 0.5 * AlCaseXY,
                0.5 * AlCaseThickThickness);
  fAlCaseThickLogical =
      new G4LogicalVolume(AlCaseThickSolid, fMatAl, "Al_case_thick");
  fThicknessMap["Al_case_thick"] = AlCaseThickThickness;
  fLogicalVolumeMap["Al_case_thick"] = fAlCaseThickLogical;

  G4double SteelCaseThickness = 9 * mm;
  G4double SteelCaseXY = 60 * cm;
  G4Box *SteelCaseSolid =
      new G4Box("Steel_case", 0.5 * SteelCaseXY, 0.5 * SteelCaseXY,
                0.5 * SteelCaseThickness);
  fSteelCaseLogical = new G4LogicalVolume(SteelCaseSolid, fMatFe, "Steel_case");
  fThicknessMap["Steel_case"] = SteelCaseThickness;
  fLogicalVolumeMap["Steel_case"] = fSteelCaseLogical;

  G4double SteelCaseThickThickness = 40 * mm;
  G4Box *SteelCaseThickSolid =
      new G4Box("Steel_case_thick", 0.5 * SteelCaseXY, 0.5 * SteelCaseXY,
                0.5 * SteelCaseThickThickness);
  fSteelCaseThickLogical =
      new G4LogicalVolume(SteelCaseThickSolid, fMatFe, "Steel_case_thick");
  fThicknessMap["Steel_case_thick"] = SteelCaseThickThickness;
  fLogicalVolumeMap["Steel_case_thick"] = fSteelCaseThickLogical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineHGCalEEAbsorbers() {
  // defintion of absorber plates in the EE part
  G4double PbAbsorberEEthickness = 4.9 * mm;
  G4double PbAbsorberEExy = 30 * cm;
  G4Box *PbAbsorberEEsolid =
      new G4Box("Pb_absorber_EE", 0.5 * PbAbsorberEExy, 0.5 * PbAbsorberEExy,
                0.5 * PbAbsorberEEthickness);
  fPbAbsorberEElogical =
      new G4LogicalVolume(PbAbsorberEEsolid, fMatPb, "Pb_absorber_EE");
  fThicknessMap["Pb_absorber_EE"] = PbAbsorberEEthickness;
  fLogicalVolumeMap["Pb_absorber_EE"] = fPbAbsorberEElogical;

  G4double FeAbsorberEEthickness = 0.3 * mm;
  G4double FeAbsorberEExy = 30 * cm;
  G4Box *FeAbsorberEEsolid =
      new G4Box("Fe_absorber_EE", 0.5 * FeAbsorberEExy, 0.5 * FeAbsorberEExy,
                0.5 * FeAbsorberEEthickness);
  fFeAbsorberEElogical =
      new G4LogicalVolume(FeAbsorberEEsolid, fMatFe, "Fe_absorber_EE");
  fThicknessMap["Fe_absorber_EE"] = FeAbsorberEEthickness;
  fLogicalVolumeMap["Fe_absorber_EE"] = fFeAbsorberEElogical;

  G4double CuAbsorberEEthickness = 6 * mm;
  G4double CuAbsorberEExy = 30 * cm;
  G4Box *CuAbsorberEEsolid =
      new G4Box("Cu_absorber_EE", 0.5 * CuAbsorberEExy, 0.5 * CuAbsorberEExy,
                0.5 * CuAbsorberEEthickness);
  fCuAbsorberEElogical =
      new G4LogicalVolume(CuAbsorberEEsolid, fMatCu, "Cu_absorber_EE");
  fThicknessMap["Cu_absorber_EE"] = CuAbsorberEEthickness;
  fLogicalVolumeMap["Cu_absorber_EE"] = fCuAbsorberEElogical;

  G4double WabsorberEEthickness = 2.8 * mm;
  G4double WabsorberEExy = 30 * cm;
  G4Box *WabsorberEEsolid =
      new G4Box("W_absorber_EE", 0.5 * WabsorberEExy, 0.5 * WabsorberEExy,
                0.5 * WabsorberEEthickness);
  fWabsorberEElogical =
      new G4LogicalVolume(WabsorberEEsolid, fMatW, "W_absorber_EE");
  fThicknessMap["W_absorber_EE"] = WabsorberEEthickness;
  fLogicalVolumeMap["W_absorber_EE"] = fWabsorberEElogical;

  G4double W2mmAbsorberEEDESY2018thickness = 2. * mm;
  G4double W2mmAbsorberEEDESY2018xy = 15 * cm;
  G4Box *W2mmAbsorberEEDESY2018solid = new G4Box(
      "W_2mm_absorber_EE_DESY2018", 0.5 * W2mmAbsorberEEDESY2018xy,
      0.5 * W2mmAbsorberEEDESY2018xy, 0.5 * W2mmAbsorberEEDESY2018thickness);
  fW2mmAbsorberEEDESY2018Logical = new G4LogicalVolume(
      W2mmAbsorberEEDESY2018solid, fMatW, "W_2mm_absorber_EE_DESY2018");
  fThicknessMap["W_2mm_absorber_EE_DESY2018"] = W2mmAbsorberEEDESY2018thickness;
  fLogicalVolumeMap["W_2mm_absorber_EE_DESY2018"] =
      fW2mmAbsorberEEDESY2018Logical;

  G4double W4mmAbsorberEEDESY2018thickness = 4. * mm;
  G4double W4mmAbsorberEEDESY2018xy = 15 * cm;
  G4Box *W4mmAbsorberEEDESY2018solid = new G4Box(
      "W_4mm_absorber_EE_DESY2018", 0.5 * W4mmAbsorberEEDESY2018xy,
      0.5 * W4mmAbsorberEEDESY2018xy, 0.5 * W4mmAbsorberEEDESY2018thickness);
  fW4mmAbsorberEEDESY2018Logical = new G4LogicalVolume(
      W4mmAbsorberEEDESY2018solid, fMatW, "W_4mm_absorber_EE_DESY2018");
  fThicknessMap["W_4mm_absorber_EE_DESY2018"] = W4mmAbsorberEEDESY2018thickness;
  fLogicalVolumeMap["W_4mm_absorber_EE_DESY2018"] =
      fW4mmAbsorberEEDESY2018Logical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineHGCalFHAbsorbers() {
  // defintion of absorber plates in the FH part
  G4double CuAbsorberFHthickness = 6 * mm;
  G4double CuAbsorberFHxy = 50 * cm;
  G4Box *CuAbsorberFHsolid =
      new G4Box("Cu_absorber_FH", 0.5 * CuAbsorberFHxy, 0.5 * CuAbsorberFHxy,
                0.5 * CuAbsorberFHthickness);
  fCuAbsorberFHlogical =
      new G4LogicalVolume(CuAbsorberFHsolid, fMatCu, "Cu_absorber_FH");
  fThicknessMap["Cu_absorber_FH"] = CuAbsorberFHthickness;
  fLogicalVolumeMap["Cu_absorber_FH"] = fCuAbsorberFHlogical;

  G4double FeAbsorberFHthickness = 40 * mm;
  G4double FeAbsorberFHxy = 50 * cm;
  G4Box *FeAbsorberFHsolid =
      new G4Box("Fe_absorber_FH", 0.5 * FeAbsorberFHxy, 0.5 * FeAbsorberFHxy,
                0.5 * FeAbsorberFHthickness);
  fFeAbsorberFHlogical =
      new G4LogicalVolume(FeAbsorberFHsolid, fMatFe, "Fe_absorber_FH");
  fThicknessMap["Fe_absorber_FH"] = FeAbsorberFHthickness;
  fLogicalVolumeMap["Fe_absorber_FH"] = fFeAbsorberFHlogical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineAHCALSiPM() {
  G4double AHCALSiPMthickness = 5.4 * mm;
  fAHCALSiPMxy = 3 * cm;
  fAHCALSiPMsolid = new G4Box("AHCAL_SiPM", 0.5 * fAHCALSiPMxy,
                              0.5 * fAHCALSiPMxy, 0.5 * AHCALSiPMthickness);
  fAHCALSiPMlogical =
      new G4LogicalVolume(fAHCALSiPMsolid, fMatPolystyrene, "AHCAL_SiPM");
  fThicknessMap["AHCAL_SiPM"] = AHCALSiPMthickness;
  fLogicalVolumeMap["AHCAL_SiPM"] = fAHCALSiPMlogical;

  G4double AHCALSiPM2x2HUBxy = 2 * 12 * fAHCALSiPMxy + 0.01 * mm;
  G4double AHCALSiPM2x2HUBthickness = AHCALSiPMthickness + 0.01 * mm;
  G4Box *AHCALSiPM2x2HUBsolid =
      new G4Box("AHCAL_SiPM_2x2HUB", 0.5 * AHCALSiPM2x2HUBxy,
                0.5 * AHCALSiPM2x2HUBxy, 0.5 * AHCALSiPM2x2HUBthickness);
  fAHCALSiPM2x2HUBlogical =
      new G4LogicalVolume(AHCALSiPM2x2HUBsolid, fMatAIR, "AHCAL_SiPM_2x2HUB");
  fThicknessMap["AHCAL_SiPM_2x2HUB"] = AHCALSiPM2x2HUBthickness;
  fLogicalVolumeMap["AHCAL_SiPM_2x2HUB"] = fAHCALSiPM2x2HUBlogical;
  int copy_counter = 0;
  for (float _dx = -11.5; _dx <= 11.5; _dx = _dx + 1.)
    for (float _dy = -11.5; _dy <= 11.5; _dy = _dy + 1.)
      new G4PVPlacement(
          0, G4ThreeVector(_dx * fAHCALSiPMxy, _dy * fAHCALSiPMxy, 0),
          fAHCALSiPMlogical, "AHCAL_SiPM", fAHCALSiPM2x2HUBlogical, false,
          copy_counter++, true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineAHCALAbsorbers() {
  G4double AlAbsorberAHCALthickness = 1 * mm;
  G4double AlAbsorberAHCALxy = 2 * 12 * fAHCALSiPMxy;
  G4Box *AlAbsorberAHCALsolid =
      new G4Box("Al_absorber_AHCAL", 0.5 * AlAbsorberAHCALxy,
                0.5 * AlAbsorberAHCALxy, 0.5 * AlAbsorberAHCALthickness);
  fAlAbsorberAHCALlogical =
      new G4LogicalVolume(AlAbsorberAHCALsolid, fMatAl, "Al_absorber_AHCAL");
  fThicknessMap["Al_absorber_AHCAL"] = AlAbsorberAHCALthickness;
  fLogicalVolumeMap["Al_absorber_AHCAL"] = fAlAbsorberAHCALlogical;

  G4double PCBAHCALthickness = 1.2 * mm;
  G4double PCBAHCALxy = 2 * 12 * fAHCALSiPMxy;
  G4Box *PCBAHCALsolid = new G4Box("PCB_AHCAL", 0.5 * PCBAHCALxy,
                                   0.5 * PCBAHCALxy, 0.5 * PCBAHCALthickness);
  fPCBAHCALlogical = new G4LogicalVolume(PCBAHCALsolid, fMatPCB, "PCB_AHCAL");
  fThicknessMap["PCB_AHCAL"] = PCBAHCALthickness;
  fLogicalVolumeMap["PCB_AHCAL"] = fPCBAHCALlogical;

  G4double FeAbsorberAHCALthickness = 17 * mm;
  G4double FeAbsorberAHCALx = 80.8 * cm;
  G4double FeAbsorberAHCALy = 65.7 * cm;
  G4Box *FeAbsorberAHCALsolid =
      new G4Box("Fe_absorber_AHCAL", 0.5 * FeAbsorberAHCALx,
                0.5 * FeAbsorberAHCALy, 0.5 * FeAbsorberAHCALthickness);
  fFeAbsorberAHCALlogical =
      new G4LogicalVolume(FeAbsorberAHCALsolid, fMatFe, "Fe_absorber_AHCAL");
  fThicknessMap["Fe_absorber_AHCAL"] = FeAbsorberAHCALthickness;
  fLogicalVolumeMap["Fe_absorber_AHCAL"] = fFeAbsorberAHCALlogical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineBeamLineElements() {
  /***** Definition of beam line elements *****/
  // scintillators
  G4double scintillatorThickness = 2 * 2 * cm;
  G4double scintillatorXY = 2 * 9.5 * cm;
  G4Box *scintillatorSolid =
      new G4Box("Scintillator", 0.5 * scintillatorXY, 0.5 * scintillatorXY,
                0.5 * scintillatorThickness);
  fScintillatorLogical =
      new G4LogicalVolume(scintillatorSolid, fMatScintillator, "Scintillator");
  fThicknessMap["Scintillator"] = scintillatorThickness;
  fLogicalVolumeMap["Scintillator"] = fScintillatorLogical;

  G4double fScintillatorThinThickness = 1 * cm;
  G4Box *fScintillatorThinSolid =
      new G4Box("Scintillator_thin", 0.5 * scintillatorXY, 0.5 * scintillatorXY,
                0.5 * fScintillatorThinThickness);
  fScintillatorThinLogical = new G4LogicalVolume(
      fScintillatorThinSolid, fMatScintillator, "Scintillator_thin");
  fThicknessMap["Scintillator_thin"] = fScintillatorThinThickness;
  fLogicalVolumeMap["Scintillator_thin"] = fScintillatorThinLogical;

  // MCPs = quartz disks
  G4double MCPthickness = 10 * mm;
  G4double MCPradius = 2 * cm;
  G4Tubs *MCPsolid =
      new G4Tubs("MCP", 0., MCPradius, MCPthickness, 0, 360 * degree);
  fMCPlogical = new G4LogicalVolume(MCPsolid, fMatQuartz, "MCP");
  fThicknessMap["MCP"] = MCPthickness;
  fLogicalVolumeMap["MCP"] = fMCPlogical;

  // CK3
  G4double CK3thickness = 2 * m;
  G4double CK3radius = 8.35 * cm;
  G4Tubs *CK3solid =
      new G4Tubs("CK3", 0., CK3radius, 0.5 * CK3thickness, 0, 360 * degree);
  fCK3logical = new G4LogicalVolume(CK3solid, fMatFreon, "CK3");
  fThicknessMap["CK3"] = CK3thickness;
  fLogicalVolumeMap["CK3"] = fCK3logical;

  // Aluminium circle for testing of chip impact
  G4double AlChipXY = 1 * cm;
  G4double AlChipThickness = 0.89 * mm; // corresponds to 1% X0
  G4Box *AlChipSolid = new G4Box("Al_chip", 0.5 * AlChipXY, 0.5 * AlChipXY,
                                 0.5 * AlChipThickness);
  fAlChipLogical = new G4LogicalVolume(AlChipSolid, fMatAl, "Al_chip");
  fThicknessMap["Al_chip"] = AlChipThickness;
  fLogicalVolumeMap["Al_chip"] = fAlChipLogical;

  // DWC related aMaterial
  G4double DWCthickness = 2 * 27.5 * mm;
  G4double DWCxy = 2 * 11 * cm;
  G4Box *DWCsolid =
      new G4Box("DWC", 0.5 * DWCxy, 0.5 * DWCxy, 0.5 * DWCthickness);
  fDWClogical = new G4LogicalVolume(DWCsolid, fMatAIR, "DWC");
  fThicknessMap["DWC"] = DWCthickness;
  fLogicalVolumeMap["DWC"] = fDWClogical;

  // WChambGas
  G4double DWCgasThickness = 2 * 22.5 * mm;
  G4double DWCgasXY = 2 * 8.5 * cm;
  G4Box *DWCgasSolid = new G4Box("DWC_gas", 0.5 * DWCgasXY, 0.5 * DWCgasXY,
                                 0.5 * DWCgasThickness);
  fDWCgasLogical = new G4LogicalVolume(DWCgasSolid, fMatArCO2, "DWC_gas");
  new G4PVPlacement(0, G4ThreeVector(0, 0., 0.), fDWCgasLogical, "DWC_gas",
                    fDWClogical, false, 0, true);

  // WChambWindow
  G4double DWCwindowThickness = 0.025 * mm;
  G4double DWCwindowXY = 2 * 5.5 * cm;
  G4Box *DWCwindowSolid =
      new G4Box("DWC_window", 0.5 * DWCwindowXY, 0.5 * DWCwindowXY,
                0.5 * DWCwindowThickness);
  auto DWCwindowLogical =
      new G4LogicalVolume(DWCwindowSolid, fMatKAPTON, "DWC_window");
  new G4PVPlacement(
      0, G4ThreeVector(0, 0., 27.5 * mm - DWCwindowThickness / 2.),
      DWCwindowLogical, "DWC_window_0", fDWClogical, true, 0, true);
  new G4PVPlacement(
      0, G4ThreeVector(0, 0., -(27.5 * mm - DWCwindowThickness / 2.)),
      DWCwindowLogical, "DWC_window_1", fDWClogical, true, 1, true);

  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(90 * deg);

  // WChambAl1
  G4double DWCal1thickness = 2 * 2.5 * mm;
  G4double DWCal1x = 2 * 8.25 * cm;
  G4double DWCal1y = 2 * 2.75 * cm;
  G4Box *DWCal1solid =
      new G4Box("DWC_al1", 0.5 * DWCal1x, 0.5 * DWCal1y, 0.5 * DWCal1thickness);
  auto DWCal1logical = new G4LogicalVolume(DWCal1solid, fMatAl, "DWC_al1");
  new G4PVPlacement(0,
                    G4ThreeVector(0.5 * DWCal1y, 0.5 * DWCal1x,
                                  (0.5 * DWCthickness - 0.5 * DWCal1thickness)),
                    DWCal1logical, "DWC_al1_0", fDWClogical, true, 0, true);
  new G4PVPlacement(rotation,
                    G4ThreeVector(-0.5 * DWCal1x, 0.5 * DWCal1y,
                                  (0.5 * DWCthickness - 0.5 * DWCal1thickness)),
                    DWCal1logical, "DWC_al1_1", fDWClogical, true, 1, true);
  new G4PVPlacement(0,
                    G4ThreeVector(-0.5 * DWCal1y, -0.5 * DWCal1x,
                                  (0.5 * DWCthickness - 0.5 * DWCal1thickness)),
                    DWCal1logical, "DWC_al1_2", fDWClogical, true, 2, true);
  new G4PVPlacement(rotation,
                    G4ThreeVector(0.5 * DWCal1x, -0.5 * DWCal1y,
                                  (0.5 * DWCthickness - 0.5 * DWCal1thickness)),
                    DWCal1logical, "DWC_al1_3", fDWClogical, true, 3, true);
  new G4PVPlacement(
      0,
      G4ThreeVector(0.5 * DWCal1y, 0.5 * DWCal1x,
                    -(0.5 * DWCthickness - 0.5 * DWCal1thickness)),
      DWCal1logical, "DWC_al1_4", fDWClogical, true, 4, true);
  new G4PVPlacement(
      rotation,
      G4ThreeVector(-0.5 * DWCal1x, 0.5 * DWCal1y,
                    -(0.5 * DWCthickness - 0.5 * DWCal1thickness)),
      DWCal1logical, "DWC_al1_5", fDWClogical, true, 5, true);
  new G4PVPlacement(
      0,
      G4ThreeVector(-0.5 * DWCal1y, -0.5 * DWCal1x,
                    -(0.5 * DWCthickness - 0.5 * DWCal1thickness)),
      DWCal1logical, "DWC_al1_6", fDWClogical, true, 6, true);
  new G4PVPlacement(
      rotation,
      G4ThreeVector(0.5 * DWCal1x, -0.5 * DWCal1y,
                    -(0.5 * DWCthickness - 0.5 * DWCal1thickness)),
      DWCal1logical, "DWC_al1_7", fDWClogical, true, 7, true);

  // WChambAl2
  G4double DWCal2thickness = 2 * 2.25 * cm;
  G4double DWCal2x = 2 * 10.75 * cm;
  G4double DWCal2y = 2 * 2.5 * mm;
  G4Box *DWCal2solid =
      new G4Box("DWC_al2", 0.5 * DWCal2x, 0.5 * DWCal2y, 0.5 * DWCal2thickness);
  auto DWCal2logical = new G4LogicalVolume(DWCal2solid, fMatAl, "DWC_al2");
  new G4PVPlacement(0, G4ThreeVector(0.5 * DWCal2y, 0.5 * DWCal2x, 0),
                    DWCal2logical, "DWC_al2_0", fDWClogical, true, 0, true);
  new G4PVPlacement(rotation, G4ThreeVector(-0.5 * DWCal2x, 0.5 * DWCal2y, 0),
                    DWCal2logical, "DWC_al2_1", fDWClogical, true, 1, true);
  new G4PVPlacement(0, G4ThreeVector(-0.5 * DWCal2y, -0.5 * DWCal2x, 0),
                    DWCal2logical, "DWC_al2_2", fDWClogical, true, 2, true);
  new G4PVPlacement(rotation, G4ThreeVector(0.5 * DWCal2x, -0.5 * DWCal2y, 0),
                    DWCal2logical, "DWC_al2_3", fDWClogical, true, 3, true);

  // WChambGasVet
  G4double DWCgasVetThickness = 2 * 2.5 * mm;
  G4double DWCgasVetX = 2 * 7.25 * cm;
  G4double DWCgasVetY = 2 * 1.25 * cm;
  G4Box *DWCgasVetSolid = new G4Box("DWC_gasVet", 0.5 * DWCgasVetX,
                                    0.5 * DWCgasVetY, 0.5 * DWCgasVetThickness);
  auto DWCgasVetLogical =
      new G4LogicalVolume(DWCgasVetSolid, fMatPolyethylene, "DWC_gasVet");
  new G4PVPlacement(0,
                    G4ThreeVector(0.5 * DWCgasVetY, 0.5 * DWCgasVetX,
                                  0.5 * (DWCgasThickness - DWCgasVetThickness)),
                    DWCgasVetLogical, "DWC_gasVet_0", fDWCgasLogical, true, 0,
                    true);
  new G4PVPlacement(rotation,
                    G4ThreeVector(-0.5 * DWCgasVetX, 0.5 * DWCgasVetY,
                                  0.5 * (DWCgasThickness - DWCgasVetThickness)),
                    DWCgasVetLogical, "DWC_gasVet_1", fDWCgasLogical, true, 1,
                    true);
  new G4PVPlacement(0,
                    G4ThreeVector(-0.5 * DWCgasVetY, -0.5 * DWCgasVetX,
                                  0.5 * (DWCgasThickness - DWCgasVetThickness)),
                    DWCgasVetLogical, "DWC_gasVet_2", fDWCgasLogical, true, 2,
                    true);
  new G4PVPlacement(rotation,
                    G4ThreeVector(0.5 * DWCgasVetX, -0.5 * DWCgasVetY,
                                  0.5 * (DWCgasThickness - DWCgasVetThickness)),
                    DWCgasVetLogical, "DWC_gasVet_3", fDWCgasLogical, true, 3,
                    true);
  new G4PVPlacement(0, G4ThreeVector(0.5 * DWCgasVetY, 0.5 * DWCgasVetX, 0),
                    DWCgasVetLogical, "DWC_gasVet_4", fDWCgasLogical, true, 4,
                    true);
  new G4PVPlacement(
      rotation, G4ThreeVector(-0.5 * DWCgasVetX, 0.5 * DWCgasVetY, 0),
      DWCgasVetLogical, "DWC_gasVet_5", fDWCgasLogical, true, 5, true);
  new G4PVPlacement(0, G4ThreeVector(-0.5 * DWCgasVetY, -0.5 * DWCgasVetX, 0),
                    DWCgasVetLogical, "DWC_gasVet_6", fDWCgasLogical, true, 6,
                    true);
  new G4PVPlacement(
      rotation, G4ThreeVector(0.5 * DWCgasVetX, -0.5 * DWCgasVetY, 0),
      DWCgasVetLogical, "DWC_gasVet_7", fDWCgasLogical, true, 7, true);
  new G4PVPlacement(
      0,
      G4ThreeVector(0.5 * DWCgasVetY, 0.5 * DWCgasVetX,
                    -0.5 * (DWCgasThickness - DWCgasVetThickness)),
      DWCgasVetLogical, "DWC_gasVet_8", fDWCgasLogical, true, 8, true);
  new G4PVPlacement(
      rotation,
      G4ThreeVector(-0.5 * DWCgasVetX, 0.5 * DWCgasVetY,
                    -0.5 * (DWCgasThickness - DWCgasVetThickness)),
      DWCgasVetLogical, "DWC_gasVet_9", fDWCgasLogical, true, 9, true);
  new G4PVPlacement(
      0,
      G4ThreeVector(-0.5 * DWCgasVetY, -0.5 * DWCgasVetX,
                    -0.5 * (DWCgasThickness - DWCgasVetThickness)),
      DWCgasVetLogical, "DWC_gasVet_10", fDWCgasLogical, true, 10, true);
  new G4PVPlacement(
      rotation,
      G4ThreeVector(0.5 * DWCgasVetX, -0.5 * DWCgasVetY,
                    -0.5 * (DWCgasThickness - DWCgasVetThickness)),
      DWCgasVetLogical, "DWC_gasVet_11", fDWCgasLogical, true, 11, true);

  // DESY DATURA beam telescope
  G4double DATURAthickness = 15 * mm;
  G4double DATURAxy = 10 * cm;
  G4double DATURAwindowX = 2 * cm;
  G4double DATURAwindowY = 1 * cm;
  G4Box *DATURAfullSolid = new G4Box("DATURA_full", 0.5 * DATURAxy,
                                     0.5 * DATURAxy, 0.5 * DATURAthickness);
  G4Box *DATURAcutSolid = new G4Box("DATURA_cut", 0.5 * DATURAwindowX,
                                    0.5 * DATURAwindowY, 0.5 * DATURAthickness);
  G4SubtractionSolid *DATURAsolid = new G4SubtractionSolid(
      "DATURA", DATURAfullSolid, DATURAcutSolid, 0, G4ThreeVector(0, 0, 0.));
  fDATURAlogical = new G4LogicalVolume(DATURAsolid, fMatAl, "DATURA");
  fThicknessMap["DATURA"] = DATURAthickness;
  fLogicalVolumeMap["DATURA"] = fDATURAlogical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::DefineHERDCalorimeter() {

  G4double HERDcalorimeterThickness = 3 * cm;
  G4double HERDcalorimeterXY = 3 * cm;
  G4Box *HERDcalorimeterSolid =
      new G4Box("HERD_calorimeter", 0.5 * HERDcalorimeterXY,
                0.5 * HERDcalorimeterXY, 0.5 * HERDcalorimeterThickness);
  fHERDcalorimeterLogical =
      new G4LogicalVolume(HERDcalorimeterSolid, fMatLYSO, "HERD_calorimeter");
  fThicknessMap["HERD_calorimeter"] = HERDcalorimeterThickness;
  fLogicalVolumeMap["HERD_calorimeter"] = fHERDcalorimeterLogical;

  G4double HERDcalorimeterSlabXY = 21 * HERDcalorimeterXY + 0.01 * mm;
  G4double HERDcalorimeterSlabThickness =
      HERDcalorimeterThickness + 0.0001 * mm;
  G4Box *HERDcalorimeterSlabSolid = new G4Box(
      "HERD_calorimeter_slab", 0.5 * HERDcalorimeterSlabXY,
      0.5 * HERDcalorimeterSlabXY, 0.5 * HERDcalorimeterSlabThickness);
  fHERDcalorimeterSlabLogical = new G4LogicalVolume(
      HERDcalorimeterSlabSolid, fMatAIR, "HERD_calorimeter_slab");
  fThicknessMap["HERD_calorimeter_slab"] = HERDcalorimeterSlabThickness;
  fLogicalVolumeMap["HERD_calorimeter_slab"] = fHERDcalorimeterSlabLogical;
  int copy_counter = 0;
  for (float _dx = -10.0; _dx <= 10.0; _dx = _dx + 1.)
    for (float _dy = -10.0; _dy <= 10.0; _dy = _dy + 1.) {
      new G4PVPlacement(
          0, G4ThreeVector(_dx * HERDcalorimeterXY, _dy * HERDcalorimeterXY, 0),
          fHERDcalorimeterLogical, "HERD_calorimeter_slab",
          fHERDcalorimeterSlabLogical, false, copy_counter++, true);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HGCalTBMaterials::PlaceItemInLogicalVolume(std::string aName,
                                                G4double &aZ0,
                                                G4LogicalVolume *aLogicMother) {
  if (aName.find("_DAISY") != std::string::npos) {
    aName.resize(aName.find("_DAISY"));
    if (fCopyCounterMap.find(aName) == fCopyCounterMap.end())
      fCopyCounterMap[aName] = 0;
    double dx_ = 2 * sin(fAlpha) * 11 * fSiPixelSideLength;
    double dy_ = 11 * fSiPixelSideLength * (2. + 2 * cos(fAlpha));
    int nRows_[3] = {1, 2, 1};
    for (int nC = 0; nC < 3; nC++) {
      for (int middle_index = 0; middle_index < nRows_[nC]; middle_index++) {
        new G4PVPlacement(
            0,
            G4ThreeVector(dy_ * (middle_index - nRows_[nC] / 2. + 0.5),
                          -nC * dx_ / 2, aZ0 + 0.5 * fThicknessMap[aName]),
            fLogicalVolumeMap[aName], aName, aLogicMother, false,
            fCopyCounterMap[aName]++, true);
        if (nC <= 0)
          continue;
        new G4PVPlacement(
            0,
            G4ThreeVector(dy_ * (middle_index - nRows_[nC] / 2. + 0.5),
                          +nC * dx_ / 2, aZ0 + 0.5 * fThicknessMap[aName]),
            fLogicalVolumeMap[aName], aName, aLogicMother, false,
            fCopyCounterMap[aName]++, true);
      }
    }
    aZ0 += fThicknessMap[aName];
  } else if (aName.find("_SUMMER2017TRIPLET") != std::string::npos) {
    aName.resize(aName.find("_SUMMER2017TRIPLET"));
    if (fCopyCounterMap.find(aName) == fCopyCounterMap.end())
      fCopyCounterMap[aName] = 0;
    double dx_ = 2 * sin(fAlpha) * 11 * fSiPixelSideLength;
    double dy_ = 11 * fSiPixelSideLength * (2. + 2 * cos(fAlpha));
    int nRows_[2] = {1, 2};
    for (int nC = 0; nC < 2; nC++) {
      new G4PVPlacement(0,
                        G4ThreeVector(-dy_ * (0 - nRows_[nC] / 2. + 0.5),
                                      -nC * dx_ / 2,
                                      aZ0 + 0.5 * fThicknessMap[aName]),
                        fLogicalVolumeMap[aName], aName, aLogicMother, false,
                        fCopyCounterMap[aName]++, true);
      if (nC <= 0)
        continue;
      new G4PVPlacement(0,
                        G4ThreeVector(-dy_ * (0 - nRows_[nC] / 2. + 0.5),
                                      +nC * dx_ / 2,
                                      aZ0 + 0.5 * fThicknessMap[aName]),
                        fLogicalVolumeMap[aName], aName, aLogicMother, false,
                        fCopyCounterMap[aName]++, true);
    }
    aZ0 += fThicknessMap[aName];
  } else if (aName.find("_rot30") != std::string::npos) {
    aName.resize(aName.find("_rot30"));
    if (fCopyCounterMap.find(aName) == fCopyCounterMap.end())
      fCopyCounterMap[aName] = 0;
    G4RotationMatrix *rot = new G4RotationMatrix;
    rot->rotateZ(30 * deg);
    new G4PVPlacement(rot,
                      G4ThreeVector(0, 0, aZ0 + 0.5 * fThicknessMap[aName]),
                      fLogicalVolumeMap[aName], aName, aLogicMother, false,
                      fCopyCounterMap[aName]++, true);
    aZ0 += fThicknessMap[aName];
  } else {
    if (fCopyCounterMap.find(aName) == fCopyCounterMap.end())
      fCopyCounterMap[aName] = 0;
    new G4PVPlacement(0, G4ThreeVector(0, 0, aZ0 + 0.5 * fThicknessMap[aName]),
                      fLogicalVolumeMap[aName], aName, aLogicMother, false,
                      fCopyCounterMap[aName]++, true);
    aZ0 += fThicknessMap[aName];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......