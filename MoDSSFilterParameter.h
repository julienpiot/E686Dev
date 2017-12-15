
// parameter holding various data needed the MWD filter in the Unpack Step


#ifndef MoDSSFilterParameter_H
#define MoDSSFilterParameter_H

#define MAX_SFP           4
#define MAX_SLAVE        12
#define N_CHA            16

#include "Go4StatusBase/TGo4Parameter.h"
#include "MoDSSUnpackProc.h"

#include <fstream>
using namespace std;

class MoDSSFilterParameter : public TGo4Parameter {

public:
           		MoDSSFilterParameter();  
           		MoDSSFilterParameter(const char* name); 
  	virtual		~MoDSSFilterParameter();
  	Int_t 		PrintParameter(const char* n, Int_t);
  	Bool_t 		UpdateFrom(TGo4Parameter *);
  	int 		IsData(ifstream &f);

 	Int_t    	fk;            	// duration of the trapezoid slope (in 20 ns)
	Int_t		fm;		// duration of the trapezoid flat top (in 20 ns)
	Int_t		ftau[MAX_SLAVE][N_CHA];	// pole-zero correction (in ns)
	UInt_t		fBL_Av;		// Number of samples used for the averaging on the baseline
	UInt_t		fBL_Start;	// Sample number to start averaging on the basline
	UInt_t		fSum_Av;	// Number of samples used for the averaging on the flat top
	UInt_t		fSum_Start;	// Sample number to start averaging on the flat top 

ClassDef(MoDSSFilterParameter,2)
};

#endif //SPAR_H
