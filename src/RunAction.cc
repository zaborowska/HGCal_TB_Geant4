
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction)
  : G4UserRunAction(),
    fEventAction(eventAction),
    fOutputFileDir("sim_HGCalTB_G4Standalone")
{

  fMessenger
    = new G4GenericMessenger(this,
                             "/Simulation/output/",
                             "Output control");

  // randomizePrimary command
  auto& fileNameCommand
    = fMessenger->DeclareProperty("file", fOutputFileDir);
  G4String guidance
    = "Define output file location.";
  fileNameCommand.SetGuidance(guidance);
  fileNameCommand.SetParameterName("filename", true);
  fileNameCommand.SetDefaultValue("sim_HGCalTB_G4Standalone");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*) {
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespacels
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output
  analysisManager->SetVerboseLevel(1);
  std::cout << "Output file is: " << fOutputFileDir << std::endl;
  analysisManager->SetFileName(fOutputFileDir);

  // Book histograms, ntuple
  //

  // Creating ntuple
  //

  if ( fEventAction ) {
    analysisManager->CreateNtuple("hits", "hits");
    analysisManager->CreateNtupleIColumn("event");    // column Id = 0
    analysisManager->CreateNtupleIColumn("pdgID");    // column Id = 1
    analysisManager->CreateNtupleIColumn("beamEnergy");    // column Id = 2
    analysisManager->CreateNtupleDColumn("beamX_cm");    // column Id = 3
    analysisManager->CreateNtupleDColumn("beamY_cm");    // column Id = 4
    analysisManager->CreateNtupleDColumn("beamZ_cm");    // column Id = 5
    // brakuje: pdgID, beamEnergy
    analysisManager->CreateNtupleIColumn("ID", fEventAction->hits_ID);    // column Id = 4
    analysisManager->CreateNtupleDColumn("x_cm", fEventAction->hits_x);    // column Id = 5
    analysisManager->CreateNtupleDColumn("y_cm", fEventAction->hits_y);    // column Id = 6
    analysisManager->CreateNtupleDColumn("z_cm", fEventAction->hits_z);    // column Id = 7
    analysisManager->CreateNtupleDColumn("Edep_keV", fEventAction->hits_Edep);    // column Id = 8
    analysisManager->CreateNtupleDColumn("EdepNonIonizing_keV", fEventAction->hits_EdepNonIonising);    // column Id = 9
    analysisManager->CreateNtupleDColumn("TOA_ns", fEventAction->hits_TOA);    // column Id = 10
    analysisManager->CreateNtupleDColumn("TOA_last_ns", fEventAction->hits_TOA_last);    // column Id = 11
    analysisManager->CreateNtupleIColumn("type", fEventAction->hits_type);    // column Id = 12

    analysisManager->CreateNtupleDColumn("signalSum_HGCAL_MeV");    // column Id = 13 +2
    analysisManager->CreateNtupleDColumn("COGZ_HGCAL_cm");    // column Id = 14 +2
    analysisManager->CreateNtupleIColumn("NHits_HGCAL");    // column Id = 15 +2

    analysisManager->CreateNtupleDColumn("signalSum_AHCAL_MeV");    // column Id = 16 +2
    analysisManager->CreateNtupleDColumn("COGZ_AHCAL_cm");    // column Id = 17 +2
    analysisManager->CreateNtupleIColumn("NHits_AHCAL");    // column Id = 18   +2
    analysisManager->FinishNtuple();

    #ifdef MATSCAN
    analysisManager->CreateNtuple("materials", "material");
    analysisManager->CreateNtupleDColumn(1, "nX0", fEventAction->material_nX0);    // column Id = 0
    analysisManager->CreateNtupleDColumn(1, "nLambda", fEventAction->material_nLambda);    // column Id = 1
    analysisManager->CreateNtupleDColumn(1, "depth", fEventAction->material_depth);    // column Id = 2
    analysisManager->CreateNtupleIColumn(1, "name", fEventAction->material_name);    // column Id = 3
    analysisManager->FinishNtuple(1);
    #endif
  }

  // Open the output file
  analysisManager->OpenFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  #ifdef MATSCAN
  std::ofstream myfile;
  myfile.open("materal2enumMap.txt");
  for (const auto &item : fEventAction->material_names_map)
  {
    myfile << item.first << "," << item.second << "\n";
  }
  myfile.close();
  // analyse material scan and print total values
  std::ofstream myfile2;
  myfile2.open("materalScan.txt");
  std::map<std::string, std::array<double, 3>> matScan; // matName -> (depth, nX0, nLambda)
  double depth = 0, nX0 = 0, nLambda = 0;
  for (const auto &item : fEventAction->material_names_map)
  {
    matScan[item.first] = {0., 0., 0.};
  }
  for (size_t iter = 0; iter < fEventAction->material_name.size(); iter++) {
    std::string matName = "";
    for (const auto &item : fEventAction->material_names_map)
      if (item.second == fEventAction->material_name[iter])
        matName = item.first;
    if (matName.find("G4_AIR") != std::string::npos) continue;
    fEventAction->material_names_map[fEventAction->material_name[iter]];
    matScan[matName][0] += fEventAction->material_depth[iter];
    matScan[matName][1] += fEventAction->material_nX0[iter];
    matScan[matName][2] += fEventAction->material_nLambda[iter];
  }
  for (const auto &item : matScan)
  {
    myfile2 << item.first << "\t" << item.second[0] << " mm\t" << item.second[1] << "X0\t"
    << item.second[2] << " lambda\n";
    G4cout << item.first << "\t" << item.second[0] << " mm\t" << item.second[1] << "X0\t"
    << item.second[2] << " lambda" << G4endl;
    depth += item.second[0];
    nX0 +=  item.second[1];
    nLambda +=item.second[2];
  }
    myfile2 << "TOTAL:" << depth << " mm\t" << nX0 << " X0\t" << nLambda << " lambda\n";
  myfile2.close();
  G4cout << "TOTAL: " << depth << " mm\t" << nX0 << " X0\t" << nLambda << " lambda" << G4endl;
  #endif
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

