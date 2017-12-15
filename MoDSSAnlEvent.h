#ifndef MoDSSANLEVENT_H
#define MoDSSANLEVENT_H

#include "MoDSSGlobals.h"

#include "Go4Event/TGo4EventElement.h"
#include "MoDSSAnlProc.h"
#include "Go4EventServer/TGo4TreeSource.h"
#include "Go4EventServer/TGo4FileSource.h"


class MoDSSAnlEvent : public TGo4EventElement
{
	public:
		MoDSSAnlEvent() ;
		MoDSSAnlEvent(Text_t * name) ;
		~MoDSSAnlEvent() ;
		// operator '=' ; only for the use in correlation processor,
		// do not use in AnlProcessor unless you know what you are doing.
		MoDSSAnlEvent operator = (const MoDSSAnlEvent &from);

		virtual Int_t Init();
		virtual Int_t Fill();
		virtual void  Clear(Option_t *t="");
		virtual void  ClearEvent();
		//virtual Int_t AddBack();
		//virtual Int_t RegroupDSSD();

		// output event from second (analysis) step - contains variables
		// needed for correlations
	
		//DSSD
		Int_t		fIDX[6];    // detector number > 0 we modify the index for storage in arrays. 
		Int_t		fIDY[6];
		Int_t		fMultX;
		Int_t		fMultY;
		Int_t		fIx;		//Index of the strip that triggered the buffer.
		Int_t		fIy;
		Double_t	fE_kevX[6];//Front side calibrated energy
		Double_t	fE_kevY[6];//Back side calibrated energy
		//BOX
		Double_t	fEb_kev[32];//Box calibrated energy
		Double_t	fTimeBox[32];//Complete Timestamps for the Box channels stored in 64 bits. in ms
		Int_t		fMultBox;
		Int_t		fIDB;
		
		//Ge
		Double_t	fegl[20]; //gamma calibrated energy
		Double_t	fTgamma[20];//Complete Timestamps for the Ge side channels stored in 64 bits. in ms
		Int_t		fMultGe;

		Double_t	fTimeTrig;
		Double_t	fTimeX[6];//Complete Timestamps for the Front side channels stored in 64 bits. in ms
		Double_t	fTimeY[6];//Complete Timestamps for the Back side channels stored in 64 bits. in ms 
		Bool_t		fB_tof;//Logical bit for the presence or absence of ToF signal.
		Double_t	fTOF;
		Double_t	fTOF1;
	
		Double_t	fGalSi;
		Double_t	fGalX;
		Double_t	fGalY;
		Double_t	fTAC_SiGe;
		Double_t	fTiD6;
		Double_t	fHF;

		Bool_t      	fUsed;         // = 1 when the event was used in spectra filling
		event_type  	fEventType;    // zero, recoil, alpha, fission, gamma
		UInt_t		fEventNum; // Event number from MBS
		MoDSSAnlEvent 	*fMama[MAXALPHA+1][NUMMOTHER];    //! (do not stream) pointer to mother generation


	private:
		// this object is streamed. Therefore pointers must be excluded!
		// Otherwise these objects are streamed as well
		MoDSSAnlProc     * fxMoDSSCP;  //! Don't put this to file
		TGo4FileSource   * fxMoDSSFS;  //! Don't put this to file



		ClassDef(MoDSSAnlEvent,2)
};
#endif //MoDSSANLEVENT_H
