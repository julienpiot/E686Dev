#include "MoDSSAnlEvent.h"

MoDSSAnlEvent::MoDSSAnlEvent()
	:TGo4EventElement("MoDSSAnlEvent")
{
	fIDB = 0;
	for (Int_t i = 0; i < 6; i++){
		fIDX[i] = 0;    
		fIDY[i] = 0;
		fE_kevX[i]  = 0;
		fE_kevY[i] = 0;
		fTimeX[i] = 0;  
		fTimeY[i] = 0;
	}
	for (Int_t i = 0; i < 32; i++){
		fEb_kev[i] = 0;
		fTimeBox[i] =0;
	}
	fTimeTrig = 0;
	for (Int_t i = 0; i < 20; i++){
		fTgamma[i]  = 0;
		fegl[i] = 0;
	} 
	fB_tof = kFALSE;
	fTOF = 0;
	fTOF1 = 0;

	fUsed    = kFALSE;
	for (Int_t i = 0; i < MAXALPHA+1; i++)
		for (Int_t j = 0; j < NUMMOTHER; j++)
			fMama[i][j] = 0;
	fEventType = zero;
	fEventNum = 0;

	fMultX = 0;
	fMultY = 0;
	fMultBox = 0;
	fMultGe = 0;

	fxMoDSSCP = 0;
	fxMoDSSFS = 0;
	fIx = 0;
	fIy = 0;
	fGalSi = 0;
	fGalX = 0;
	fGalY = 0;
	fTAC_SiGe = 0;
	fTiD6 = 0;
	fHF = 0;
}
//-------------
MoDSSAnlEvent::MoDSSAnlEvent(Text_t * name) 
	:TGo4EventElement(name)
{
	fIDB = 0;
	for (Int_t i = 0; i < 6; i++){
		fIDX[i] = 0;    
		fIDY[i] = 0;
		fE_kevX[i]  = 0;
		fE_kevY[i] = 0;
		fTimeX[i] = 0;  
		fTimeY[i] = 0;
	}
	for (Int_t i = 0; i < 32; i++){
		fEb_kev[i] = 0;
		fTimeBox[i] =0;
	}
	fTimeTrig = 0;
	for (Int_t i = 0; i < 20; i++){
		fTgamma[i]  = 0;
		fegl[i] = 0;
	} 
	fB_tof = kFALSE;
	fTOF = 0;
	fTOF1 = 0;

	fUsed    = kFALSE;
	for (Int_t i = 0; i < MAXALPHA+1; i++)
		for (Int_t j = 0; j < NUMMOTHER; j++)
			fMama[i][j] = 0;
	fEventType = zero;
	fEventNum = 0;


	fMultX = 0;
	fMultY = 0;
	fMultBox = 0;
	fMultGe = 0;
	fxMoDSSCP = 0;
	fxMoDSSFS = 0;
	fIx = 0;
	fIy = 0;
	fGalSi = 0;
	fGalX = 0;
	fGalY = 0;
	fTAC_SiGe = 0;
	fTiD6 = 0;
	fHF = 0;
}
//-----------------------
MoDSSAnlEvent::~MoDSSAnlEvent() 
{
}
//----------------------------
// assignment operator '='

