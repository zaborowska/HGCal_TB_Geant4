#include "config22_October2018_1.hh"


void add_EE(uint EEid, std::vector<std::pair<std::string, G4double> > &dz_map, double airBefore, double airMid) {
  dz_map.push_back(std::make_pair("Fe_absorber_EE", airBefore)); // airBefore AIR + 0.3 mm Fe
  dz_map.push_back(std::make_pair("Pb_absorber_EE", 0)); // 0 + 4.9mm Pb
  dz_map.push_back(std::make_pair("Fe_absorber_EE", 0)); // 0 + 0.3 mm Fe
  dz_map.push_back(std::make_pair("PCB", airMid)); // airMid AIR +1.3mm 
  dz_map.push_back(std::make_pair("Cu_baseplate_175um", 0.)); // 0 + 0.175mm Cu
  dz_map.push_back(std::make_pair("Si_wafer", 0.)); // 0.3 mm
  dz_map.push_back(std::make_pair("Cu_baseplate_25um", 0.)); //  0 + 0.025mm Cu
  dz_map.push_back(std::make_pair("Kapton_layer", 0.)); // 0.075 mm
  if (EEid == 11 || EEid == 12) dz_map.push_back(std::make_pair("Cu_baseplate", 0.)); //1.2 mm
  dz_map.push_back(std::make_pair("CuW_baseplate", 0.)); //1.2 mm
  if (EEid == 13) dz_map.push_back(std::make_pair("CuW_baseplate_550um", 0.)); // 0.55 mm
  if (EEid == 14) dz_map.push_back(std::make_pair("CuW_baseplate_610um", 0.)); // 0.61 mm
  dz_map.push_back(std::make_pair("Cu_absorber_EE", 0.)); // 6 mm
  if (EEid == 13) dz_map.push_back(std::make_pair("CuW_baseplate_610um", 0.)); // 0.61 mm
  if (EEid == 14) dz_map.push_back(std::make_pair("CuW_baseplate_710um", 0.)); // 0.71 mm
  dz_map.push_back(std::make_pair("CuW_baseplate", 0.)); // 1.2 mm
  if (EEid == 11 || EEid == 12) dz_map.push_back(std::make_pair("Cu_baseplate", 0.)); //1.2 mm
  dz_map.push_back(std::make_pair("Kapton_layer", 0.)); // 0.075 mm
  dz_map.push_back(std::make_pair("Cu_baseplate_25um", 0.)); //  0 + 0.025mm Cu
  dz_map.push_back(std::make_pair("Si_wafer", 0.)); // 0.3 mm
  dz_map.push_back(std::make_pair("Cu_baseplate_175um", 0.)); // 0 + 0.175mm Cu
  dz_map.push_back(std::make_pair("PCB", 0)); // 1.3 mm
}

void add_FH(uint FHid, std::vector<std::pair<std::string, G4double> > &dz_map, double airBefore, double airMid) {
  std::string layout = "";
  if (FHid < 10) layout = "_DAISY";
  if ( ! (FHid == 1 || FHid == 7) ) dz_map.push_back(std::make_pair("Fe_absorber_FH", airBefore)); // airBefore AIR + 40 mm Fe
  dz_map.push_back(std::make_pair("PCB"+layout, airMid)); // airMid AIR + 1.3 mm
  dz_map.push_back(std::make_pair("Cu_baseplate_175um"+layout, 0.)); // 0.175mm Cu
  dz_map.push_back(std::make_pair("Si_wafer"+layout, 0.));  // 0.3 mm
  if (FHid == 5) {
    dz_map.push_back(std::make_pair("PCB_thin"+layout, 0)); // 1.2 mm
  }
  if(FHid != 5) {
    dz_map.push_back(std::make_pair("Cu_baseplate_25um"+layout, 0.)); // 0.025mm Cu
    dz_map.push_back(std::make_pair("Kapton_layer"+layout, 0.)); // 0.075 mm
  }
  if (FHid == 6) {
    dz_map.push_back(std::make_pair("Cu_baseplate_25um"+layout, 0.)); // 0.025mm Cu
    dz_map.push_back(std::make_pair("Kapton_layer"+layout, 0.)); // 0.075 mm
  }
  if (FHid != 5 && FHid < 9)  dz_map.push_back(std::make_pair("Cu_baseplate"+layout, 0.)); // 1.2 mm
  if (FHid == 9 || FHid == 10)  dz_map.push_back(std::make_pair("CuW_baseplate"+layout, 0.)); // 1.2 mm  
  if (FHid != 10) dz_map.push_back(std::make_pair("Cu_baseplate"+layout, 0.)); // 1.2 mm
  dz_map.push_back(std::make_pair("Cu_absorber_FH", 0.)); // 6 * mm
  }

