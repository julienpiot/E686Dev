#include <stdio.h>
#include "MoDSSPrintEventParameter.h"
#include <fstream>
#include <iostream>


using namespace std;


//***********************************************************
// default constructor

MoDSSPrintEventParameter::MoDSSPrintEventParameter() 
: TGo4Parameter("MoDSSPrintEventParameter")
{ 
  // cout << "MoDSSPrintEventParameter - Default constructor used, parameters set to 0\n";
  fbEnablePrinting = kFALSE;
  fbPrt2scr        = kFALSE;
  fbPrt2file       = kFALSE;
  strcpy(fPrtFilename,"default-output.dat");
  fbTime           = kFALSE;
  fTimeFrom        = 0.;
  fTimeTo          = 0.;
  fbEkev           = kFALSE;
  fEkevFrom        = 0.;
  fEkevTo          = 0.;
  fbEmev           = kFALSE;
  fEmevFrom        = 0.;
  fEmevTo          = 0.;
  fbTof            = kFALSE;
  fTofFrom         = 0.;
  fTofTo           = 0.;
  fbGamma          = kFALSE;
  fGammaFrom       = 0.;
  fGammaTo         = 0.;
  fbAllStrips      = kFALSE;
  fStripNo         = 0;
  fbXylt           = kFALSE;
  fXyltFrom        = 0.;
  fXyltTo          = 0.;
  fbXylb           = kFALSE;
  fXylbFrom        = 0.;
  fXylbTo          = 0.;
  fbXyht           = kFALSE;
  fXyhtFrom        = 0.;
  fXyhtTo          = 0.;
  fbXyhb           = kFALSE;
  fXyhbFrom        = 0.;
  fXyhbTo          = 0.;
  fbEkevbox        = kFALSE;
  fEkevboxFrom     = 0.;
  fEkevboxTo       = 0.;
  fbEmevbox        = kFALSE;
  fEmevboxFrom     = 0.;
  fEmevboxTo       = 0.;
  fbPause          = kFALSE;
  fbPulse          = kFALSE;
  fbAntitof        = kFALSE;
  fbAntiveto       = kFALSE;
  fbT2bit          = kFALSE;
  fbT3bit          = kFALSE;
  
  // variables to print
  fBtime           = kFALSE;
  fBtmp            = kFALSE;
  fBpausebit       = kFALSE;
  fBstrip          = kFALSE;
  fBekev           = kFALSE;
  fBxylt           = kFALSE;
  fBxylb           = kFALSE;
  fBemev           = kFALSE;
  fBxyht           = kFALSE;
  fBxyhb           = kFALSE;
  fBboxstrip       = kFALSE;
  fBekevbox        = kFALSE;
  fBemevbox        = kFALSE;
  fBegl1           = kFALSE;
  fBegl2           = kFALSE;
  fBegl3           = kFALSE;
  fBegl4           = kFALSE;
  fBegh1           = kFALSE;
  fBegh2           = kFALSE;
  fBegh3           = kFALSE;
  fBegh4           = kFALSE;
  fBtacga1         = kFALSE;
  fBtacga2         = kFALSE;
  fBtacga3         = kFALSE;
  fBtacga4         = kFALSE;
  fBtofbit         = kFALSE;
  fBtof            = kFALSE;
	fBtof1           = kFALSE;
	fBtof2           = kFALSE;
	fBtof3           = kFALSE;
  fBvetobit        = kFALSE;
  fBt2bit          = kFALSE;
  fBt3bit          = kFALSE;
  fBpu1            = kFALSE;
  fBpu2            = kFALSE;
  fBpu3            = kFALSE;
  fBpu4            = kFALSE;
  fBpu5            = kFALSE;
  fBpu6            = kFALSE;
  fBpu7            = kFALSE;
  fBpu8            = kFALSE;
}

//***********************************************************
// constructor

