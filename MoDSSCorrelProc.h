
#ifndef MoDSSCORRELPROCESSOR_H
#define MoDSSCORRELPROCESSOR_H

#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TCutG.h"
#include "TGraph.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "Riostream.h"
#include <iostream>

#include "Go4Event/TGo4EventElement.h"
#include "Go4Event/TGo4EventProcessor.h"
#include "Go4Analysis/TGo4Analysis.h"
#include "Go4EventServer/TGo4MbsFile.h"

#include "MoDSSAnlEvent.h"
#include "MoDSSCalibParameter.h"
#include "MoDSSCorrelParameter.h"
#include "MoDSSHardwareParameter.h"
#include "MoDSSParameter.h"

#include "Go4ConditionsBase/TGo4WinCond.h"
#include "Go4ConditionsBase/TGo4PolyCond.h"

#include "MoDSSGlobals.h"

// macro max
#define  maxdef(a,b) ( ((a) > (b)) ? (a) : (b) )


class MoDSSCorrelEvent;

class MoDSSCorrelProc : public TGo4EventProcessor {
	public:
		MoDSSCorrelProc();
		MoDSSCorrelProc(const char* name);
		virtual ~MoDSSCorrelProc() ;
		void MoDSSCorrelationSearch(MoDSSCorrelEvent* OutputEvent);

		TGo4PolyCond		*fCondEYvsTOF;      // TOF vs Energy high
		TGo4PolyCond		*fCondEXvsTOF;      // TOF vs Energy low
		// TGo4PolyCond		*fCondGaGa;      // Gamma - Gamma window (for scattered gamma)

		// correlation functions
		Bool_t   TestPositions(Int_t IDX1, Int_t IDY1, Int_t IDX2, Int_t IDY2);
		Bool_t   TestRecoilLow(MoDSSAnlEvent *re);
		Bool_t   TestRecoilHigh(MoDSSAnlEvent *re);
		void     ProcessRecoil();
		void     ProcessAlpha();
		void     ProcessFission();
		void     ProcessGamma();
		Int_t    RecoilAlpha(Double_t dtmin, Double_t dtmax, Int_t gen_prt);
		Int_t    GammaAlpha(Double_t dtmin, Double_t dtmax, Int_t gen_prt);
		Int_t    RecoilFission();
		Int_t    RecoilGamma();
		Int_t    AlphaGamma(Int_t gen, Double_t dtmin, Double_t dtmax);
		Int_t    AlphaFission(Int_t gen, Double_t dtmin, Double_t dtmax);
		Int_t    AlphaAlpha(Int_t gen, Double_t dtmin, Double_t dtmax, Int_t gen_prt);
		Int_t    GammaFission();
		Int_t    GammaGamma();

		Int_t    PrintChain(MoDSSAnlEvent *event, Int_t gen_last, Int_t chnum, Int_t gen_to);
		void     PrintThisChain(MoDSSAnlEvent *chain[MAXALPHA+1]);
		Int_t    FillRecoilSpectra(MoDSSAnlEvent *re, MoDSSAnlEvent *nextMember);
		Int_t    FillAlphaSpectra(MoDSSAnlEvent *al, MoDSSAnlEvent *nextMember, Int_t gen);
		Int_t    FillFissionSpectra(MoDSSAnlEvent *fi);
		Int_t    FillGammaSpectra(MoDSSAnlEvent *gamma, MoDSSAnlEvent *nextMember);
		Int_t    FillGamma2Spectra(MoDSSAnlEvent *gamma);

		//Julien
		Bool_t 	GammaSingleFired();
		Bool_t 	GammaOnlySingleFired();
		Bool_t 	GammaDiagFired();
		Double_t	SumGamma();	

		//ofstream Log;

		// input event (from Anl step)
		MoDSSAnlEvent        *fInpEvent;
		// parameters
		MoDSSCorrelParameter *fCorrelPar;
		MoDSSParameter       *fParam;
		MoDSSCalibParameter  *fCalPar;
		MoDSSHardwareParameter           *fHardwareParam;

