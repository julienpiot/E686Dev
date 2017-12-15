//-------------------------------------------------------------
//        Go4 Release Package v3.03-05 (build 30305)
//                      05-June-2008
//---------------------------------------------------------------
//   The GSI Online Offline Object Oriented (Go4) Project
//   Experiment Data Processing at EE department, GSI
//---------------------------------------------------------------
//
//Copyright (C) 2000- Gesellschaft f. Schwerionenforschung, GSI
//                    Planckstr. 1, 64291 Darmstadt, Germany
//Contact:            http://go4.gsi.de
//----------------------------------------------------------------
//This software can be used under the license agreements as stated
//in Go4License.txt file which is part of the distribution.
//----------------------------------------------------------------
#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "MoDSSUnpackProc.h"
#include "stdint.h"
#include "Riostream.h"

#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCutG.h"
#include "snprintf.h"

#include "TGo4MbsEvent.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4Picture.h"
//#include "MoDSSUnpackParam.h"
#include "TGo4Fitter.h"
#include "TMath.h"
#include "MoDSSCalibParameter.h"
#include "MoDSSFilterParameter.h"


//#define WR_TIME_STAMP     1   // white rabbit time stamp is head of data
#define USE_MBS_PARAM     1   //

#define MAX_SFP           4
#define MAX_SLAVE        12
#define N_CHA            16

#ifdef WR_TIME_STAMP
#define SUB_SYSTEM_ID      0x100 // sub-system identifier pci express

#define TS__ID_L16         0x0f7
#define TS__ID_M16         0x1f7
#define TS__ID_H16         0x2f7
#endif // WR_TIME_STAMP

#ifdef USE_MBS_PARAM
#define MAX_TRACE_SIZE    4000   // in samples
#define MAX_TRAPEZ_N_AVG  1000   // in samples
#else
#define     TRACE_SIZE    1024   // in samples
#define     TRAPEZ_N_AVG    64   // in samples
// nr of slaves on SFP 0    1  2  3
//                     |    |  |  |
#define NR_SLAVES    { 12,  0, 0, 0}
#define ADC_TYPE     {  0,  0, 0, 0} // 12 bit: 0, 14 bit: 1
                                        // bit 0 fuer slave module 0
                                        // bit n fuer slave module n (max n = 31)
#endif

#define RON  "\x1B[7m"
#define RES  "\x1B[0m"

#define BASE_LINE_SUBT_START  0
#define BASE_LINE_SUBT_SIZE   150

#include "TGo4EventProcessor.h"
#include "MoDSSUnpackEvent.h"

#include <iostream>
#include <fstream>
using namespace std;

//#include "MWD2.h"

class MoDSSFilterParameter;
class TGo4Fitter;
using std::vector;
class MoDSSUnpackProc : public TGo4EventProcessor {
public:
	MoDSSUnpackProc() ;
	MoDSSUnpackProc(const char* name);
	virtual ~MoDSSUnpackProc() ;
	void f_make_histo (Int_t);

	Int_t trace_count;
	ofstream traces_Th;

	Bool_t BuildEvent(TGo4EventElement* target); // event processing function

private:
	TGo4MbsEvent  *fInput;  //!
	MoDSSUnpackEvent* fOutput; //!
	MoDSSCalibParameter *uCal;
	MoDSSFilterParameter *pFil;

	TH1	*h_trace        [MAX_SFP][MAX_SLAVE][N_CHA];  //!
	TH1	*h_trace_blr    [MAX_SFP][MAX_SLAVE][N_CHA];  //!
	TH1    	*h_peak 	[MAX_SFP][MAX_SLAVE][N_CHA];  //!
	TH1	*h_peak_back    [MAX_SFP][MAX_SLAVE][N_CHA];  //!
	TH1	*h_peak_box     [MAX_SFP][MAX_SLAVE][N_CHA];  //!
	//Double_t TOF	[MAX_SFP][MAX_SLAVE][N_CHA];
	Double_t trace_height  	[1][12][16];
	TH1	*h_peak_gamma	[MAX_SFP][MAX_SLAVE][N_CHA];  //!
	TH1	*histo_gated;
	TH1	*h_Ynj		[MAX_SFP][MAX_SLAVE][N_CHA];
	TH1	*h_fpga_e	[MAX_SFP][MAX_SLAVE][N_CHA];
	TH1	*h_mwd_E	[MAX_SFP][MAX_SLAVE][N_CHA];  //! MWD
        TH1	*h_mwd_traps	[MAX_SFP][MAX_SLAVE][N_CHA];  //! MWD
        TH1	*h_mwd_traps2	[MAX_SFP][MAX_SLAVE][N_CHA];  //! MWD
        TH1	*h_trapez_fpga	[MAX_SFP][MAX_SLAVE][N_CHA];  //! MWD
        TH1     *h_trace_filt        [MAX_SFP][MAX_SLAVE][N_CHA];  //!
        TH1     *h_macro;  //!
        TH1     *h_tof;  //!
        TH1     *h_macro_fill;  //!
        TH1     *h_tof_fill;  //!
	//Check the time difference between times of events and subevents.
	TH1	*fhD_ll_time;
	TH1	*fhD_Inv_ll_time;
	TH1	*fhD_ll_trg_time;
	TH1	*fh_l_hit_time;
	TH2	*fhD_l_hit_time;	
	TH2	*fh_X1vsX2_hit_time_zero;
	TH2	*fh_X1vsX2_hit_time_281;	
	TH2	*fh_E1vsE2_hit_time_zero;
	TH2	*fh_E1vsE2_hit_time_281;	

