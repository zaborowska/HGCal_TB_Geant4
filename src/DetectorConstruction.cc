#include "DetectorConstruction.hh"

#include "HGCalTBMaterials.hh"
#include "SiPMSD.hh"
#include "SiliconPixelSD.hh"
#include "config22_October2018_1.hh"
#include "test_configs.hh"

#include "G4Box.hh"
#include "G4GenericMessenger.hh"
#include "G4LogicalVolume.hh"
#include "G4ProductionCuts.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"

#ifdef MATSCAN
#include <fstream>
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), fConfiguration(-1) {
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // definition of the fMaterials
  fMaterials = new HGCalTBMaterials();
  fMaterials->SetEventDisplayColorScheme();

  /***** Definition of the world = beam line *****/

  // World = Beam line
  G4Box *solidWorld = new G4Box("World", 0.5 * fMaterials->GetBeamLineXY(),
                                0.5 * fMaterials->GetBeamLineXY(),
                                0.5 * fMaterials->GetBeamLineLength());

  G4Material *world_mat = fMaterials->GetAir();
  fLogicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");

  G4VPhysicalVolume *physWorld = new G4PVPlacement(
      0, G4ThreeVector(0., 0., 0.), fLogicWorld, "World", 0, false, 0, true);

  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructHGCal() {

  G4double z0 = -fMaterials->GetBeamLineLength() / 2.;

  std::cout << "Constructing configuration " << fConfiguration << std::endl;

  /*****    START GENERIC PLACEMENT ALGORITHM  FOR THE SETUP  *****/
  for (size_t item_index = 0; item_index < fElementsMap.size(); item_index++) {
    std::string item_type = fElementsMap[item_index].first;
    G4double dz = fElementsMap[item_index].second;
    z0 += dz;

    // places the item at inside the world at z0, z0 is incremented by the
    // item's thickness
    fMaterials->PlaceItemInLogicalVolume(item_type, z0, fLogicWorld);
  }

#ifdef MATSCAN
  std::ofstream myfile;
  myfile.open("replicate_mat_EEplusFH.txt");
  for (const auto &item : fElementsMap) {
    auto itemName = item.first;
    if (itemName.find("_DAISY") != std::string::npos)
      itemName.resize(itemName.find("_DAISY"));
    auto matName =
        fMaterials->GetLogicalVolume(itemName)->GetMaterial()->GetName();
    std::size_t found = matName.find("G4_");
    if (found != std::string::npos)
      matName.erase(found, found + 3);
    // special treatment for Si (3 x 0.1 mm)
    if (fMaterials->GetLogicalVolume(itemName)->GetName().find("Si_wafer") !=
        std::string::npos) {
      matName = "Si";
    }
    if (item.second != 0)
      myfile << std::fixed << std::setprecision(3) << item.second << ",Air"
             << "\n";
    if (matName == "Si" && fMaterials->GetThickness(itemName) == 0.3)
      for (int i = 0; i < 3; i++)
        myfile << std::fixed << std::setprecision(3)
               << fMaterials->GetThickness(itemName) / 3 << "," << matName
               << "\n";
    else
      myfile << std::fixed << std::setprecision(3)
             << fMaterials->GetThickness(itemName) << "," << matName << "\n";
  }
  myfile.close();
#endif

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/vis/drawVolume");
  UImanager->ApplyCommand("/vis/viewer/set/targetPoint 0 0 " +
                          std::to_string(fVisViewpoint / m) + " m");
  UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
  UImanager->ApplyCommand("/vis/scene/add/hits");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField() {
  G4SDManager *sdman = G4SDManager::GetSDMpointer();

  SiliconPixelSD *sensitiveSilicon = new SiliconPixelSD(
      (fMaterials->GetSiPixelLogical()->GetName() + "_sensitive").c_str());
  sdman->AddNewDetector(sensitiveSilicon);
  fMaterials->GetSiPixelLogical()->SetSensitiveDetector(sensitiveSilicon);

  SiPMSD *sensitiveSiPM = new SiPMSD(
      (fMaterials->GetAHCALSiPMlogical()->GetName() + "_sensitive").c_str());
  sdman->AddNewDetector(sensitiveSiPM);
  fMaterials->GetAHCALSiPMlogical()->SetSensitiveDetector(sensitiveSiPM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SelectConfiguration(G4int val) {

  if (fConfiguration != -1)
    return;

  fVisViewpoint = 0;
  if (val == 0) {
    return;
  } else if (val == 22)
    defineConfig22_October2018_1(fElementsMap, fVisViewpoint);
  else if (val == 100)
    defineTestConfig100(fElementsMap, fVisViewpoint);
  else {
    std::cout << "Configuration " << val << " not implemented --> return";
    return;
  }
  fConfiguration = val;

  ConstructHGCal();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetStepSizeSilicon(G4double val) {
  // setting the step size in silicon:
  G4double maxTrackLength = val * 0.001 * mm;
  fMaterials->GetSiPixelLogical()->SetUserLimits(
      new G4UserLimits(0, maxTrackLength));

  G4Region *reg = fMaterials->GetSiPixelLogical()->GetRegion();
  G4ProductionCuts *cuts = new G4ProductionCuts;
  cuts->SetProductionCut(maxTrackLength);
  reg->SetProductionCuts(cuts);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineCommands() {
  // Define /B5/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this, "/HGCalTestbeam/setup/",
                                      "Configuration specifications");

  // configuration command
  auto &configCmd = fMessenger->DeclareMethod(
      "configuration", &DetectorConstruction::SelectConfiguration,
      "Select the configuration (22 for October 2018 or 100 for test setup)");
  configCmd.SetParameterName("index", true);
  configCmd.SetDefaultValue("22");

  auto &SiStepSizeCmd = fMessenger->DeclareMethod(
      "stepSilicon", &DetectorConstruction::SetStepSizeSilicon,
      "Maximum step size in silicon pixels, unit: microns");
  SiStepSizeCmd.SetParameterName("size", true);
  SiStepSizeCmd.SetDefaultValue("30.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
