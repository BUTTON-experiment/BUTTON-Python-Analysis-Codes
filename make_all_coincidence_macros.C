void make_macros(int,int,string);

void make_all_coincidence_macros() {

  int photon_num[13] = {50,100,150,200,250,300,350,400,450,500,550,600,800};
  int num_simulated = 1000;

  string input_filepath = "/home/eellingw/ratpac/Testing_Daniel_Analysis_Codes/";

  ofstream macroscript;
  string macroscript_name = "run_coincidence_macros.sh";
  macroscript.open(macroscript_name);

  ofstream readscript;
  string readscript_name = "run_read_root_file.sh";
  readscript.open(readscript_name);
  cout<<input_filepath+"macros"<<endl;
  string macros_path = input_filepath+"macros";
  if (std::filesystem::create_directory(macros_path)){cout<<"**** macros directory created"<<endl;}
  if (std::filesystem::create_directory(input_filepath+"ntuple_root")){cout<<"**** ntuple_root directory created"<<endl;}
  if (std::filesystem::create_directory(input_filepath+"coincidence")){cout<<"**** coincidence directory created"<<endl;}
  
  for (int i=0;i<13;i++) {
    macroscript<<"button "+input_filepath+"macros/nonvis_coincidenceTest_ph"+to_string(photon_num[i])+".mac"<<endl;
    readscript<<"python3 Root_File_Reader.py "+input_filepath+"ntuple_root/coincidenceTest"+to_string(photon_num[i])+"Photons_lb.ntuple.root"<<" "<<to_string(num_simulated)<<endl;
    make_macros(photon_num[i],num_simulated,input_filepath);
  }
  macroscript.close();
  readscript.close();
}