		// correlation variables
		Bool_t           fbNewSubmit;   // TRUE if new analysis was just submited
		Bool_t           fbAllAlphas;   // determines if all alphas must be present in the printed chain
		Int_t            fGenTotal;     // total number of generation requested
		Int_t            fNumCorrHelp;
		MoDSSAnlEvent   *fReTable;     // recoil table
		Int_t            fReLast;       // next position in the table (after the last added recoil)
		Int_t            fReCounter;    // number of recoils in the table
		Double_t         fReTableTimeLimit;  // time limit for recoil table
		MoDSSAnlEvent   *fAlTable;     // alpha table
		Int_t            fAlLast;       // next position in the table (after the last added alpha)
		Int_t            fAlCounter;    // number of alphas in the table
		Double_t         fAlTableTimeLimit;  // time limit for alpha table
		Double_t         fAlEnMin;      // minimum energy limit for alpha table
		Double_t         fAlEnMax;      // maximum  -------  ||  ----------
		MoDSSAnlEvent   *fGaTable;     // gamma table
		Int_t            fGaLast;       // next position in the table (after the last added alpha)
		Int_t            fGaCounter;    // number of alphas in the table
		Double_t         fGaTableTimeLimit;  // time limit for alpha table			
		TGo4MbsFile     *fMBSfile;      // pointer to mbs file (to access filename) 

		TTree *tcorrel_tree;
		//   TFile *fcorrel_tree;

		Double_t ekev_X;
		Double_t ekev_Y;

		Int_t Idx;
		Int_t Idy;
		Double_t ekev_Gam1;
		Double_t ekev_Gam2;
		Double_t ekev_Gam3;
		Double_t ekev_Gam4;
		Double_t Beam_Pulse;
		Double_t tof1;
		Double_t sample_time_x;
		Double_t sample_time_y;
		Double_t sample_time_ge;
		Long64_t trig_time; //Time of the trigger



		// conditions

		// histograms
		TH1D		*fhCorTIME;      // number of correlations in time
		TH2D		*fhChainPos;     // position of the chain (first member) in the stop detector (top position taken)
		TH2D		*fhAlphaMo_vs_AlphaDa;		//(BA)
		TH1D		 *fh_dT_ER_Alpha;
		// --- recoil    
		TH1D		*fhReTOF;        // TOF of correlated recoil
		TH1D		*fhReLGaCoinc;		// Low Energy Gammas coinciding recoil
		TH1D		*fhReHGaCoinc;		// High Energy Gammas coinciding recoil
		TH1D		*fhCountRe;      // recoil counts in the stop detector
		TH1D		*fhDTReFi;       // time difference recoil - fission
		TH1D			*fhDTReGamma; // time difference recoil - fission for specific clover
		TH1D		*fhDTReGammaBG;    // time difference recoil - gamma in random pulses
		TH1D		*fhDTReAl;       // time difference recoil - alpha
		TH2D		*fhDTvsEAl;      // 2D - DT (re-al) vs alpha energy


		//==========================================// Boris
		TH2D		*fhReGavsGaCoinc;
		TH2D		*fhReGavsTAC;		
		TH2D		*fhReGavsTAC1;		
		TH2D		*fhReGavsTAC2;		
		TH2D		*fhReGavsTAC3;		
		TH2D		*fhReGavsTAC4;		
		TH2D		*fhReGavsAlpha;
		TH2D		*fhReGavsAlpha_BOXincl;
		TH1D		*fhReLGaCoincAnti;
		TH1D		*fhReLGaCoinc1;
		TH1D		*fhReLGaCoinc2;
		TH1D		*fhReLGaCoinc3;
		TH1D		*fhReLGaCoinc4;
		TH2D		*fhTopLowvsHigh_strip[16];
		TH2D		*fhBottomLowvsHigh_strip[16];
		TH2D		*fhElGa1Coinc_strip[16];
		TH2D		*fhRe_EGvsEBOX_strip[32];
		TH2D		*fhDyReAlvsEtop;
		TH2D		*fhDyReAlvsEbottom;
		TH2D		*fhSTOPvsBOX;
		TH2D		*fhSTOPvsBOX_rtg;
		TH2D		*fhSTOPvsBOX_rtg_ac;
		TH2D		*fhSTOPvsEG_BOX;
		TH1D		*fhRe_EBKEV;
		TH2D		*fhElGa1Coinc_BOX;
		//==========================================
		//==========================================//Julien
		Int_t		ix; //Index on multiplicity X to follow the currently used hit
		Int_t		iy;//Index on multiplicity Y to follow the currently used hit
		Int_t		fepsT; //Time difference threshold for X-Y correlations
		Int_t		fepsE; //Energy difference between X and Y signal
		Double_t	fThresEx;
		Double_t	fThresEy;

