#include <iostream>
#include "MoDSSParameter.h"

using namespace std;

//***********************************************************
// default constructor

MoDSSParameter::MoDSSParameter() 
: TGo4Parameter("MoDSSParameter")
{ 
  cout << "MoDSSParameter - Default constructor used, parameters set to 0\n";
  fPosInMM = 0;
  fYH_YL_Calib = 0;
  fBack_Calib = 0;
  fEback = 0.;
  fBalistic_Calib = 0;
  fEbalistic = 0.;
  for (Int_t i = 0; i < 2; i++) {
    fTmp1[i] = 0;
    fTmp2[i] = 0;
    fTmp3[i] = 0;
    fTmp4[i] = 0;
    fEL[i] = 0;
    fEH[i] = 0;
    fELB[i] = 0;
    fEG[i] = 0;
    fTAC[i] = 0;
	fTOF[i] = 0;
  }
  fTof_limit_low = 0;
  fTof_limit_high = 0;
  fNumBinTime = 0;
  fTime_res_low = 0;
  fTime_res_high = 0;
  fStopStrip = 0;
  fBoxStrip = 0;
  fEHstrip = 0;
  fANL_PolyX = 0;
  fANL_PolyY = 0;
}

//***********************************************************
// constructor

MoDSSParameter::MoDSSParameter(const char* name) 
: TGo4Parameter(name)
{
  ifstream    file;
  
  file.open("MoDSS-input.dat");
  if (file.fail()) {
    cout << "ERROR: MoDSSParameter - Could not open file: MoDSS-input.dat ! Params set to 0\n";
    fPosInMM = 0;
    fYH_YL_Calib = 0;
    fBack_Calib = 0;
    fEback = 0.;
    fBalistic_Calib = 0;
    fEbalistic = 0.;
    for (Int_t i = 0; i < 2; i++) {
      fTmp1[i] = 0;
      fTmp2[i] = 0;
      fTmp3[i] = 0;
      fTmp4[i] = 0;
      fEL[i] = 0;
      fEH[i] = 0;
      fELB[i] = 0;
      fEG[i] = 0;
      fTAC[i] = 0;
	  fTOF[i] = 0;
    }
	fTof_limit_low = 0;
	fTof_limit_high = 0;
	fNumBinTime = 0;
	fTime_res_low = 0;
	fTime_res_high = 0;
  	fStopStrip = 0;
  	fBoxStrip = 0;
	fEHstrip = 0;
	fANL_PolyX = 0;
	fANL_PolyY = 0;
  }
  else {
    cout << "MoDSSParameter - reading from : MoDSS-input.dat\n";
    if (IsData(file)) file >> fPosInMM;
    if (IsData(file)) file >> fYH_YL_Calib;
    if (IsData(file)) file >> fBack_Calib;
    if (IsData(file)) file >> fEback;
    if (IsData(file)) file >> fBalistic_Calib;
    if (IsData(file)) file >> fEbalistic;
    if (IsData(file)) file >> fTmp1[0] >> fTmp1[1];
    if (IsData(file)) file >> fTmp2[0] >> fTmp2[1];
    if (IsData(file)) file >> fTmp3[0] >> fTmp3[1];
    if (IsData(file)) file >> fTmp4[0] >> fTmp4[1];
    if (IsData(file)) file >> fEL[0] >> fEL[1];
    if (IsData(file)) file >> fEH[0] >> fEH[1];
    if (IsData(file)) file >> fELB[0] >> fELB[1];
    if (IsData(file)) file >> fEG[0] >> fEG[1];
    if (IsData(file)) file >> fTAC[0] >> fTAC[1];
		if (IsData(file)) file >> fTOF[0] >> fTOF[1];
		if (IsData(file)) file >> fTof_limit_low >> fTof_limit_high;
		if (IsData(file)) file >> fNumBinTime >> fTime_res_low >> fTime_res_high;	
		if (IsData(file)) file >> fStopStrip >> fBoxStrip;
		if (IsData(file)) file >> fEHstrip;
		if (IsData(file)) file >> fANL_PolyX;
		if (IsData(file)) file >> fANL_PolyY;
    if (file.fail()) cout << "ERROR reading MoDSS-input.dat\n";
  }
  file.close();
}