void make_macros(int num_ph, int nEv,string filepath) {
  ofstream myfile;
  string macro_name = filepath+"macros/nonvis_coincidenceTest_ph"+to_string(num_ph)+".mac";
  myfile.open(macro_name);
  myfile<<"/glg4debug/glg4param omit_muon_processes  1.0"<<endl;
  myfile<<"/glg4debug/glg4param omit_hadronic_processes  1.0\n"<<endl;

  myfile<<"#set the d parameters"<<endl;
  myfile<<"/rat/db/set DETECTOR experiment \"BUTTON\""<<endl;
  myfile<<"/rat/db/set DETECTOR geo_file \"BUTTON/button_frame.geo\"\n"<<endl;
  
  myfile<<"# Colors"<<endl;
  myfile<<"#/rat/db/set GEO[world] invisible 1"<<endl;
  myfile<<"#/rat/db/set GEO[tank] invisible 1"<<endl;
  myfile<<"#/rat/db/set GEO[black_sheet] invisible 1\n"<<endl;
  myfile<<"/run/initialize\n"<<endl;

  myfile<<"#/tracking/storeTrajectory 1\n"<<endl;

  myfile<<"##### Visualization ##########################\n"<<endl;

  myfile<<"#/vis/open OGLIQt"<<endl;
  myfile<<"#/vis/scene/create"<<endl;
  myfile<<"#/vis/scene/add/trajectories rich smooth"<<endl;
  myfile<<"#/tracking/storeTrajectory 1"<<endl;
  myfile<<"#/tracking/FillPointCont 1"<<endl;
  myfile<<"#/vis/scene/add/volume"<<endl;
  myfile<<"#/vis/scene/add/hits"<<endl;
  myfile<<"#/vis/sceneHandler/attach scene-0"<<endl;
  myfile<<"#/vis/viewer/set/upVector 0.0 0.0 1.0"<<endl;
  myfile<<"#/vis/viewer/set/viewpointThetaPhi 90 0"<<endl;
  myfile<<"#/vis/viewer/set/style s"<<endl;
  myfile<<"#/vis/viewer/flush\n"<<endl;

  myfile<<"#/vis/modeling/trajectories/create/drawByParticleID"<<endl;
  myfile<<"#/vis/modeling/trajectories/drawByParticleID-0/setDefaultRGBA 0 255 0 255\n"<<endl;

  myfile<<"## Cut a plane through the d\n"<<endl;

  myfile<<"#/vis/viewer/addCutawayPlane 0 0 0 cm 1 0 0"<<endl;
  myfile<<"#/vis/viewer/addCutawayPlane 0 -100 0 cm 0 1 0"<<endl;
  myfile<<"#/vis/viewer/set/viewpointThetaPhi -90 0"<<endl;
  myfile<<"#/vis/viewer/zoomTo 10"<<endl;
  myfile<<"#/vis/viewer/panTo 10 0\n"<<endl;

  myfile<<"#/rat/proc lesssimpledaq"<<endl;
  myfile<<"#/rat/proc count"<<endl;
  myfile<<"#/rat/procset update 1\n"<<endl;

  myfile<<"#/rat/proc simpledaq"<<endl;
  myfile<<"# - takes all the hits and just puts them all in\n"<<endl;

  myfile<<"#/rat/proc bonsai"<<endl;
  myfile<<"#/rat/proc buttondaq"<<endl;
  myfile<<"/rat/proc forcedtrigger\n"<<endl;

  myfile<<"# Use IO.default_output_filename\n"<<endl;

  myfile<<"## OUTPUT PYTHON-LIKE FORMAT"<<endl;
  myfile<<"## Needs RAT2"<<endl;
  myfile<<"/rat/proclast outntuple"<<endl;
  myfile<<"/rat/procset include_tracking 1"<<endl;
  myfile<<"/rat/procset include_mcparticles 1"<<endl;
  myfile<<"/rat/procset include_pmthits 1"<<endl;
  myfile<<"/rat/procset include_untriggered_events 1"<<endl;
  myfile<<"/rat/procset include_digitizerhits 1"<<endl;
  myfile<<"/rat/procset include_digitizerfits 1"<<endl;
  myfile<<"#/rat/procset include_digitizerwaveforms 1"<<endl;
  myfile<<"/rat/procset file \""+filepath+"ntuple_root/coincidenceTest"<<num_ph<<"Photons_lb.ntuple.root\"\n"<<endl;

  myfile<<"# Need waveform analysis in order to get things like digitNhits stored in the ntuple"<<endl;
  myfile<<"#/rat/proc WaveformPrep\n"<<endl;

  myfile<<"##### GENERATORS #################\n"<<endl;
  
  myfile<<"#/generator/add combo pray:point:poisson\n"<<endl;
  
  myfile<<"# Lower diffuser # DA37"<<endl;
  myfile<<"#/generator/vtx/set 150 405 90 90"<<endl;
  myfile<<"#/generator/pos/set 70.0 -1358.8 -540.0\n"<<endl;
  
  myfile<<"# x = 0+70mm, position of diffuser relative to frame"<<endl;
  myfile<<"# y = -1487.5+9 + 19.2mm to go past liner as there are no holes for diffusers yet"<<endl;
  myfile<<"# z = -540mm for lower half diffuser\n"<<endl;
 
  myfile<<"#/generator/rate/set 1e7\n"<<endl;

  myfile<<"# Upper diffuser opposite: # DA38"<<endl;
  myfile<<"#/generator/vtx/set 20000 405 90 -90"<<endl;
  myfile<<"#/generator/pos/set -70.0 1358.8 540.0\n"<<endl;
  
  myfile<<"# Perpendicular lower diffuser needs to be clockwise (looking down) from the other lower diffuser: # DA40"<<endl;
  myfile<<"#/generator/vtx/set 20000 405 90 0"<<endl;
  myfile<<"#/generator/pos/set -1358.8 -70.0 -540.0\n"<<endl;
  
  myfile<<"# Perpendicular upper diffuser needs to be opposite the previous one: # DA34"<<endl;
  myfile<<"#/generator/vtx/set 20000 405 90 180"<<endl;
  myfile<<"#/generator/pos/set 1358.8 70.0 540.0\n"<<endl;
  
  myfile<<"/generator/add combo pbomb:point:uniform"<<endl;
  myfile<<"/generator/vtx/set "<<num_ph<<" 405"<<endl;
  myfile<<"/generator/pos/set 640. 0. 860."<<endl;
  myfile<<"/generator/rate/set 1e4"<<endl;
  myfile<<"#/generator/add combo gun2:point:poisson"<<endl;
  myfile<<"#/generator/vtx/set e+ 0 0 0 0 0.1 0.10 #this is a positron fired in the uniformly form the center of the tank between the energy 1mev to 10mev"<<endl;
  myfile<<"#/generator/pos/set 0.0 0.0 0.0 #detector+"<<endl;
  myfile<<"#/generator/rate/set 1\n"<<endl;
  
  myfile<<"#/rat/proc outroot\n"<<endl;
  
  myfile<<"##### RUN ###########"<<endl;
  myfile<<"/run/beamOn "<<nEv<<endl;

}