		TH1D		*fhEX_Single;
		TH1D		*fhEXAl_Single;
		TH1D		*fhEY_Single;
		TH1D		*fhEYAl_Single;

		TH2D		*fhExEy;

		TH1D		*fhdTXY;

		TH2D		*fdReInput_EyTOF;
		//Check what is sent in the tables Re and Al
		//Recoil - TestRecoilHigh
	/*	TH1D		*fhReTRH_True_Ex;
		TH1D		*fhReTRH_True_Ey;
		TH1D		*fhReTRH_True_X;
		TH1D		*fhReTRH_True_Y;
		TH1D		*fhReTRH_True_TOF;
		TH1D		*fhReTRH_False_Ex;
		TH1D		*fhReTRH_False_Ey;
		TH1D		*fhReTRH_False_X;
		TH1D		*fhReTRH_False_Y;
		TH1D		*fhReTRH_False_TOF;

		TH2D		*fdReTRH_True_EyTOF;
		TH2D		*fdReTRH_False_EyTOF;
		//Recoil - Sent to Table
		TH1D		*fhReTable_True_Ex;
		TH1D		*fhReTable_True_Ey;
		TH1D		*fhReTable_True_X;
		TH1D		*fhReTable_True_Y;
		TH1D		*fhReTable_True_TOF;
		TH1D		*fhReTable_False_Ex;
		TH1D		*fhReTable_False_Ey;
		TH1D		*fhReTable_False_X;
		TH1D		*fhReTable_False_Y;
		TH1D		*fhReTable_False_TOF;

		TH2D		*fdReTable_True_EyTOF;
		TH2D		*fdReTable_False_EyTOF;

		//Alpha - TestRecoilHigh
		TH1D		*fhAlTRH_True_Ex;
		TH1D		*fhAlTRH_True_Ey;
		TH1D		*fhAlTRH_True_X;
		TH1D		*fhAlTRH_True_Y;
		TH1D		*fhAlTRH_True_TOF;
		TH1D		*fhAlTRH_False_Ex;
		TH1D		*fhAlTRH_False_Ey;
		TH1D		*fhAlTRH_False_X;
		TH1D		*fhAlTRH_False_Y;
		TH1D		*fhAlTRH_False_TOF;

		//Alpha - Sent to Table
		TH1D		*fhAlTable_True_Ex;
		TH1D		*fhAlTable_True_Ey;
		TH1D		*fhAlTable_True_X;
		TH1D		*fhAlTable_True_Y;
		TH1D		*fhAlTable_True_TOF;
		TH1D		*fhAlTable_False_Ex;
		TH1D		*fhAlTable_False_Ey;
		TH1D		*fhAlTable_False_X;
		TH1D		*fhAlTable_False_Y;
		TH1D		*fhAlTable_False_TOF;
		*/

		//=========================================

		// recoil in low energy branch
		TH1D		*fhEReX;       // energy of cor. recoil (low branch)
		TH2D		*fhTOFvsEReX;  // tof vs. recoil energy  
		TH2D		*fhTOFvsEReX_Al;  // tof vs. recoil energy with alpha selection
		// ------ || -----  recoil - fission
		TH2D		*fhEReXvsEFi;  // recoil energy (low) vs fission energy
		TH1D		*fhDYtopReLowFi;    // position difference recoil-fission (top)
		TH1D		*fhDYtopReLowFi_strip[16];  // position difference recoil-fission (top) - for each strip
		TH1D		*fhDYbottomReLowFi;    // position difference recoil-fission (bottom)
		TH1D		*fhDYbottomReLowFi_strip[16];  // position difference recoil-fission (bottom) - for each strip
		// ------ || -----  recoil - alpha 
		TH2D		*fhEReXvsEAl;  // recoil energy vs alpha energy
		TH1D		*fhDYtopReLowAl;    // position difference recoil-alpha (top)
		TH1D		*fhDYtopReLowAl_strip[16];  // position difference recoil-alpha (top) - for each strip
		TH1D		*fhDYbottomReLowAl;    // position difference recoil-alpha (bottom)
		TH1D		*fhDYbottomReLowAl_strip[16];  // position difference recoil-alpha (bottom) - for each strip

