#include "config22_October2018_1.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void add_EE(G4int aEEid, std::vector<std::pair<G4String, G4double>> &aDzMap,
            G4double aAirBefore, G4double aAirMid) {
  aDzMap.push_back(std::make_pair("Fe_absorber_EE",
                                  aAirBefore)); // aAirBefore AIR + 0.3 mm Fe
  aDzMap.push_back(std::make_pair("Pb_absorber_EE", 0)); // 0 + 4.9mm Pb
  aDzMap.push_back(std::make_pair("Fe_absorber_EE", 0)); // 0 + 0.3 mm Fe
  aDzMap.push_back(std::make_pair("PCB", aAirMid));      // aAirMid AIR +1.3mm
  aDzMap.push_back(std::make_pair("Cu_baseplate_175um", 0.)); // 0 + 0.175mm Cu
  aDzMap.push_back(std::make_pair("Si_wafer", 0.));           // 0.3 mm
  aDzMap.push_back(std::make_pair("Cu_baseplate_25um", 0.));  //  0 + 0.025mm Cu
  aDzMap.push_back(std::make_pair("Kapton_layer", 0.));       // 0.075 mm
  if (aEEid == 11 || aEEid == 12)
    aDzMap.push_back(std::make_pair("Cu_baseplate", 0.)); // 1.2 mm
  aDzMap.push_back(std::make_pair("CuW_baseplate", 0.));  // 1.2 mm
  if (aEEid == 13)
    aDzMap.push_back(std::make_pair("CuW_baseplate_550um", 0.)); // 0.55 mm
  if (aEEid == 14)
    aDzMap.push_back(std::make_pair("CuW_baseplate_610um", 0.)); // 0.61 mm
  aDzMap.push_back(std::make_pair("Cu_absorber_EE", 0.));        // 6 mm
  if (aEEid == 13)
    aDzMap.push_back(std::make_pair("CuW_baseplate_610um", 0.)); // 0.61 mm
  if (aEEid == 14)
    aDzMap.push_back(std::make_pair("CuW_baseplate_710um", 0.)); // 0.71 mm
  aDzMap.push_back(std::make_pair("CuW_baseplate", 0.));         // 1.2 mm
  if (aEEid == 11 || aEEid == 12)
    aDzMap.push_back(std::make_pair("Cu_baseplate", 0.));     // 1.2 mm
  aDzMap.push_back(std::make_pair("Kapton_layer", 0.));       // 0.075 mm
  aDzMap.push_back(std::make_pair("Cu_baseplate_25um", 0.));  //  0 + 0.025mm Cu
  aDzMap.push_back(std::make_pair("Si_wafer", 0.));           // 0.3 mm
  aDzMap.push_back(std::make_pair("Cu_baseplate_175um", 0.)); // 0 + 0.175mm Cu
  aDzMap.push_back(std::make_pair("PCB", 0));                 // 1.3 mm
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void add_FH(G4int aFHid, std::vector<std::pair<G4String, G4double>> &aDzMap,
            G4double aAirBefore, G4double aAirMid) {
  std::string layout = "";
  if (aFHid < 10)
    layout = "_DAISY";
  if (!(aFHid == 1 || aFHid == 7))
    aDzMap.push_back(std::make_pair("Fe_absorber_FH",
                                    aAirBefore)); // aAirBefore AIR + 40 mm Fe
  aDzMap.push_back(
      std::make_pair("PCB" + layout, aAirMid)); // aAirMid AIR + 1.3 mm
  aDzMap.push_back(
      std::make_pair("Cu_baseplate_175um" + layout, 0.));    // 0.175mm Cu
  aDzMap.push_back(std::make_pair("Si_wafer" + layout, 0.)); // 0.3 mm
  if (aFHid == 5) {
    aDzMap.push_back(std::make_pair("PCB_thin" + layout, 0)); // 1.2 mm
  }
  if (aFHid != 5) {
    aDzMap.push_back(
        std::make_pair("Cu_baseplate_25um" + layout, 0.));         // 0.025mm Cu
    aDzMap.push_back(std::make_pair("Kapton_layer" + layout, 0.)); // 0.075 mm
  }
  if (aFHid == 6) {
    aDzMap.push_back(
        std::make_pair("Cu_baseplate_25um" + layout, 0.));         // 0.025mm Cu
    aDzMap.push_back(std::make_pair("Kapton_layer" + layout, 0.)); // 0.075 mm
  }
  if (aFHid != 5 && aFHid < 9)
    aDzMap.push_back(std::make_pair("Cu_baseplate" + layout, 0.)); // 1.2 mm
  if (aFHid == 9 || aFHid == 10)
    aDzMap.push_back(std::make_pair("CuW_baseplate" + layout, 0.)); // 1.2 mm
  if (aFHid != 10)
    aDzMap.push_back(std::make_pair("Cu_baseplate" + layout, 0.)); // 1.2 mm
  aDzMap.push_back(std::make_pair("Cu_absorber_FH", 0.));          // 6 * mm
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void defineConfig22_October2018_1(
    std::vector<std::pair<G4String, G4double>> &aDzMap,
    G4double &aViewpoint) {
  aViewpoint = 0.75 * m;

  // map means: position this material starting at z, where:
  // z += <second>
  // z = z0+ 0.5 * thickness[<first>]

  // WChambUpstream
  // world (and beam) starts at -45 m
  aDzMap.push_back(std::make_pair("DWC", 12.2825 * m)); // at -32.7175 m
  aDzMap.push_back(std::make_pair("CK3", 0.0 * m));     // at -30.69 m
  aDzMap.push_back(std::make_pair("DWC", 56.2 * cm));   // at -30.073 cm
  aDzMap.push_back(std::make_pair("DWC", 101.5 * cm));  // at -2900.3 cm
  aDzMap.push_back(std::make_pair("Scintillator_thin", 23.9 * cm));
  aDzMap.push_back(std::make_pair("Scintillator_thin", 13.9 * cm));
  aDzMap.push_back(std::make_pair("Scintillator_thin", 13.9 * cm));
  aDzMap.push_back(std::make_pair("Scintillator_thin", 103.8 * cm));
  aDzMap.push_back(std::make_pair("DWC", 34.8 * cm)); // at -2700.5 cm
  // HaloCounters not implemented (as offset in X or Y)

  // WChambDown
  aDzMap.push_back(std::make_pair("DWC", 18.15 * m)); // at -880 cm
  aDzMap.push_back(std::make_pair("DWC", 7.145 * m)); // at -160 cm

  // S5
  aDzMap.push_back(std::make_pair("Scintillator", 1.354 * m));
  // S6
  aDzMap.push_back(std::make_pair("Scintillator", 14 * cm));

  aDzMap.push_back(std::make_pair("Al_case_thick", 2.6 * cm)); // 5 mm Al
  aDzMap.push_back(std::make_pair("Al_case", 0));              // 2.1 mm Al

  // EE1
  add_EE(1, aDzMap, 119.7 * mm, 4.7 * mm);

  // EE2
  add_EE(2, aDzMap, 7.7 * mm, 3.7 * mm);

  // EE3
  add_EE(3, aDzMap, 7.7 * mm, 3.7 * mm);

  // EE4
  add_EE(4, aDzMap, 8.7 * mm, 3.7 * mm);

  // EE5
  add_EE(5, aDzMap, 8.7 * mm, 3.7 * mm);

  // EE6
  add_EE(6, aDzMap, 8.7 * mm, 3.7 * mm);

  // EE7
  add_EE(7, aDzMap, 6.7 * mm, 3.7 * mm);

  // EE8
  add_EE(8, aDzMap, 6.7 * mm, 3.7 * mm);

  // EE9
  add_EE(9, aDzMap, 6.7 * mm, 3.7 * mm);

  // EE10
  add_EE(10, aDzMap, 6.7 * mm, 3.7 * mm);

  // EE11
  add_EE(11, aDzMap, 6.7 * mm, 5.5 * mm);

  // EE12
  add_EE(12, aDzMap, 9.5 * mm, 5.5 * mm);

  // EE13
  add_EE(13, aDzMap, 9.5 * mm, 3.145 * mm);

  // EE14
  add_EE(14, aDzMap, 10.09 * mm, 3.095 * mm);

  aDzMap.push_back(std::make_pair("Steel_case_thick", 0)); // 40 mm
  aDzMap.push_back(std::make_pair("Al_case", 44 * mm));    // 2.1 mm

  // beginning of FH
  aDzMap.push_back(std::make_pair("Steel_case", 0)); // 9 mm

  // FH1
  add_FH(1, aDzMap, 0, 8.8 * mm);

  // FH2
  add_FH(2, aDzMap, 8 * mm, 8.8 * mm);

  // FH3
  add_FH(3, aDzMap, 3 * mm, 13.8 * mm);

  // FH4
  add_FH(4, aDzMap, 5 * mm, 12.8 * mm);

  // FH5
  add_FH(5, aDzMap, 7 * mm, 9.8 * mm);

  // FH6
  add_FH(6, aDzMap, 6 * mm, 10.7 * mm);

  // cases
  aDzMap.push_back(std::make_pair("Steel_case", 4 * mm));      // 9 mm
  aDzMap.push_back(std::make_pair("Fe_absorber_FH", 36 * mm)); // 40 mm
  aDzMap.push_back(std::make_pair("Steel_case", 52 * mm));     // 9 mm
  // FH7
  add_FH(7, aDzMap, 0, 8.8 * mm);

  // FH8
  add_FH(8, aDzMap, 7 * mm, 16.8 * mm);

  // FH9
  add_FH(9, aDzMap, 9 * mm, 14.8 * mm);

  // FH10
  add_FH(10, aDzMap, 10 * mm, 18 * mm);

  // FH11
  add_FH(11, aDzMap, 8 * mm, 17 * mm);

  // FH12
  add_FH(12, aDzMap, 7 * mm, 17 * mm);

  aDzMap.push_back(std::make_pair("Steel_case", 29 * mm));

  // AHCAL
  aDzMap.push_back(std::make_pair("Fe_absorber_AHCAL", 50.0 * cm));
  for (int l = 0; l < 39; l++) {
    aDzMap.push_back(std::make_pair("Al_absorber_AHCAL", 0.5 * cm));
    aDzMap.push_back(std::make_pair("AHCAL_SiPM_2x2HUB", 0.));
    aDzMap.push_back(std::make_pair("Al_absorber_AHCAL", 0.));
    aDzMap.push_back(std::make_pair("Fe_absorber_AHCAL", 0.5 * cm));
  }
  aDzMap.push_back(std::make_pair("Fe_absorber_AHCAL", 1.1 * cm));
  aDzMap.push_back(std::make_pair("Fe_absorber_AHCAL", 1.1 * cm));
  aDzMap.push_back(std::make_pair("Al_absorber_AHCAL", 0.5 * cm));
  aDzMap.push_back(std::make_pair("AHCAL_SiPM_2x2HUB", 0.));
  aDzMap.push_back(std::make_pair("Al_absorber_AHCAL", 0.));
  aDzMap.push_back(std::make_pair("Fe_absorber_AHCAL", 0.5 * cm));
}
