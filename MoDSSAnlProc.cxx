//A.K.Mistry
//1.07.15
//Include Pulse/pause, TOF coin should be similar to the TOF coin

//B. Andel
//03/2016
//re-designed analysis part and structure of variables to correspond to specific detectors
//instead of using variables l_E[12][16], l_T[12][16]
//properly implemented MapHardware, included parameter files MoDSS-input.dat and MoDSS-hardware.dat,
//implemented box detectors, polygonal conditions for recoils, defined output event (AnlEvent)
//for correlation step and other changes

//J. Piot
//03/2016
//Adapted to the LISE E686 experiment


#include <iomanip>
#include "MoDSSAnlProc.h"
#include "MoDSSAnlEvent.h"
#include "MoDSSCalibParameter.h"
#include "TFile.h"
#include "TTree.h"

ofstream   gOutputFile;	
ofstream 	aLog;

//***********************************************************
// Constructor - Initialization of all class members
// this one is used in MoDSSAnlFact

MoDSSAnlProc::MoDSSAnlProc(Text_t * cname):
	TGo4EventProcessor(cname)
{
	//aLog.open("/home/e656/E686_Analyse/localcopy/VerifTable.txt",ios::app);
	//if(aLog)cout<<"aLog Anl open"<<endl;
	//else cout<<"Pb , aLog not opened"<<endl;	
	//===================== initialize all to zero ===========================
	for(int i=0;i<60;i++){
		EnonCalX[i] = 0;
		EkevX[i] = 0;
		TimeX[i] = 0;
		EnonCalY[i] = 0;
		EkevY[i] = 0;
		TimeY[i] = 0;
		if(i<32){
			EnonCalBox[i] = 0;
			EkevBox[i] = 0;
			TimeBox[i] = 0;
		}
		if(i<20){
			EnonCalGam[i] = 0;
			EkevGam[i] = 0;
			TimeGam[i] = 0;
		}
	}
	E_kevX = 0;
	E_kevY = 0;
	E_kevBox = 0;
	IDX = 0; 
	IDY = 0;
	IDB = 0;
	sample_time_x = 0;
	sample_time_y = 0;
	sample_time_box = 0;
	for(int i=0; i<20; i++) sample_time_ge[i] = 0;

	l_t_trig=-1;
	l_sfp = -1;
	l_feb = -1;
	l_cha = -1;

	NbEventX = 0;
	NbEventY = 0;

	/////////////////////////////////////////////



	// boolean member variable used to tell the static variables in MoDSSEventAnalysis()
	// to be cleared each time a new analysis settings were submitted
	fbNewSubmit = kTRUE;

	//// init user analysis objects:

	// ---- 1. parameters
	// parameters are created in MoDSSAnalysis. (NOT ANY MORE !!!)

	cout.flags(ios::fixed); // now 'precision' means number of digits after dot
	cout.precision(4);

	// fParam contains info whether or not to fill
	// the YL-YH calib (fYH_YL_Calib) and Back detector calib (fBack_Calib) graphs.
	fParam = new MoDSSParameter("MoDSSPar");
	AddParameter(fParam);
	//fParam = (MoDSSParameter*)  GetParameter("MoDSSPar"); // get parameter from the memory (or autosave file)
	if (fParam) fParam->PrintParameter(0,0);
	else cout << "**** ERRR - MoDSSPar doesn't exist - program will crash.\n";

	//hardware parameter
	fHardwareParam = (MoDSSHardwareParameter*)  GetParameter("MoDSSHardwarePar");
	if(!fHardwareParam)
	{
		fHardwareParam = new MoDSSHardwareParameter("MoDSSHardwarePar");
		AddParameter(fHardwareParam);
		// if Hardware parameter set exists - print it out, otherwise warn the user.
		if (fHardwareParam) fHardwareParam->PrintParameter(0,0);
		else cout << "***** ERR - HardwareParam doesn't exist - program will crash.\n";
	}
	if (fHardwareParam) fHardwareParam->PrintParameter(0,0);
	else cout << "***** ERR - HardwareParam doesn't exist - program will crash.\n";

	// calibration parameter - contains all calibration values
	fCal = new MoDSSCalibParameter("CalibPar");
	AddParameter(fCal);
	// fCal = (MoDSSCalibParameter*) GetParameter("CalibPar");
	if (fCal) fCal->PrintParameter(0,0);
	else cout << "**** ERRR - CalibPar doesn't exist - program will crash.\n";

	// we must check, if the conditions have been restored from the auto-save file
	if (GetAnalysisCondition("CondELB") == 0) 
	{  // not found, create them	

		fCondELB = new TGo4WinCond("CondELB");  // ELB condition
		fCondELB -> SetValues(fParam->fELB[0], fParam->fELB[1]);
		AddAnalysisCondition(fCondELB);
		fCondELB -> Enable();	

		//Polygonal TOFvsEX window
		Double_t TOFXvalues[6]={1362,1379,1398,1398,1362,1362};
		Double_t EXvalues[6]={17.3,20.4,26.0,40.0,40.0,17.3};
		TCutG* TOFvsEXcut = new TCutG("initialcut",6,TOFXvalues,EXvalues);
		fCondEXvsTOF_ANL = new TGo4PolyCond("TOFvsEX_ANL");
		fCondEXvsTOF_ANL -> SetValues(TOFvsEXcut);	
		AddAnalysisCondition(fCondEXvsTOF_ANL);
		fCondEXvsTOF_ANL -> Enable();
		delete TOFvsEXcut;

		//Polygonal TOFvsEY window
		TFile FCutY("/home/e656/E686_Analyse/localcopy/Db_E-ToFCut-23-05-16.root");
		if(FCutY.IsOpen()){cout<<"rootfile for TCut open"<<endl;
			//Double_t TOFYvalues[9]={-210.641,-208.849,-203.472,-183.758,-78.0164,65.3615,303.727,533.132,-210.641};
			//Double_t EYvalues[9]={15890.8,15936.3,9069.23,6022.26,3202.68,2247.66,1565.51,1429.08,15890.8};
			//Double_t TOFYvalues[6]={1362,1379,1398,1398,1362,1362};
			//Double_t EYvalues[6]={17.3,20.4,26.0,40.0,40.0,17.3};
			//TCutG* TOFvsEYcut = new TCutG("initialcut",9,TOFYvalues,EYvalues);

			RemoveAnalysisCondition("TOFvsEY_ANL");
			fCondEYvsTOF_ANL = new TGo4PolyCond("TOFvsEY_ANL");
			//fCondEYvsTOF_ANL -> SetValues(TOFvsEYcut);	
			AddAnalysisCondition(fCondEYvsTOF_ANL);
			fCondEYvsTOF_ANL -> Enable();
			//delete TOFvsEYcut;
			*fCondEYvsTOF_ANL = *( (TGo4PolyCond*) FCutY.Get("TOFvsEY"));
			FCutY.Close();
		}
	}
	else
	{
		fCondELB = (TGo4WinCond *)GetAnalysisCondition("CondELB");
		fCondEXvsTOF_ANL = (TGo4PolyCond *)GetAnalysisCondition("TOFvsEX_ANL");
		fCondEYvsTOF_ANL = (TGo4PolyCond *)GetAnalysisCondition("TOFvsEY_ANL");
	}

	//====================== Creating histograms =========================
	//variables
	float max_x_nocalib = 8192.;
	float min_x_nocalib =0.;
	float num_bin_x_nocalib = 4048.;

	float max_y_nocalib = 8192.;
	float min_y_nocalib =0.;
	float num_bin_y_nocalib = 4048.;

	float min_strip_dssd = 1;
	float max_strip_dssd = 60;
	float bin_strip_dssd = 60;

	float min_strip_box = 1;
	float max_strip_box = 32;
	float bin_strip_box = 32;

	float min_strip_ge = 1;
	float max_strip_ge = 20;
	float bin_strip_ge = 20;

	float max_x_calib = 16000.;
	float min_x_calib = 0.;
	float num_bin_x_cal = 4000.;

	float max_y_calib = 70000.;
	float min_y_calib = 0.;
	float num_bin_y_cal = 7000.;

	float max_box_calib = 16000.;
	float min_box_calib = 0.;
	float num_bin_box_cal = 4000.;

	float max_ge_calib = 2000.;
	float min_ge_calib = 0.;
	float num_bin_ge_cal = 2000;

	float max_tof = 1050;
	float min_tof = -750;
	float num_bin_tof = 1800;

	float max_y_calib2D = 12000;             //for 2D histograms
	float min_y_calib2D = 4000;
	float num_bin_y_cal2D = 2000;

	float num_bin_x_cal2D = 2000;

	float max_ge_calib2D = 2000;
	float min_ge_calib2D = 0;
	float num_bin_ge_cal2D = 2000;

	Text_t    name[50], descr[50], name1[50], descr1[50], name2[50], descr2[50], name4[50], descr4[50], name5[50], descr5[50], name6[50], descr6[50], name7[50], descr7[50];
	Int_t     i;



	//sum spectra
	//DSSD X antiTOF
	sprintf(name2,"EA_KEVX");
	sprintf(descr2, "Energy X antiTOF");
	RemoveHistogram(name2);
	hEA_KEVX=new TH1D(name2,descr2,num_bin_x_cal, min_x_calib, max_x_calib);
	AddHistogram(hEA_KEVX,"Anl/Energy-DSSD");

	sprintf(name, "EA_KEVX_Strip20_NoCal");
	sprintf(descr, "Energy X antiTOF, strip 20 No Cal");
	RemoveHistogram(name);
	hEA_KEVX_Strip20 = new TH1D (name, descr, num_bin_x_cal, min_x_calib, max_x_calib);
	AddHistogram(hEA_KEVX_Strip20, "Anl/Energy-strips");



	//DSSD X 
	sprintf(name2,"E_KEVX");
	sprintf(descr2, "Energy X");
	RemoveHistogram(name2);
	hE_KEVX=new TH1D(name2,descr2,num_bin_x_cal, min_x_calib, max_x_calib);
	AddHistogram(hE_KEVX,"Anl/Energy-DSSD");

	//DSSD Y antiTOF
	sprintf(name2,"EA_KEVY");
	sprintf(descr2, "Energy Y antiTOF");
	RemoveHistogram(name2);
	hEA_KEVY=new TH1D(name2,descr2,num_bin_y_cal, min_y_calib, max_y_calib);
	AddHistogram(hEA_KEVY,"Anl/Energy-DSSD");

	//DSSD Y
	sprintf(name2,"E_KEVY");
	sprintf(descr2, "Energy Y");
	RemoveHistogram(name2);
	hE_KEVY=new TH1D(name2,descr2,num_bin_y_cal, min_y_calib, max_y_calib);
	AddHistogram(hE_KEVY,"Anl/Energy-DSSD");

	//DSSD Y out to AnlEvent
	sprintf(name2,"E_KEVY_Out");
	sprintf(descr2, "Energy Y sent to AnlEvent");
	RemoveHistogram(name2);
	hE_KEVY_Out=new TH1D(name2,descr2,num_bin_y_cal, min_y_calib, max_y_calib);
	AddHistogram(hE_KEVY_Out,"Anl/Energy-DSSD");

	//DSSD X out to AnlEvent
	sprintf(name2,"E_KEVX_Out");
	sprintf(descr2, "Energy X sent to AnlEvent");
	RemoveHistogram(name2);
	hE_KEVX_Out=new TH1D(name2,descr2,num_bin_x_cal, min_x_calib, max_x_calib);
	AddHistogram(hE_KEVX_Out,"Anl/Energy-DSSD");

	//BOX 
	sprintf(name2,"EBP_KEV");
	sprintf(descr2, "Energy Box");
	RemoveHistogram(name2);
	hEBP_KEV=new TH1D(name2,descr2,num_bin_box_cal, min_box_calib, max_box_calib);
	AddHistogram(hEBP_KEV,"Anl/Energy-BOX");

	//BOX antiTOF
	sprintf(name2,"EBA_KEV");
	sprintf(descr2, "Energy Box antiTOF");
	RemoveHistogram(name2);
	hEBA_KEV=new TH1D(name2,descr2,num_bin_box_cal, min_box_calib, max_box_calib);
	AddHistogram(hEBA_KEV,"Anl/Energy-BOX");

	for (int i=0;i<60;i++)
	{
		// X

		//anti TOF
		sprintf(name, "EA_KEVX_Strip%02d", i);
		sprintf(descr, "Energy X antiTOF, strip %02d", i);
		RemoveHistogram(name);
		hEA_KEVX_Strip[i] = new TH1D (name, descr, num_bin_x_cal, min_x_calib, max_x_calib);
		AddHistogram(hEA_KEVX_Strip[i], "Anl/Energy-strips/EA_KEVX-strips");

		//DSSD X Strips
		sprintf(name, "EPulse_KEVX_Strip%02d", i);
		sprintf(descr, "Energy X , strip %02d", i);
		RemoveHistogram(name);
		hEPulse_KEVX_Strip[i] = new TH1D (name, descr, num_bin_x_cal, min_x_calib, max_x_calib);
		AddHistogram(hEPulse_KEVX_Strip[i], "Anl/Energy-strips/EPulse_KEVX-strips");

		// Y

		//anti TOF
		sprintf(name, "EA_KEVY_Strip%02d", i);
		sprintf(descr, "Energy Y antiTOF, strip %02d", i);
		RemoveHistogram(name);
		hEA_KEVY_Strip[i] = new TH1D (name, descr, num_bin_y_cal, min_y_calib, max_y_calib);
		AddHistogram(hEA_KEVY_Strip[i], "Anl/Energy-strips/EA_KEVY-strips");

		//DSSD Y Strips
		sprintf(name, "EPulse_KEVY_Strip%02d", i);
		sprintf(descr, "Energy Y , strip %02d", i);
		RemoveHistogram(name);
		hEPulse_KEVY_Strip[i] = new TH1D (name, descr, num_bin_y_cal, min_y_calib, max_y_calib);
		AddHistogram(hEPulse_KEVY_Strip[i], "Anl/Energy-strips/EPulse_KEVY-strips");


		//Box WARNING Check the channel numbers / detector !!!!
		if(i<32){
			//Box antitof
			sprintf(name1, "EBA_KEV_Strip%02d", i);
			sprintf(descr1, "Energy Box antiTOF, strip %02d", i);
			RemoveHistogram(name1);
			hEBA_KEV_Strip[i] = new TH1D (name1, descr1, num_bin_box_cal, min_box_calib, max_box_calib);
			AddHistogram(hEBA_KEV_Strip[i], "Anl/Energy-strips/EBA_KEV-strips");

			//Box in pulse
			sprintf(name1, "EBPulse_KEV_Strip%02d", i);
			sprintf(descr1, "Energy Box, strip %02d", i);
			RemoveHistogram(name1);
			hEBPulse_KEV_Strip[i] = new TH1D (name1, descr1, num_bin_box_cal, min_box_calib, max_box_calib);
			AddHistogram(hEBPulse_KEV_Strip[i], "Anl/Energy-strips/EBPulse_KEV-strips");
		}

	}



	//Ge
	for(int i = 0; i < 20; i++){
		sprintf(name1, "Ge_Crystal%02d", i);
		sprintf(descr1, "Energy Ge Crystal %02d", i);
		RemoveHistogram(name1);
		hEGe[i] = new TH1D (name1, descr1, num_bin_ge_cal, min_ge_calib, max_ge_calib);
		AddHistogram(hEGe[i], "Anl/Energy-Gamma");

	}

	//Sum Gamma
	sprintf(name2,"EgammaAll");
	sprintf(descr2, "Energy Clover all");
	RemoveHistogram(name2);
	hEgammaAll=new TH1D(name2,descr2,num_bin_ge_cal, min_ge_calib, max_ge_calib);
	AddHistogram(hEgammaAll,"Anl/Energy-Gamma");

	//Sum Gamma Anti TOF
	sprintf(name2,"EgammaAll_antiTOF");
	sprintf(descr2, "Energy Clover all antiTOF");
	RemoveHistogram(name2);
	hEgammaAll_antiTOF=new TH1D(name2,descr2,num_bin_ge_cal, min_ge_calib, max_ge_calib);
	AddHistogram(hEgammaAll_antiTOF,"Anl/Energy-Gamma");

	//Sum Gamma Recoil X Gated
	sprintf(name2,"EgammaAll_ERX_gated");
	sprintf(descr2, "Energy Clover all ER X gated");
	RemoveHistogram(name2);
	hEgammaAll_ERX_gated=new TH1D(name2,descr2,num_bin_ge_cal, min_ge_calib, max_ge_calib);
	AddHistogram(hEgammaAll_ERX_gated,"Anl/ER-Gated");

	//Sum Gamma Recoil Y Gated
	sprintf(name2,"EgammaAll_ERY_gated");
	sprintf(descr2, "Energy Clover all ER Y gated");
	RemoveHistogram(name2);
	hEgammaAll_ERY_gated=new TH1D(name2,descr2,num_bin_ge_cal, min_ge_calib, max_ge_calib);
	AddHistogram(hEgammaAll_ERY_gated,"Anl/ER-Gated");

	//Delta Time Gamma Recoil X Gated
	sprintf(name2,"DTGamma_ERX");
	sprintf(descr2, "DT  Clover all ER X gated");
	RemoveHistogram(name2);
	hDTGamma_ERX=new TH1D(name2,descr2,500, -20, 30);
	AddHistogram(hDTGamma_ERX,"Anl/ER-Gated");

	//Delta Time Recoil Y Gated
	sprintf(name2,"DTGamma_ERY");
	sprintf(descr2, "DT Clover all ER Y gated");
	RemoveHistogram(name2);
	hDTGamma_ERY=new TH1D(name2,descr2,500, -20, 30);
	AddHistogram(hDTGamma_ERY,"Anl/ER-Gated");


	//TAC DSSD-TOF
	sprintf(name1, "TAC TOF-DSSD");  sprintf(descr1, "TAC TOF-DSSD");
	RemoveHistogram(name1);
	h_tac_si_tof = new TH1D (name1, descr1, num_bin_x_nocalib, min_x_nocalib, max_x_nocalib);  //
	AddHistogram(h_tac_si_tof, "Anl/Calibrated/TAC");

	//TAC Si-Ge
	sprintf(name1, "TAC DSSD-Ge");  sprintf(descr1, "TAC DSSD-Ge");	
	RemoveHistogram(name1);
	h_tac_si_ge = new TH1D (name1, descr1, num_bin_x_nocalib, min_x_nocalib, max_x_nocalib);  //
	AddHistogram(h_tac_si_ge, "Anl/Calibrated/TAC");

	//Ti D6
	sprintf(name1, "Ti D6");  sprintf(descr1, "Ti D6");
	RemoveHistogram(name1);
	h_ti_d6 = new TH1D (name1, descr1, num_bin_x_nocalib, min_x_nocalib, max_x_nocalib);  //
	AddHistogram(h_ti_d6, "Anl/Calibrated/TAC");

	//HF Cyclo
	sprintf(name1, "HF Cyclo");  sprintf(descr1, "HF Cyclo");
	RemoveHistogram(name1);
	h_HF_cyclo = new TH1D (name1, descr1, num_bin_x_nocalib, min_x_nocalib, max_x_nocalib);  //
	AddHistogram(h_HF_cyclo, "Anl/Calibrated/TAC");

	//Position Galotte
	sprintf(name1, "Galotte XY");  sprintf(descr1, "Galotte XY");
	RemoveHistogram(name1);
	h_pos_gal = new TH2D (name1, descr1, num_bin_x_nocalib, min_x_nocalib, max_x_nocalib,num_bin_y_nocalib, min_y_nocalib, max_y_nocalib); 		AddHistogram(h_HF_cyclo, "Anl/Calibrated/TAC");

	//Efficiency Galotte 
	sprintf(name1, "EffGalotte");  sprintf(descr1, "EffGalotte");
	RemoveHistogram(name1);
	h_EffGal = new TH1D (name1, descr1, 5,0,5);  //
	AddHistogram(h_EffGal,"Anl");

	//Alpha Gamma Coincidences
	sprintf(name2,"AlGa_Sum");
	sprintf(descr2, "Alpha Gamma coincidences all crystals");
	RemoveHistogram(name2);
	hAlGa_coinc_sum=new TH2D(name2,descr2,num_bin_y_cal2D, min_y_calib2D, max_y_calib2D, num_bin_ge_cal2D, min_ge_calib2D, max_ge_calib2D);
	AddHistogram(hAlGa_coinc_sum,"Anl/Alpha-Gamma/");

	sprintf(name2,"AlGa_Sum_ToF");
	sprintf(descr2, "Al Ga coinc all crystals, ToF coinc");
	RemoveHistogram(name2);
	hAlGa_coinc_sum_tof=new TH2D(name2,descr2,num_bin_y_cal2D, min_y_calib2D, max_y_calib2D, num_bin_ge_cal2D, min_ge_calib2D, max_ge_calib2D);
	AddHistogram(hAlGa_coinc_sum_tof,"Anl/Alpha-Gamma/");

	sprintf(name2,"AlGa_Sum_AntiToF");
	sprintf(descr2, "Al Ga coinc all crystals antiToF");
	RemoveHistogram(name2);
	hAlGa_coinc_sum_antitof=new TH2D(name2,descr2,num_bin_y_cal2D, min_y_calib2D, max_y_calib2D, num_bin_ge_cal2D, min_ge_calib2D, max_ge_calib2D);
	AddHistogram(hAlGa_coinc_sum_antitof,"Anl/Alpha-Gamma/");


	//Hit patterns DSSD & Multiplicity & reconstructed events
	sprintf(name2,"ProfileX");
	sprintf(descr2, "Counts vs strip number X");
	RemoveHistogram(name2);
	h_ProfileX=new TH1D(name2,descr2,61, 0, 61);
	AddHistogram(h_ProfileX,"Anl/Various");

	sprintf(name2,"ProfileY");
	sprintf(descr2, "Counts vs strip number Y");
	RemoveHistogram(name2);
	h_ProfileY=new TH1D(name2,descr2,61, 0, 61);
	AddHistogram(h_ProfileY,"Anl/Various"); 

	sprintf(name2,"HitsMultiplicityX");
	sprintf(descr2, "Number of simultaneous hits X");
	RemoveHistogram(name2);
	hHitsMultiX=new TH1D(name2,descr2,20, 0, 20);
	AddHistogram(hHitsMultiX,"Anl/Various");    

	sprintf(name2,"HitsMultiplicityY");
	sprintf(descr2, "Number of simultaneous hits Y");
	RemoveHistogram(name2);
	hHitsMultiY=new TH1D(name2,descr2,20, 0, 20);
	AddHistogram(hHitsMultiY,"Anl/Various");

	sprintf(name2,"MultiX1vs2");
	sprintf(descr2, "2 highest X values within 1 event");
	RemoveHistogram(name2);
	hMultiX1vs2=new TH2D(name2,descr2,1000, 0, 16000,1000,0,16000);
	AddHistogram(hMultiX1vs2,"Anl/Various");

	sprintf(name2,"MultiY1vs2");
	sprintf(descr2, "2 highest Y values within 1 event");
	RemoveHistogram(name2);
	hMultiY1vs2=new TH2D(name2,descr2,1000, 0, 70000,1000,0,70000);
	AddHistogram(hMultiY1vs2,"Anl/Various"); 

	sprintf(name2,"XReconstructed");
	sprintf(descr2, "Sum of X from two neighbouring strips");
	RemoveHistogram(name2);
	hXReconstructed=new TH1D(name2,descr2,4000, 0, 16000);
	AddHistogram(hXReconstructed,"Anl/Various");        

	//2D spectra
	//X
	//E vs Id X
	sprintf(name4,"Ex_vs_Idx");
	sprintf(descr4, "Ex_vs_Idx");
	RemoveHistogram(name4);
	h_dssd_ExIdx=new TH2D(name4,descr4,bin_strip_dssd, min_strip_dssd,max_strip_dssd,num_bin_x_cal,min_x_calib,max_x_calib );
	AddHistogram(h_dssd_ExIdx,"Anl/2D_spectra");

		//E vs Id X TOF coinc
	sprintf(name4,"Ex_vs_Idx_coin");
	sprintf(descr4, "Ex_vs_Idx_coin");
	RemoveHistogram(name4);
	h_dssd_ExIdx_coin=new TH2D(name4,descr4,bin_strip_dssd, min_strip_dssd,max_strip_dssd,num_bin_x_cal2D,min_x_calib,max_x_calib );
	AddHistogram(h_dssd_ExIdx_coin,"Anl/2D_spectra");

	//E vs Id X TOF anticoinc
	sprintf(name4,"Ex_vs_Idx_anticoin");
	sprintf(descr4, "Ex_vs_Idx_anticoin");
	RemoveHistogram(name4);
	h_dssd_ExIdx_anticoin=new TH2D(name4,descr4,bin_strip_dssd, min_strip_dssd,max_strip_dssd,num_bin_x_cal2D,min_x_calib,max_x_calib );
	AddHistogram(h_dssd_ExIdx_anticoin,"Anl/2D_spectra");

	//Alpha gated X vs Y matrix
	sprintf(name7,"Idx_vs_Idy_Al_Gated");
	sprintf(descr7, "Idx_vs_Idy_Al_Gated");
	RemoveHistogram(name7);
	h_dssd_idx_idy_Al_Gated=new TH2D(name7,descr7,bin_strip_dssd, min_strip_dssd,max_strip_dssd, bin_strip_dssd, min_strip_dssd,max_strip_dssd);
	AddHistogram(h_dssd_idx_idy_Al_Gated,"Anl/2D_spectra");

	
	//Y
	//E vs Id
	sprintf(name5,"Ey_vs_Idy");
	sprintf(descr5, "Ey_vs_Idy");
	RemoveHistogram(name5);
	h_dssd_EyIdy=new TH2D(name5,descr5,bin_strip_dssd, min_strip_dssd,max_strip_dssd,num_bin_y_cal2D,min_y_calib,max_y_calib );
	AddHistogram(h_dssd_EyIdy,"Anl/2D_spectra");

	//E vs Id TOF coinc
	sprintf(name5,"Ey_vs_Idy_coin");
	sprintf(descr5, "Ey_vs_Idy_coin");
	RemoveHistogram(name5);
	h_dssd_EyIdy_coin=new TH2D(name5,descr5,bin_strip_dssd, min_strip_dssd,max_strip_dssd,num_bin_y_cal2D,min_y_calib,max_y_calib );
	AddHistogram(h_dssd_EyIdy_coin,"Anl/2D_spectra");

	//Ex vs Ey
	sprintf(name6,"Ex_vs_Ey");   
	sprintf(descr6, "Ex_vs_Ey");
	RemoveHistogram(name6);
	h_dssd_ExEy=new TH2D(name6,descr6,num_bin_x_cal2D,min_x_calib,max_x_calib,num_bin_y_cal2D,min_y_calib,max_y_calib);
	AddHistogram(h_dssd_ExEy,"Anl/2D_spectra");

	//Id X vs Id Y
	sprintf(name7,"Idx_vs_Idy");
	sprintf(descr7, "Idx_vs_Idy");
	RemoveHistogram(name7);
	h_dssd_idx_idy=new TH2D(name7,descr7,bin_strip_dssd, min_strip_dssd,max_strip_dssd, bin_strip_dssd, min_strip_dssd,max_strip_dssd);
	AddHistogram(h_dssd_idx_idy,"Anl/2D_spectra");

	//Id X vs Id Y TOF Coinc
	sprintf(name7,"Idx_vs_Idy_coin");
	sprintf(descr7, "Idx_vs_Idy");
	RemoveHistogram(name7);
	h_dssd_idx_idy_coin=new TH2D(name7,descr7,bin_strip_dssd, min_strip_dssd,max_strip_dssd, bin_strip_dssd, min_strip_dssd,max_strip_dssd);
	AddHistogram(h_dssd_idx_idy_coin,"Anl/2D_spectra");

	//E Box vs Id  
	sprintf(name5,"Box_E_vs_Id");
	sprintf(descr5, "Box_E_vs_Id");
	RemoveHistogram(name5);
	h_box_EId=new TH2D(name5,descr5,bin_strip_box, min_strip_box,max_strip_box,num_bin_box_cal,min_box_calib,max_box_calib );
	AddHistogram(h_box_EId,"Anl/2D_spectra");

	//E Ge vs Id
	sprintf(name5,"Ge_E_vs_Id");
	sprintf(descr5, "Ge_E_vs_Id");
	RemoveHistogram(name5);
	h_ge_EId=new TH2D(name5,descr5,bin_strip_ge, min_strip_ge,max_strip_ge,num_bin_ge_cal,min_ge_calib,max_ge_calib );
	AddHistogram(h_ge_EId,"Anl/2D_spectra");

	//E Ge vs Id TOF Coinc
	sprintf(name5,"Ge_E_vs_Id_coin");
	sprintf(descr5, "Ge_E_vs_Id");
	RemoveHistogram(name5);
	h_ge_EId_coin=new TH2D(name5,descr5,bin_strip_dssd, min_strip_dssd,max_strip_dssd,num_bin_ge_cal,min_ge_calib,max_ge_calib );
	AddHistogram(h_ge_EId_coin,"Anl/2D_spectra");

	//EY vs TOF
	sprintf(name5,"EY_vs_ToF1");
	sprintf(descr5, "EY_vs_ToF1");
	RemoveHistogram(name5);
	h_E_Tof1=new TH2D(name5,descr5,num_bin_tof,min_tof,max_tof ,num_bin_y_cal/2, min_y_calib,max_y_calib);
	h_E_Tof1->SetYTitle("Energy YY(keV)");
	h_E_Tof1->SetXTitle("ToF (u.a.)");
	AddHistogram(h_E_Tof1,"Anl/Calibrated/2D_spectra");
	//EX vs TOF
	sprintf(name5,"EX_vs_ToF1");
	sprintf(descr5, "EX_vs_ToF1");
	RemoveHistogram(name5);
	h_EX_Tof1=new TH2D(name5,descr5,num_bin_tof,min_tof,max_tof ,num_bin_y_cal/2, min_y_calib,max_y_calib);
	h_EX_Tof1->SetYTitle("Energy YY(keV)");
	h_EX_Tof1->SetXTitle("ToF (u.a.)");
	AddHistogram(h_EX_Tof1,"Anl/Calibrated/2D_spectra");
	//EX vs TAC 
	sprintf(name5,"EY_vs_TAC");
	sprintf(descr5, "EY_vs_TAC");
	RemoveHistogram(name5);
	h_E_TAC=new TH2D(name5,descr5,4096,0,16384 ,num_bin_y_cal/2, min_y_calib,max_y_calib);
	h_E_TAC->SetYTitle("Energy Y (keV)");
	h_E_TAC->SetXTitle("TAC (u.a.)");
	AddHistogram(h_E_TAC,"Anl/Calibrated/2D_spectra");

	//1D TOF Spectra 
	sprintf(name5,"TOFms");
	sprintf(descr5, "TOF in ms");
	RemoveHistogram(name5);
	hTOFms = new TH1D (name5, descr5, num_bin_x_nocalib, min_x_nocalib, max_x_nocalib);  //
	AddHistogram(hTOFms, "Anl/");

	sprintf(name5,"TOF20ns");
	sprintf(descr5, "TOF in 20 ns");
	RemoveHistogram(name5);
	hTOF20ns = new TH1D (name5, descr5, num_bin_x_nocalib, min_x_nocalib, max_x_nocalib);  //
	AddHistogram(hTOF20ns, "Anl/");

	//Julien - Tests sur le temps

	sprintf(name5,"Dl_T");
	sprintf(descr5, "Delta l_T");
	RemoveHistogram(name5);
	hDl_T = new TH1D (name5, descr5, 3001, -1500, 1500);  //
	AddHistogram(hDl_T, "Anl/");

	sprintf(name5,"E1vsE2_zero");
	sprintf(descr5, "E1 vs E2 for Dl_T = 0;E1 (keV);E2 (keV)");
	RemoveHistogram(name5);
	dE1vsE2_zero = new TH2D (name5, descr5, num_bin_x_cal, min_x_calib, max_x_calib,num_bin_x_cal, min_x_calib, max_x_calib);  //
	AddHistogram(dE1vsE2_zero, "Anl/");

	sprintf(name5,"E1vsE2_deux");
	sprintf(descr5, "E1 vs E2 for Dl_T = XXX;E1 (keV);E2 (keV)");
	RemoveHistogram(name5);
	dE1vsE2_deux = new TH2D (name5, descr5, num_bin_x_cal, min_x_calib, max_x_calib,num_bin_x_cal, min_x_calib, max_x_calib);  //
	AddHistogram(dE1vsE2_deux, "Anl/");

	sprintf(name5,"X1vsX2_zero");
	sprintf(descr5, "X1 vs X2 for Dl_T = 0;X1 (keV);X2 (keV)");
	RemoveHistogram(name5);
	dX1vsX2_zero = new TH2D (name5, descr5, 64, 0, 64,64, 0, 64);  //
	AddHistogram(dX1vsX2_zero, "Anl/");

	sprintf(name5,"X1vsX2_deux");
	sprintf(descr5, "X1 vs X2 for Dl_T = XXX;X1 (keV);X2 (keV)");
	RemoveHistogram(name5);
	dX1vsX2_deux = new TH2D (name5, descr5, 64, 0, 64,64, 0, 64);  //
	AddHistogram(dX1vsX2_deux, "Anl/");

	sprintf(name5,"l_hit_out");
	sprintf(descr5,"Difference between 2 successive l_hit_time");
	RemoveHistogram(name5);
	l_hit_out = new TH1D(name5,descr5,3001,-1500,1500);
	AddHistogram(l_hit_out, "Unpack/");

	sprintf(name5,"dT_Gamma");
	sprintf(descr5,"Difference between 2 successive l_hit_time Gamma");
	RemoveHistogram(name5);
	h_dT_Gamma = new TH1D(name5,descr5,3001,-1500,1500);
	AddHistogram(h_dT_Gamma, "Anl/");

	if (fbNewSubmit) {
		for (int i = 0; i < 32; i++) {
			num_back[i] = 0;

		}
	}
//cout<<"****MoDSSAnlProc initialization done"<<endl;

}  //end of the MoDSSAnlProc::MoDSSAnlProc(Text_t * cname)....//temporary here
//=================end of histograms =========================================