		// recoil in high energy branch
		TH1D		*fhEReY;      // energy of cor. recoil (high branch)
		TH1D		*fhReEAl;      // energy of cor. recoil alpha (high branch)
		TH2D		*fhTOFvsEReY; // tof vs. recoil energy  
		TH2D		*fhTOFvsEReY_Al; // tof vs. recoil energy with alpha selection
		// ------ || -----  recoil - fission
		TH2D		*fhEReYvsEFi;  // recoil energy (high) vs fission energy
		TH1D		*fhDYtopReHighFi;    // position difference recoil-fission (top)
		TH1D		*fhDYtopReHighFi_strip[16];  // position difference recoil-fission (top) - for each strip
		TH1D		*fhDYbottomReHighFi;    // position difference recoil-fission (bottom)
		TH1D		*fhDYbottomReHighFi_strip[16];  // position difference recoil-fission (bottom) - for each strip
		// ------ || -----  recoil - alpha
		TH2D		*fhEReYvsEAl;  // recoil energy vs alpha energy
		TH1D		*fhDYtopReHighAl;     // position difference recoil-alpha (top)
		TH1D		*fhDYtopReHighAl_strip[16];   // position difference recoil-alpha (top) - for each strip
		TH1D		*fhDYbottomReHighAl;     // position difference recoil-alpha (bottom)
		TH1D		*fhDYbottomReHighAl_strip[16];  // position difference recoil-alpha (bottom) - for each strip
		TGraph		*fhcorYHTvsYLT[16];      // position calibration: YLT vs YHT from correlations
		TGraph		*fhcorYHBvsYLB[16];      // position calibration: YLB vs YHB from correlations
		TGraph		*fhcorYHTvsYLTcal[16];      // position calibrated: YLT vs YHT from correlations
		TGraph		*fhcorYHBvsYLBcal[16];      // position calibrated: YLB vs YHB from correlations

		TH2D 		*h_dssd_ExdT;

		//julien
		TH1D		*fhEReY_check;
		TH1D		*fhEReX_check;
		TH1D		*fhEReY_check2;
		TH1D		*fhEReX_check2;
		TH1D		*fhGammaMult;

		// --- alpha
		TH1D    	*fhCountAl;		// alpha counts in the stop detector
		TH1D		*fhEAlphaAll;		// alpha energy
		TH2D		*fhCorAlphaGamma2D;	// alpha - gamma coincidences for correl alphas
		TH1D		*fhEAlpha[MAXALPHA];	// alpha energy for each generation separately
		TH1D		*fhGamAl[MAXALPHA];	// gamma coincident to alpha (for each generation separately)
		TH2D		*fhEGvsEAl_1_corr;	// Al-Ga coincidences for first generation alpha
		//--Tests pour la transmission des alphas dans la correlation
		TH1D		*fhEAlphaTable;		//Alpha energies stored in fAlTable
		TH1D		*fhEAlphaReAl;		//Alpha energies transmitted to RecoilAlpha()
		TH1D		*fhEAlphaCorred;	//Alpha energies correlated to a Re in RecoilAlpha();
		TH1D		*fhEAlphaTimeOK;	//Alpha energies correlated in time to a Re 
		TH1D		*fhEAlphaTimePosOK;	//Alpha energies correlated in time and position to a Re
		TH1D		*fhEAlphaPrint;		//Alpha energies correlated in time and position to a Re and sent to PrintChain