//***********************************************************
// default destructor

MoDSSParameter::~MoDSSParameter()
{
}

//***********************************************************

//-----------------------------------------------------------

Int_t MoDSSParameter::PrintParameter(const char* n, Int_t){
  cout << "Parameter " << GetName() << " : \n";
  cout << "\t Positions in ";
  if (fPosInMM) cout << "mm (0-35) \n";
  else cout << "channels (0-900) \n";
  cout << "\t Fill YH-YL calib. graphs : ";
  if (fYH_YL_Calib) cout << "YES \n";
  else  cout << "NO \n";
  cout << "\t Fill back detector calib. graphs : ";
  if (fBack_Calib) {
    cout << "YES \n" << "\t SET ELB CONDITION !!!\n";
    cout << "\t Alpha energy used in back calib: " << fEback << endl;
  }
  else  cout << "NO \n";
  cout << "\t Fill balistic calib. graphs : ";
  if (fBalistic_Calib) {
    cout << "YES \n" << "\t SET EL CONDITION !!!\n";
    cout << "\t Alpha energy (not calibrated) used in balistic calib: " << fEbalistic << endl;
  }
  else  cout << "NO \n";
  cout << "\tConditions: \n";
  cout << "\t TMP1 : " << fTmp1[0] << " - " << fTmp1[1] << endl;
  cout << "\t TMP2 : " << fTmp2[0] << " - " << fTmp2[1] << endl;
  cout << "\t TMP3 : " << fTmp3[0] << " - " << fTmp3[1] << endl;
  cout << "\t TMP4 : " << fTmp4[0] << " - " << fTmp4[1] << endl;
  cout << "\t EL : " << fEL[0] << " - " << fEL[1] << endl;
  cout << "\t EH : " << fEH[0] << " - " << fEH[1] << endl;
  cout << "\t ELB : " << fELB[0] << " - " << fELB[1] << endl;
  cout << "\t EG : " << fEG[0] << " - " << fEG[1] << endl;
  cout << "\t TAC : " << fTAC[0] << " - " << fTAC[1] << endl;
  cout << "\t TOF : " << fTOF[0] << " - " << fTOF[1] << endl;
  cout << "\t Limits for TOF spectra : " << fTof_limit_low << " - " << fTof_limit_high << endl;
  cout << "\t Settings for time spectra : " << fNumBinTime << " - " << fTime_res_low << " - " << fTime_res_high << endl;
  cout << "\t Settings for STOPvsBOX spectra : " << fStopStrip << " - " << fBoxStrip << endl;
  cout << "\t High energy strip spectra are switch : " << fEHstrip<<endl;
  cout << "\t Polygonal condition X used for recoils sent to Correlation step: " << fANL_PolyX << endl;
  cout << "\t Polygonal condition Y used for recoils sent to Correlation step: " << fANL_PolyY << endl;
  
  return 0;
}

//-----------------------------------------------------------
// this should not be implemented because it will freeze the analysis
// when the parameter is updates (graphs are created in constructor of
// analysis step and this is called only once at the start of analysis)

Bool_t MoDSSParameter::UpdateFrom(TGo4Parameter *pp){
 /* if(pp->InheritsFrom("MoDSSParameter"))
  {
    MoDSSParameter * from = (MoDSSParameter *) pp;

    fYH_YL_Calib    = from->fYH_YL_Calib;
    fBack_Calib     = from->fBack_Calib;
    fEback          = from->fEback;
    fBalistic_Calib = from->fBalistic_Calib;
    fEbalistic      = from->fEbalistic;
    cout << "MoDSSParameter - Parameter : " << GetName() << " UPDATED\n";
  }
  else
    cout << "Wrong parameter object: " << pp->ClassName() << endl;
  */
  cout << "CAN NOT BE UPDATED ! Change file MoDSS-input.dat and restart analysis.\n";
  return kTRUE;
}

// ==========================================================
// function IsData skips comment lines (begining with #) in file f

int MoDSSParameter::IsData(ifstream &f) {
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

// ----------------------------------------------------------
ClassImp(MoDSSParameter)


