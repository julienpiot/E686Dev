/*
Stano:
15.3. - 	Defining of more alphas w/o seleecting of alpha-alpha correlation
	search caused unwanted search of alpha-alpha correlation. This was
	fixed by testing of fNumAl and assigning to correct value.
*/


#include "MoDSSCorrelParameter.h"
#include <fstream>
#include <iostream>

using namespace std;

//***********************************************************
// default constructor

MoDSSCorrelParameter::MoDSSCorrelParameter() 
: TGo4Parameter()
{ 
  //cout << "MoDSSCorrelParameter - Do not use default constructor\n";
}

//***********************************************************
// constructor
// reads correlation parameters from the file MoDSS-correl.dat

MoDSSCorrelParameter::MoDSSCorrelParameter(const char* name) 
: TGo4Parameter(name)
{
  ifstream    file;
  
  file.open("MoDSS-correl.dat");
  if (file.fail()) {
    cout << "ERROR: MoDSSCorrelParameter - Could not open file: MoDSS-correl.dat ! Params set to 0\n";
    fbToScr    = 0;
    fbToFile   = 0;
    strcpy(fCorFile,"NoFile");
    fbCorReAl  = 0;
    fbCorReFi  = 0;
    fbCorAlAl  = 0;
    fbCorAlFi  = 0;
    fbCorReGa  = 0;
    fbCorAlGa  = 0;
    fbCorGaAl  = 0;
    fbCorGaFi  = 0;
    fbCorGaGa  = 0;
    fbBack     = 0;
    fbEnXorY   = 0;
    fNumAl     = 0;
    fNumPrt    = 0;
    fbTopAndBot= 0;
    fbTopOrBot = 0;
    fbOnlyTop  = 0;
    fbOnlyBot  = 0;
    fReTableSize = 0;
    fAlTableSize = 0;
    fGaTableSize = 0;
    fbReEL     = 0;
    fbReEH     = 0;
    fbReELpoly = 0;
    fbReEHpoly = 0;
    fReElLow   = 0;
    fReEhLow   = 0;
    fReElHigh  = 0;
    fReEhHigh  = 0;
    fReTOFLow  = 0;
    fReTOFHigh = 0;
    fbFiPause  = 0;
    fFiEl      = 0;
    fFiEh      = 0;
    fFiTl      = 0;
    fFiTh      = 0;
		// 1st gamma
    fbGaPause  = 0;
		fbDetOnlyOne = 0;
		fbDetAtLeastOne = 0;
		fbDetDiagonal = 0;
		fbGammaHigh = 0;
    fGaEl      = 0;
    fGaEh      = 0;
		fbElectronReq = 0;
    fElectronEl= 0;
    fElectronEh= 0;   
    fGaTl      = 0;
    fGaTh      = 0;
		// 2nd gamma
		fbGa2Pause  = 0;
		fGa2El      = 0;
    fGa2Eh      = 0;
		fbElectron2Req = 0;
    fElectron2El= 0;
    fElectron2Eh= 0;   
    fGa2Tl      = 0;
    fGa2Th      = 0;
		
    fbAlPause  = 0;
    fbAlOnePause = 0;
    for (Int_t i = 0; i < MAXALPHA; i++) {
      fAlEl[i] = 0;
      fAlEh[i] = 0;
      fAlTl[i] = 0;
      fAlTh[i] = 0;
    }
  }
  else {
    cout << "MoDSSCorrelParameter - reading from : MoDSS-correl.dat\n";
// Print correlation on screen ? To file ? Filename.
    if (IsData(file)) file >> fbToScr >> fbToFile >> fCorFile;
// What correlations to look for ? Re-Al; Re-Fi; Al-Al; Al-Fi, 
//	Re-Ga, Al-Ga, Ga-Al, Ga-Fi, Ga-Ga
    if (IsData(file)) file >> fbCorReAl >> fbCorReFi >> fbCorAlAl >> fbCorAlFi;
		if (IsData(file)) file >>fbCorReGa >> fbCorAlGa >> fbCorGaAl >> fbCorGaFi >> fbCorGaGa;
// Use back detector in correlations ?
    if (IsData(file)) file >> fbBack;
// Use (X or Y) in low energy correlations? (Otherwise only X energy is used)
    if (IsData(file)) file >> fbEnXorY;    
// Number of alphas. Minimum number of chain members to print the chain
    if (IsData(file)) file >> fNumAl >> fNumPrt;
		cout << "Reading number of alpha " << fNumAl <<endl;
    // Test of the situation when more alpha generation are defined 
    // and no alpha alpha correlation search is set (this situation may cause crash)
    // if yes - only the firstgeneration is used for the correlation search
    if ((fNumAl > 1) && (!fbCorGaAl && !fbCorAlGa && !fbCorAlAl)) {
    fNumAl = 1;
    cout << "WARNING: More alpha generations defined but no Alpha-Alpha correlation selected" << endl;
    cout << "         The number of alpha generation is changed to value NumAl ="<<fNumAl<<endl;
    cout << "         Only first alpha generation is taken" << endl;
    }
// when al-al corr are activated, the number of alphas sholud be larger than 1
    if ((fbCorAlAl) && (fNumAl < 2)) 
      cout << "When al-al correlation are activated, the number of alphas should be larger than 1";
		if ((fbCorGaAl && fbCorAlGa) && (fNumAl < 2))
			cout << "When Al-Ga and Ga-Al correlation are activated, the number of alphas should be larger than 1";
// which position shall be tested ? (only one choise possible)
// top and bottom # top or bottom (default) # only top # only bottom
    if (IsData(file)) file >> fbTopAndBot >> fbTopOrBot >> fbOnlyTop >> fbOnlyBot;
// size of the recoil table # size of the alpha table
    if (IsData(file)) file >> fReTableSize >> fAlTableSize >> fGaTableSize;
// recoil parameters
// recoil in low energy branch ? low energy limit ; high energy limit [keV]
    if (IsData(file)) file >> fbReEL >> fbReELpoly >> fReElLow >> fReEhLow;
// recoil in high energy branch ? low energy limit ; high energy limit [ch]	 
    if (IsData(file)) file >> fbReEH >> fbReEHpoly >> fReElHigh >> fReEhHigh;
// recoil TOF window
    if (IsData(file)) file >> fReTOFLow >> fReTOFHigh;
// fission parameters
// fission during pause ?
    if (IsData(file)) file >> fbFiPause;
// fission energy window:  from - to [ch]
    if (IsData(file)) file >> fFiEl >> fFiEh;
// fission time window: from - to [ms]
    if (IsData(file)) file >> fFiTl >> fFiTh;
// gamma parameters
// gamma during pause ?
    if (IsData(file)) file >> fbGaPause >> fbGammaHigh;
		// how to consider gamma clovers?
		if (IsData(file)) file >> fbDetOnlyOne >> fbDetAtLeastOne >> fbDetDiagonal;
// gamma energy window:  from - to [ch]
    if (IsData(file)) file >> fGaEl >> fGaEh;
// bit for electron requirement and electron energy window:  from - to [ch]
    if (IsData(file)) file >> fbElectronReq >> fElectronEl >> fElectronEh;    
// gamma time window: from - to [ms]
    if (IsData(file)) file >> fGaTl >> fGaTh;  
//=============================================== 
// gamma2 parameters
// gamma2 during pause ?
    if (IsData(file)) file >> fbGa2Pause; 
// gamma2 energy window:  from - to [ch]
    if (IsData(file)) file >> fGa2El >> fGa2Eh;
// bit for electron requirement and electron energy window for 2nd gamma:  from - to [ch]
    if (IsData(file)) file >> fbElectron2Req >> fElectron2El >> fElectron2Eh;    
// gamma2 time window: from - to [ms]
    if (IsData(file)) file >> fGa2Tl >> fGa2Th; 
//===============================================
// alpha parameters
// all alphas in pause ? one alpha from the correlation pair in the pause ?
    if (IsData(file)) file >> fbAlPause >> fbAlOnePause;
// alpha generations (fNumAl)
// to be consistent within code, count alpha generation from 1. (0 is recoil)
// energy window [keV] and time window [ms]
    for (Int_t i = 1; i <= fNumAl; i++)
      if (IsData(file)) file >> fAlEl[i] >> fAlEh[i] >> fAlTl[i] >> fAlTh[i];
// set remaining alpha generation parameters to 0
    fAlEl[0] = 0; fAlEh[0] = 0; fAlTl[0] = 0; fAlTh[0] = 0;
    for (Int_t i = fNumAl+1; i < MAXALPHA; i++) {
      fAlEl[i] = 0; fAlEh[i] = 0; fAlTl[i] = 0; fAlTh[i] = 0;
    }

    if (file.fail()) cout << "ERROR reading MoDSS-correl.dat\n";
  }
  file.close();
}