//***********************************************************
// default constructor - not used in program

MoDSSAnlProc::MoDSSAnlProc()
	: TGo4EventProcessor("MoDSSAnlProc")
{
	//  cout << "MoDSSAnlProc ---- DO NOT USE THIS CONSTRUCTOR !!!!\n";
}

//***********************************************************
// Default Destructor

MoDSSAnlProc::~MoDSSAnlProc()
{
	// cout << "**** MoDSSAnlProc: Closing output file: " << fPrintPar->fPrtFilename << endl;
	//gOutputFile.close();
}

//***********************************************************
// ---------- BitTest function --------------------
// returns 1 if at 'position' in 'tested' a '1' was found
// positions are starting from 1 (corresponding to bit 0) for
// more convenient usage

Bool_t MoDSSAnlProc::BitTest(const Int_t &tested, Int_t position)
{
	position = position - 1;
	return (tested & (1 << position));
}


//***********************************************************
//***********************************************************
// ==========================================================
// here the input event is beeing analysed and output event (which
// comes as a parameter) is filled

void MoDSSAnlProc::MoDSSEventAnalysis(MoDSSAnlEvent* pOutEvent)
{
//cout<<"**** MoDSSAnlProc - Start MoDSSEventAnalysis"<<endl;
	//==================== variable definition ===========================


	Double_t EkevX1 = 0;
	Double_t EkevX2 = 0;

	Double_t EkevY1 = 0;
	Double_t EkevY2 = 0;

	Double_t EkevXReconstructed = 0;
	Double_t EkevXPrevious = 0;

	//Float_t ex[4][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	//Float_t ey[4][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	//Float_t eb[1][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	//Float_t eg[2][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	//Float_t ex_sum=0;
	//Float_t ey_sum=0;
	//Float_t eb_sum=0;
	//Float_t eg_sum=0;

	//static Int_t el_bit_all = 0, el_2b = 0, eh_bit_all = 0, eh_2b = 0;

	//================= asign crate positions to local variables =============================

	// get input event from the previous (UNPACK) step
	fInput  = (MoDSSUnpackEvent*) GetInputEvent();
	//cout<<"Anl.MoDSSEventAnalysis()"<<endl;
	// output events are not stored until kTRUE is set
	pOutEvent->SetValid(kTRUE);
	pOutEvent->ClearEvent();

	//Reinitialise to zero
	for(int i=0;i<60;i++){
		EnonCalX[i] = 0;
		EkevX[i] = 0;
		TimeX[i] = 0;
		EnonCalY[i] = 0;
		EkevY[i] = 0;
		TimeY[i] = 0;
		if(i<32){
			EnonCalBox[i] = 0;
			EkevBox[i] = 0;
			TimeBox[i] = 0;
		}
		if(i<20){
			EnonCalGam[i] = 0;
			EkevGam[i] = 0;
			TimeGam[i] = 0;
		}
	}
	E_kevX = 0;
	E_kevY = 0;
	E_kevBox = 0;
	IDX = 0; 
	IDY = 0;
	IDB = 0;
	sample_time_x = 0;
	sample_time_y = 0;
	sample_time_box = 0;
	for(int i=0; i<20; i++) sample_time_ge[i] = 0;

	l_sfp=0;
	l_feb=0;
	l_cha=0;
	l_t_trig=-1;
	TOF = 0;
	DTime = 0;
	TAC_GalSi = 0;
	TAC_GalX = 0;
	TAC_GalY = 0;
	TAC_SiGe = 0;
	TiD6 = 0;
	HF = 0;

	multiplicityX = 0;
	multiplicityY = 0;

	tof = kFALSE;
	MapHardware();
	// ========================= Calibrations ====================
	for(int i = 0;i < 60; i++){
		//======================== Calibration and filling dssd X spectra =====================
		if(EnonCalX[i] > 0){
			if (multiplicityX>6) break;
			multiplicityX++;
			NbEventX++;
			//cout<<"Ecal = "<<EkevX[i]<<" = A["<<i<<"]="<<fCal->Ax[i]<<"* "<< EnonCalX[i]<< " + B["<<i<<"]="<<fCal->Bx[i]<<endl;
			//EkevX[i] = fCal->Ax[i] *(EnonCalX[i]+fCal->R3->Rndm()/10-0.05) + fCal->Bx[i];//Calibrated energy
			EkevX[i] = fCal->Ax[i] *(EnonCalX[i]+rand()%1000*0.001) + fCal->Bx[i];//Calibrated energy
			//cout<<"Ecal = "<<EkevX[i]<<" = A["<<i<<"]="<<fCal->Ax[i]<<"* "<< EnonCalX[i]<< " + B["<<i<<"]="<<fCal->Bx[i]<<endl;

			h_dssd_ExIdx->Fill(i+1,EkevX[i]);//Energy vs Strip number
			h_ProfileX->Fill(i+1);	//Counts per strip		
			hE_KEVX->Fill(EkevX[i]);
			hEPulse_KEVX_Strip[i]->Fill(EkevX[i]);//Energy X in anticoincidence with a TOF per strip

			if(tof==kFALSE){
				//if(EkevX[i]>7200 &&EkevX[i]<10000) cout<<"Anl Cal X - Fill EA_KEVX"<<endl;
				hEA_KEVX_Strip[i]->Fill(EkevX[i]);//Energy X in anticoincidence with a TOF per strip
				hEA_KEVX->Fill(EkevX[i]); //Sum Energy X in TOF anticoincidence 
				h_dssd_ExIdx_anticoin->Fill(i+1,EkevX[i]);//Energy X vs Strip # in coincidence with a TOF
				if(i==20)hEA_KEVX_Strip20->Fill(EnonCalX[i]);
			}
			

			if(tof==kTRUE){
				h_dssd_ExIdx_coin->Fill(i+1,EkevX[i]);//Energy X vs Strip # in coincidence with a TOF
			}

			if(EkevXPrevious > 0)
				EkevXReconstructed = EkevXPrevious + EkevX[i];//Sum of charge sharing between two strips

			EkevXPrevious = EkevX[i];

			if(EkevX2<EkevX[i]){//Stored to plot E Strip1 vs E Strip2
				EkevX1 = EkevX2;
				EkevX2 = EkevX[i];
			}
			//prepare output event X
			//if(fParam->fANL_PolyX && fCondEXvsTOF_ANL->Test(TimeGalSiTOF, EkevX[i]))
			//{
				pOutEvent->fE_kevX[multiplicityX-1] = EkevX[i];
				pOutEvent->fIDX[multiplicityX-1] = i;
				pOutEvent->fTimeX[multiplicityX-1] = (l_t_trig+TimeX[i])*0.00002; 			//times in [ms]!
				pOutEvent->fMultX = multiplicityX;
				//if(EkevX[i]>5500 )cout<<"AnlXPol- EventNum "<<EventNum<<" - "<<EkevX[i]<<" - "<<pOutEvent->fTimeX[i]<<endl;
		/*	}
			else if(fParam->fANL_PolyX && tof == kFALSE)
			{
				pOutEvent->fE_kevX[multiplicityX-1] = EkevX[i];
				pOutEvent->fIDX[multiplicityX-1] = i;
				pOutEvent->fTimeX[multiplicityX-1] = (l_t_trig+TimeX[i])*0.00002; 			//times in [ms]!
				pOutEvent->fMultX = multiplicityX;
				//if(EkevX[i]>5500)cout<<"AnlXTof- EventNum "<<EventNum<<" - "<<EkevX[i]<<" - "<<pOutEvent->fTimeX[i]<<endl;
				//		if(EventNum>443007353 && EventNum<450000000)aLog<<"AnlX- EventNum "<<EventNum<<" - "<<setprecision(25)<<hex<<l_t_trig<<" / "<<pOutEvent->fTimeX[i]<<dec<<endl;
			}  
			else if(!fParam->fANL_PolyX)
			{
				pOutEvent->fE_kevX[multiplicityX-1] = EkevX[i];
				pOutEvent->fIDX[multiplicityX-1] = i;
				pOutEvent->fTimeX[multiplicityX-1] = (l_t_trig+TimeX[i])*0.00002; 			//times in [ms]!
				pOutEvent->fMultX = multiplicityX;
				//if(EkevX[i]>5500)cout<<"AnlXNoPol- EventNum "<<EventNum<<" - "<<EkevX[i]<<" - "<<tof<<" - "<<pOutEvent->fTimeX[i]<<endl;
				//	if(EventNum>443007353 && EventNum<450000000)aLog<<"AnlX- EventNum "<<EventNum<<" - "<<setprecision(25)<<hex<<l_t_trig<<" / "<<pOutEvent->fTimeX[i]<<dec<<endl;
			} */
			//cout<<"X cal end"<<endl;
		}
		//====================== Calibration and filling dssd Y spectra =========================================
		if(EnonCalY[i]>0){
			if (multiplicityY>6) break;

			multiplicityY++;
			NbEventY++;
			EkevY[i] = fCal->Ay[i] *(EnonCalY[i]+rand()%1000*0.001) + fCal->By[i];

			h_dssd_EyIdy->Fill(i+1,EkevY[i]);			
			h_ProfileY->Fill(i+1);			
			hE_KEVY->Fill(EkevY[i]);
			hEPulse_KEVY_Strip[i]->Fill(EkevX[i]);//Energy X in anticoincidence with a TOF per strip

			if(tof==0){
				hEA_KEVY_Strip[i]->Fill(EkevY[i]);
				hEA_KEVY->Fill(EkevY[i]); //TOF anticoincidence
			}

			if(tof==1){
				h_dssd_EyIdy_coin->Fill(i+1,EkevY[i]);
				TOF = (TimeY[i]-TimeGalSi)*0.00002; 			//times in [ms]!CALCUL_TOF
				TOF2 = (TimeY[i]-TimeGalSi); 			//times in 20 ns units!CALCUL_TOF
				hTOFms->Fill(TOF);
				hTOF20ns->Fill(TOF2);
			}

			if(EkevY2<EkevY[i]){
				EkevY1 = EkevY2;
				EkevY2 = EkevY[i];
			}
	//cout<<"Anl Y l_t_trig="<<l_t_trig<<"  "<<TimeY[i]<<" = ";
			//prepare output event Y
	//		if(fParam->fANL_PolyY && fCondEYvsTOF_ANL->Test(TOF, EkevY[i]))
	//		{
				pOutEvent->fE_kevY[multiplicityY-1] = EkevY[i];
				pOutEvent->fIDY[multiplicityY-1] = i;
				pOutEvent->fTimeY[multiplicityY-1] = (l_t_trig+TimeY[i])*0.00002;
				pOutEvent->fMultY = multiplicityY;
				//if(EventNum>443007353 && EventNum<450000000)aLog<<"AnlY- EventNum "<<EventNum<<" - "<<setprecision(25)<<hex<<l_t_trig<<" / "<<pOutEvent->fTimeY[i]<<dec<<endl;
	/*		}
			else if(fParam->fANL_PolyY && tof == kFALSE)
			{
				pOutEvent->fE_kevY[multiplicityY-1] = EkevY[i];
				pOutEvent->fIDY[multiplicityY-1] = i;
				pOutEvent->fTimeY[multiplicityY-1] = (l_t_trig+TimeY[i])*0.00002;
				pOutEvent->fMultY = multiplicityY;
				//if(EventNum>443007353 && EventNum<450000000)aLog<<"AnlY- EventNum "<<EventNum<<" - "<<setprecision(25)<<hex<<l_t_trig<<" / "<<pOutEvent->fTimeY[i]<<dec<<endl;
			}  
			else if(!fParam->fANL_PolyY)
			{
				pOutEvent->fE_kevY[multiplicityY-1] = EkevY[i];
				pOutEvent->fIDY[multiplicityY-1] = i;
				pOutEvent->fTimeY[multiplicityY-1] = (l_t_trig+TimeY[i])*0.00002;
				pOutEvent->fMultY = multiplicityY;
				//if(EventNum>443007353 && EventNum<450000000)aLog<<"AnlY- EventNum "<<EventNum<<" - "<<setprecision(25)<<hex<<l_t_trig<<" / "<<pOutEvent->fTimeY[i]<<dec<<endl;
			} */   	    
	//cout<<pOutEvent->fTimeY[i]<<endl;
		}
	}
	hHitsMultiX->Fill(multiplicityX);
	hHitsMultiY->Fill(multiplicityY);

	hMultiX1vs2->Fill(EkevX1, EkevX2);
	hMultiY1vs2->Fill(EkevY1, EkevY2);
	if(EkevXReconstructed) hXReconstructed->Fill(EkevXReconstructed);

	//cout<<"-------------------------"<<endl;
	//---------------------------- Calibration Box -----------------------------------

	for(int i = 0; i < 32; i++){
		if((EnonCalBox[i]>0) && (E_kevX > 0)){
			EkevBox[i]=fCal->Ab[i] *(EnonCalBox[i]+ rand()%1000*0.001)  + fCal->Bb[i];
			//EkevBox[i] = EkevBox[i] + E_kevX;


			if(tof == 0)
			{
				hEBA_KEV_Strip[i]->Fill(EkevBox[i]);
				hEBA_KEV->Fill(EkevBox[i]);					
			}
			hEBP_KEV->Fill(EkevBox[i]);					

			h_box_EId->Fill(i+1,EkevBox[i]);
			pOutEvent->fEb_kev[i] = EkevBox[i];
			pOutEvent->fTimeBox[i] = (l_t_trig+TimeBox[i])*0.00002;
		}
	}     
	//cout<<"calib for Box done"<<endl;
	//=========================== Calibration and filling Ge spectra =======================================

	for(int i = 0; i < 20; i++){
		if((EnonCalGam[i] > 0)){
                        EkevGam[i]=fCal->Cg[i] *(EnonCalGam[i]+rand()%1000*0.001)*(EnonCalGam[i]+rand()%1000*0.001) + fCal->Ag[i] *(EnonCalGam[i]+rand()%1000*0.001)  + fCal->Bg[i];
			hEGe[i]->Fill(EkevGam[i]);
			hEgammaAll->Fill(EkevGam[i]);

			if(tof==0){
				hEgammaAll_antiTOF->Fill(EkevGam[i]);
			}

			h_ge_EId->Fill(i,EkevGam[i]);

			sample_time_ge[i] = TimeGam[i];

			/*if(fCondEXvsTOF_ANL->Test(TOF, E_kevX))
			{
				hEgammaAll_ERX_gated->Fill(EkevGam[i]);
				hDTGamma_ERX->Fill((TimeGam[i] - sample_time_x)*0.02);
			}

			if(fCondEYvsTOF_ANL->Test(TOF, E_kevY))//ATTENTION PB ICI !!!!
			{
				hEgammaAll_ERY_gated->Fill(EkevGam[i]);
				hDTGamma_ERY->Fill((TimeGam[i] - sample_time_y)*0.02);
			}*/
			for(int j = 0; j < 60; j++){                  //Alpha - Gamma coincidences, only y energy used
				if((EkevX[j] >= 4000) && (EkevX[j] <= 12000)){
					//hAlGa_coinc[j]->Fill(EkevY[j],EkevGam[i]);
					hAlGa_coinc_sum->Fill(EkevX[j],EkevGam[i]);

					//Alpha gated + ToF
					if(tof==1){
						//hAlGa_coinc_coin[0][l]->Fill(EkevX[j],EkevGam[i]);
						hAlGa_coinc_sum_tof->Fill(EkevX[j],EkevGam[i]);
					}
					if(tof==0){
						//hAlGa_coinc_anticoin[0][l]->Fill(EkevX[j],EkevGam[i]);
					hAlGa_coinc_sum_antitof->Fill(EkevX[j],EkevGam[i]);
					}
				}
			}
			pOutEvent->fTgamma[i] = (l_t_trig+sample_time_ge[i])*0.00002;
			pOutEvent->fegl[i] = EkevGam[i];   
			if(pOutEvent->fegl[i]>0 && i>0)h_dT_Gamma->Fill(sample_time_ge[i-1]-sample_time_ge[i]);
			//cout<<"AnlProc : fegl["<<i<<"]="<<pOutEvent->fegl[i]<<endl;
		}
	}

	//cout<<"==============================="<<endl;
	//cout<<"Anl - Calib done"<<endl;

	//Julien Debug - Transmit only mutliplicity 1 event in the DSSD : 
	//
	/*if(multiplicityX != 1 || multiplicityY != 1){
		for(int j=0; j<60; j++){
			pOutEvent->fE_kevX[j] = 0;
			pOutEvent->fE_kevY[j] = 0;
			pOutEvent->fTimeX[j] = 0;
			pOutEvent->fTimeY[j] = 0;
			pOutEvent->fIDX[j] = 0;
			pOutEvent->fIDY[j] = 0;
		}
	}*/
			//Check the effect of the lines in the l_hit_time distribution : 
			/*if(((TimeX[i]-l_TS_sav)<= 10) && ((TimeX[i]-l_TS_sav)>=-10) && (i<64)){
				dE1vsE2_zero->Fill(EkevX[i],l_E_sav);
				dX1vsX2_zero->Fill(i,strip_sav);
				l_hit_out->Fill(TimeX[i]-l_TS_sav);
			}
			else if(((TimeX[i]-l_TS_sav)<= 290) && ((TimeX[i]-l_TS_sav)>= 270) && (i<64)){
				dE1vsE2_deux->Fill(EkevX[i],l_E_sav);
				dX1vsX2_deux->Fill(i*16+j,strip_sav);
				l_hit_out->Fill(TimeX[i]-l_TS_sav);
			}
			l_TS_sav = TimeX[i];	
			l_E_sav = EkevX[i];
			strip_sav = i;*/

	//===================== Fill Single event Spectra ===============================
	//Fill TOF and TACs ADJUST WITH RESPECT TO CHANNELS USED
	//cout<<"filling spectra"<<endl;
	//TAC TOF1
	if(TAC_GalSi>TOF_THRES){
		h_tac_si_tof->Fill(TAC_GalSi);
	}
	//TAC Si=Ge
	if(TAC_SiGe>0){
		h_tac_si_ge->Fill(TAC_SiGe);
	}
	//Beam intensity in TID6 - Useless for the moment.
	if(TiD6>0){
		h_ti_d6->Fill(TiD6);
	}
	//Cyclotron HF 
	if(HF>0){
		h_HF_cyclo->Fill(HF);
	}
	//Position on the Galotte
	if(TAC_GalX != -10 && TAC_GalY != -10){
		h_pos_gal->Fill(TAC_GalX,TAC_GalY);
	}
	//2D spectra
	//E_Tof with the energy from the Y side
	for (int i = 0; i < 60; i++){
		if(EkevY[i]>0 && TAC_GalSi>TOF_THRES) {
			DTime = (TimeY[i]-TimeGalSi);//CALCUL_TOF in 20ns units
			
			h_E_Tof1->Fill((Double_t)DTime,EkevY[i]);//E ToF in mus units
			//h_E_TAC->Fill(TAC_GalSi,EkevY[i]);//E ToF in mus units
			//cout<<"DT "<<DTime<<" for "<<l_E_cal[i][j]<<endl;
		}
			
		//Energy Ex vs Ey in DSSD;
		for(int j=0;j<60;j++){
			if(EkevX[j]>0 && EkevY[i]>0){
				h_EX_Tof1->Fill((Double_t)DTime,EkevX[j]);//E ToF in mus units
				h_dssd_ExEy->Fill(EkevX[j],EkevY[i]);
				h_dssd_idx_idy->Fill(j+1,i+1);
				if(tof == 1){
					h_dssd_idx_idy_coin->Fill(j+1,i+1);
				}
				if(EkevY[i] >= (ALPHA_GATE-100) && EkevY[i] <= (ALPHA_GATE+100) && (tof==1)){
					h_dssd_idx_idy_Al_Gated->Fill(j+1,i+1);
				}
			}
		}
	}
	// 	//XY Gal
	// 	if(l_E_cal[0][11][6]>0 && l_E_cal[0][11][5]>0){
	// 		h_gal_XY->Fill(l_E_cal[0][11][5],l_E_cal[0][11][6]);
	// 	}

	//cout<<"Anl - Histos filled"<<endl;
	//=================== Fill output event =====================
	Bool_t DebugOut = kFALSE;
	if(DebugOut) cout<<E_kevX<<"\t"<<E_kevY<<"\t"<<IDX<<"\t"<<IDY<<"\t"<<l_t_trig*0.00002<<"\t"<<tof<<"\t"<<DTime<<endl;

	//Perform Addback modifications on the Ge & Box
	//pOutEvent->AddBack();

	//fill output event
	pOutEvent->fTimeTrig = l_t_trig*0.00002;   
	pOutEvent->fB_tof = tof;//Boolean tagging a tof
	pOutEvent->fTOF = DTime;// ToF value from the timestamp difference between Back (Y) event and the TAC signal from the Galotte. 
	//cout<<"---- MoDSSAnlProc -  Sotring eventNum"<<endl;
	pOutEvent->fEventNum = EventNum;//Event number from the Unpack step. Not corrected for the unfolding of the DSSD hits. 

	for(int k=0; k<multiplicityX; k++){
		if(pOutEvent->fE_kevX[k]>0) hE_KEVX_Out->Fill(pOutEvent->fE_kevX[k]);
	}
	for(int k=0; k<multiplicityY; k++){
		if(pOutEvent->fE_kevY[k]>0) hE_KEVY_Out->Fill(pOutEvent->fE_kevY[k]);
	}
	//cout<<pOutEvent->fEventNum<<" - "<<multiplicityX<<" X events, "<<multiplicityY<<" Y events"<<endl;
	//cout<<pOutEvent->fEventNum<<" - "<<NbEventX<<" X events, "<<NbEventY<<" Y events"<<endl;
	fInput->ClearEvent();
	//cout<<"---- MoDSSAnlProc - OutEvent filled, moving on"<<endl;
}//end of modss analysis event
//--------------------------------------------
// convert position from channels to mm


