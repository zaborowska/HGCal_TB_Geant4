
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "SiliconPixelHit.hh"
#include "SiPMHit.hh"

#ifdef MATSCAN
#include "G4Material.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
	: G4UserEventAction()
{
	hitTimeCut = -1;
	toaThreshold = 0;
	DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
	primaries_PDG.clear();
	primaries_energy.clear();
	primaries_x.clear();
	primaries_y.clear();
	primaries_z.clear();

	Si_hits_ID.clear();
	Si_hits_x.clear();
	Si_hits_y.clear();
	Si_hits_z.clear();
	Si_hits_Edep.clear();
	Si_hits_EdepNonIonising.clear();
	Si_hits_TOA.clear();
	Si_hits_TOA_last.clear();
	Si_hits_type.clear();

	SiPM_hits_ID.clear();
	SiPM_hits_x.clear();
	SiPM_hits_y.clear();
	SiPM_hits_z.clear();
	SiPM_hits_Edep.clear();
	SiPM_hits_EdepNonIonising.clear();
	SiPM_hits_TOA.clear();
	SiPM_hits_TOA_last.clear();
	SiPM_hits_type.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	// sanity check
	if (event->GetNumberOfPrimaryVertex() == 0) return;

	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->FillNtupleIColumn(0, event->GetEventID());
	// fill for all primary input particles
	for (G4int iVertex = 0; iVertex < event->GetNumberOfPrimaryVertex(); iVertex++)
	{
		auto vertex = event->GetPrimaryVertex(iVertex);
		primaries_x.push_back(vertex->GetX0() / CLHEP::cm);
		primaries_y.push_back(vertex->GetY0() / CLHEP::cm);
		primaries_z.push_back(vertex->GetZ0() / CLHEP::cm);
		for (G4int iParticle = 0; iParticle < vertex->GetNumberOfParticle(); iParticle++)
		{
			auto particle = vertex->GetPrimary(iParticle);
			primaries_PDG.push_back(particle->GetPDGcode());
			primaries_energy.push_back(particle->GetTotalEnergy() / CLHEP::GeV);
		}
	}

	auto hce = event->GetHCofThisEvent();
	auto sdManager = G4SDManager::GetSDMpointer();
	G4int collId;


	//HGCAL EE + FH
	collId = sdManager->GetCollectionID("SiliconPixelHitCollection");
	auto hc = hce->GetHC(collId);
	if ( ! hc ) return;
	double esum_HGCAL = 0; double cogz_HGCAL = 0; int Nhits_HGCAL = 0;
	for (unsigned int i = 0; i < hc->GetSize(); ++i) {
		auto hit = static_cast<SiliconPixelHit*>(hc->GetHit(i));
		hit->Digitise(hitTimeCut / CLHEP::ns, toaThreshold / CLHEP::keV );

		if (hit->isValidHit()) {
			Si_hits_ID.push_back(hit->ID());
			Si_hits_x.push_back(hit->GetX());
			Si_hits_y.push_back(hit->GetY());
			Si_hits_z.push_back(hit->GetZ());
			Si_hits_Edep.push_back(hit->GetEdep());
			Si_hits_EdepNonIonising.push_back(hit->GetEdepNonIonizing());
			Si_hits_TOA.push_back(hit->GetTOA());
			Si_hits_TOA_last.push_back(hit->GetLastTOA());
			Si_hits_type.push_back(0);

			Nhits_HGCAL++;
			esum_HGCAL += hit->GetEdep() * CLHEP::keV / CLHEP::MeV;
			cogz_HGCAL += hit->GetZ() * hit->GetEdep();
		}
	}
	if (esum_HGCAL > 0) cogz_HGCAL /= esum_HGCAL;

	analysisManager->FillNtupleDColumn(24, esum_HGCAL / CLHEP::GeV);
	analysisManager->FillNtupleDColumn(25, cogz_HGCAL);
	analysisManager->FillNtupleIColumn(26, Nhits_HGCAL);


	//AHCAL 
	collId = sdManager->GetCollectionID("SiPMHitCollection");
	hc = hce->GetHC(collId);
	if ( ! hc ) return;
	double esum_AHCAL = 0; double cogz_AHCAL = 0; int Nhits_AHCAL = 0;
	for (unsigned int i = 0; i < hc->GetSize(); ++i) {
		auto hit = static_cast<SiPMHit*>(hc->GetHit(i));
		hit->Digitise(-1, 0 );

		if (hit->isValidHit()) {
			SiPM_hits_ID.push_back(hit->ID());
			SiPM_hits_x.push_back(hit->GetX());
			SiPM_hits_y.push_back(hit->GetY());
			SiPM_hits_z.push_back(hit->GetZ());
			SiPM_hits_Edep.push_back(hit->GetEdep());
			SiPM_hits_EdepNonIonising.push_back(hit->GetEdepNonIonizing());
			SiPM_hits_TOA.push_back(hit->GetTOA());
			SiPM_hits_type.push_back(1);

			Nhits_AHCAL++;
			esum_AHCAL += hit->GetEdep() * CLHEP::keV / CLHEP::MeV;
			cogz_AHCAL += hit->GetZ() * hit->GetEdep();
		}
	}
	if (esum_AHCAL > 0) cogz_AHCAL /= esum_AHCAL;

	analysisManager->FillNtupleDColumn(27, esum_AHCAL / CLHEP::GeV);
	analysisManager->FillNtupleDColumn(28, cogz_AHCAL);
	analysisManager->FillNtupleIColumn(29, Nhits_AHCAL);

	analysisManager->AddNtupleRow();
	#ifdef MATSCAN
	analysisManager->AddNtupleRow(1);
	#endif
}


void EventAction::DefineCommands() {

	fMessenger
	    = new G4GenericMessenger(this,
	                             "/Simulation/hits/",
	                             "Primary generator control");


	// time cut command
	auto& timeCutCmd
	    = fMessenger->DeclarePropertyWithUnit("timeCut", "ns", hitTimeCut,
	            "Size of time window for hit digitalisation");
	timeCutCmd.SetParameterName("timeCut", true);
	timeCutCmd.SetRange("timeCut>=-1");
	timeCutCmd.SetDefaultValue("-1");

	// toa threshold command
	auto& toaThresholdCmd
	    = fMessenger->DeclarePropertyWithUnit("toaThreshold", "keV", toaThreshold,
	            "Threshold for TOA activation");
	toaThresholdCmd.SetParameterName("toaThreshold", true);
	toaThresholdCmd.SetRange("toaThreshold>=0");
	toaThresholdCmd.SetDefaultValue("0");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef MATSCAN
void EventAction::AddStep(G4double aStepLength, G4Material* aMaterial) {
	material_depth.push_back(aStepLength);
	material_nX0.push_back(aStepLength / aMaterial->GetRadlen());
	material_nLambda.push_back(aStepLength / aMaterial->GetNuclearInterLength());
	G4int matId;
	auto matFind = material_names_map.find(aMaterial->GetName());
	if(matFind != material_names_map.end()) {
		matId = matFind->second;
	} else {
		matId = material_names_map.size();
		material_names_map.insert({aMaterial->GetName(), matId});
	}
	material_name.push_back(matId);
}
#endif