//***********************************************************
// default destructor

MoDSSCorrelParameter::~MoDSSCorrelParameter() 
{
}

//***********************************************************

//-----------------------------------------------------------

Int_t MoDSSCorrelParameter::PrintParameter(const char* n, Int_t){
  cout << "\n\tParameter " << GetName() << " : \n";
  
  if (fbToScr) cout << "Correlations printed to screen\n";
  if (fbToFile) cout << "Correlations printed to file: " << fCorFile << endl;
  cout << "Searching for: \n";
  cout << "\tRecoil - Alpha: ";
  if (fbCorReAl) cout << "YES\n";
  else cout << "NO\n";
  cout << "\tRecoil - Fission: ";
  if (fbCorReFi) cout << "YES\n";
  else cout << "NO\n";
  cout << "\tAlpha - Alpha: ";
  if (fbCorAlAl) cout << "YES\n";
  else cout << "NO\n";
  cout << "\tAlpha - Fission: ";
  if (fbCorAlFi) cout << "YES\n";
  else cout << "NO\n";
  cout << "\tRecoil - Gamma: ";
  if (fbCorReGa) cout << "YES\n";
  else cout << "NO\n";
  cout << "\tAlpha - Gamma: ";
  if (fbCorAlGa) cout << "YES\n";
  else cout << "NO\n";  
  cout << "\tGamma - Alpha: ";
  if (fbCorGaAl) cout << "YES\n";
  else cout << "NO\n"; 
  cout << "\tGamma - Fission: ";
  if (fbCorGaFi) cout << "YES\n";
  else cout << "NO\n";   
	cout << "\tGamma - Gamma: "; 
	if (fbCorGaGa) cout << "YES\n";
  else cout << "NO\n";   
  cout << "Use back detector in correlations ?  ";
  if (fbBack) cout << "YES\n";
  else cout << "NO\n";
  cout << "Use (X or Y) energy in low energy correlations?  ";
  if(fbEnXorY) cout << "YES\n";
  else cout << "Only X energy is used\n";
  cout << "Number of alphas in chain: " << fNumAl << endl;
  cout << "Minimum number of chain members to print the chain: " << fNumPrt << endl;
  cout << "Tested position: ";
  if (fbTopAndBot) cout << "IDX AND IDY\n";
  else if (fbTopOrBot) cout << "IDX OR IDY\n";
  else if (fbOnlyTop) cout << "ONLY IDX\n";
  else if (fbOnlyBot) cout << "ONLY IDY\n";
  cout << "Size of the Recoil Table: " << fReTableSize << endl;
  cout << "Size of the Alpha Table: " << fAlTableSize << endl;
  cout << "Size of the Gamma Table: " << fGaTableSize << endl;  
  cout << "\tRECOIL: \n";
  if (fbReEL && !fbReELpoly){
    cout << "Recoil in low energy branch: " << fReElLow << " - " << fReEhLow << " keV\n";
		cout << "TOF window: " << fReTOFLow << " - " << fReTOFHigh << endl;
		}
  if (fbReEH && !fbReELpoly){
    cout << "Recoil in high energy branch: " << fReElHigh << " - " << fReEhHigh << " ch\n";
  	cout << "TOF window: " << fReTOFLow << " - " << fReTOFHigh << endl;
		}
		
	if (fbReELpoly)
		cout<<"Polygonal window for low  energy range will be used"<<endl;
	if (fbReEHpoly)
		cout<<"Polygonal window for high energy range will be used"<<endl;		

  cout << "\tFISSION: \n";
  cout << "Fission during pause ? ";
  if (fbFiPause) cout << "YES\n";
  else cout << "NO\n";
  cout << "fission energy window: " << fFiEl << " - " << fFiEh << " [ch]\n";
  cout << "fission time window:" << fFiTl << " - " << fFiTh << " [ms]\n";
	// gamma1 part
	//===============================================
  cout << "\tGAMMA 1: \n";  
  cout << "Gamma 1 during pause ? ";
  if (fbGaPause) cout << "YES\n";
  else cout << "NO\n";  
	cout << "Gamma 1 in high energy branch ? ";
  if (fbGammaHigh) cout << "YES\n";
  else cout << "NO\n"; 
	if (fbDetOnlyOne) cout << "Only in one clover must fire\n";
	else if (fbDetDiagonal) cout << "No coincidence in neighbouring clovers is allowed (only diagonal)\n";
	else if (fbDetAtLeastOne) cout << "At least one clover should fire (coincidence allowed)\n";
  cout << "gamma 1 energy window: " << fGaEl << " - " << fGaEh << " [ch]\n";
	if (fbElectronReq)
  cout << "Electron energy - coincidencing with gamma 1: " << fElectronEl << " - " << fElectronEh << " [ch]\n";
  cout << "gamma 1 time window:" << fGaTl << " - " << fGaTh << " [ms]\n";  
	// ================================
	// gamma2 part
	//===============================================
  cout << "\tGAMMA 2: \n";  
  cout << "Gamma 2 during pause ? ";
  if (fbGa2Pause) cout << "YES\n";
  else cout << "NO\n";  
  cout << "gamma 2 energy window: " << fGa2El << " - " << fGa2Eh << " [ch]\n";
	if (fbElectron2Req)
  cout << "Electron energy - coincidencing with gamma 2: " << fElectron2El << " - " << fElectron2Eh << " [ch]\n";
  cout << "gamma 2 time window:" << fGa2Tl << " - " << fGa2Th << " [ms]\n";  
	// ================================
  cout << "\tALPHA: \n";
  cout << "all alphas in pause ? ";
  if (fbAlPause) cout << "YES\n";
  else cout << "NO\n";
  cout << "one alpha from the correlation pair in the pause ? ";
  if (fbAlOnePause) cout << "YES\n";
  else cout << "NO\n";
// energy window [keV] and time window [ms]
  for (Int_t i = 1; i <= fNumAl; i++) {
    cout << "gen: " << i << " energy window: " << fAlEl[i] << " - " 
         << fAlEh[i] << " keV ; time window: " << fAlTl[i] << " - " 
         << fAlTh[i] << " ms\n";
  }
  
  return 0;
}

