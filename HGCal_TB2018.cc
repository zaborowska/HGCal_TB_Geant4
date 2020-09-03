//16 July 2018, Thorben Quast


#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "FTFPCMS_BERT_EMM.hh"
#include "G4PhysListFactory.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  //
#if defined(G4MULTITHREADED) && !defined(MATSCAN)
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics List name defined via environment variable
  // or the default FTFP_BERT is used
  G4VModularPhysicsList* physicsList = nullptr;
  G4String physName = "";
  char* PLenv = getenv("PHYSLIST");
  if (PLenv)
    physName = G4String(PLenv);
  if ("FTFP_BERT_EMM" == physName)
    physicsList = new FTFPCMS_BERT_EMM;
  else if ("" == physName)
  {
    physName = "FTFP_BERT";
    physicsList = new FTFP_BERT;
  }
  if (!physicsList)
  {
    G4PhysListFactory factory;
    physicsList = factory.GetReferencePhysList(physName);
  }
  if (!physicsList)
  {
    std::cerr << "Unknown physics list defined in environment variable PHYSLIST: " << PLenv
    << ". Consider extension of main to take it into account." << std::endl;
    return -1;
  }
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());
  

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // Initialize visualization
    G4VisManager *visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
    // interactive mode
    //UImanager->ApplyCommand("/control/execute init_vis.mac");
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
    delete visManager;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
