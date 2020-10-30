#include "RunAction.hh"
#include "EventAction.hh"

#include "G4UserRunAction.hh"
#include "G4GenericMessenger.hh"
#include "G4String.hh"
#include "g4root.hh"
#include <iostream>


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

  if (fEventAction) {
    analysisManager->CreateNtuple("hits", "hits");
    analysisManager->CreateNtupleIColumn("event"); // column Id = 0
    analysisManager->CreateNtupleIColumn(
        "pdgID", fEventAction->fPrimariesPDG); // column Id = 1
    analysisManager->CreateNtupleDColumn(
        "beamEnergy", fEventAction->fPrimariesEnergy); // column Id = 2
    analysisManager->CreateNtupleDColumn(
        "beamX_cm", fEventAction->fPrimariesX); // column Id = 3
    analysisManager->CreateNtupleDColumn(
        "beamY_cm", fEventAction->fPrimariesY); // column Id = 4
    analysisManager->CreateNtupleDColumn(
        "beamZ_cm", fEventAction->fPrimariesZ); // column Id = 5
    analysisManager->CreateNtupleIColumn("siliconHits_ID",
                                         fEventAction->fSiHitsID);
    analysisManager->CreateNtupleDColumn("siliconHits_x_cm",
                                         fEventAction->fSiHitsX);
    analysisManager->CreateNtupleDColumn("siliconHits_y_cm",
                                         fEventAction->fSiHitsY);
    analysisManager->CreateNtupleDColumn("siliconHits_z_cm",
                                         fEventAction->fSiHitsZ);
    analysisManager->CreateNtupleDColumn("siliconHits_Edep_keV",
                                         fEventAction->fSiHitsEdep);
    analysisManager->CreateNtupleDColumn("siliconHits_EdepNonIonizing_keV",
                                         fEventAction->fSiHitsEdepNonIonising);
    analysisManager->CreateNtupleDColumn("siliconHits_TOA_ns",
                                         fEventAction->fSiHitsTOA);
    analysisManager->CreateNtupleDColumn("siliconHits_TOA_last_ns",
                                         fEventAction->fSiHitsTOA);
    analysisManager->CreateNtupleIColumn("siliconHits_type",
                                         fEventAction->fSiHitsType);

    analysisManager->CreateNtupleIColumn("SiPMHits_ID",
                                         fEventAction->fSiPMhitsID);
    analysisManager->CreateNtupleDColumn("SiPMHits_x_cm",
                                         fEventAction->fSiPMhitsX);
    analysisManager->CreateNtupleDColumn("SiPMHits_y_cm",
                                         fEventAction->fSiPMhitsY);
    analysisManager->CreateNtupleDColumn("SiPMHits_z_cm",
                                         fEventAction->fSiPMhitsZ);
    analysisManager->CreateNtupleDColumn("SiPMHits_Edep_keV",
                                         fEventAction->fSiPMhitsEdep);
    analysisManager->CreateNtupleDColumn(
        "SiPMHits_EdepNonIonizing_keV", fEventAction->fSiPMhitsEdepNonIonising);
    analysisManager->CreateNtupleDColumn("SiPMHits_TOA_ns",
                                         fEventAction->fSiPMhitsTOA);
    analysisManager->CreateNtupleIColumn("SiPMHits_type",
                                         fEventAction->fSiPMhitsType);

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
        1, "nX0", fEventAction->fMaterialNX0); // column Id = 0
    analysisManager->CreateNtupleDColumn(
        1, "nLambda", fEventAction->fMaterialNLambda); // column Id = 1
    analysisManager->CreateNtupleDColumn(
        1, "depth", fEventAction->fMaterialDepth); // column Id = 2
    analysisManager->CreateNtupleIColumn(
        1, "name", fEventAction->fMaterialName); // column Id = 3
    analysisManager->FinishNtuple(1);
#endif
  }

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
  G4cout << "TOTAL (w/o air):\t" << depth << " mm\t" << nX0 << " X0\t"
         << nLambda << " lambda" << G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