//-----------------------------------------------------------

Bool_t MoDSSCorrelParameter::UpdateFrom(TGo4Parameter *pp){
  if(pp->InheritsFrom("MoDSSCorrelParameter"))
  {
    MoDSSCorrelParameter *from = (MoDSSCorrelParameter *) pp;

    fbToScr     = from->fbToScr;
    fbToFile    = from->fbToFile;
    //fCorFile    = from->fCorFile;
    strcpy(fCorFile, from->fCorFile);
    fbCorReAl   = from->fbCorReAl;
    fbCorReFi   = from->fbCorReFi;
    fbCorAlAl   = from->fbCorAlAl;
    fbCorAlFi   = from->fbCorAlFi;
    fbCorReGa   = from->fbCorReGa;
    fbCorAlGa   = from->fbCorAlGa;    
    fbCorGaAl   = from->fbCorGaAl;
    fbCorGaFi   = from->fbCorGaFi;
		fbCorGaGa   = from->fbCorGaGa;
    fbBack      = from->fbBack;
    fbEnXorY    = from->fbEnXorY;
    fNumAl      = from->fNumAl;
    // Test of the situation when more alpha generation are defined 
    // and no alpha alpha correlation search is set (this situation may cause crash)
    // if yes - only the firstgeneration is used for the correlation search
    if ((fNumAl > 1) && (!fbCorGaAl && !fbCorAlGa && !fbCorAlAl)) { 
    	fNumAl = 1;
    	cout << "WARNING: More alpha generations defined but no Alpha-Alpha correlation selected" << endl;
    	cout << "         The number of alpha generation is changed to value NumAl ="<<fNumAl<<endl;
    	cout << "         Only first alpha generation is taken" << endl;
    }
    fNumPrt     = from->fNumPrt;
    fbTopAndBot = from->fbTopAndBot;
    fbTopOrBot  = from->fbTopOrBot;
    fbOnlyTop   = from->fbOnlyTop;
    fbOnlyBot   = from->fbOnlyBot;
    fReTableSize = from->fReTableSize;
    fAlTableSize = from->fAlTableSize;
    fGaTableSize = from->fGaTableSize;    
    fbReEL      = from->fbReEL;
    fbReEH      = from->fbReEH;
    fbReELpoly  = from->fbReELpoly;
    fbReEHpoly  = from->fbReEHpoly;
    fReElLow    = from->fReElLow;
    fReEhLow    = from->fReEhLow;
    fReElHigh   = from->fReElHigh;
    fReEhHigh   = from->fReEhHigh;
    fReTOFLow   = from->fReTOFLow;
    fReTOFHigh  = from->fReTOFHigh;
    fbFiPause   = from->fbFiPause;
    fFiEl       = from->fFiEl;
    fFiEh       = from->fFiEh;
    fFiTl       = from->fFiTl;
    fFiTh       = from->fFiTh;
    fbGaPause   = from->fbGaPause;
		fbDetAtLeastOne = from->fbDetAtLeastOne;
		fbDetOnlyOne = from->fbDetOnlyOne;
		fbDetDiagonal = from->fbDetDiagonal;
		fbGammaHigh  = from->fbGammaHigh;
    fGaEl       = from->fGaEl;
    fGaEh       = from->fGaEh;
		fbElectronReq = from->fbElectronReq;
    fElectronEl = from->fElectronEl;
    fElectronEh = from->fElectronEh;    
    fGaTl       = from->fGaTl;
    fGaTh       = from->fGaTh;    
// ===========================
    fbGa2Pause   = from->fbGa2Pause;
    fGa2El       = from->fGa2El;
    fGa2Eh       = from->fGa2Eh;
		fbElectron2Req = from->fbElectron2Req;
    fElectron2El = from->fElectron2El;
    fElectron2Eh = from->fElectron2Eh;    
    fGa2Tl       = from->fGa2Tl;
    fGa2Th       = from->fGa2Th; 
// ==============================
    fbAlPause   = from->fbAlPause;
    fbAlOnePause = from->fbAlOnePause;
    for (Int_t i = 0; i < MAXALPHA; i++) {
      fAlEl[i]  = from->fAlEl[i];
      fAlEh[i]  = from->fAlEh[i];
      fAlTl[i]  = from->fAlTl[i];
      fAlTh[i]  = from->fAlTh[i];
    }
    cout << "MoDSSCorrelParameter - Parameter : " << GetName() << " UPDATED\n";
  }
  else
    cout << "Wrong parameter object: " << pp->ClassName() << endl;
  return kTRUE;
}