// =================================================================
// print a serie of 0 and 1

// void MoDSSAnlProc::PrintBitPattern(const Int_t &pattern, ostream &output)
// {
// // 	for (Int_t pos = 1; pos < 17; pos++) {
// // 		if (BitTest(pattern, pos)) output << "1";
// // 		else output << "0";
// 	//}
// }





//================================ Map Hardware =====================================================//
//											             //
//Create new if clause for every new arangement of channels/detectors. Regardless                    //
//the orientation of the DSSD, for proper work of correlation procedure and to avoid                 //
//confusion, X is always the side with better resolution and smaller energy range!!!                 //
//Y is the side with worse resolution!!!                                                             //
//===================================================================================================//		

// ------------- assign crate positions to local variables ------------
void MoDSSAnlProc::MapHardware() 
{
	chit_dssdX=0;
	chit_dssdY=0;
	chit_gamma=0;
	chit_box=0;
	bEffGal = kFALSE;

	//	if(fHardwareParam->fRunID == 297){
	//	}

	//	else {						//Default mapping
	for(int i=0;i<12;i++){
		for(int j=0;j<16;j++){
			if(fInput->l_E[i][j]>0){
				if( i <4){
					chit_dssdX++;
					if((i*16+j) < 30){
						EnonCalX[i*16+j] = fInput->l_E[i][j];
						//cout<<"Map : EnonCalX["<<i<<"*16+"<<j<<"] = "<<EnonCalX[i*16+j]<<endl;
						TimeX[i*16+j] = fInput->l_T[i][j];
						if(fInput->l_E[11][4]>0) bEffGal = kTRUE;
					}
					if((32 < (i*16+j)) && ((i*16+j) < 62)){
						EnonCalX[i*16+j-2] = fInput->l_E[i][j];
						//cout<<"Map : EnonCalX["<<i<<"*16+"<<j<<"] = "<<EnonCalX[i*16+j]<<endl;
						TimeX[i*16+j-2] = fInput->l_T[i][j];
						if(fInput->l_E[11][4]>0) bEffGal = kTRUE;
					}
				}
				if( i>=4 && i<8){
					chit_dssdY++;
					if((i*16+j) < 94){
						EnonCalY[i*16+j-64] = fInput->l_E[i][j];
						TimeY[i*16+j-64] = fInput->l_T[i][j];
					}
					if((96 < (i*16+j)) && ((i*16+j) < 126)){
						EnonCalY[i*16+j-66] = fInput->l_E[i][j];
						TimeY[i*16+j-66] = fInput->l_T[i][j];
					}
				}
				if(i==8 || i==9){
					chit_box++;
					EnonCalBox[(i-8)*16+j] = fInput->l_E[i][j];
					TimeBox[(i-8)*16+j] = fInput->l_T[i][j];
				}
				if(i == 10 || (i==11 && j < 4)){
					chit_gamma++;
					EnonCalGam[(i-10)*16+j] = fInput->l_E[i][j];
					TimeGam[(i-10)*16+j] = fInput->l_T[i][j];
				}
				if(i==11) {
					if(j==4 && fInput->l_E[i][j]>TAC_OK){
						TAC_GalSi = fInput->l_E[i][j];
						TimeGalSi = fInput->l_T[i][j];
						tof = kTRUE;
						//cout<<"TAC Gal Si = "<<TAC_GalSi<<endl;
					}
					if(j==5){
						TAC_GalX = fInput->l_E[i][j];
						//tof = kTRUE;
					}
					if(j==6){
						TAC_GalY = fInput->l_E[i][j];
						//tof = kTRUE;
					}
					//if(TAC_GalSi == 0 && (TAC_GalX>0 || TAC_GalY>0)) cout<<"Discrepancy in the galottes ! "<<TAC_GalSi<<"\t"<<TAC_GalX<<"\t"<<TAC_GalY<<endl;
					if(j==7)TAC_SiGe = fInput->l_E[i][j];
					if(j==8)TiD6 = fInput->l_E[i][j];
					if(j==9)HF = fInput->l_E[i][j];
				}
			}

			//Check the effect of the lines in the l_hit_time distribution : 
			if(((fInput->l_T[i][j]-l_TS_sav)<= 10) && ((fInput->l_T[i][j]-l_TS_sav)>=-10) && (i<4)){
				dE1vsE2_zero->Fill(fInput->l_E[i][j],l_E_sav);
				dX1vsX2_zero->Fill(i*16+j,strip_sav);
				l_hit_out->Fill(fInput->l_T[i][j]-l_TS_sav);
			}
			else if(((fInput->l_T[i][j]-l_TS_sav)<= 290) && ((fInput->l_T[i][j]-l_TS_sav)>= 270) && (i<4)){
				dE1vsE2_deux->Fill(fInput->l_E[i][j],l_E_sav);
				dX1vsX2_deux->Fill(i*16+j,strip_sav);
				l_hit_out->Fill(fInput->l_T[i][j]-l_TS_sav);
			}
			l_TS_sav = fInput->l_T[i][j];	
			l_E_sav = fInput->l_E[i][j];
			strip_sav = i*16+j;
		}
	}
	l_t_trig= fInput->l_t_trig;
	//cout<<"Anl.MapHardware - trig time = "<<l_t_trig<<endl;
	l_sfp 	= fInput->l_sfp;
	l_feb 	= fInput->l_feb;
	l_cha 	= fInput->l_cha;

	EventNum = fInput->event_number;
	//cout<<"Mapping event"<<EventNum<<endl;

	if(bEffGal== kTRUE) h_EffGal->Fill(1);
	else h_EffGal->Fill(3);
	h_EffGal->Fill(4);

	//}
}

// =================================================================
ClassImp(MoDSSAnlProc)
