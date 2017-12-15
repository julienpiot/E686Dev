#ifndef MoDSSANLPROCESSOR_H
#define MoDSSANLPROCESSOR_H

//#define MAX_SFP           1
//#define MAX_SLAVE        12
//#define N_CHA            16
#include "MoDSSUnpackProc.h"

#include "TRandom.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TCutG.h"
#include "TGraph.h"

#include "Go4Event/TGo4EventElement.h"
#include "Go4Event/TGo4EventProcessor.h"
#include "Go4Analysis/TGo4Analysis.h"

#include "MoDSSUnpackEvent.h"
#include "MoDSSCalibParameter.h"
#include "MoDSSParameter.h"
//#include "MoDSSPrintEventParameter.h"

#include "MoDSSHardwareParameter.h"

#include "Go4ConditionsBase/TGo4WinCond.h"
#include "Go4ConditionsBase/TGo4PolyCond.h"



// macro max
//#define  max(a,b) ( ((a) > (b)) ? (a) : (b) )
#define ALPHA_GATE 7000
//Galotte Threshold
#define TOF_THRES  0
#define TAC_OK 5940
#define PULSE 0 //Change to 1

class MoDSSAnlEvent;
class MoDSSAnlProc : public TGo4EventProcessor {

public:
	MoDSSAnlProc() ;
	MoDSSAnlProc(Text_t * name);
	void     MoDSSEventAnalysis(MoDSSAnlEvent* target);
	void     MapHardware();
	virtual ~MoDSSAnlProc() ;
	Bool_t   BitTest(const Int_t &tested, Int_t position);
	void     PrintBitPattern(const Int_t &pattern, ostream &output);

	Bool_t   fbNewSubmit;   // TRUE if new analysis was just submited

  // input event (from unpack step)
	MoDSSUnpackEvent  *fInput;

	// parameters
	MoDSSParameter      *fParam;
	MoDSSCalibParameter *fCal;
	MoDSSHardwareParameter           *fHardwareParam;


	//	TGo4WinCond		*fCondTMP[5];     // tmp conditions (ignore the 0th condition)
	TGo4WinCond	*fCondELB;        // box low energy window (used in filling calibration graphs)
	TGo4PolyCond	*fCondEXvsTOF_ANL;
	TGo4PolyCond	*fCondEYvsTOF_ANL;

	//Read the Data from UnpackEvent in MapHardware : 
	Double_t	EnonCalX[60];//Read the Energies from UnpackEvent
	Double_t	EnonCalY[60];//Read the Energies from UnpackEvent
	Double_t	EnonCalBox[32];//Read the Energies from UnpackEvent
	Double_t	EnonCalGam[20];//Read the Energies from UnpackEvent

	Long64_t	TimeX[60];//Read the Time from UnpackEvent
	Long64_t	TimeY[60];//Read the Time from UnpackEvent
	Long64_t	TimeBox[32];//Read the Time from UnpackEvent
	Long64_t	TimeGam[20];//Read the Time from UnpackEvent
	Long64_t 	DTime;

	Double_t 	TAC_GalSi;//Read the Value from UnpackEvent
	Double_t 	TAC_GalX;//Read the Value from UnpackEvent
	Double_t 	TAC_GalY;//Read the Value from UnpackEvent
	Double_t 	TAC_SiGe;//Read the Value from UnpackEvent
	Double_t 	TiD6;//Read the Value from UnpackEvent
	Double_t 	HF;//Read the Value from UnpackEvent

	Long64_t	TimeGalSi;//Read the Time from TACGalSi from UnpackEvent

	Int_t		l_sfp;//Read the sfp number from UnpackEvent
	Int_t		l_feb;//Read the febex number from UnpackEvent
	Int_t 		l_cha;//Read the channel number from UnpackEvent
	Long64_t	l_t_trig;//Read the trigger time from UnpackEvent

	//Store the Energies for treatment
	Double_t	EkevX[60];
	Double_t	EkevY[60];
	Double_t	EkevBox[32];
	Double_t	EkevGam[20];

	//Prepare the output for AnlEvent
	Double_t 	E_kevX;//        
	Double_t 	E_kevY;
	Double_t 	E_kevBox;
	Int_t    	IDX; 
	Int_t    	IDY;
	Int_t    	IDB;
	Double_t 	TOF;//Calculated in ms
	Double_t 	TOF2;//Calculated in 20 ns clock samples.
	UInt_t		EventNum;

	//Store the sampling time
	Long64_t 	sample_time_x;
	Long64_t 	sample_time_y;
	Long64_t 	sample_time_box;
	Long64_t 	sample_time_ge[20];

	

	Int_t 		chit_dssdX;
	Int_t 		chit_dssdY;
	Int_t		chit_gamma;
	Int_t 		chit_box;
	Int_t 		num_back[32];