		// ------ || -----  alpha - fission
		TH1D		*fhDTAlFi;                 // time difference alpha - fission
		TH1D		*fhDTAlGamma;              // time difference alpha - gamma
		TH2D		*fhEAlvsEFi;               // alpha energy vs fission energy
		TH1D		*fhDYtopAlFi;              // position difference alpha-fission (top)
		TH1D		*fhDYbottomAlFi;           // position difference alpha-fission (bottom)
		TH1D		*fhDYtopAlFi_strip[16];    //  position difference alpha-fission (top) - for each strip
		TH1D		*fhDYbottomAlFi_strip[16]; //  position difference alpha-fission (bottom) - for each strip
		// ------ || -----  alpha - alpha
		TH2D		*fhEAlMothervsEAlDaughter; // 2D energy - alpha mother vs. alpha daughter (1st chain only)
		TH2D		*fhEAlMothervsEAlDaughter_BoxIncl; // 2D energy - alpha mother vs. alpha daughter with included box (1st chain only)
		TH2D		*fhEAlMothervsEAlDaughter_AllChains; // 2D energy - alpha mother vs. alpha daughter (Alternative chains incl.)
		TH2D		*fhEAlMothervsEAlDaughter_BoxIncl_AllChains; // 2D energy - alpha mother vs. alpha daughter with included box (Alternative chains incl.)
		TH1D		*fhDTAlAl[MAXALPHA-1];     // time difference alpha-alpha  - for each gen (except 1st and last)
		TH1D		*fhDYtopAlAl;              // position difference alpha-alpha (top)
		TH1D		*fhDYbottomAlAl;           // position difference alpha-alpha (bottom)
		TH1D		*fhDYtopAlAl_strip[16];    //  position difference alpha-alpha (top) - for each strip
		TH1D		*fhDYbottomAlAl_strip[16]; //  position difference alpha-alpha (bottom) - for each strip
		TH1D		*fhXAl;				//X Position of first gen alpha
		TH1D		*fhYAl;				//Y position of first gen alpha

		TH2D		*fhDTAl1Al2vsEAl2; // pridala som tento riadok (Zdenka)
		TH2D		*fhDTGa1Ga2vsEGa2; // pridala som tento riadok (Zdenka)

		// --- fission
		TH1D		*fhEFissionStopDet;     // fission energy stop detector part
		TH1D		*fhEFissionStopDetOnly;     // fission energy events with stop detector only
		TH1D		*fhEFissionStopBoxDet;     // fission energy events with stop+box detector
		TH1D		*fhDTGaFi;				//GaFi time
		// --- gamma ------------------
		TH1D		*fhEGamma;      // gamma energy
		TH1D		*fhEGammaClo[20];      // gamma energy - Crystal 1
		TH1D		*fhEGammaBestRes;      // gamma energy for crystals with best resolution
		TH2D		*fhEGvsT;      // gamma energy vs. correlation time
		TH2D		*fhEGvsT_RNDM;      // gamma energy vs. correlation time for random correlations
		TH2D		*fhEGvsT_TMP;      // gamma energy vs. correlation time from TMP
		TH2D		*fhEGvsT_RNDM_TMP;      // gamma energy vs. correlation time for random correlations from TMP
		TH2D		*fhEGvsT_Clover[NUMCRYSTAL];    // gamma energy vs correlation time for different crystals
		TH2D		*fhEGvsPulse;      // gamma energy vs. time within pulse
		TH1D		*fhEGammaHigh;  // gamma energy - High energy
		TH2D		*fhEGCoinc2D;		//coincidences of delayed gammas
		TH2D		*fhEGCoincVsGaGaSum; // gamma incoincidecne with GaGa coincidence in window fCondGaGa
		//TH1D		*fhEGCoincToGaGa; // gamma incoincidecne with GaGa coincidence in window fCondGaGa
		TH2D		*fhEGvsTAC;		  //TAC (stop-gamma) vs Energy gamma
		//TH1D		*fhEGCoinc1D;     // gamma energy
		TH2D		*fhElGa1Coinc;  // Electron gamma 1 coincidences
		TH2D		*fhElGa1CoincBestRes;  // Electron gamma 1 coincidences	for cyrstals with best resolution
		// ---- gamma 2 -----------------
		TH1D		*fhEGamma2;     // gamma energy
		TH1D		*fhEGamma2High;     // gamma energy - High energy
		TH2D		*fhEG2Coinc2D;		//coincidences of delayed gammas
		TH2D		*fhEG2EG1corr;     // gamma1 - gamma2 energy
		TH1D		*fhDTGa1Ga2;       // time difference gamma1 - gamma2
		TH2D		*fhElGa2Coinc; // Electron gamma 2 coincidences
		// ------ || -----  alpha - gamma
		TH2D		*fhEAlEGadelayed; // 2D energy - alpha mother vs. gamma daughter
		TH2D		*fhEGaEAldelayed; // 2D energy - alpha daughter vs. gamma mother

		ClassDef(MoDSSCorrelProc,1)   
};

#endif //MoDSSCORRELPROCESSOR_H