MoDSSPrintEventParameter::MoDSSPrintEventParameter(const char* name) 
: TGo4Parameter(name)
{
  ifstream    file;
  
  file.open("MoDSS-print.dat");
  if (file.fail()) {
    cout << "ERROR: MoDSSPrintEventParameter - Could not open file: MoDSS-print.dat ! Params set to 0\n";
    fbEnablePrinting = kFALSE;
    fbPrt2scr        = kFALSE;
    fbPrt2file       = kFALSE;
    strcpy(fPrtFilename,"default-output.dat");
    fbTime           = kFALSE;
    fTimeFrom        = 0.;
    fTimeTo          = 0.;
    fbEkev           = kFALSE;
    fEkevFrom        = 2000.;
    fEkevTo          = 4000.;
    fbEmev           = kFALSE;
    fEmevFrom        = 0.;
    fEmevTo          = 0.;
    fbTof            = kFALSE;
    fTofFrom         = 0.;
    fTofTo           = 0.;
    fbGamma          = kFALSE;
    fGammaFrom       = 0.;
    fGammaTo         = 0.;
    fbAllStrips      = kTRUE;
    fStripNo         = 8;
    fbXylt           = kFALSE;
    fXyltFrom        = 0.;
    fXyltTo          = 0.;
    fbXylb           = kFALSE;
    fXylbFrom        = 0.;
    fXylbTo          = 0.;
    fbXyht           = kFALSE;
    fXyhtFrom        = 0.;
    fXyhtTo          = 0.;
    fbXyhb           = kFALSE;
    fXyhbFrom        = 0.;
    fXyhbTo          = 0.;
    fbEkevbox        = kFALSE;
    fEkevboxFrom     = 0.;
    fEkevboxTo       = 0.;
    fbEmevbox        = kFALSE;
    fEmevboxFrom     = 0.;
    fEmevboxTo       = 0.;
    fbPause          = kFALSE;
    fbPulse          = kFALSE;
    fbAntitof        = kFALSE;
    fbAntiveto       = kFALSE;
    fbT2bit          = kFALSE;
    fbT3bit          = kFALSE;
    fBtime           = kTRUE;
    fBtmp            = kTRUE;
    fBpausebit       = kTRUE;
    fBstrip          = kFALSE;
    fBekev           = kTRUE;
    fBxylt           = kTRUE;
    fBxylb           = kTRUE;
    fBemev           = kFALSE;
    fBxyht           = kFALSE;
    fBxyhb           = kFALSE;
    fBboxstrip       = kFALSE;
    fBekevbox        = kFALSE;
    fBemevbox        = kFALSE;
    fBegl1          = kFALSE;
    fBegl2          = kFALSE;
    fBegl3          = kFALSE;
    fBegl4          = kFALSE;
    fBegh1          = kFALSE;
    fBegh2          = kFALSE;
    fBegh3          = kFALSE;
    fBegh4          = kFALSE;
    fBtacga1        = kFALSE;
    fBtacga2        = kFALSE;
    fBtacga3        = kFALSE;
    fBtacga4        = kFALSE;
    fBtofbit         = kFALSE;
    fBtof            = kFALSE;
    fBtof1            = kFALSE;
    fBtof2            = kFALSE;
    fBtof3            = kFALSE;
    fBvetobit        = kFALSE;
    fBt2bit          = kFALSE;
    fBt3bit          = kFALSE;
    fBpu1            = kFALSE;
    fBpu2            = kFALSE;
    fBpu3            = kFALSE;
    fBpu4            = kFALSE;
    fBpu5            = kFALSE;
    fBpu6            = kFALSE;
    fBpu7            = kFALSE;
    fBpu8            = kFALSE;
  }
  else {
    cout << "MoDSSPrintEventParameter - reading from : MoDSS-print.dat\n";
    if (IsData(file)) file >> fbEnablePrinting;
    if (IsData(file)) file >> fbPrt2scr;
    if (IsData(file)) file >> fbPrt2file >> fPrtFilename;
    if (IsData(file)) file >> fbTime >> fTimeFrom >> fTimeTo;
    if (IsData(file)) file >> fbEkev >> fEkevFrom >> fEkevTo;
    if (IsData(file)) file >> fbEmev >> fEmevFrom >> fEmevTo;
    if (IsData(file)) file >> fbTof >> fTofFrom >> fTofTo;
    if (IsData(file)) file >> fbGamma >> fGammaFrom >> fGammaTo;
    if (IsData(file)) file >> fbAllStrips >> fStripNo;
    if (IsData(file)) file >> fbXylt >> fXyltFrom >> fXyltTo;
    if (IsData(file)) file >> fbXylb >> fXylbFrom >> fXylbTo;
    if (IsData(file)) file >> fbXyht >> fXyhtFrom >> fXyhtTo;
    if (IsData(file)) file >> fbXyhb >> fXyhbFrom >> fXyhbTo;
    if (IsData(file)) file >> fbEkevbox >> fEkevboxFrom >> fEkevboxTo;
    if (IsData(file)) file >> fbEmevbox >> fEmevboxFrom >> fEmevboxTo;
    if (IsData(file)) file >> fbPause;
    if (IsData(file)) file >> fbPulse;
    if (IsData(file)) file >> fbAntitof;
    if (IsData(file)) file >> fbAntiveto;
    if (IsData(file)) file >> fbT2bit;
    if (IsData(file)) file >> fbT3bit;
    if (IsData(file)) file >> fBtime;
    if (IsData(file)) file >> fBtmp;
    if (IsData(file)) file >> fBpausebit;
    if (IsData(file)) file >> fBstrip;
    if (IsData(file)) file >> fBekev;
    if (IsData(file)) file >> fBxylt;
    if (IsData(file)) file >> fBxylb;
    if (IsData(file)) file >> fBemev;
    if (IsData(file)) file >> fBxyht;
    if (IsData(file)) file >> fBxyhb;
    if (IsData(file)) file >> fBboxstrip;
    if (IsData(file)) file >> fBekevbox;
    if (IsData(file)) file >> fBemevbox;
    if (IsData(file)) file >> fBegl1;
    if (IsData(file)) file >> fBegl2;
    if (IsData(file)) file >> fBegl3;
    if (IsData(file)) file >> fBegl4;
    if (IsData(file)) file >> fBegh1;
    if (IsData(file)) file >> fBegh2;
    if (IsData(file)) file >> fBegh3;
    if (IsData(file)) file >> fBegh4;
    if (IsData(file)) file >> fBtacga1;
    if (IsData(file)) file >> fBtacga2;
    if (IsData(file)) file >> fBtacga3;
    if (IsData(file)) file >> fBtacga4;
    if (IsData(file)) file >> fBtofbit;
    if (IsData(file)) file >> fBtof;
    if (IsData(file)) file >> fBtof1;
    if (IsData(file)) file >> fBtof2;
    if (IsData(file)) file >> fBtof3;
    if (IsData(file)) file >> fBvetobit;
    if (IsData(file)) file >> fBt2bit;
    if (IsData(file)) file >> fBt3bit;
    if (IsData(file)) file >> fBpu1;
    if (IsData(file)) file >> fBpu2;
    if (IsData(file)) file >> fBpu3;
    if (IsData(file)) file >> fBpu4;
    if (IsData(file)) file >> fBpu5;
    if (IsData(file)) file >> fBpu6;
    if (IsData(file)) file >> fBpu7;
    if (IsData(file)) file >> fBpu8;
    
    if (file.fail()) cout << "ERROR reading MoDSS-print.dat\n";
  }
  file.close();
}