	//Julien 
	Long64_t	NbEventX;
	Long64_t	NbEventY;
	Int_t 		multiplicityX;
	Int_t 		multiplicityY;
	Long64_t	l_TS_sav;
	Double_t	l_E_sav;
	Int_t		strip_sav;
        
	// histograms

	//1D histos (calibrated)
	TH1D *hEP_KEVX_Strip[60];
	TH1D *hEA_KEVX_Strip[60];
	TH1D *hEA_KEVX_Strip20;
	TH1D *hEPulse_KEVX_Strip[60];

	TH1D *hEP_KEVY_Strip[60];
	TH1D *hEA_KEVY_Strip[60];
	TH1D *hEPulse_KEVY_Strip[60];

	TH1D *hEBP_KEV_Strip[32];
	TH1D *hEBA_KEV_Strip[32];
	TH1D *hEBPulse_KEV_Strip[32];

	TH1D *hEGe[20];
        TH2D *hAlGa_coinc[20];


        TH1D *h_tac_tof1;
        TH1D *h_tac_tof2;
        TH1D *h_tac_tof3;
	TH1D *h_tac_si_tof;
	TH1D *h_tac_posX_gal;
	TH1D *h_tac_posY_gal;
	TH1D *h_tac_si_ge;
	TH1D *h_ti_d6;
	TH1D *h_HF_cyclo;

	TH1D *hEP_KEVX;
	TH1D *hEP_KEVY;

	TH1D *hEBP_KEV;
	TH1D *h_ge_sum;


	TH1D *hEA_KEVX;
	TH1D *hEA_KEVY;


	TH1D *hEgammaAll;
	
	TH1D *hEgammaAll_ERX_gated;
	TH1D *hEgammaAll_ERY_gated;
	
	TH1D *hDTGamma_ERX;
	TH1D *hDTGamma_ERY;

        TH1D *hEgammaAll_antiTOF;
        TH1D *h_bPulse;
        
        TH1D *h_ProfileX;
        TH1D *h_ProfileY;
        TH1D *hHitsMultiX;
        TH1D *hHitsMultiY;
        
        TH2D *hMultiX1vs2;
	TH2D *hMultiY1vs2;
	TH1D *hXReconstructed;

        TH2D *hAlGa_coinc_sum;
        TH2D *hAlGa_coinc_sum_pulse;
        TH2D *hAlGa_coinc_sum_pause;
        TH2D *hAlGa_coinc_sum_tof;
        TH2D *hAlGa_coinc_sum_antitof;

        TH1D *hE_KEVX;
        TH1D *hE_KEVY;
        TH1D *h_dssd_back_sum_pulse;
        TH1D *hEB_KEV;
        TH1D *hEgammaAll_Pulse;

        TH1D *h_dssd_back_sum_pause;
        TH1D *hEBA_KEV;
        TH1D *hEgammaAll_Pause;

        TH2D *hAlGa_coinc_pulse[20];
        TH2D *hAlGa_coinc_pause[20];
        TH2D *hAlGa_coinc_coin[20];
        TH2D *hAlGa_coinc_anticoin[20];

    	TH2D *hTOFvsEkeVX;
    	TH2D *hTOFvsEkeVY;

	TH2D *h_dssd_ExIdx;
	TH2D *h_dssd_ExIdx_coin;
	TH2D *h_dssd_ExIdx_anticoin;
	TH2D *h_dssd_EyIdy;
	TH2D *h_dssd_EyIdy_coin;
	TH2D *h_dssd_ExEy;
	TH2D *h_dssd_idx_idy;
	TH2D *h_dssd_idx_idy_coin;
	
	TH2D *h_dssd_idx_idy_Al_Gated;
	TH2D *h_E_Tof1;
	TH2D *h_EX_Tof1;
	TH2D *h_E_TAC;

	TH2D *h_box_EId;

	TH2D *h_ge_EId;
	TH2D *h_ge_EId_coin;
//         TH2D *h_E_Tof;
//         TH2D *h_E_Tof2;
	TH2D *h_pos_gal;

	TH1D *h_EffGal;
	Bool_t bEffGal;

//Julien
	TH1D *hE_KEVY_Out;
	TH1D *hE_KEVX_Out;

	TH1D *hTOFms;
	TH1D *hTOF20ns;

	TH1D	*hDl_T;
	TH2D	*dE1vsE2_zero;
	TH2D	*dE1vsE2_deux;
	TH2D	*dX1vsX2_zero;
	TH2D	*dX1vsX2_deux;
	TH1D	*l_hit_out;

	TH1D	*h_dT_Gamma;
	

//============= output events =================

      Bool_t newsetup; //check this out

      Float_t frontX[60];
      Float_t backY[60];

      Bool_t tof;//Store the presence of a TAC value from UnpackEvent

ClassDef(MoDSSAnlProc,1)
};
#endif //MoDSSANLPROCESSOR_H

//----------------------------END OF GO4 SOURCE FILE ---------------------
