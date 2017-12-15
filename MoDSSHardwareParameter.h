
// parameter holding various data needed for program running


#ifndef SHARDWAREPAR_H
#define SHARDWAREPAR_H

#include "Go4StatusBase/TGo4Parameter.h"

#include <fstream>
using namespace std;

class MoDSSHardwareParameter : public TGo4Parameter {

public:
           MoDSSHardwareParameter();  
           MoDSSHardwareParameter(const char* name); 
  virtual ~MoDSSHardwareParameter();
  Int_t PrintParameter(const char* n, Int_t);
  Bool_t UpdateFrom(TGo4Parameter *);

  int IsData(ifstream &f);

  Bool_t    fNewSetup;            // true if amux setup is used
	Int_t			fRunID;								// Run ID
	Bool_t		fHW_PulseBit;					// is hardware pause bit used?
	Int_t			fUnpackFilterLevel;		// level for rejecting of the events in Unpack Routine 
	Int_t			fAnalFilterLevel;			// level for rejecting of the events in Analysis part

ClassDef(MoDSSHardwareParameter,1)
};

#endif //SPAR_H