//***********************************************************
// default destructor

MoDSSPrintEventParameter::~MoDSSPrintEventParameter() 
{
  // cout << "MoDSSPrintEventParameter - destructor used.\n";
}

//***********************************************************

//-----------------------------------------------------------

Int_t MoDSSPrintEventParameter::PrintParameter(const char* n, Int_t) 
{
  cout << "Parameter " << GetName() << " : \n";
  cout << "\t Event printout enabled ? : ";
  if (fbEnablePrinting) cout << "YES \n"; 
  else  cout << "NO \n";
  if (fbEnablePrinting) {
    cout << "\t Printing to screen ? : ";
    if (fbPrt2scr) cout << "YES. \n";
    else  cout << "NO \n";
    cout << "\t Printing to file ? : ";
    if (fbPrt2file) {
      cout << "YES. \t";
      cout << "Filename : " << fPrtFilename << endl;
    }
    else  cout << "NO \n";
    
    cout << "\t Time condition enabled ? : ";
    if (fbTime) {
      cout << "YES. \t";
      cout << "Time limits : " << fTimeFrom << " - " << fTimeTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Ekev condition enabled ? : ";
    if (fbEkev) {
      cout << "YES. \t";
      cout << "limits : " << fEkevFrom << " - " << fEkevTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Emev condition enabled ? : ";
    if (fbEmev) {
      cout << "YES. \t";
      cout << "limits : " << fEmevFrom << " - " << fEmevTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t TOF condition enabled ? : ";
    if (fbTof) {
      cout << "YES. \t";
      cout << "limits : " << fTofFrom << " - " << fTofTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Gamma condition enabled ? : ";
    if (fbTof) {
      cout << "YES. \t";
      cout << "limits : " << fGammaFrom << " - " << fGammaTo << " s\n";
    }
    else  cout << "NO \n";
    if (fbAllStrips) cout << "\t All strips used.\n";
    else cout << "Strip No. " << fStripNo << " used. \n";
    cout << "\t Xylt condition enabled ? : ";
    if (fbXylt) {
      cout << "YES. \t";
      cout << "limits : " << fXyltFrom << " - " << fXyltTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Xylb condition enabled ? : ";
    if (fbXylb) {
      cout << "YES. \t";
      cout << "limits : " << fXylbFrom << " - " << fXylbTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Xyht condition enabled ? : ";
    if (fbXyht) {
      cout << "YES. \t";
      cout << "limits : " << fXyhtFrom << " - " << fXyhtTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Xyhb condition enabled ? : ";
    if (fbXyhb) {
      cout << "YES. \t";
      cout << "limits : " << fXyhbFrom << " - " << fXyhbTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Ekevbox condition enabled ? : ";
    if (fbEkevbox) {
      cout << "YES. \t";
      cout << "limits : " << fEkevboxFrom << " - " << fEkevboxTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Emevbox condition enabled ? : ";
    if (fbEmevbox) {
      cout << "YES. \t";
      cout << "limits : " << fEmevboxFrom << " - " << fEmevboxTo << " s\n";
    }
    else  cout << "NO \n";
    cout << "\t Events in pause ? : ";
    if (fbPause) cout << "YES \n"; 
    else  cout << "NO \n";
    cout << "\t Events in pulse ? : ";
    if (fbPulse) cout << "YES \n"; 
    else  cout << "NO \n";
    cout << "\t Events in antitof ? : ";
    if (fbAntitof) cout << "YES \n"; 
    else  cout << "NO \n";
    cout << "\t Events in antiveto ? : ";
    if (fbAntiveto) cout << "YES \n"; 
    else  cout << "NO \n";
    cout << "\t Events in T2 time ? : ";
    if (fbT2bit) cout << "YES \n"; 
    else  cout << "NO \n";
    cout << "\t Events in T3 time ? : ";
    if (fbT3bit) cout << "YES \n"; 
    else  cout << "NO \n";
    cout << "    PARAMETERS TO PRINT : ";
    if (fBtime) cout << "\t time \n";
    if (fBtmp) cout << "\t tmp \n";
    if (fBpausebit) cout << "\t pausebit \n";
    if (fBstrip) cout << "\t strip \n";
    if (fBekev) cout << "\t ekev \n";
    if (fBxylt) cout << "\t xylt \n";
    if (fBxylb) cout << "\t xylb \n";
    if (fBemev) cout << "\t emev \n";
    if (fBxyht) cout << "\t xyht \n";
    if (fBxyhb) cout << "\t xyhb \n";
    if (fBboxstrip) cout << "\t boxstrip \n";
    if (fBekevbox) cout << "\t ekev box \n";
    if (fBemevbox) cout << "\t emev box \n";
    if (fBegl1) cout << "\t egl1 \n";
    if (fBegl2) cout << "\t egl2 \n";
    if (fBegl3) cout << "\t egl3 \n";
    if (fBegl4) cout << "\t egl4 \n";
    if (fBegh1) cout << "\t egh1 \n";
    if (fBegh2) cout << "\t egh2 \n";
    if (fBegh3) cout << "\t egh3 \n";
    if (fBegh4) cout << "\t egh4 \n";
    if (fBtacga1) cout << "\t tacga1 \n";
    if (fBtacga2) cout << "\t tacga2 \n";
    if (fBtacga3) cout << "\t tacga3 \n";
    if (fBtacga4) cout << "\t tacga4 \n";
    if (fBtofbit) cout << "\t tofbit \n";
    if (fBtof) cout << "\t tof \n";
    if (fBtof1) cout << "\t tof1 \n";
    if (fBtof2) cout << "\t tof2 \n";
    if (fBtof3) cout << "\t tof3 \n";
    if (fBvetobit) cout << "\t vetobit \n";
    if (fBt2bit) cout << "\t t2bit \n";
    if (fBt3bit) cout << "\t t3bit \n";
    if (fBpu1) cout << "\t pu1 \n";
    if (fBpu2) cout << "\t pu2 \n";
    if (fBpu3) cout << "\t pu3 \n";
    if (fBpu4) cout << "\t pu4 \n";
    if (fBpu5) cout << "\t pu5 \n";
    if (fBpu6) cout << "\t pu6 \n";
    if (fBpu7) cout << "\t pu7 \n";
    if (fBpu8) cout << "\t pu8 \n";
  }
  return 0;
}

//-----------------------------------------------------------

Bool_t MoDSSPrintEventParameter::UpdateFrom(TGo4Parameter *pp) {
  
  if (pp->InheritsFrom("MoDSSPrintEventParameter")) {
    
    MoDSSPrintEventParameter * from = (MoDSSPrintEventParameter *) pp;
    
    fbEnablePrinting = from->fbEnablePrinting;
    fbPrt2scr        = from->fbPrt2scr;
    fbPrt2file       = from->fbPrt2file;
    strcpy(fPrtFilename,from->fPrtFilename);
    fbTime           = from->fbTime;
    fTimeFrom        = from->fTimeFrom;
    fTimeTo          = from->fTimeTo;
    fbEkev           = from->fbEkev;
    fEkevFrom        = from->fEkevFrom;
    fEkevTo          = from->fEkevTo;  
    fbEmev           = from->fbEmev;
    fEmevFrom        = from->fEmevFrom;
    fEmevTo          = from->fEmevTo;
    fbTof            = from->fbTof;
    fTofFrom         = from->fTofFrom;
    fTofTo           = from->fTofTo;
    fbGamma          = from->fbGamma;
    fGammaFrom       = from->fGammaFrom;
    fGammaTo         = from->fGammaTo;
    fbAllStrips      = from->fbAllStrips;
    fStripNo         = from->fStripNo;
    fbXylt           = from->fbXylt;
    fXyltFrom        = from->fXyltFrom;
    fXyltTo          = from->fXyltTo;
    fbXylb           = from->fbXylb;
    fXylbFrom        = from->fXylbFrom;
    fXylbTo          = from->fXylbTo;
    fbXyht           = from->fbXyht;
    fXyhtFrom        = from->fXyhtFrom;
    fXyhtTo          = from->fXyhtTo;
    fbXyhb           = from->fbXyhb;
    fXyhbFrom        = from->fXyhbFrom;
    fXyhbTo          = from->fXyhbTo;
    fbEkevbox        = from->fbEkevbox;
    fEkevboxFrom     = from->fEkevboxFrom;
    fEkevboxTo       = from->fEkevboxTo;
    fbEmevbox        = from->fbEmevbox;
    fEmevboxFrom     = from->fEmevboxFrom;
    fEmevboxTo       = from->fEmevboxTo;
    fbPause          = from->fbPause;
    fbPulse          = from->fbPulse;
    fbAntitof        = from->fbAntitof;
    fbAntiveto       = from->fbAntiveto;
    fbT2bit          = from->fbT2bit;
    fbT3bit          = from->fbT3bit;
    fBtime           = from->fBtime;
    fBtmp            = from->fBtmp;
    fBpausebit       = from->fBpausebit;
    fBstrip          = from->fBstrip;
    fBekev           = from->fBekev;
    fBxylt           = from->fBxylt;
    fBxylb           = from->fBxylb;
    fBemev           = from->fBemev;
    fBxyht           = from->fBxyht;
    fBxyhb           = from->fBxyhb;
    fBboxstrip       = from->fBboxstrip;
    fBekevbox        = from->fBekevbox;
    fBemevbox        = from->fBemevbox;
    fBegl1           = from->fBegl1;
    fBegl2           = from->fBegl2;
    fBegl3           = from->fBegl3;
    fBegl4           = from->fBegl4;
    fBegh1           = from->fBegh1;
    fBegh2           = from->fBegh2;
    fBegh3           = from->fBegh3;
    fBegh4           = from->fBegh4;
    fBtacga1         = from->fBtacga1;
    fBtacga2         = from->fBtacga2;
    fBtacga3         = from->fBtacga3;
    fBtacga4         = from->fBtacga4;
    fBtofbit         = from->fBtofbit;
    fBtof            = from->fBtof;
    fBtof1           = from->fBtof1;
    fBtof2           = from->fBtof2;
    fBtof3           = from->fBtof3;
    fBvetobit        = from->fBvetobit;
    fBt2bit          = from->fBt2bit;
    fBt3bit          = from->fBt3bit;
    fBpu1            = from->fBpu1;
    fBpu2            = from->fBpu2;
    fBpu3            = from->fBpu3;
    fBpu4            = from->fBpu4;
    fBpu5            = from->fBpu5;
    fBpu6            = from->fBpu6;
    fBpu7            = from->fBpu7;
    fBpu8            = from->fBpu8;
    
    cout << "MoDSSPrintEventParameter - Parameter : " << GetName() << " UPDATED\n";
  }
  else
    cout << "Wrong parameter object: " << pp->ClassName() << endl;
  
  return kTRUE;
}

//-----------------------------------------------------------
// function IsData skips comment lines (begining with #) in file f

int MoDSSPrintEventParameter::IsData(ifstream &f) {
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
// save current parameter to file (MoDSS-print.dat assumed)

void MoDSSPrintEventParameter::SaveToFile() {
  ofstream  f;
  
  cout << "Saving PrintParameter to file ... " << endl;
  
  f.open("MoDSS-print.dat");
  if (f.fail()) {
    cout << "ERROR - MoDSSPrintEventParameter - Cannot open file MoDSS-print.dat !!!" << endl;
    return;
  }
  
	f.flags(ios::fixed); // will set a fixed style of number writing
	f.precision(3);				// number of digits in decimal part
  f << "# Input file for MoDSSPrintEventParameter \n";
  f << "# - this parameter controls the printout of events \n";
  f << "#   from analysis step of data analysis \n";
  f << "# - these are only initial settings. parameter values \n";
  f << "#   should be changed from within UserGUI in Go4 \n";
  f << "# - use this file to set parameter values when Go4 \n";
  f << "#   analysis is running in the batch mode \n";
  f << "# \n";
  f << "# comment lines are starting with '#' \n";
  f << "# --------------------------------------------- \n";
  f << "# --------------------------------------------- \n";
  f << "# \n";
  f << "# enable printing ? 0 = no \n";
  f << fbEnablePrinting << endl;
  f << "# print to screen ? \n";
  f << fbPrt2scr << endl;
  f << "# print to file ?  # filename \n";
  f << fbPrt2file << "\t" << fPrtFilename << endl;
  f << "# --------------------------------------------- \n";
  f << "# PRINTING CONDITIONS: \n";
  f << "# \n";
  f << "# time condition activated ? # time limits in seconds \n";
  f << fbTime << "\t" << fTimeFrom << "\t" << fTimeTo << endl;
  f << "# ekev condition activated ? # ekev limits in kev \n";
  f << fbEkev << "\t" << fEkevFrom << "\t" << fEkevTo << endl;
  f << "# emev condition activated ? # emev limits in ch \n";
  f << fbEmev << "\t" << fEmevFrom << "\t" << fEmevTo << endl;
  f << "# tof condition activated ? # tof limits in ch \n";
  f << fbTof << "\t" << fTofFrom << "\t" << fTofTo << endl;
  f << "# egamma condition activated ? # egamma limits in kev \n";
  f << fbGamma << "\t" << fGammaFrom << "\t" << fGammaTo << endl;
  f << "# all strip ? # if no, give number of strip \n";
  f << fbAllStrips << "\t" << fStripNo << endl;
  f << "# xylt condition activated ? # xylt limits in ch \n";
  f << fbXylt << "\t" << fXyltFrom << "\t" << fXyltTo << endl;
  f << "# xylb condition activated ? # xylb limits in ch \n";
  f << fbXylb << "\t" << fXylbFrom << "\t" << fXylbTo << endl;
  f << "# xyht condition activated ? # xyht limits in ch \n";
  f << fbXyht << "\t" << fXyhtFrom << "\t" << fXyhtTo << endl;
  f << "# xyhb condition activated ? # xyhb limits in ch \n";
  f << fbXyhb << "\t" << fXyhbFrom << "\t" << fXyhbTo << endl;
  f << "# ekevbox condition activated ? # ekevbox limits in kev \n";
  f << fbEkevbox << "\t" << fEkevboxFrom << "\t" << fEkevboxTo << endl;
  f << "# emevbox condition activated ? # emevbox limits in ch \n";
  f << fbEmevbox << "\t" << fEmevboxFrom << "\t" << fEmevboxTo << endl;
  f << "# in pause ? \n";
  f << fbPause << endl;
  f << "# in pulse ? \n";
  f << fbPulse << endl;
  f << "# antitof ? \n";
  f << fbAntitof << endl;
  f << "# antiveto ? \n";
  f << fbAntiveto << endl;
  f << "# t2 bit ? \n";
  f << fbT2bit << endl;
  f << "# t3 bit ? \n";
  f << fbT3bit << endl;
  f << "# --------------------------------------------- \n";
  f << "# WHICH VALUES TO PRINT: \n";
  f << "# \n";
  f << "# time \n";
  f << fBtime << endl;
  f << "# tmp \n";
  f << fBtmp << endl;
  f << "# pausebit \n";
  f << fBpausebit << endl;
  f << "# strip \n";
  f << fBstrip << endl;
  f << "# ekev \n";
  f << fBekev << endl;
  f << "# xylt \n";
  f << fBxylt << endl;
  f << "# xylb \n";
  f << fBxylb << endl;
  f << "# emev \n";
  f << fBemev << endl;
  f << "# xyht \n";
  f << fBxyht << endl;
  f << "# xyhb \n";
  f << fBxyhb << endl;
  f << "# back strip \n";
  f << fBboxstrip << endl;
  f << "# ekev box \n";
  f << fBekevbox << endl;
  f << "# emev box \n";
  f << fBemevbox << endl;
  f << "# egl1 \n";
  f << fBegl1 << endl;
  f << "# egl2 \n";
  f << fBegl2 << endl;
  f << "# egl3 \n";
  f << fBegl3 << endl;
  f << "# egl4 \n";
  f << fBegl4 << endl;
  f << "# egh1 \n";
  f << fBegh1 << endl;
  f << "# egh2 \n";
  f << fBegh2 << endl;
  f << "# egh3 \n";
  f << fBegh3 << endl;
  f << "# egh4 \n";
  f << fBegh4 << endl;
  f << "# tac gamma 1 \n";
  f << fBtacga1 << endl;
  f << "# tac gamma 2 \n";
  f << fBtacga2 << endl;
  f << "# tac gamma 3 \n";
  f << fBtacga3 << endl;
  f << "# tac gamma 4 \n";
  f << fBtacga4 << endl;
	f << "# tofbit \n";
  f << fBtofbit << endl;
  f << "# tof \n";
  f << fBtof << endl;
  f << "# tof1 \n";
  f << fBtof1 << endl;
  f << "# tof2 \n";
  f << fBtof2 << endl;
  f << "# tof3 \n";
  f << fBtof3 << endl;
  f << "# vetobit \n";
  f << fBvetobit << endl;
  f << "# t2 bit \n";
  f << fBt2bit << endl;
  f << "# t3 bit \n";
  f << fBt3bit << endl;
  f << "# pu1 \n";
  f << fBpu1 << endl;
  f << "# pu2 \n";
  f << fBpu2 << endl;
  f << "# pu3 \n";
  f << fBpu3 << endl;
  f << "# pu4 \n";
  f << fBpu4 << endl;
  f << "# pu5 \n";
  f << fBpu5 << endl;
  f << "# pu6 \n";
  f << fBpu6 << endl;
  f << "# pu7 \n";
  f << fBpu7 << endl;
  f << "# pu8 \n";
  f << fBpu8 << endl;
  
  f.close();
}


// ----------------------------------------------------------
ClassImp(MoDSSPrintEventParameter)