void defineConfig22_October2018_1(std::vector<std::pair<std::string, G4double> > &dz_map, G4double &viewpoint) {
  viewpoint = 9.3 * m;

  dz_map.push_back(std::make_pair("DWC", 0.0 * m));
  dz_map.push_back(std::make_pair("DWC", 2.0 * m));
  dz_map.push_back(std::make_pair("DWC", 0.3 * m));
  dz_map.push_back(std::make_pair("Scintillator", 1.5 * m));
  dz_map.push_back(std::make_pair("DWC", 0.3 * m));
  dz_map.push_back(std::make_pair("DWC", 15. * m));
  dz_map.push_back(std::make_pair("DWC", 7. * m));

  dz_map.push_back(std::make_pair("Scintillator", 0.3 * m));
  dz_map.push_back(std::make_pair("Scintillator", 2.0 * m));

  dz_map.push_back(std::make_pair("MCP", 0.0 * m));
  dz_map.push_back(std::make_pair("MCP", 0.0 * m));


  // map means: position this material starting at z, where:
  // z += <second>
  // z = z0+ 0.5 * thickness[<first>]
 
  dz_map.push_back(std::make_pair("Al_case_thick", 0)); // 5 mm Al
  dz_map.push_back(std::make_pair("Al_case", 0)); // 2.1 mm Al

  //EE1
  add_EE(1, dz_map, 119.7 * mm, 4.7 * mm);

  //EE2
  add_EE(2, dz_map, 7.7 * mm, 3.7 * mm);

  //EE3
  add_EE(3, dz_map, 7.7 * mm, 3.7 * mm);

  //EE4
  add_EE(4, dz_map, 8.7 * mm, 3.7 * mm);

  //EE5
  add_EE(5, dz_map, 8.7 * mm, 3.7 * mm);

  //EE6
  add_EE(6, dz_map, 8.7 * mm, 3.7 * mm);

  //EE7
  add_EE(7, dz_map, 6.7 * mm, 3.7 * mm);

  //EE8
  add_EE(8, dz_map, 6.7 * mm, 3.7 * mm);

  //EE9
  add_EE(9, dz_map, 6.7 * mm, 3.7 * mm);

  //EE10
  add_EE(10, dz_map, 6.7 * mm, 3.7 * mm);

  //EE11
  add_EE(11, dz_map, 6.7 * mm, 5.5 * mm);

  //EE12
  add_EE(12, dz_map, 9.5 * mm, 5.5 * mm);

  //EE13
  add_EE(13, dz_map, 9.5 * mm, 3.145 * mm);

  //EE14
  add_EE(14, dz_map, 10.09 * mm, 3.095 * mm);

  dz_map.push_back(std::make_pair("Steel_case_thick", 0)); // 40 mm
  dz_map.push_back(std::make_pair("Al_case", 44 * mm)); // 2.1 mm

  //beginning of FH
  dz_map.push_back(std::make_pair("Steel_case", 0)); // 9 mm

  //FH1
  add_FH(1, dz_map, 0, 8.8 * mm);

  //FH2
  add_FH(2, dz_map, 8 * mm, 8.8 * mm);

  //FH3
  add_FH(3, dz_map, 3 * mm, 13.8 * mm);

  //FH4
  add_FH(4, dz_map, 5 * mm, 12.8 * mm);

  //FH5
  add_FH(5, dz_map, 7 * mm, 9.8 * mm);

  //FH6
  add_FH(6, dz_map, 6 * mm, 10.7 * mm);

  // cases
  dz_map.push_back(std::make_pair("Steel_case", 4 * mm)); // 9 mm
  dz_map.push_back(std::make_pair("Fe_absorber_FH", 36 * mm)); // 40 mm
  dz_map.push_back(std::make_pair("Steel_case", 52 * mm)); // 9 mm
  //FH7
  add_FH(7, dz_map, 0, 8.8 * mm);

  //FH8
  add_FH(8, dz_map, 7 * mm, 16.8 * mm);

  //FH9
  add_FH(9, dz_map, 9 * mm, 14.8 * mm);

  //FH10
  add_FH(10, dz_map, 10 * mm, 18 * mm);

  //FH11
  add_FH(11, dz_map, 8 * mm, 17 * mm);

  //FH12
  add_FH(12, dz_map, 7 * mm, 17 * mm);

  dz_map.push_back(std::make_pair("Steel_case", 29 * mm));

  //AHCAL
  dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 50.0 * cm));
  for (int l = 0; l < 39; l++) {
    dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.5 * cm));
    dz_map.push_back(std::make_pair("AHCAL_SiPM_2x2HUB", 0.));
    dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.));
    dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 0.5 * cm));
  }
  dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 1.1 * cm));
  dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 1.1 * cm));
  dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.5 * cm));
  dz_map.push_back(std::make_pair("AHCAL_SiPM_2x2HUB", 0.));
  dz_map.push_back(std::make_pair("Al_absorber_AHCAL", 0.));
  dz_map.push_back(std::make_pair("Fe_absorber_AHCAL", 0.5 * cm));


}
