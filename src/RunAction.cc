
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
// #include "Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction *eventAction)
    : G4UserRunAction(), fEventAction(eventAction),
      fOutputFileDir("sim_HGCalTB_G4Standalone") {

  fMessenger =
      new G4GenericMessenger(this, "/HGCalTestbeam/output/", "Output control");

  // randomizePrimary command
  auto &fileNameCommand = fMessenger->DeclareProperty("file", fOutputFileDir);
  G4String guidance = "Define output file location.";
  fileNameCommand.SetGuidance(guidance);
  fileNameCommand.SetParameterName("filename", true);
  fileNameCommand.SetDefaultValue("sim_HGCalTB_G4Standalone");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run *) {
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespaces
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

// Default settings
#ifndef WITHROOT
  // Do not merge if input particles are read from several files
  analysisManager->SetNtupleMerging(true);
#endif
  analysisManager->SetVerboseLevel(1);
  std::cout << "Output file is: " << fOutputFileDir << std::endl;
  analysisManager->SetFileName(fOutputFileDir);

  // Book histograms, ntuple
  //

  // Creating ntuple
  //

  if (fEventAction) {
    analysisManager->CreateNtuple("hits", "hits");
    analysisManager->CreateNtupleIColumn("event"); // column Id = 0
    analysisManager->CreateNtupleIColumn(
        "pdgID", fEventAction->primaries_PDG); // column Id = 1
    analysisManager->CreateNtupleDColumn(
        "beamEnergy", fEventAction->primaries_energy); // column Id = 2
    analysisManager->CreateNtupleDColumn(
        "beamX_cm", fEventAction->primaries_x); // column Id = 3
    analysisManager->CreateNtupleDColumn(
        "beamY_cm", fEventAction->primaries_y); // column Id = 4
    analysisManager->CreateNtupleDColumn(
        "beamZ_cm", fEventAction->primaries_z); // column Id = 5
    analysisManager->CreateNtupleIColumn("siliconHits_ID",
                                         fEventAction->Si_hits_ID);
    analysisManager->CreateNtupleDColumn("siliconHits_x_cm",
                                         fEventAction->Si_hits_x);
    analysisManager->CreateNtupleDColumn("siliconHits_y_cm",
                                         fEventAction->Si_hits_y);
    analysisManager->CreateNtupleDColumn("siliconHits_z_cm",
                                         fEventAction->Si_hits_z);
    analysisManager->CreateNtupleDColumn("siliconHits_Edep_keV",
                                         fEventAction->Si_hits_Edep);
    analysisManager->CreateNtupleDColumn("siliconHits_EdepNonIonizing_keV",
                                         fEventAction->Si_hits_EdepNonIonising);
    analysisManager->CreateNtupleDColumn("siliconHits_TOA_ns",
                                         fEventAction->Si_hits_TOA);
    analysisManager->CreateNtupleDColumn("siliconHits_TOA_last_ns",
                                         fEventAction->Si_hits_TOA_last);
    analysisManager->CreateNtupleIColumn("siliconHits_type",
                                         fEventAction->Si_hits_type);

    analysisManager->CreateNtupleIColumn("SiPMHits_ID",
                                         fEventAction->SiPM_hits_ID);
    analysisManager->CreateNtupleDColumn("SiPMHits_x_cm",
                                         fEventAction->SiPM_hits_x);
    analysisManager->CreateNtupleDColumn("SiPMHits_y_cm",
                                         fEventAction->SiPM_hits_y);
    analysisManager->CreateNtupleDColumn("SiPMHits_z_cm",
                                         fEventAction->SiPM_hits_z);
    analysisManager->CreateNtupleDColumn("SiPMHits_Edep_keV",
                                         fEventAction->SiPM_hits_Edep);
    analysisManager->CreateNtupleDColumn(
        "SiPMHits_EdepNonIonizing_keV",
        fEventAction->SiPM_hits_EdepNonIonising);
    analysisManager->CreateNtupleDColumn("SiPMHits_TOA_ns",
                                         fEventAction->SiPM_hits_TOA);
    analysisManager->CreateNtupleIColumn("SiPMHits_type",
                                         fEventAction->SiPM_hits_type);

    analysisManager->CreateNtupleDColumn(
        "signalSum_HGCAL_MeV");                            // column Id = 23
    analysisManager->CreateNtupleDColumn("COGZ_HGCAL_cm"); // column Id = 24
    analysisManager->CreateNtupleIColumn("NHits_HGCAL");   // column Id = 25

    analysisManager->CreateNtupleDColumn(
        "signalSum_AHCAL_MeV");                            // column Id = 26
    analysisManager->CreateNtupleDColumn("COGZ_AHCAL_cm"); // column Id = 27
    analysisManager->CreateNtupleIColumn("NHits_AHCAL");   // column Id = 28
    analysisManager->FinishNtuple();

#ifdef MATSCAN
    analysisManager->CreateNtuple("materials", "material");
    analysisManager->CreateNtupleDColumn(
        1, "nX0", fEventAction->material_nX0); // column Id = 0
    analysisManager->CreateNtupleDColumn(
        1, "nLambda", fEventAction->material_nLambda); // column Id = 1
    analysisManager->CreateNtupleDColumn(
        1, "depth", fEventAction->material_depth); // column Id = 2
    analysisManager->CreateNtupleIColumn(
        1, "name", fEventAction->material_name); // column Id = 3
    analysisManager->FinishNtuple(1);
#endif
  }

  // Open the output file
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

#ifdef MATSCAN
  std::ofstream myfile;
  myfile.open("materal2enumMap.txt");
  for (const auto &item : fEventAction->material_names_map) {
    myfile << item.first << "," << item.second << "\n";
  }
  myfile.close();
  // analyse material scan and print total values
  std::ofstream myfile2;
  myfile2.open("materalScan.txt");
  std::map<std::string, std::array<double, 3>>
      matScan; // matName -> (depth, nX0, nLambda)
  double depth = 0, nX0 = 0, nLambda = 0;
  for (const auto &item : fEventAction->material_names_map) {
    matScan[item.first] = {0., 0., 0.};
  }
  for (size_t iter = 0; iter < fEventAction->material_name.size(); iter++) {
    std::string matName = "";
    for (const auto &item : fEventAction->material_names_map)
      if (item.second == fEventAction->material_name[iter])
        matName = item.first;
    // if (matName.find("G4_AIR") != std::string::npos) continue;
    fEventAction->material_names_map[fEventAction->material_name[iter]];
    matScan[matName][0] += fEventAction->material_depth[iter];
    matScan[matName][1] += fEventAction->material_nX0[iter];
    matScan[matName][2] += fEventAction->material_nLambda[iter];
  }
  for (const auto &item : matScan) {
    myfile2 << item.first << "," << item.second[0] << "," << item.second[1]
            << "," << item.second[2] << "\n";
    G4cout << item.first << "\t" << item.second[0] << " mm\t" << item.second[1]
           << "X0\t" << item.second[2] << " lambda" << G4endl;
    if (item.first.find("G4_AIR") == std::string::npos) {
      depth += item.second[0];
      nX0 += item.second[1];
      nLambda += item.second[2];
    }
  }
  myfile2 << "TOTAL (w/o air)," << depth << "," << nX0 << "," << nLambda
          << "\n";
  myfile2.close();
  G4cout << "TOTAL (w/o air):\t" << depth << " mm\t" << nX0 << " X0\t" << nLambda
         << " lambda" << G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