// ==========================================================
// function IsData skips comment lines (begining with #) in file f

int MoDSSCorrelParameter::IsData(ifstream &f) {
  char dum;
  char dumstr[300];
  int retval = 0;

  /* 'operator >>' does not read End-of-line, therefore check if read 
      character is not EOL (10) */
  do {
    dum = f.get();
    if (dum == '#')    // comment line => read whole line and throw it away
      f.getline(dumstr,300);
  }
  while ((dum == '#') || ((int)dum == 10)); 

  f.unget();   // go one character back
  retval = 1;
  return retval;
}

//-----------------------------------------------------------
// save current parameter to file (MoDSS-correl.dat assumed)

void MoDSSCorrelParameter::SaveToFile() {
  ofstream  f;
  Text_t dumstr[300];
  
  cout << "Saving CorrelParameter to file ... " << endl;
  
  f.open("MoDSS-correl.dat");
  if (f.fail()) {
    cout << "ERROR - MoDSSCorrelParameter - Cannot open file MoDSS-correl.dat !!!" << endl;
    return;
  }
  
  f << "# Input file for MoDSS correlation analysis \n";
  f << "# - these are only initial settings. parameter values \n";
  f << "#   should be changed from within UserGUI in Go4 \n";
  f << "# - use this file to set parameter values when Go4 \n";
  f << "#   analysis is running in the batch mode \n";
  f << "# \n";
  f << "# comment lines are starting with '#' \n";
  f << "# --------------------------------------------- \n";
  f << "# \n";
  f << "# --------------------------------------------- \n";
  f << "# \n";
  f << "# Print correlation on screen ? (1=yes 0=no) # To file ? # filename \n";
  f << fbToScr << "\t" << fbToFile << "\t" << fCorFile << endl;
  f << "# What correlations to look for ? \n";
  f << "# When looking just for Re-Al or Al-Fi correlation with one alpha, \n";
  f << "# be sure to set number of alphas to 1 !!! (otherwise the search will \n";
  f << "# continue with further generations) \n";
  f << "# Re-Al   # Re-Fi  # Al-Al  # Al-Fi \n";
  f << fbCorReAl << "\t" << fbCorReFi << "\t" << fbCorAlAl << "\t" << fbCorAlFi << endl;
	f << "# Re-Ga # Al-Ga # Ga-Al # Ga-Fi # Ga-Ga \n";
	f << fbCorReGa << "\t" << fbCorAlGa << "\t" << fbCorGaAl << "\t" << fbCorGaFi << "\t" << fbCorGaGa << endl;
  f << "# Use back detector in correlations ?\n";
  f << fbBack << endl;
  f << "# Use (X or Y) energy in low energy correlations ? (Otherwise only X energy is used)\n";
  f << fbEnXorY << endl;
  f << "# Number of alphas ?  # minimum number of chain members to print the chain \n";
  f << fNumAl << "\t" << fNumPrt << endl;
  f << "# which position shall be tested ? (only one choise possible) \n";
  f << "# IDX and IDY # IDX or IDY (default) # only IDX # only IDY \n";
  f << fbTopAndBot << "\t" << fbTopOrBot << "\t" << fbOnlyTop << "\t" << fbOnlyBot << endl;
  f << "# size of the recoil table (defaul=5000) # size of the alpha table (default=5000) # size of the gamma table (default=5000) \n";
  f << fReTableSize << "\t" << fAlTableSize << "\t" << fGaTableSize << endl;
  f << "# recoil parameters \n";
  f << "# recoil in low energy branch ? # used poly low energy window? # low energy limit # high energy limit [keV] \n";
  f << fbReEL << "\t" << fbReELpoly << "\t" << fReElLow << "\t" << fReEhLow << endl;
  f << "# recoil in high energy branch ? # used poly high energy window? # low energy limit # high energy limit [ch] \n";
  f << fbReEH << "\t" << fbReEHpoly << "\t" << fReElHigh << "\t" << fReEhHigh << endl;
  f << "# recoil low TOF limit # high TOF limit \n";
  f << fReTOFLow << "\t" << fReTOFHigh << endl;
  f << "# fission parameters (should be present also when they are not needed) \n";
  f << "# fission during the pause ?\n";
  f << fbFiPause << endl;
  f << "# fission energy window:  from - to [ch] \n";
  f << fFiEl << "\t" << fFiEh << endl;
  f << "# fission time window: from - to [ms] \n";
  f << fFiTl << "\t" << fFiTh << endl;
  f << "# gamma parameters \n";  
  f << "# gamma during the pause? (1- yes , 0 - no) #search gamma in the high energy branch \n";
  f << fbGaPause << "\t" << fbGammaHigh << endl;  
	f << "# How to consider clovers?\n";
	f << "# Only one detektor # At least one detektor # Diagonal crystal taken\n";
	f << fbDetOnlyOne << "\t" << fbDetAtLeastOne << "\t" << fbDetDiagonal << endl;
  f << "# gamma energy window:  from - to [keV] \n";
  f << fGaEl << "\t" << fGaEh << endl;
  f << "# Electron required? # Electron energy - coincidencing with gamma: from - to [keV] \n";
  f << fbElectronReq << "\t" << fElectronEl << "\t" << fElectronEh << endl;
  f << "# gamma time window: from - to [ms] \n";
  f << fGaTl << "\t" << fGaTh << endl;
  f << "# gamma 2 parameters \n";  
  f << "# gamma 2 during the pause? (1- yes , 0 - no) \n";
  f << fbGa2Pause << endl;  
  f << "# gamma 2 energy window:  from - to [keV] \n";
  f << fGa2El << "\t" << fGa2Eh << endl;
  f << "# Electron required? # Electron energy - coincidencing with gamma 2: from - to [keV] \n";
  f << fbElectron2Req << "\t" << fElectron2El << "\t" << fElectron2Eh << endl;
  f << "# gamma 2 time window: from - to [ms] \n";
  f << fGa2Tl << "\t" << fGa2Th << endl;    
  f << "# alpha parameters \n";
  f << "# all alphas in pause ? # one alpha from the correlation pair in the pause ? \n";
  f << fbAlPause << "\t" << fbAlOnePause << endl;
  f << "# next line MUST BE repeated for each alpha generation (number of alphas given above) \n";
  f << "# energy window: from - to [keV] # time window: from - to [ms] \n";
  for (Int_t i = 1; i < MAXALPHA; i++) {
    sprintf(dumstr, "# %d alpha generation",i);
    f << dumstr << endl;
    f << fAlEl[i] << "\t" << fAlEh[i] << "\t" << fAlTl[i] << "\t" << fAlTh[i] << endl;
  }
  
  f.close();
}

// ----------------------------------------------------------

ClassImp(MoDSSCorrelParameter)

