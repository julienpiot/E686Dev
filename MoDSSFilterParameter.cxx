
/*
Julien:
8.2.2017 - Created to input the MWD filter parameters in the unpack step.
*/

#include "MoDSSFilterParameter.h"
#include <fstream>
#include <iostream>

using namespace std;


//***********************************************************
// default constructor

MoDSSFilterParameter::MoDSSFilterParameter() 
: TGo4Parameter("MoDSSFilterParameter")
{ 
  // cout << "MoDSSFilterParameter - Default constructor used, parameters set to 0\n";
 	fk = 0;        
        fm = 0;	   
	for(int i=0; i<MAX_SLAVE; i++){
       		for(int j=0; j<N_CHA; j++){
			ftau[i][j] = 0;
		}
	}	   
        fBL_Av = 0;	   
        fBL_Start = 0; 
        fSum_Av = 0;   
        fSum_Start = 0;
  
}

//***********************************************************
// constructor

	MoDSSFilterParameter::MoDSSFilterParameter(const char* name) 
: TGo4Parameter(name)
{
	ifstream    file;

	file.open("MoDSS-filter.dat");
	if (file.fail())
	{
		cout << "ERROR: MoDSSFilterParameter - Could not open file: MoDSS-filter.dat ! Params set to 0\n";
		fk = 60;        
		fm = 60;	   
		for(int i=0; i<10; i++){
			for(int j=0; j<N_CHA; j++){
				ftau[i][j] = 11200;
			}
		}	   
		for(int i=10; i<MAX_SLAVE; i++){
			for(int j=0; j<N_CHA; j++){
				ftau[i][j] = 45800;
			}
		}	   
		fBL_Av = 58;	   
		fBL_Start = 183; 
		fSum_Av = 11;   
		fSum_Start = 355;

	}
	else 
	{
		cout << "MoDSSFilterParameter - reading from : MoDSS-filter.dat\n";
		if (IsData(file)) file >> fk;
		if (IsData(file)) file >> fm;
		for(int i=0; i<MAX_SLAVE; i++){
				if (IsData(file)) file >> ftau[i][0]>> ftau[i][1]>> ftau[i][2]>> ftau[i][3]>> ftau[i][4]>> ftau[i][5]>> ftau[i][6]>> ftau[i][7]>> ftau[i][8]>> ftau[i][9]>> ftau[i][10]>> ftau[i][11]>> ftau[i][12]>> ftau[i][13]>> ftau[i][14]>> ftau[i][15];
		}	   
		if (IsData(file)) file >> fBL_Av;
		if (IsData(file)) file >> fBL_Start;
		if (IsData(file)) file >> fSum_Av;
		if (IsData(file)) file >> fSum_Start;
		if (file.fail()) cout << "ERROR reading MoDSS-filter.dat\n";
	}
	file.close();
}

//***********************************************************
// default destructor

MoDSSFilterParameter::~MoDSSFilterParameter()
{
}

//***********************************************************

//-----------------------------------------------------------

Int_t MoDSSFilterParameter::PrintParameter(const char* n, Int_t){
	cout << "Parameter " << GetName() << " : \n"
		<< "k is : "<<fk<<endl        
		<< "m is : "<<fm<<endl;	   
	for(int i=0; i<MAX_SLAVE; i++){
		for(int j=0; j<N_CHA; j++){
			cout<<" tau["<<i<<"]["<<j<<"] : "<<ftau[i][j]<<endl;
		}
	}	   
	cout<< "BL average is : "<<fBL_Av<<endl	   
		<<"BL start is : "<<fBL_Start<<endl 
		<<"Sum average is : "<<fSum_Av<<endl   
		<<"Sum start is : "<<fSum_Start<<endl;
	return 0;
}

//-----------------------------------------------------------
// this should not be implemented because it will freeze the analysis
// when the parameter is updates (graphs are created in constructor of
// analysis step and this is called only once at the start of analysis)

Bool_t MoDSSFilterParameter::UpdateFrom(TGo4Parameter *pp){
	if(pp->InheritsFrom("MoDSSFilterParameter"))
	{
		MoDSSFilterParameter * from = (MoDSSFilterParameter *) pp;    
		fk = from->fk;
		fm = from->fm;
		for(int i=0; i<MAX_SLAVE; i++){
			for(int j=0; j<N_CHA; j++){
				ftau[i][j] = from->ftau[i][j];
			}
		}	   
		fBL_Av = from->fBL_Av;
		fBL_Start = from->fBL_Start;
		fSum_Av = from->fSum_Av;
		fSum_Start = from->fSum_Start;
		cout << "MoDSSFilterParameter - Parameter : " << GetName() << " UPDATED\n";
	}
	else
		cout << "Wrong parameter object: " << pp->ClassName() << endl;
	//cout << "CAN NOT BE UPDATED ! Change file MoDSS-input.dat and restart analysis.\n";
	return kTRUE;
}

// ==========================================================
// function IsData skips comment lines (begining with #) in file f

int MoDSSFilterParameter::IsData(ifstream &f) {
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
ClassImp(MoDSSFilterParameter)


