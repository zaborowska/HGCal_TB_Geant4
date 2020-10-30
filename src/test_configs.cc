#include "test_configs.hh"
#include "G4SystemOfUnits.hh"

void defineTestConfig100(std::vector<std::pair<G4String, G4double>> &aDzMap,
                         G4double &aViewpoint) {
  aViewpoint = 1 * m;

  aDzMap.push_back(std::make_pair("PCB", 150.3 * mm));
  aDzMap.push_back(std::make_pair("Si_wafer", 0.));
  aDzMap.push_back(std::make_pair("Kapton_layer", 0.3 * mm));
  aDzMap.push_back(std::make_pair("CuW_baseplate", 0.));
  aDzMap.push_back(std::make_pair("Cu_absorber_EE", 0.));
}
