#include "test_configs.hh"

void defineTestConfig100(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
	viewpoint = 1 * m;

	 dz_map.push_back(std::make_pair("PCB", 150.3 * mm));
	 dz_map.push_back(std::make_pair("Si_wafer", 0.));
	 dz_map.push_back(std::make_pair("Kapton_layer", 0.3 * mm));
	 dz_map.push_back(std::make_pair("CuW_baseplate", 0.));
	 dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.));
}
