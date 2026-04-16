Collection of python codes used for analysing ntuple.root files from ratpac-two simulations for the BUTTON experiment.

Calibration Analysis Cleaner is the main calibration analysis script, Calibration Analysis Testing is older and some parts are out of date, but has some functions that may be useful.

Coincidence Study uses results from Root File Reader to look at the rate of multiple photons hitting the same PMT within a sample width.

Root File Reader also has some general ntuple.root processing using uproot.



# Instructions for running programs
Added by Emma Ellingwood 16/04/26
**Note :** Daniel wrote detailed breakdowns of the functions in the different analyses in his [Documentation for Codes](https://stfc365.sharepoint.com/:w:/r/sites/BoleynPrototype/_layouts/15/Doc.aspx?sourcedoc=%7B7C4FAD90-35EA-43A2-B98A-B7C8671E6622%7D&file=Documentation%20for%20Codes.docx&action=default&mobileredirect=true).

## Coincidence Study
1. Open *make_all_coincidence_macros.C*
2. Change the input_filepath where the macro is located and num_simulated, the number of simulated photons, if desired
3. You can go to the make_macros() function and change anything about the macro that you want
4. This code creates all of the macros in that specified directory, creates a script to run all of those macros and also creates a script to read 
5. Type `bash run_coincidence_macros.sh` in terminal to run all of the macros for different numbers of photons, will take a while, that will save the output ntuple.root files in the specified directory in a sub-directory called ntuple_root. These can be run separately if you want.
6. Type `bash run_read_root_file.sh` to take all of the files in the ntuple_root sub-directory and runs the coincidence. The input file path and the number of events should be filled in automatically from the format used to make the scripts in make_all_coincidence_macros.C. Outputs in coincidence sub-directory
7. In CoincidenceStudy.py change the number of events (num_events) and pmtNumber if you want. Run it by typing `python3 CoincidenceStudy.py`. Coincidence plot will be saved in the data directory with *coincidence_plot.png*.

### Additional notes
- make_all_coincidence_macros.C was created just to streamline the process of creating all of the macros and scripts necessary to run CoincidenceStudy.py. If you need to change something about the macros or scripts please change it here to it applies to all of them.
- Root_File_Reader.py has been modified from Daniel's version due to the automation of the process. It now takes in two arguments just for the file path and the total number of events simulated, these are both populated automatically if you use make_allcoincidence_macros.C and run Read_file_Reader using run_read_root_file.sh.
- A lot of the file path modifications I made assume that BUTTON-RAT2 and BUTTON-Python-Analysis-Codes are at the same directory level. If that is not true for you you will need to change that.

## Calibration Analysis
1. Open nonvis_integrationTestButtonWave.mac to change different macro properties like the output file name (/rat/procset file), number of particles simulated per event (/generator/vtx/set first number), total number of events simulated (/run/beamOn), properties of the diffuser and generator.
2. Run the macro by typing 'button nonvis_integrationTestButtonWave.mac' if you want to run it locally.
3. Open Calibration_Analysis_Cleaner.py and change file paths as necessary in the main function at the end of the file. The input .ntuple.root files are specified around lines 597 called fileNames. It can take multiple input files and each will be analysed separately.
4. Run the calibration analysis program by typing 'python3 Calibration_Analysis_Cleaner.py'. The output is essentially all printed to terminal.

### Additional Notes
- By default the macro creates an isotropic photon point source (photon bomb) with a default 150 photons of 405 nm per event.
- The pulse is integrated in the macro using the processor buttonWave which is created in BUTTON-RAT2/src/ButtonWaveformProc.cc so any changes that need to be done to the processor should happen here.
- Around lines 466-467 you have the variables useSaveData and saveCombinedData. Just set them to saveCombinedData=True and useSaveData=False. This is from the development stage when the integration was not done in the processor yet, no real need to touch it anymore.