MoDSSAnlEvent MoDSSAnlEvent::operator = (const MoDSSAnlEvent &from)
{
	if (this == &from) return *this; // selfassignment
	fIDB = from.fIDB;
	for (Int_t i = 0; i < 6; i++){
		fIDX[i] = from.fIDX[i];    
		fIDY[i] = from.fIDY[i];
		fE_kevX[i] = from.fE_kevX[i];       
		fE_kevY[i] = from.fE_kevY[i];
		fTimeX[i] = from.fTimeX[i];  
		fTimeY[i] = from.fTimeY[i];
	}
	for (Int_t i = 0; i < 32; i++){
		fEb_kev[i] = from.fEb_kev[i];
		fTimeBox[i] = from.fTimeBox[i];
	}
	fTimeTrig = from.fTimeTrig;
	for (Int_t i = 0; i < 20; i++){
		fTgamma[i]  = from.fTgamma[i];
		fegl[i] = from.fegl[i]; 
	}
	fB_tof = from.fB_tof;
	fTOF = from.fTOF;
	fTOF1 = from.fTOF1;


	fUsed    = from.fUsed;
	for (Int_t i = 0; i < MAXALPHA+1; i++)
		for (Int_t j = 0; j < NUMMOTHER; j++)
			fMama[i][j] = from.fMama[i][j];
	fEventType  = from.fEventType;
	fEventNum = from.fEventNum;

	fMultX = from.fMultX;
	fMultY = from.fMultY;
	fMultBox = from.fMultBox;
	fMultGe = from.fMultGe;
	fIx = from.fIx;
	fIy = from.fIy;
	fGalSi = from.fGalSi;
	fGalX = from.fGalX;
	fGalY = from.fGalY;
	fTAC_SiGe = from.fTAC_SiGe;
	fTiD6 = from.fTiD6;
	fHF = from.fHF;
	fxMoDSSCP    = 0;
	fxMoDSSFS    = 0;

	return *this;
}
//--------------------------
Int_t MoDSSAnlEvent::Init()
{
	// check for different source types
	Int_t rev = 0;
	Clear();
	// is it used by Anl step as output ?
	if(CheckEventSource("MoDSSAnlProc")){
		fxMoDSSCP = (MoDSSAnlProc*)GetEventSource();
		cout << "**** MoDSSAnlEvent init for Analysis step" << endl;
	}
	// or is it used from Correlation step as input ?
	else if(CheckEventSource("TGo4FileSource")) {
		fxMoDSSFS = (TGo4FileSource*)GetEventSource();
		cout << "**** MoDSSAnlEvent init for Correlation step" << endl;
	}
	else rev = 1;
	return rev;
}
//------------------------
Int_t MoDSSAnlEvent::Fill()
{
	Int_t rev = 0;
	Clear();
	// check for different source types
	if(fxMoDSSCP) fxMoDSSCP->MoDSSEventAnalysis(this); // process input - analyze and calibrate ...
	if(fxMoDSSFS) fxMoDSSFS->BuildEvent(this); // Go4 framework method - restore event from tree
	return rev;
}
//-----------------------------
void MoDSSAnlEvent::Clear(Option_t *t)
{ 
	fIDB = 0;
	for (Int_t i = 0; i < 6; i++){
		fIDX[i] = 0;    
		fIDY[i] = 0;
		fE_kevX[i]  = 0;
		fE_kevY[i] = 0;
		fTimeX[i] = 0;  
		fTimeY[i] = 0;
	}
	for (Int_t i = 0; i < 32; i++){
		fEb_kev[i] = 0;
		fTimeBox[i] =0;
	}
	fTimeTrig = 0;
	for (Int_t i = 0; i < 20; i++){
		fTgamma[i]  = 0;
		fegl[i] = 0;
	} 
	fB_tof = kFALSE;
	fTOF = 0;
	fTOF1 = 0;

	fUsed    = kFALSE;
	for (Int_t i = 0; i < MAXALPHA+1; i++)
		for (Int_t j = 0; j < NUMMOTHER; j++)
			fMama[i][j] = 0;
	fEventType = zero;
	fEventNum =0;
	fMultX = 0;
	fMultY = 0;
	fMultBox = 0;
	fMultGe = 0;
	fIx = 0;
	fIy = 0;
	fGalSi = 0;
	fGalX = 0;
	fGalY = 0;
	fTAC_SiGe = 0;
	fTiD6 = 0;
	fHF = 0;
}

void MoDSSAnlEvent::ClearEvent()
{ 
	fIDB = 0;
	for (Int_t i = 0; i < 6; i++){
		fIDX[i] = 0;    
		fIDY[i] = 0;
		fE_kevX[i]  = 0;
		fE_kevY[i] = 0;
		fTimeX[i] = 0;  
		fTimeY[i] = 0;
	}
	for (Int_t i = 0; i < 32; i++){
		fEb_kev[i] = 0;
		fTimeBox[i] =0;
	}
	fTimeTrig = 0;
	for (Int_t i = 0; i < 20; i++){
		fTgamma[i]  = 0;
		fegl[i] = 0;
	} 
	fB_tof = kFALSE;
	fTOF = 0;
	fTOF1 = 0;


	fUsed    = kFALSE;
	for (Int_t i = 0; i < MAXALPHA+1; i++)
		for (Int_t j = 0; j < NUMMOTHER; j++)
			fMama[i][j] = 0;

	fEventType = zero;
	fEventNum =0;
	fMultX = 0;
	fMultY = 0;
	fMultBox = 0;
	fMultGe = 0;
	fIx = 0;
	fIy = 0;
	fGalSi = 0;
	fGalX = 0;
	fGalY = 0;
	fTAC_SiGe = 0;
	fTiD6 = 0;
	fHF = 0;

}

//----- Handle Addback in Ge or Box detector
/*Int_t MoDSSAnlEvent::AddBack()
{
	//Only addback in line for adjecent crystals in Ge 
	for (Int_t i=0; i<20; i++){
		if((fegl[i]>0) && (fegl[i+1]>0) && (((i+1)%4)!=0))
		{
			if(fegl[i]>=fegl[i+1])
			{
				fegl[i] += fegl[i+1];
				fegl[i+1] = 0;
			}
			else{
				fegl[i+1] += fegl[i];
				fegl[i] = 0;
			}
		}	 
	}
	//Addback for Box
	for (Int_t i=0; i<32; i++){
		if((fEb_kev[i]>0) && (fEb_kev[i+1]>0) && (((i+1)%8)!=0))
		{
			if(fEb_kev[i]>=fEb_kev[i+1])
			{
				fEb_kev[i] += fEb_kev[i+1];
				fEb_kev[i+1] = 0;
			}
			else{
				fEb_kev[i+1] += fEb_kev[i];
				fEb_kev[i] = 0;
			}
		}	 
	}
	return 1;
}*/


//-----------------------------
ClassImp(MoDSSAnlEvent)