	TH1 	*h_hits;

	Int_t 	c_X;
	Int_t	c_Y;

	UInt_t         l_i, l_j, l_k, l_l;
	uint32_t      *pl_se_dat;
	uint32_t      *pl_tmp;

	UInt_t         l_dat_len;
	UInt_t         l_dat_len_byte;

	UInt_t         l_dat;
	UInt_t         l_trig_type;
	UInt_t         l_trig_type_triva;
	UInt_t         l_sfp_id;
	UInt_t         l_sfp_id_sav;
	UInt_t         l_feb_id;
	UInt_t         l_feb_id_sav;
	UInt_t         l_cha_id;
	UInt_t         l_cha_id_sav;
	UInt_t         l_n_hit;
	UInt_t         l_hit_id;
	UInt_t         l_hit_cha_id;
	UInt_t         l_hit_cha_id_sav;
	Long64_t       ll_time;
	Long64_t       ll_trg_time;
	Long64_t       ll_hit_time;
	Long64_t       ll_time_sav;
	Long64_t       ll_trg_time_sav;
	Long64_t       l_hit_time_sav;
	Long64_t       l_hit_time_first;
	Double_t 	Ener;
	Double_t 	Ener_sav;

	Int_t		subcount;
	
	UInt_t         l_ch_hitpat   [MAX_SFP][MAX_SLAVE][N_CHA];
	UInt_t         l_ch_hitpat_tr[MAX_SFP][MAX_SLAVE][N_CHA];
	UInt_t         l_first_trace [MAX_SFP][MAX_SLAVE];

	UInt_t         	l_cha_head;
	UInt_t         	l_cha_size;
	UInt_t         	l_trace_head;
	UInt_t         	l_trace_size;
	UInt_t         	l_trace_trail;

	UInt_t         	l_spec_head;
	UInt_t         	l_spec_trail;
	UInt_t         	l_n_hit_in_cha;
	UInt_t         	l_only_one_hit_in_cha;
	UInt_t         	l_more_than_1_hit_in_cha;
	UInt_t         	l_e_filt_out_of_trig_wind;
	UInt_t         	l_hit_time_sign[MAX_SFP][MAX_SLAVE][N_CHA];
	Int_t		l_hit_time[MAX_SFP][MAX_SLAVE][N_CHA];
	UInt_t         	l_hit_cha_id2;
	UInt_t        	l_fpga_energy_sign;
	Int_t         	l_fpga_energy;

	UInt_t         l_trapez_e_found [MAX_SFP][MAX_SLAVE][N_CHA];
	UInt_t         l_fpga_e_found   [MAX_SFP][MAX_SLAVE][N_CHA];
	UInt_t         l_trapez_e       [MAX_SFP][MAX_SLAVE][N_CHA];
	UInt_t         l_fpga_e         [MAX_SFP][MAX_SLAVE][N_CHA];

// 	UInt_t         l_dat_fir;
// 	UInt_t         l_dat_sec;

	UInt_t         l_bls_start;
	UInt_t         l_bls_stop;
	Double_t       f_bls_val;

	Int_t       l_fpga_filt_on_off;
	Int_t       l_fpga_filt_mode;
	Int_t       l_dat_trace;
	Int_t       l_dat_filt;
	Int_t       l_filt_sign;

	Bool_t 		b_first_event;

        UInt_t Macrocount;
        UInt_t Tofcount;
        //UInt_t 	    gal_gate;
       // UInt_t 	    trace_height;
	
	//Variables for MWD
	Int_t tau;
	Int_t alpha;
	Int_t adj;
	bool Z;
 	Int_t k;
	Int_t m;
	Int_t c;
	Double_t  d[MAX_TRACE_SIZE];
	Double_t  S2[MAX_TRACE_SIZE];
	Double_t  r2[MAX_TRACE_SIZE];
	double Sum;
	double BL;
	double dX;
	Short_t X[MAX_TRACE_SIZE];
	Double_t Par[3];
	Double_t Tval;
	TF1 *gfit;
	TH2I * hFitTau;
	TH1 *hTau[160];
	TF1 *gfit2;
	Double_t Par2[3];
/*	
	TH1* h_mwd_res_adj[100];
	TH1* h_mwd_res_tau[100];
	TH1* h_mwd_res_c[100];*/
	//Int_t vtau;
	//Int_t vc;
	//Int_t vadj;
	//Int_t ctau;
	//Int_t cadj;
	//Int_t cc;


	TGo4MbsSubEvent* psubevt;

	ClassDef(MoDSSUnpackProc,2)
};
#endif //TUNPACKPROCESSOR_H

//----------------------------END OF GO4 SOURCE FILE ---------------------
