
/*
Stano:
27.3.2005 - Only small change... MoDSS-hardware.dat is used as 
a input file here. The incorrect printouts were fixed.
*/

#include "MoDSSHardwareParameter.h"
#include <fstream>
#include <iostream>

using namespace std;


//***********************************************************
// default constructor

MoDSSHardwareParameter::MoDSSHardwareParameter() 
: TGo4Parameter("MoDSSHardwareParameter")
{ 
  // cout << "MoDSSHardwareParameter - Default constructor used, parameters set to 0\n";
  fNewSetup = true;
	fRunID = 263;
	fHW_PulseBit = 1;
	fUnpackFilterLevel = 0;
	fAnalFilterLevel = 0;
 
  
}

//***********************************************************
// constructor

MoDSSHardwareParameter::MoDSSHardwareParameter(const char* name) 
: TGo4Parameter(name)
{
  ifstream    file;
  
  file.open("MoDSS-hardware.dat");
  if (file.fail())
  {
    cout << "ERROR: MoDSSHardwareParameter - Could not open file: MoDSS-hardware.dat ! Params set to 0\n";
    fNewSetup = true;
		fRunID = 263; 
		fHW_PulseBit = 0;
		fUnpackFilterLevel = 0;
		fAnalFilterLevel = 0;	
  }
  else 
  {
    cout << "MoDSSHardwareParameter - reading from : MoDSS-hardware.dat\n";
    if (IsData(file)) file >> fNewSetup;
		if (IsData(file)) file >> fRunID;
		if (IsData(file)) file >> fHW_PulseBit;
		if (IsData(file)) file >> fUnpackFilterLevel;
		if (IsData(file)) file >> fAnalFilterLevel;
    if (file.fail()) cout << "ERROR reading MoDSS-hardware.dat\n";
  }
  file.close();
}

//***********************************************************
// default destructor

MoDSSHardwareParameter::~MoDSSHardwareParameter()
{
}

//***********************************************************

//-----------------------------------------------------------

Int_t MoDSSHardwareParameter::PrintParameter(const char* n, Int_t){
  cout << "Parameter " << GetName() << " : \n";
  cout << "New setup is: " << fNewSetup << " : \n";
	cout << "Run ID is: " <<  fRunID << endl;
	if (fHW_PulseBit) cout << "Hardware pause bit is being used... \n";
	else cout << "Software pause bit is being used ... \n";
	cout << "Level for Unpac Filter is: " << fUnpackFilterLevel << endl;
	cout << "Level for Analysis Filter is: " << fAnalFilterLevel << endl;
  return 0;
}

//-----------------------------------------------------------
// this should not be implemented because it will freeze the analysis
// when the parameter is updates (graphs are created in constructor of
// analysis step and this is called only once at the start of analysis)

Bool_t MoDSSHardwareParameter::UpdateFrom(TGo4Parameter *pp){
 if(pp->InheritsFrom("MoDSSHardwareParameter"))
  {
    MoDSSHardwareParameter * from = (MoDSSHardwareParameter *) pp;    
    fNewSetup    = from->fNewSetup;
    fHW_PulseBit = from->fHW_PulseBit;
		fUnpackFilterLevel = from->fUnpackFilterLevel;
    cout << "MoDSSHardwareParameter - Parameter : " << GetName() << " UPDATED\n";
  }
  else
    cout << "Wrong parameter object: " << pp->ClassName() << endl;
  //cout << "CAN NOT BE UPDATED ! Change file MoDSS-input.dat and restart analysis.\n";
  return kTRUE;
}

// ==========================================================
// function IsData skips comment lines (begining with #) in file f

int MoDSSHardwareParameter::IsData(ifstream &f) {
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
ClassImp(MoDSSHardwareParameter)


