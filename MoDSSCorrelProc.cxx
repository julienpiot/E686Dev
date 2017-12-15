//B.Andel
//03/2016
//TSHIPCorrelProc along with parameter files re-designed to serve as Correl step of MoDSS analysis
//
//TSHIP code originally written by P.Cagarda and further developed by S.Antalic



#include "MoDSSCorrelProc.h"
#include "MoDSSCorrelEvent.h"
#include <iomanip>



// file to store printed correlations
// has to be global variable - doesn't work as data member.
ofstream   gPrtFile;




// **********************************************************
// Constructor - Initialization of all class members

	MoDSSCorrelProc::MoDSSCorrelProc(const char* cname)
:TGo4EventProcessor(cname)
{

	Int_t     i;
	Double_t  max;
	Text_t    name[40], descr[80];

	// boolean member variable used to tell the static variable in PrintThisChain()
	// to be cleared each time a new analysis settings were submitted
	fbNewSubmit = kTRUE;

	// access unpack step and get pointer to the event source
	// used in printout to obtain the filename of currently opened input file
	fMBSfile = 0;
	if (TGo4Analysis::Instance()->GetInputEvent("Unpack")) 
	{
		TGo4EventSource *source = TGo4Analysis::Instance()->GetInputEvent("Unpack")->GetEventSource();
		if (source)
			if (source->InheritsFrom("TGo4MbsFile"))
				fMBSfile = dynamic_cast<TGo4MbsFile*> (source);
	}
	//cout<<"opening Log file"<<endl;
	//Log.open("/home/e656/E686_Analyse/localcopy/VerifTable.txt",ios::app);
	/*if (Log){
	       	cout<<"Création LogFile"<<endl;
	}
	else{ 
		cout<<"Erreur création LogFile"<<endl;
	}*/
	//// init user analysis objects:

	// ---- 1. initialization of parameters
	// Hardware parameter set - contains information 
	// about hardware settinf and possible event filters
	fHardwareParam = (MoDSSHardwareParameter*)  GetParameter("MoDSSHardwarePar");
	if(!fHardwareParam)
	{
		fHardwareParam = new MoDSSHardwareParameter("MoDSSHardwarePar");
		AddParameter(fHardwareParam);
		// if Hardware parameter set exists - print it out, otherwise warn the user.
		if (fHardwareParam) fHardwareParam->PrintParameter(0,0);
		else cout << "***** ERR - HardwareParam doesn't exist - program will crash.\n";
	}

	// correlation parameter - contains all values needed for correlation search
	// it's not necessary to check if the Correl Parameters exist. They 
	// shouldn't exist yet.
	fCorrelPar = new MoDSSCorrelParameter("CorrelPar");
	AddParameter(fCorrelPar);
	if (fCorrelPar) fCorrelPar->PrintParameter(0,0);
	else cout << "***** ERROR - CorrelPar doesn't exist - program will crash.\n";

	// MoDSS parameter - contain some general parameters
	fParam = (MoDSSParameter*) GetParameter("MoDSSPar"); // get parameter from the memory (or autosave file)
	if (!fParam) {
		fParam = new MoDSSParameter("MoDSSPar");
		AddParameter(fParam);
		if (fParam) fParam->PrintParameter(0,0);
		else cout << "***** ERR - MoDSSPar doesn't exist - program will crash.\n";
	}

	// calib parameter (necessary for some spectra initialisation)
	fCalPar = (MoDSSCalibParameter*) GetParameter("CalibPar"); // get parameter from the memory (or autosave file)
	if (!fCalPar) {
		fCalPar = new MoDSSCalibParameter("CalibPar");
		AddParameter(fCalPar);
		if (fCalPar) fCalPar->PrintParameter(0,0);
		else cout << "***** ERR - CalibPar doesn't exist - program will crash.\n";
	}

	// ---- 3. polygonal windows initialization

	// Here a polygonal TOF vs Energy window
	// used for the recoil selection is created
	cout<<"laoding polywindows"<<endl;

	// high energy window
	RemoveAnalysisCondition("TOFvsEY");
	//Double_t TOFvalues[6]={1362,1379,1398,1398,1362,1362};
	Double_t TOFvalues[9]={-197.048,-139.281,-152.118,-30.1669,220.154,566.752,829.91,277.92,-197.048};
	//Double_t EHvalues[6]={17.3,20.4,26.0,40.0,40.0,17.3};
	Double_t EHvalues[9]={8773.49,8773.49,4545.93,2249.48,1205.64,1205.64,5433.19,8669.1,8773.49};
	TFile FCutYCorr("Cut_ETof_Th-1.root");
	if(FCutYCorr.IsOpen())cout<<"rootfile for TCut open"<<endl;
	//TCutG* TOFvsEHcut = (TCutG*) FCutYCorr.Get("TOFvsEY");
	//cout<<"Points for the cut :"<<endl;		
	//for(int i=0;i<TOFvsEHcut->GetN();i++){
	//	cout<<i<<" : "<<TOFvsEHcut->GetX()<<" - "<<TOFvsEHcut->GetY();
	//}
	TCutG* TOFvsEHcut = new TCutG("initialcut",6,TOFvalues,EHvalues);
	cout<<"1"<<endl;
	fCondEYvsTOF = new TGo4PolyCond("TOFvsEY");
	cout<<"2"<<endl;
	fCondEYvsTOF -> SetValues(TOFvsEHcut);	
	cout<<"3"<<endl;
	//fCondEYvsTOF =  (TGo4PolyCond*) FCutYCorr.Get("TOFvsEY");
	AddAnalysisCondition(fCondEYvsTOF);
	cout<<"4"<<endl;
	fCondEYvsTOF -> Enable();
	cout<<"5"<<endl;
	delete TOFvsEHcut;
	cout<<"6"<<endl;

	//*fCondEYvsTOF = *( (TGo4PolyCond*) FCutYCorr.Get("TOFvsEY"));
	FCutYCorr.Close();
	cout<<"7"<<endl;

	// low energy window
	//Double_t ELvalues[6]={17.3,20.4,26.0,40.0,40.0,17.3};
	Double_t ELvalues[9]={8773.49,8773.49,4545.93,2249.48,1205.64,1205.64,5433.19,8669.1,8773.49};
	TCutG* TOFvsELcut = new TCutG("initialcut",6,TOFvalues,ELvalues);
	fCondEXvsTOF = new TGo4PolyCond("TOFvsEX");
	fCondEXvsTOF -> SetValues(TOFvsELcut);	
	AddAnalysisCondition(fCondEXvsTOF);
	fCondEXvsTOF -> Enable();
	delete TOFvsELcut;


	cout<<"Correlation variables"<<endl;
	// ---- 3. correlation variables
	fReTable   = new MoDSSAnlEvent[fCorrelPar->fReTableSize];
	fReLast    = 0;
	fReCounter = 0;
	fAlTable   = new MoDSSAnlEvent[fCorrelPar->fAlTableSize];
	fAlLast    = 0;
	fAlCounter = 0;
	fGaTable   = new MoDSSAnlEvent[fCorrelPar->fGaTableSize];
	fGaLast    = 0;
	fGaCounter = 0;

	// set 'fbAllAlphas'. this parameter determines if all alhas must be present
	// in the chain.
	// fGenTotal is total number of chain members
	fGenTotal = 0;
	if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorAlAl) ||
			(fCorrelPar->fbCorAlFi) || (fCorrelPar->fbCorAlGa) || 
			(fCorrelPar->fbCorGaAl)) fGenTotal = fCorrelPar->fNumAl; // is alpha search for? how many?
	//cout << "test 0 passed... Total generation = "<<fGenTotal<<endl;
	//if ((fCorrelPar->fbCorGaAl) && (fCorrelPar->fbCorAlGa)) fGenTotal++; // are there two alphas around gamma?
	cout << "test 1 passed... Total generation = "<<fGenTotal<<endl;
	if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorReGa) || (fCorrelPar->fbCorReFi))fGenTotal++; // is recoil search for?
	cout << "test 2 passed... Total generation = "<<fGenTotal<<endl;
	if ((fCorrelPar->fbCorAlFi) || ((fCorrelPar->fbCorGaFi) && !(fCorrelPar->fbCorGaAl))) fGenTotal++;	// is fission search for? If not paralel to the GaAl
	cout << "test 3 passed... Total generation = "<<fGenTotal<<endl;
	if ((fCorrelPar->fbCorGaAl) || (fCorrelPar->fbCorGaFi) || (fCorrelPar->fbCorAlGa) || (fCorrelPar->fbCorReGa)) fGenTotal++; // is gamma search for?
	if (fCorrelPar->fbCorReFi) fGenTotal++; // is fission search for?
	cout << "test 4 passed... Total generation = "<<fGenTotal<<endl;
	if (fCorrelPar->fbCorGaGa) fGenTotal++; // are the two gammas?
	cout << "test 5 passed... Total generation = "<<fGenTotal<<endl;
	if (fGenTotal > fCorrelPar->fNumPrt) fbAllAlphas = kFALSE;
	else fbAllAlphas = kTRUE;
	cout<<"All Alphas bit set to "<<fbAllAlphas<<endl;

	// set fNumCorrHelp, an auxiliary variable used in correlation search.
	// it is the number of last chain member from which we look for correlation
	// up to first member. saves us the time and some warnings are suppressed too.
	// (recoil doesn't play a role here)
	fNumCorrHelp = 0;
	if (fCorrelPar->fbCorAlAl) {
		fNumCorrHelp = fCorrelPar->fNumAl - fCorrelPar->fNumPrt + 2;
		if ((fCorrelPar->fbCorAlFi) || (fCorrelPar->fbCorAlGa))
			fNumCorrHelp += 1;
	} 

	// set energy limits for alpha table. minimum set to lowest value and
	// maximum to highest value of alpha energies for which we are searching
	fAlEnMin = 20000.;  // lower limit
	fAlEnMax = 0.;      // upper limit
	// re-al, al-al or al-fi correlation activated ? 
	if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorAlAl) || (fCorrelPar->fbCorAlFi) || (fCorrelPar->fbCorAlGa)) {
		for (Int_t gen = 1; gen <= fCorrelPar->fNumAl; gen++) {
			if (fAlEnMin > fCorrelPar->fAlEl[gen]) fAlEnMin = fCorrelPar->fAlEl[gen];
			if (fAlEnMax < fCorrelPar->fAlEh[gen]) fAlEnMax = fCorrelPar->fAlEh[gen];
		}
	}

	// calculate time limit for recoil table. table size should be large enough
	// to contain recoils within this time.
	fReTableTimeLimit = 0.;
	if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorReFi) || (fCorrelPar->fbCorReGa)) {
		if ((fCorrelPar->fbCorReFi) && (fCorrelPar->fbCorReGa)) {
			fReTableTimeLimit = maxdef(fCorrelPar->fFiTh,fCorrelPar->fGaTh);
			cout << "Calculating The Recoil Table Time Limit as a maximum from "<<fCorrelPar->fFiTh<<" and "<<fCorrelPar->fGaTh<<". It is "<<fReTableTimeLimit<<endl;
			cout << "The Recoil Table Time Limit is set to :"<<fReTableTimeLimit<<endl;
		}
		else if (fCorrelPar->fbCorReFi) {
			fReTableTimeLimit = fCorrelPar->fFiTh;
			cout << "The Recoil Table Time Limit is set to :"<<fReTableTimeLimit<<endl;
		}
		else if (fCorrelPar->fbCorReGa) {
			fReTableTimeLimit = fCorrelPar->fGaTh;
			cout << "The Recoil Table Time Limit is set to :"<<fReTableTimeLimit<<endl;
		}    
		if ((fCorrelPar->fbCorReAl) && (fbAllAlphas)) {
			fReTableTimeLimit = fCorrelPar->fAlTh[1];
			cout << "Recoil Table Time Limit is set to : "<<fReTableTimeLimit<<endl;
		}
		if ((fCorrelPar->fbCorReAl) && (!fbAllAlphas)){
			for (i = 1, fReTableTimeLimit = 0.; i <= (fGenTotal - fCorrelPar->fNumPrt + 1);
					fReTableTimeLimit += fCorrelPar->fAlTh[i], i++);
			if ((fCorrelPar->fbCorAlFi) && (fCorrelPar->fbCorAlGa)) {
				fReTableTimeLimit += maxdef(fCorrelPar->fFiTh,fCorrelPar->fGaTh);
				cout << "Recoil Table time limit increased by value "<< maxdef(fCorrelPar->fFiTh,fCorrelPar->fGaTh)<< " to value " << fReTableTimeLimit <<endl;
			}
			else if (fCorrelPar->fbCorAlFi) {
				fReTableTimeLimit += fCorrelPar->fFiTh;
				cout << "Recoil Table time limit increased by value "<< fCorrelPar->fFiTh<< " to value " << fReTableTimeLimit <<endl;
			}
			else if (fCorrelPar->fbCorAlGa) {
				fReTableTimeLimit += fCorrelPar->fGaTh;
				cout << "Recoil Table time limit increased by value "<< fCorrelPar->fGaTh<< " to value " << fReTableTimeLimit <<endl;
			}
			//if (fCorrelPar->fbCorAlFi) fReTableTimeLimit += fCorrelPar->fFiTh;
			//if (fCorrelPar->fbCorAlGa) fReTableTimeLimit += fCorrelPar->fGaTh;
		}
	}

	// calculate time limit for alpha table. table size should be large enough
	// to contain alpha within this time.
	fAlTableTimeLimit = 0.;
	if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorAlAl) || (fCorrelPar->fbCorAlFi) || (fCorrelPar->fbCorAlGa)) {
		if ((fCorrelPar->fbCorAlFi) && (fCorrelPar->fbCorAlGa)) {
			fAlTableTimeLimit = maxdef(fCorrelPar->fFiTh,fCorrelPar->fGaTh);
			cout << "Alpha Table Time Limit set to "<< fAlTableTimeLimit<<" as a maximum of " <<fCorrelPar->fFiTh<<" and " << fCorrelPar->fGaTh <<endl;
		}
		else if (fCorrelPar->fbCorAlFi) {
			fAlTableTimeLimit = fCorrelPar->fFiTh;
			cout << "Alpha Table Time Limit set to "<< fAlTableTimeLimit<<endl;
		}
		else if (fCorrelPar->fbCorAlGa) {
			fAlTableTimeLimit = fCorrelPar->fGaTh;
			cout << "Alpha Table Time Limit set to "<< fAlTableTimeLimit<<endl;
		}
		if (fCorrelPar->fbCorAlAl) {
			for (i = 2, fAlTableTimeLimit = 0.; i <= fCorrelPar->fNumAl;
					fAlTableTimeLimit += fCorrelPar->fAlTh[i], i++);
			if ((fCorrelPar->fbCorAlFi) && (fCorrelPar->fbCorAlGa)) {
				fAlTableTimeLimit += maxdef(fCorrelPar->fFiTh,fCorrelPar->fGaTh);
				cout << "Alpha Table time limit increased by value "<< maxdef(fCorrelPar->fFiTh,fCorrelPar->fGaTh)<< " to value " << fAlTableTimeLimit <<endl;
			}
			else if (fCorrelPar->fbCorAlFi) {
				fAlTableTimeLimit += fCorrelPar->fFiTh;
				cout << "Alpha Table time limit increased by value "<< fCorrelPar->fFiTh<< " to value " << fAlTableTimeLimit <<endl;
			}
			else if (fCorrelPar->fbCorAlGa) {
				fAlTableTimeLimit += fCorrelPar->fGaTh;
				cout << "Alpha Table time limit increased by value "<< fCorrelPar->fGaTh<< " to value " << fAlTableTimeLimit <<endl;
			}
		}
	}

	fGaTableTimeLimit = 0.;
	if ((fCorrelPar->fbCorGaAl) || (fCorrelPar->fbCorGaFi) || (fCorrelPar->fbCorGaGa)) {
		if ((fCorrelPar->fbCorGaFi) && (fCorrelPar->fbCorGaAl)){ 
			if (fbAllAlphas) {
				if (fCorrelPar->fbCorReGa) fGaTableTimeLimit = maxdef(fCorrelPar->fFiTh,fCorrelPar->fAlTh[1]);
				if (fCorrelPar->fbCorAlGa) fGaTableTimeLimit = maxdef(fCorrelPar->fFiTh,fCorrelPar->fAlTh[2]);
			}
			else {
				for (i = 1, fGaTableTimeLimit = 0.; i <= (fGenTotal - fCorrelPar->fNumPrt + 1); 
						fGaTableTimeLimit += fCorrelPar->fAlTh[i], i++);
				fGaTableTimeLimit = maxdef(fGaTableTimeLimit,fCorrelPar->fFiTh);
			} //else
		} //if ((fCorrelPar->fbCorGaFi) && (fCorrelPar->fbCorGaAl))
		else { // GaFi or GaAl of GaGA
			if (fCorrelPar->fbCorGaFi) fGaTableTimeLimit = fCorrelPar->fFiTh;
			if (fCorrelPar->fbCorGaAl)	{
				if (fbAllAlphas) {
					if (fCorrelPar->fbCorReGa) fGaTableTimeLimit = fCorrelPar->fAlTh[1];
					if (fCorrelPar->fbCorAlGa) fGaTableTimeLimit = fCorrelPar->fAlTh[2];
				}
				if (!fbAllAlphas) {
					for (i = 1, fGaTableTimeLimit = 0.; i <= (fGenTotal - fCorrelPar->fNumPrt + 1); 
							fGaTableTimeLimit += fCorrelPar->fAlTh[i], i++);
					//fGaTableTimeLimit = maxdef(fGaTableTimeLimit,fCorrelPar->fFiTh);
				}//if (!fbAllAlphas)
			} // if (fCorrelPar->fbCorGaAl)
			if (fCorrelPar->fbCorGaGa) fGaTableTimeLimit = fCorrelPar->fGaTh;
		} //else
	} //if ((fCorrelPar->fbCorGaAl) || (fCorrelPar->fbCorGaFi))

	cout << "Re Table Time Limit :" <<fReTableTimeLimit<<"\t" << " Al Table Time Limit :"<<fAlTableTimeLimit<< endl;

	// ---- 4. histograms

	// we must check, if the histograms have been restored from auto-save file 
	// DON'T GET THE HISTOGRAMS FROM MEMORY OR AUTOSAVE FILE!!! 
	// CREATE NEW EACH TIME A NEW ANALYSIS IS SUBMITTED !

	//  if (GetHistogram("corTIME") == 0)  { // no auto-save file, create new

	cout << "**** MoDSSCorrelProc: Creating Histograms" << endl;

	RemoveHistogram("corTIME");
	fhCorTIME    = new TH1D ("corTIME", "Number of correlation in time", fParam->fNumBinTime, fParam->fTime_res_low, fParam->fTime_res_high);
	AddHistogram(fhCorTIME, "Correlations");

	// --- recoil spectra

	//if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorReFi)) {


	RemoveHistogram("ReGa_EGvsDT");
	fhEGvsT  = new TH2D ("ReGa_EGvsDT", "Gamma energy vs correlation time", 2000, 0,1000,200,fCorrelPar->fGaTl,fCorrelPar->fGaTh); /* Boris:
	//previous parameters fCorrelPar->fGaEh-fCorrelPar->fGaEl, fCorrelPar->fGaEl,fCorrelPar->fGaEh,100,fCorrelPar->fGaTl,fCorrelPar->fGaTh); */
	AddHistogram(fhEGvsT, "Correlations/recoil");

	// Histogram bellow is basically the same as the one above, but it's supposed for random correlations
	// The only change later in the code should be that the Y coordinate should be shifted by lower limit of the time interval.
	// In such a case we could simply look for correlations outside of the expected time range where no real correlations hsould be present.
	// If the length of time interval is the same, we could simply substract distribution of these random correlations from the upper one
	// to get the background substracted time distribution.
	// Check please if the number of bins is the same!

	RemoveHistogram("ReGa_EGvsDT_RNDM");
	fhEGvsT_RNDM  = new TH2D ("ReGa_EGvsDT_RNDM", "Gamma energy vs correlation time - Random", fCorrelPar->fGaEh-fCorrelPar->fGaEl, fCorrelPar->fGaEl,fCorrelPar->fGaEh,100,0,fCorrelPar->fGaTh-fCorrelPar->fGaTl);
	AddHistogram(fhEGvsT_RNDM, "Correlations/recoil");


	for (int i = 0; i < NUMCRYSTAL ; i++)
	{
		sprintf(name, "ReGa_EGvsDT_Clover%02d",i+1);
		sprintf(descr, "Gamma energy vs correlation time, clover %02d", i+1);
		RemoveHistogram(name);
		fhEGvsT_Clover[i] = new TH2D (name, descr, fCorrelPar->fGaEh-fCorrelPar->fGaEl, fCorrelPar->fGaEl, fCorrelPar->fGaEh, 100,fCorrelPar->fGaTl, fCorrelPar->fGaTh);
		AddHistogram(fhEGvsT_Clover[i], "Correlations/recoil");
	}

	RemoveHistogram("ReGa-EGvsPulse");
	fhEGvsPulse  = new TH2D ("ReGa-EGvsPulse", "Gamma energy vs time in pulse", 1000, fCorrelPar->fGaEl,fCorrelPar->fGaEh,200,-10,10);
	AddHistogram(fhEGvsPulse, "Correlations/recoil");

	RemoveHistogram("ReTOF");
	RemoveHistogram("ReLGaCoinc");
	RemoveHistogram("ReCounts");
	RemoveHistogram("DTReFi");
	RemoveHistogram("DTReGamma");
	RemoveHistogram("DTReGammaBG");
	RemoveHistogram("DTReAl");
	RemoveHistogram("DT(re-al)vsEAl");





	fhReTOF        = new TH1D ("ReTOF", "Recoil TOF", 1751, -750, 1000);
	fhReLGaCoinc= new TH1D ("ReLGaCoinc", "Low energy Ga in coinc with Re", 8000, 0, 2000);
	fhCountRe      = new TH1D ("ReCounts", "Recoil counts in stop det.", 62, -0.5, 61.5);
	fhDTReFi       = new TH1D ("DTReFi", "Time difference Re - Fi", 200, fCorrelPar->fFiTl, fCorrelPar->fFiTh);
	//      fhDTReGamma    = new TH1D ("DTReGamma", "Time difference Re - Gamma", 200, 0, fCorrelPar->fGaTh);
	fhDTReGamma    = new TH1D ("DTReGamma", "Time difference Re - Gamma", 200, fCorrelPar->fGaTl,fCorrelPar->fGaTh);
	fhDTReGammaBG = new TH1D ("DTReGammaBG", "Time position Re - Gamma in Pulse", 200, -10, 10);
	fhDTReAl       = new TH1D ("DTReAl", "Time difference Re - Al", 200, fCorrelPar->fAlTl[1], fCorrelPar->fAlTh[1]);
	fhDTvsEAl      = new TH2D ("DT(re-al)vsEAl", "DT(re-al) vs Alpha Energy", 300, 0, 30, 1000, 6000, 10000);
	fhDTvsEAl->SetXTitle("log2(DT) (ms)");
	fhDTvsEAl->SetYTitle("Alpha Energy (keV)");



	AddHistogram(fhReTOF, "Correlations/recoil");
	AddHistogram(fhReLGaCoinc, "Correlations/recoil");
	AddHistogram(fhCountRe, "Correlations/recoil");
	AddHistogram(fhDTReFi, "Correlations/recoil");
	AddHistogram(fhDTReGamma, "Correlations/recoil");
	AddHistogram(fhDTReGammaBG, "Correlations/recoil");
	AddHistogram(fhDTReAl, "Correlations/recoil");
	AddHistogram(fhDTvsEAl, "Correlations/recoil");




	//=============================================================// Boris

	RemoveHistogram("ReGavsGaCoinc");
	RemoveHistogram("ReGavsTAC");
	RemoveHistogram("ReGavsAlpha");
	RemoveHistogram("ReGavsAlpha_BOXincl");

	RemoveHistogram("ReLGaCoincAnti");



	RemoveHistogram("STOPvsBOX");
	//	RemoveHistogram("STOPvsBOX_rtg");
	//	RemoveHistogram("STOPvsBOX_rtg_ac");
	RemoveHistogram("STOPvsEG_BOX");



	fhReGavsGaCoinc = new TH2D ("ReGavsGaCoinc", "Ga - Ga coincidences in coincidence with recoil", 2000, 0, 1000, 2000, 0, 1000);
	fhReGavsTAC = new TH2D ("ReGavsTAC", "Gamma in coincidence with recoil vs TAC", 2000, 0, 2000, 60, -0.03, 0.03);
	fhReGavsAlpha = new TH2D ("ReGavsAlpha", "(re-gamma)-alpha: EG vs E alpha", 2000, 4000, 8000, 2000, 0, 1000);
	fhReGavsAlpha_BOXincl = new TH2D ("ReGavsAlpha_BOXincl", "(re-gamma)-alpha: EG vs Eb alpha", 2500, 4000, 10000, 2000, 0, 1000);
	fhReLGaCoincAnti = new TH1D ("ReLGaCoincAnti", "ReLGaCoinc, signal only without coincidences with other crystals", 8000, 0, 2000);
	fhSTOPvsBOX = new TH2D ("STOPvsBOX", "E stop vs (E box - E stop)", 2000, 0, 8000, 1000, 0, 2000); 
	//	fhSTOPvsBOX_rtg = new TH2D ("STOPvsBOX_rtg", "E stop vs (E box - E stop), rtg gated", 2000, 0, 8000, 1000, 0, 2000); 
	//	fhSTOPvsBOX_rtg_ac = new TH2D ("STOPvsBOX_rtg_ac", "E stop vs (E box - E stop), rtg gated, anti compton", 2000, 0, 8000, 1000, 0, 2000); 
	fhSTOPvsEG_BOX = new TH2D ("STOPvsEG_BOX", "E stop vs E gamma, BOX gated", 2000, 0, 8000, 1000, 0, 2000); 



	AddHistogram(fhReGavsGaCoinc, "Correlations/recoil");
	AddHistogram(fhReGavsTAC, "Correlations/recoil");
	AddHistogram(fhReGavsAlpha, "Correlations/recoil");
	AddHistogram(fhReGavsAlpha_BOXincl, "Correlations/recoil");
	AddHistogram(fhReLGaCoincAnti, "Correlations/recoil");
	AddHistogram(fhSTOPvsBOX, "Correlations/alpha");
	//	AddHistogram(fhSTOPvsBOX_rtg, "Correlations/recoil");
	//	AddHistogram(fhSTOPvsBOX_rtg_ac, "Correlations/recoil");
	AddHistogram(fhSTOPvsEG_BOX, "Correlations/alpha");





	RemoveHistogram("ElecGam1Coinc_BOX"); //Electron Gamma 1 coincidences
	fhElGa1Coinc_BOX = new TH2D ("ElecGam1Coinc_BOX", "Electron Gamma1 coincidences", 1000, 0, 1000, 1000, 0, 1000);
	AddHistogram(fhElGa1Coinc_BOX, "Correlations/gamma");


	//=============================================================//Julien
	//X Singles
	RemoveHistogram("EX_Single");
	fhEX_Single      = new TH1D ("EX_Single", "Recoil energy X Single", 5000, 0, 20000);
	AddHistogram(fhEX_Single, "Correlations/recoil/EnergyReX");

	RemoveHistogram("EXAl_Single");
	fhEXAl_Single      = new TH1D ("EXAl_Single", "Alpha energy X Single", 5000, 0, 20000);
	AddHistogram(fhEXAl_Single, "Correlations/alpha");

	//Y Singles
	RemoveHistogram("EY_Single");
	fhEY_Single      = new TH1D ("EY_Single", "Recoil energy Y Single", 10000, 0, 40000);
	AddHistogram(fhEY_Single, "Correlations/recoil/EnergyReY");

	RemoveHistogram("EYAl_Single");
	fhEYAl_Single      = new TH1D ("EYAl_Single", "Alpha energy Y Single", 10000, 0, 40000);
	AddHistogram(fhEYAl_Single, "Correlations/alpha");

	//Delta t between X and Y hits
	RemoveHistogram("dTXY");
	fhdTXY= new TH1D ("dTXY", "Delta t X-Y", 100000, 0,1);
	AddHistogram(fhdTXY, "Correlations");

	//Alpha energies stored in fAlTable
	RemoveHistogram("hEAlphaTable");
	fhEAlphaTable= new TH1D ("hEAlphaTable", "Alpha energies stored in fAlTable", 5000, 0,20000);
	AddHistogram(fhEAlphaTable, "Correlations");
	
	//Alpha energies transmitted to RecoilAlpha()
	RemoveHistogram("hEAlphaReAl");
	fhEAlphaReAl= new TH1D ("hEAlphaReAl", "Alpha energies transmited to RecoilAlpha()", 5000, 0,20000);
	AddHistogram(fhEAlphaReAl, "Correlations");

	//Alpha energies with time diff ok
	RemoveHistogram("hEAlphaTimeOK");
	fhEAlphaTimeOK= new TH1D ("hEAlphaTimeOK", "Alpha energies correlated in time to Re", 5000, 0,20000);
	AddHistogram(fhEAlphaTimeOK, "Correlations");

	//Alpha energies with time diff and position ok
	RemoveHistogram("hEAlphaTimePosOK");
	fhEAlphaTimePosOK= new TH1D ("hEAlphaTimePosOK", "Alpha energies correlated in time and position to Re", 5000, 0,20000);
	AddHistogram(fhEAlphaTimePosOK, "Correlations");

	//Alpha energies with found correlation
	RemoveHistogram("hEAlphaCorred");
	fhEAlphaCorred= new TH1D ("hEAlphaCorred", "Alpha energies correlated to a Re", 5000, 0,20000);
	AddHistogram(fhEAlphaCorred, "Correlations");

	//Alpha energies sent to PrintChain
	RemoveHistogram("hEAlphaPrint");
	fhEAlphaPrint= new TH1D ("hEAlphaPrint", "Alpha energies correlated to a Re sent to PrintChain", 5000, 0,20000);
	AddHistogram(fhEAlphaPrint, "Correlations");
	
	//X energies received from AnlEvent
	RemoveHistogram("hEReX_check");
	fhEReX_check= new TH1D ("hEReX_check", "X energies received from AnlEvent", 5000, 0,20000);
	AddHistogram(fhEReX_check, "Correlations");
	RemoveHistogram("hEReX_check2");
	fhEReX_check2= new TH1D ("hEReX_check2", "X energies received from AnlEvent 2", 5000, 0,20000);
	AddHistogram(fhEReX_check2, "Correlations");
	
	//Y Energies received from AnlEvent
	RemoveHistogram("hEReY_check");
	fhEReY_check= new TH1D ("hEReY_check", "Y energies received from AnlEvent", 7000, 0,70000);
	AddHistogram(fhEReY_check, "Correlations");
	RemoveHistogram("hEReY_check2");
	fhEReY_check2= new TH1D ("hEReY_check2", "Y energies received from AnlEvent 2", 7000, 0,70000);
	AddHistogram(fhEReY_check2, "Correlations");

	//Ex vs Ey 
	RemoveHistogram("hExEy");
	fhExEy= new TH2D ("hExEy", "Ex vs Ey", 1000,0,20000, 2000,0,40000);
	AddHistogram(fhExEy, "Correlations");
	//Multiplicity Gamma
	RemoveHistogram("hGammaMult");
	fhGammaMult= new TH1D ("hGammaMult", "Gamma Mutlipicity", 20,0,20);
	AddHistogram(fhGammaMult, "Correlations");
/*
	RemoveHistogram("fhReTRH_True_Ex");
	RemoveHistogram("fhReTRH_True_Ey");
	RemoveHistogram("fhReTRH_True_X");
	RemoveHistogram("fhReTRH_True_Y");
	RemoveHistogram("fhReTRH_True_TOF");
	RemoveHistogram("fhReTRH_False_Ex");
	RemoveHistogram("fhReTRH_False_Ey");
	RemoveHistogram("fhReTRH_False_X");
	RemoveHistogram("fhReTRH_False_Y");
	RemoveHistogram("fhReTRH_False_TOF");

	RemoveHistogram("fhReTable_True_Ex");
	RemoveHistogram("fhReTable_True_Ey");
	RemoveHistogram("fhReTable_True_X");
	RemoveHistogram("fhReTable_True_Y");
	RemoveHistogram("fhReTable_True_TOF");
	RemoveHistogram("fhReTable_False_Ex");
	RemoveHistogram("fhReTable_False_Ey");
	RemoveHistogram("fhReTable_False_X");
	RemoveHistogram("fhReTable_False_Y");
	RemoveHistogram("fhReTable_False_TOF");

	RemoveHistogram("fhAlTRH_True_Ex");
	RemoveHistogram("fhAlTRH_True_Ey");
	RemoveHistogram("fhAlTRH_True_X");
	RemoveHistogram("fhAlTRH_True_Y");
	RemoveHistogram("fhAlTRH_True_TOF");
	RemoveHistogram("fhAlTRH_False_Ex");
	RemoveHistogram("fhAlTRH_False_Ey");
	RemoveHistogram("fhAlTRH_False_X");
	RemoveHistogram("fhAlTRH_False_Y");
	RemoveHistogram("fhAlTRH_False_TOF");

	RemoveHistogram("fhAlTable_True_Ex");
	RemoveHistogram("fhAlTable_True_Ey");
	RemoveHistogram("fhAlTable_True_X");
	RemoveHistogram("fhAlTable_True_Y");
	RemoveHistogram("fhAlTable_True_TOF");
	RemoveHistogram("fhAlTable_False_Ex");
	RemoveHistogram("fhAlTable_False_Ey");
	RemoveHistogram("fhAlTable_False_X");
	RemoveHistogram("fhAlTable_False_Y");
	RemoveHistogram("fhAlTable_False_TOF");

	fhReTRH_True_Ex		= new TH1D ("hReTRH_True_Ex"	,"hReTRH_True_Ex"	, 5000, 0,20000);
	fhReTRH_True_Ey		= new TH1D ("hReTRH_True_Ey"	,"hReTRH_True_Ey"	, 7000, 0,70000);
	fhReTRH_True_X		= new TH1D ("hReTRH_True_X"	,"hReTRH_True_X"	, 62, -0.5, 61.5);
	fhReTRH_True_Y		= new TH1D ("hReTRH_True_Y"	,"hReTRH_True_Y"	, 62, -0.5, 61.5);
	fhReTRH_True_TOF	= new TH1D ("hReTRH_True_TOF"	,"hReTRH_True_TOF"	, 1751, -750, 1000);
	fhReTRH_False_Ex	= new TH1D ("hReTRH_False_Ex"  	,"hReTRH_False_Ex"  	, 5000, 0,20000);
	fhReTRH_False_Ey	= new TH1D ("hReTRH_False_Ey"  	,"hReTRH_False_Ey"  	, 7000, 0,70000);
	fhReTRH_False_X		= new TH1D ("hReTRH_False_X"  	,"hReTRH_False_X"  	, 62, -0.5, 61.5);
	fhReTRH_False_Y		= new TH1D ("hReTRH_False_Y"   	,"hReTRH_False_Y"   	, 62, -0.5, 61.5);
	fhReTRH_False_TOF	= new TH1D ("hReTRH_False_TOF"	,"hReTRH_False_TOF"	, 1751, -750, 1000);
	//- Sent to Table
	fhReTable_True_Ex	= new TH1D ("hReTable_True_Ex"	,"hReTable_True_Ex"	, 5000, 0,20000);
	fhReTable_True_Ey	= new TH1D ("hReTable_True_Ey"	,"hReTable_True_Ey"	, 7000, 0,70000);
	fhReTable_True_X	= new TH1D ("hReTable_True_X"	,"hReTable_True_X"	, 62, -0.5, 61.5);
	fhReTable_True_Y	= new TH1D ("hReTable_True_Y"	,"hReTable_True_Y"	, 62, -0.5, 61.5);
	fhReTable_True_TOF	= new TH1D ("hReTable_True_TOF"	,"hReTable_True_TOF"	, 1751, -750, 1000);
	fhReTable_False_Ex	= new TH1D ("hReTable_False_Ex"	,"hReTable_False_Ex"  	, 5000, 0,20000);
	fhReTable_False_Ey	= new TH1D ("hReTable_False_Ey"	,"hReTable_False_Ey"  	, 7000, 0,70000);
	fhReTable_False_X	= new TH1D ("hReTable_False_X" 	,"hReTable_False_X"  	, 62, -0.5, 61.5);
	fhReTable_False_Y	= new TH1D ("hReTable_False_Y" 	,"hReTable_False_Y"   	, 62, -0.5, 61.5);
	fhReTable_False_TOF	= new TH1D ("hReTable_False_TOF","hReTable_False_TOF"	, 1751, -750, 1000);
	//TestRecoilHigh
	fhAlTRH_True_Ex		= new TH1D ("hAlTRH_True_Ex"	,"hAlTRH_True_Ex"	, 5000, 0,20000);
	fhAlTRH_True_Ey		= new TH1D ("hAlTRH_True_Ey"	,"hAlTRH_True_Ey"	, 7000, 0,70000);
	fhAlTRH_True_X		= new TH1D ("hAlTRH_True_X"	,"hAlTRH_True_X"	, 62, -0.5, 61.5);
	fhAlTRH_True_Y		= new TH1D ("hAlTRH_True_Y"	,"hAlTRH_True_Y"	, 62, -0.5, 61.5);
	fhAlTRH_True_TOF	= new TH1D ("hAlTRH_True_TOF"	,"hAlTRH_True_TOF"	, 1751, -750, 1000);
	fhAlTRH_False_Ex	= new TH1D ("hAlTRH_False_Ex"  	,"hAlTRH_False_Ex"  	, 5000, 0,20000);
	fhAlTRH_False_Ey	= new TH1D ("hAlTRH_False_Ey"  	,"hAlTRH_False_Ey"  	, 7000, 0,70000);
	fhAlTRH_False_X		= new TH1D ("hAlTRH_False_X"  	,"hAlTRH_False_X"  	, 62, -0.5, 61.5);
	fhAlTRH_False_Y		= new TH1D ("hAlTRH_False_Y"   	,"hAlTRH_False_Y"   	, 62, -0.5, 61.5);
	fhAlTRH_False_TOF	= new TH1D ("hAlTRH_False_TOF"	,"hAlTRH_False_TOF"	, 1751, -750, 1000);
	//Sent to Table
	fhAlTable_True_Ex	= new TH1D ("hAlTable_True_Ex"	,"hAlTable_True_Ex"	, 5000, 0,20000);
	fhAlTable_True_Ey	= new TH1D ("hAlTable_True_Ey"	,"hAlTable_True_Ey"	, 7000, 0,70000);
	fhAlTable_True_X	= new TH1D ("hAlTable_True_X"	,"hAlTable_True_X"	, 62, -0.5, 61.5);
	fhAlTable_True_Y	= new TH1D ("hAlTable_True_Y"	,"hAlTable_True_Y"	, 62, -0.5, 61.5);
	fhAlTable_True_TOF	= new TH1D ("hAlTable_True_TOF"	,"hAlTable_True_TOF"	, 1751, -750, 1000);
	fhAlTable_False_Ex	= new TH1D ("hAlTable_False_Ex"	,"hAlTable_False_Ex"  	, 5000, 0,20000);
	fhAlTable_False_Ey	= new TH1D ("hAlTable_False_Ey"	,"hAlTable_False_Ey"  	, 7000, 0,70000);
	fhAlTable_False_X	= new TH1D ("hAlTable_False_X" 	,"hAlTable_False_X"  	, 62, -0.5, 61.5);
	fhAlTable_False_Y	= new TH1D ("hAlTable_False_Y" 	,"hAlTable_False_Y"   	, 62, -0.5, 61.5);
	fhAlTable_False_TOF	= new TH1D ("hAlTable_False_TOF","hAlTable_False_TOF"	, 1751, -750, 1000);

	AddHistogram(fhReTRH_True_Ex	, "Correlations");
	AddHistogram(fhReTRH_True_Ey	, "Correlations");
	AddHistogram(fhReTRH_True_X	, "Correlations");
	AddHistogram(fhReTRH_True_Y	, "Correlations");
	AddHistogram(fhReTRH_True_TOF	, "Correlations");
	AddHistogram(fhReTRH_False_Ex	, "Correlations");
	AddHistogram(fhReTRH_False_Ey	, "Correlations");
	AddHistogram(fhReTRH_False_X	, "Correlations");
	AddHistogram(fhReTRH_False_Y	, "Correlations");
	AddHistogram(fhReTRH_False_TOF	, "Correlations");
                     //- Sent to Table
	AddHistogram(fhReTable_True_Ex	, "Correlations");
	AddHistogram(fhReTable_True_Ey	, "Correlations");
	AddHistogram(fhReTable_True_X	, "Correlations");
	AddHistogram(fhReTable_True_Y	, "Correlations");
	AddHistogram(fhReTable_True_TOF	, "Correlations");
	AddHistogram(fhReTable_False_Ex	, "Correlations");
	AddHistogram(fhReTable_False_Ey	, "Correlations");
	AddHistogram(fhReTable_False_X	, "Correlations");
	AddHistogram(fhReTable_False_Y	, "Correlations");
	AddHistogram(fhReTable_False_TOF, "Correlations");
                     //TestRecoilHigh
	AddHistogram(fhAlTRH_True_Ex	, "Correlations");
	AddHistogram(fhAlTRH_True_Ey	, "Correlations");
	AddHistogram(fhAlTRH_True_X	, "Correlations");
	AddHistogram(fhAlTRH_True_Y	, "Correlations");
	AddHistogram(fhAlTRH_True_TOF	, "Correlations");
	AddHistogram(fhAlTRH_False_Ex	, "Correlations");
	AddHistogram(fhAlTRH_False_Ey	, "Correlations");
	AddHistogram(fhAlTRH_False_X	, "Correlations");
	AddHistogram(fhAlTRH_False_Y	, "Correlations");
	AddHistogram(fhAlTRH_False_TOF	, "Correlations");
                     //Sent to Table
	AddHistogram(fhAlTable_True_Ex	, "Correlations");
	AddHistogram(fhAlTable_True_Ey	, "Correlations");
	AddHistogram(fhAlTable_True_X	, "Correlations");
	AddHistogram(fhAlTable_True_Y	, "Correlations");
	AddHistogram(fhAlTable_True_TOF	, "Correlations");
	AddHistogram(fhAlTable_False_Ex	, "Correlations");
	AddHistogram(fhAlTable_False_Ey	, "Correlations");
	AddHistogram(fhAlTable_False_X	, "Correlations");
	AddHistogram(fhAlTable_False_Y	, "Correlations");
	AddHistogram(fhAlTable_False_TOF, "Correlations");

	RemoveHistogram("fdReInput_EyTOF");
	RemoveHistogram("fdReTRH_True_EyTOF");
	RemoveHistogram("fdReTRH_False_EyTOF");
	RemoveHistogram("fdReTable_True_EyTOF");
	RemoveHistogram("fdReTable_False_EyTOF");
	fdReInput_EyTOF			= new TH2D ("dReInput_EyTOF		","dReInput_EyTOF	",1751, -750, 1000, 7000, 0,70000);
	fdReTRH_True_EyTOF		= new TH2D ("dReTRH_True_EyTOF		","dReTRH_True_EyTOF	",1751, -750, 1000, 7000, 0,70000);
	fdReTRH_False_EyTOF		= new TH2D ("dReTRH_False_EyTOF	","dReTRH_False_EyTOF		",1751, -750, 1000, 7000, 0,70000);
	fdReTable_True_EyTOF		= new TH2D ("dReTable_True_EyTOF	","dReTable_True_EyTOF	",1751, -750, 1000, 7000, 0,70000);
	fdReTable_False_EyTOF		= new TH2D ("dReTable_False_EyTOF	","dReTable_False_EyTOF	",1751, -750, 1000, 7000, 0,70000);
	AddHistogram(fdReInput_EyTOF		, "Correlations");
	AddHistogram(fdReTRH_True_EyTOF		, "Correlations");
	AddHistogram(fdReTRH_False_EyTOF		, "Correlations");
	AddHistogram(fdReTable_True_EyTOF		, "Correlations");
	AddHistogram(fdReTable_False_EyTOF		, "Correlations");*/
	//=============================================================//

	// recoil in low energy branch
	//if (fCorrelPar->fbReEL) {
	RemoveHistogram("EReX");
	RemoveHistogram("TOFvsEReX");
	fhEReX      = new TH1D ("EReX", "Recoil energy X", 5000, 0, 20000);
	fhTOFvsEReX  = new TH2D ("TOFvsEReX", "TOF vs. Recoil energy X", 1751, -750, 1000, 1000, 0, 20000);
	fhTOFvsEReX->SetXTitle("Time of Flight (a.u.)");
	fhTOFvsEReX->SetYTitle("Recoil Energy (keV)");
	AddHistogram(fhEReX, "Correlations/recoil/EnergyReX");
	AddHistogram(fhTOFvsEReX, "Correlations/recoil/EnergyReX");

	// ----- || -----  recoil - fission
	//if (fCorrelPar->fbCorReFi) {
	RemoveHistogram("EReX_vs_EFi");
	fhEReXvsEFi  = new TH2D ("EReX_vs_EFi", "Recoil energy X vs. Fission energy", 1000, 0, 20000, 500, 0, 70000); 
	AddHistogram(fhEReXvsEFi, "Correlations/recoil/EnergyReX");

	//} // re-fi


	// ----- || -----  recoil - alpha
	//if (fCorrelPar->fbCorReAl) {
	RemoveHistogram("EReX_vs_EAl");
	fhEReXvsEAl  = new TH2D ("EReX_vs_EAl", "Recoil energy X vs. Alpha energy", 1000, 0, 20000, 2000, 0, 12000);
	AddHistogram(fhEReXvsEAl, "Correlations/recoil/EnergyReX");

	RemoveHistogram("ReEAl");
	fhReEAl  = new TH1D ("ReEAl", "Re-al gated Alpha energy", 10000, 0, 20000);
	AddHistogram(fhReEAl, "Correlations/alpha");
	//} // re-al
	//} // recoil in low energy branch

	// recoil in high energy branch
	//if (fCorrelPar->fbReEH) {
	//max = fCalPar->Ay[1] * 50000 + fCalPar->By[1] - 0.1;  // upper limit of histogram emev-axis (suppose 9000 channels) (-0.1 is safety distance for ROOT)
	RemoveHistogram("EReY");
	fhEReY      = new TH1D ("EReY", "Recoil energy Y", 7000, 0, 70000);
	RemoveHistogram("TOFvsEReY");
	fhTOFvsEReY = new TH2D ("TOFvsEReY", "TOF vs. Recoil energy Y", 1751, -750, 1050, 7000, 0, 70000);
	fhTOFvsEReY->SetXTitle("Time of Flight (a.u.)");
	fhTOFvsEReY->SetYTitle("Recoil Energy (keV)");
	AddHistogram(fhEReY, "Correlations/recoil/EnergyReY");
	AddHistogram(fhTOFvsEReY, "Correlations/recoil/EnergyReY");


	// ----- || -----  recoil - fission
	//if (fCorrelPar->fbCorReFi) {
	RemoveHistogram("EReY_vs_EFi");
	fhEReYvsEFi  = new TH2D ("EReY_vs_EFi", "Recoil energy Y vs. Fission energy)", 1000, 0, 7000, 1000, 0, 70000);

	AddHistogram(fhEReYvsEFi, "Correlations/recoil/EnergyReY");
	//} // re-fi

	// ----- || -----  recoil - alpha
	//if (fCorrelPar->fbCorReAl) {
	RemoveHistogram("EReYvsEAl");
	fhEReYvsEAl  = new TH2D ("EReYvsEAl", "Recoil energy Y vs. Alpha energy)", 1000, 0, 70000, 2000, 0, 12000);  
	AddHistogram(fhEReYvsEAl, "Correlations/recoil/EnergyReY");
	//} // re-al
	//} // recoil in high energy branch
	//    } // recoil spectra


	// --- alpha spectra
	//if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorAlAl) || (fCorrelPar->fbCorAlFi)) {
	RemoveHistogram("AlCounts");
	fhCountAl      = new TH1D ("AlCounts", "Alpha counts in stop det.", 62, -0.5, 61.5);
	RemoveHistogram("EAlphaAll");
	fhEAlphaAll    = new TH1D ("EAlphaAll", "Energy of correlated alphas", 8000, 0, 16000);
	AddHistogram(fhEAlphaAll, "Correlations/alpha");
	AddHistogram(fhCountAl, "Correlations/alpha");
	for (i = 0; i < MAXALPHA; i++) {
		sprintf(name, "EAlpha_gen_%d", i+1);
		sprintf(descr, "Energy of correlated alpha (gen %d)", i+1);
		RemoveHistogram(name);
		fhEAlpha[i] = new TH1D (name, descr, 8000, 0, 16000);
		AddHistogram(fhEAlpha[i], "Correlations/alpha");
	}
	for (i = 0; i < MAXALPHA; i++) 
	{
		sprintf(name, "EGamCoincToAlpha_gen_%d", i+1);
		sprintf(descr, "Gamma Energy coincident to alpha gen %d", i+1);
		RemoveHistogram(name);
		fhGamAl[i] = new TH1D (name, descr, 6000, 0, 1500);
		AddHistogram(fhGamAl[i], "Correlations/alpha");
	}
	RemoveHistogram("EGvsEAl_1_corr");
	fhEGvsEAl_1_corr = new TH2D ("EGvsEAl_1_corr","Alpha-Gamma coincidences (1st gen. alpha)", 2000, 0, 10000, 1500, 0, 1500);
	AddHistogram(fhEGvsEAl_1_corr, "Correlations/alpha");

	// alpha - fission
	//if (fCorrelPar->fbCorAlFi) {
	RemoveHistogram("DTAlFi");
	fhDTAlFi     = new TH1D ("DTAlFi", "Time difference Al - Fi", 1000, 0, fCorrelPar->fFiTh);
	max = 70000; 
	RemoveHistogram("EAl_vs_EFi");
	fhEAlvsEFi     = new TH2D ("EAl_vs_EFi", "Alpha energy vs. Fission energy)", 2600, 0, 13000, 800, 0, max);

	AddHistogram(fhDTAlFi, "Correlations/alpha");
	AddHistogram(fhEAlvsEFi, "Correlations/alpha");

	//} //al-fi

	// alpha - gamma
	RemoveHistogram("DTAlGamma");
	fhDTAlGamma     = new TH1D ("DTAlGamma", "Time difference Al - Gamma", 500, 0, 500);
	AddHistogram(fhDTAlGamma, "Correlations/alpha");

	// alpha - alpha
	//if (fCorrelPar->fbCorAlAl) {
	RemoveHistogram("EAlMo_vs_EAlDa");
	fhEAlMothervsEAlDaughter = new TH2D ("EAlMo_vs_EAlDa", "Energy - Alpha mother vs. Alpha daughter (1st chain only)", 2500, 0, 10000, 1500, 4000, 10000);
	AddHistogram(fhEAlMothervsEAlDaughter, "Correlations/alpha");

	RemoveHistogram("EAlMo_vs_EAlDa_BoxIncl");
	fhEAlMothervsEAlDaughter_BoxIncl = new TH2D ("EAlMo_vs_EAlDa_BoxIncl", "Energy (only with X) - Alpha mother vs. Alpha daughter - BOX included (1st chain only)", 2600, 0, 13000, 2600, 0, 13000);
	AddHistogram(fhEAlMothervsEAlDaughter_BoxIncl, "Correlations/alpha");

	RemoveHistogram("EAlMo_vs_EAlDa_AllChains");
	fhEAlMothervsEAlDaughter_AllChains = new TH2D ("EAlMo_vs_EAlDa_AllChains", "Energy - Alpha mother vs. Alpha daughter (alternative chains incl.)", 5000, 4000, 9000, 5000, 4000, 9000);
	AddHistogram(fhEAlMothervsEAlDaughter_AllChains, "Correlations/alpha");

	RemoveHistogram("EAlMo_vs_EAlDa_BoxIncl_AllChains");
	fhEAlMothervsEAlDaughter_BoxIncl_AllChains = new TH2D ("EAlMo_vs_EAlDa_BoxIncl_AllChains", "Energy (only with X)- Alpha mother vs. Alpha daughter - BOX included (alternative chains incl.)", 2600, 0, 13000, 2600, 0, 13000);
	AddHistogram(fhEAlMothervsEAlDaughter_BoxIncl_AllChains, "Correlations/alpha");


	for (i = 1; i < MAXALPHA-1; i++) {
		sprintf(name, "DTAl_%d_Al_%d", i, i+1);
		sprintf(descr, "Time difference Al_%d - Al_%d", i, i+1);
		RemoveHistogram(name);
		fhDTAlAl[i] = new TH1D (name, descr, 1000, -0.1, fCorrelPar->fAlTh[i+1]);
		//if (i > 0) AddHistogram(fhDTAlAl[i], "Correlations/alpha"); // don't register first one
		AddHistogram(fhDTAlAl[i], "Correlations/alpha");
	}

	RemoveHistogram("DTAl1Al2vsEAl2"); // pridala som tento riadok (Zdenka);
	fhDTAl1Al2vsEAl2 = new TH2D ("DTAl1Al2vsEAl2", "DT(Al1-Al2) vs Alpha2 Energy", 500,0,fCorrelPar->fAlTh[2],1000,fCorrelPar->fAlEl[2],fCorrelPar->fAlEh[2]); // pridala som tento riadok (Zdenka)
	AddHistogram(fhDTAl1Al2vsEAl2, "Correlations/alpha"); // pridala som tento riadok (Zdenka)

	//al-al
	//alpha spectra
	RemoveHistogram("hXAl"); //Position in X of the first generation of correlated alpha
	fhXAl = new TH1D ("hXAl", "Position X First gen Alpha", 62,0.5,61.5); // 
	AddHistogram(fhXAl, "Correlations/alpha"); // 
	RemoveHistogram("hYAl"); //Position in Y of the first generation of correlated alpha 
	fhYAl = new TH1D ("hYAl", "Position Y First gen Alpha", 62,0.5,61.5); // 
	AddHistogram(fhYAl, "Correlations/alpha"); // 

	// --- fission spectra
	//if ((fCorrelPar->fbCorReFi) || (fCorrelPar->fbCorAlFi)) {
	max = 70000;  // upper limit of histogram emev-axis (suppose 9000 channels) (-0.1 is safety distance for ROOT)
	RemoveHistogram("EFissionStopDet");
	fhEFissionStopDet  = new TH1D ("EFissionStopDet", "Fission energy - stop detector - all events", 8000, 0, max);
	AddHistogram(fhEFissionStopDet, "Correlations/fission");

	RemoveHistogram("EFissionStopDetOnly");
	fhEFissionStopDetOnly  = new TH1D ("EFissionStopDetOnly", "Fission energy - stop detector only events", 8000, 0, max);
	AddHistogram(fhEFissionStopDetOnly, "Correlations/fission");

	RemoveHistogram("EFissionStopBoxDet");
	fhEFissionStopBoxDet  = new TH1D ("EFissionStopBoxDet", "Fission energy - stop + box events", 8000, 0, max);
	AddHistogram(fhEFissionStopBoxDet, "Correlations/fission");

	RemoveHistogram("DTGammaFi");
	fhDTGaFi       = new TH1D ("DTGammaFi", "Time difference Gamma - Fission", 500, 0, fCorrelPar->fFiTh);
	AddHistogram(fhDTGaFi, "Correlations/fission");
	//}

	// ------- gamma spectra
	RemoveHistogram("EGamma");
	fhEGamma  = new TH1D ("EGamma", "Gamma energy", 8000, 0, 2000);
	AddHistogram(fhEGamma, "Correlations/gamma");

	RemoveHistogram("EGammaBestRes");
	fhEGammaBestRes  = new TH1D ("EGammaBestRes", "Gamma energy from crystal with best resolution", 4000, 0, 1000);
	AddHistogram(fhEGammaBestRes, "Correlations/gamma");

	for(int i=0; i<20; i++){
		sprintf(name, "EGammaClo[%d]", i);
		sprintf(descr, "Gamma energy from crystal %d", i);
		RemoveHistogram(name);
		fhEGammaClo[i]  = new TH1D (name,descr , 1000, 0, 1000);
		AddHistogram(fhEGammaClo[i], "Correlations/gamma");
	}

	RemoveHistogram("EGammaHigh"); // single spectrum for gamma 2
	fhEGammaHigh  = new TH1D ("EGammaHigh", "Gamma energy high", 1000, 0, 8000);
	AddHistogram(fhEGammaHigh, "Correlations/gamma");

	//RemoveHistogram("EGCoinc1D");
	//fhEGCoinc1D = new TH1D ("EGCoinc1D", "Gamma energy of coincident gammas", 2000, 0, 2000);
	//AddHistogram(fhEGCoinc1D, "Correlations/gamma");

	/*	RemoveHistogram("EGammavsTAC");
		fhEGvsTAC = new TH2D ("EGammavsTAC", "Delayed coincident gammas", 1000, 0, 1000, 2000, fCalPar->TAC_stop_gamma_A[1]*0 + fCalPar->TAC_stop_gamma_B[1], fCalPar->TAC_stop_gamma_A[1]*8000 + fCalPar->TAC_stop_gamma_B[1]);
		AddHistogram(fhEGvsTAC, "Correlations/gamma");*/

	RemoveHistogram("EGCoinc2D");
	fhEGCoinc2D = new TH2D ("EGCoinc2D", "Delayed coincident gammas", 2000, 0, 1000, 2000, 0, 1000); // Boris: #bins 700 -> 2000, x axis (0, 1400) -> (0, 1000)
	AddHistogram(fhEGCoinc2D, "Correlations/gamma");

	RemoveHistogram("EGCoincVsGaGaSum");
	fhEGCoincVsGaGaSum  = new TH2D ("EGCoincVsGaGaSum", "Gammas coinc. to Gamma Gamma sum coinc", 700, 0, 1400, 700, 0, 1400);
	AddHistogram(fhEGCoincVsGaGaSum, "Correlations/gamma");

	RemoveHistogram("ElecGam1Coinc"); //Electron Gamma 1 coincidences
	fhElGa1Coinc = new TH2D ("ElecGam1Coinc", "Electron Gamma1 coincidences", 1000, 0, 1000, 300, 0, 900);
	AddHistogram(fhElGa1Coinc, "Correlations/gamma");

	RemoveHistogram("ElecGam1CoincBestRes"); //Electron Gamma 1 coincidences
	fhElGa1CoincBestRes = new TH2D ("ElecGam1CoincBestRes", "El-Ga1 coincidences for crystals with best resolution", 1000, 0, 1000, 300, 0, 900);
	AddHistogram(fhElGa1CoincBestRes, "Correlations/gamma");


	// ------ || -----  alpha - gamma

	RemoveHistogram("AlGaDelayed");
	fhEAlEGadelayed = new TH2D ("AlGaDelayed", "Alpha (including back)) Gamma delayed (alpha is mother)", 1000, 5000, 10000, 1500, 0, 1500);
	AddHistogram(fhEAlEGadelayed, "Correlations/gamma");

	RemoveHistogram("GaAlDelayed");
	fhEGaEAldelayed = new TH2D ("GaAlDelayed", "Gamma Alpha (including back)) delayed (Gamma is mother)", 1000, 5000, 10000, 1500, 0, 1500);
	AddHistogram(fhEGaEAldelayed, "Correlations/gamma");


	//========================== Gamma 2 related spectra ============
	RemoveHistogram("EGamma2"); // single spectrum for gamma 2
	fhEGamma2  = new TH1D ("EGamma2", "Gamma 2 energy", 2000, 0, 2000);
	AddHistogram(fhEGamma2, "Correlations/gamma2");

	/*RemoveHistogram("EGamma2High"); // single spectrum for gamma 2 high energy part
	  fhEGamma2High  = new TH1D ("EGamma2High", "Gamma 2 energy high", 2000, 0, 8000);
	  AddHistogram(fhEGamma2High, "Correlations/gamma2");*/

	RemoveHistogram("EG2Coinc2D"); // gamma-gamma matrix for coincident gammas for gamma2
	fhEG2Coinc2D = new TH2D ("EG2Coinc2D", "Delayed coincident gammas for gamma 2", 750, 0, 1500, 750, 0, 1500);
	AddHistogram(fhEG2Coinc2D, "Correlations/gamma2");

	RemoveHistogram("EG2EG1corr"); // gamma1-gamma2 correlations (filled in gamma 1 part)
	fhEG2EG1corr = new TH2D ("EG2EG1corr", "Corr. gamma2 to gamma1", 750, 0, 1500, 750, 0, 1500);
	AddHistogram(fhEG2EG1corr, "Correlations/gamma2");

	RemoveHistogram("DTGa1Ga2"); // time difference between Gamma2 and Gamma1
	fhDTGa1Ga2     = new TH1D ("DTGa1Ga2", "Time difference Gamma1 - Gamma2", 500, 0, fCorrelPar->fGa2Th);
	AddHistogram(fhDTGa1Ga2, "Correlations/gamma2");

	RemoveHistogram("ElGa2Coinc"); //Electron Gamma 2 coincidences
	fhElGa2Coinc = new TH2D ("ElGa2Coinc", "Electron Gamma2 coincidences", 300, 0, 900, 1500, 0, 1500);
	AddHistogram(fhElGa2Coinc, "Correlations/gamma2");







	RemoveHistogram("DTGa1Ga2vsEGa2"); // pridala som tento riadok (Zdenka);
	fhDTGa1Ga2vsEGa2 = new TH2D ("DTGa1Ga2vsEGa2", "DT(Ga1-Ga2) vs Gamma2 Energy", 500,0,fCorrelPar->fGa2Th,2000,fCorrelPar->fGa2El,fCorrelPar->fGa2Eh); // pridala som tento riadok (Zdenka)
	AddHistogram(fhDTGa1Ga2vsEGa2, "Correlations/gamma2"); // pridala som tento riadok (Zdenka)




	// DON'T GET HISTOGRAMS FROM THE MEMORY OR AUTOSAVE FILE!!!


	// open file to save found correlation (if requested)
	// parameters from SHIP-correl.dat :
	// fCorrelPar->fbToFile - print correlation to file (fCorrelPar->fCorFile[200] - filename)
	if (fCorrelPar->fbToFile) {
		cout << "**** MoDSSCorrelProc: Opening output file for correlation: "
			<< fCorrelPar->fCorFile << endl;
		gPrtFile.open(fCorrelPar->fCorFile);
		if (!gPrtFile.is_open()) {
			cout << "**** MoDSSCorrelProc: Error opening file: " << fCorrelPar->fCorFile << endl;
			exit(0);
		}
	}
}

// **********************************************************
// default constructor - not used in program

MoDSSCorrelProc::MoDSSCorrelProc()
	: TGo4EventProcessor("MoDSSCorrelProc")
{
	cout << "MoDSSCorrelProc ---- DO NOT USE THIS CONSTRUCTOR !!!!\n";
}

// **********************************************************
// Default Destructor

MoDSSCorrelProc::~MoDSSCorrelProc()
{
	delete []fReTable;
	delete []fAlTable;
	delete []fGaTable;  
	cout << "**** MoDSSCorrelProc: Closing output file for correlation: "
		<< fCorrelPar->fCorFile << endl;
	gPrtFile.close();



}

// **********************************************************
// **********************************************************

// ==========================================================
// test if given position differences are within the position window.
// both positions and poswin are in the same units (mm or channels)

Bool_t MoDSSCorrelProc::TestPositions(Int_t IDX1, Int_t IDY1, Int_t IDX2, Int_t IDY2)
{
	if ((fCorrelPar->fbTopOrBot) && (((IDX1 == IDX2) && (IDX1 != 0)) || ((IDY1 == IDY2) && (IDY1 != 0)))){
		//std::cout<<"X: "<<IDX1<<" - Y: "<<IDY1<<std::endl;
		return kTRUE;
	}
	else if ((fCorrelPar->fbTopAndBot) && (((IDX1 == IDX2)||(IDX1 == IDX2+1)||(IDX1 == IDX2-1)) && ((IDY1 == IDY2)||(IDY1 == IDY2+1)||(IDY1 == IDY2-1))) && ((IDX1 != 0) && (IDY1 != 0))){
		//std::cout<<"X: "<<IDX1<<" - Y: "<<IDY1<<std::endl;
		return kTRUE;
	}
	else if ((fCorrelPar->fbOnlyTop) && (IDX1 == IDX2) && (IDX1 != 0)){
		//std::cout<<"X: "<<IDX1<<" - Y: "<<IDY1<<std::endl;
		return kTRUE;
	}
	else if ((fCorrelPar->fbOnlyBot) && (IDY1 == IDY2) && (IDY1 != 0)){ 
		//std::cout<<"X: "<<IDX1<<" - Y: "<<IDY1<<std::endl;
		return kTRUE;
	}
	else{ 
		//std::cout<<"alpha X: "<<IDX1<<" - Y: "<<IDY1<<" | Re X: "<<IDX2<<" - Y: "<<IDY2<<std::endl;
		return kFALSE;
	}
	//else return kTRUE; //blocked position testing
}

// ==========================================================
// GetPositionDifference() returns difference between pos1 and pos2 if both exist.
// alse returns 999



// ==========================================================
// TestRecoilLow
// returns true if recoil in low energy branch was requested and energy
// condition was fulfilled

Bool_t MoDSSCorrelProc::TestRecoilLow(MoDSSAnlEvent *re) 
{
	if (!(fCorrelPar->fbReELpoly) && (fCorrelPar->fbReEL) && (re->fE_kevX[re->fIx] > fCorrelPar->fReElLow) && (re->fE_kevX[re->fIx] < fCorrelPar->fReEhLow)) 
	{
		return kTRUE;
	}
	else if ((fCorrelPar->fbReELpoly) && (fCondEXvsTOF->Test(re->fTOF,re->fE_kevX[re->fIx])))
	{	
		return kTRUE;
	}
	else
		return kFALSE;
}

// ==========================================================
// TestRecoilHigh
// returns true if recoil in high energy branch was requested and energy
// condition was fulfilled

Bool_t MoDSSCorrelProc::TestRecoilHigh(MoDSSAnlEvent *re) 
{
	if (!(fCorrelPar->fbReEHpoly) && (fCorrelPar->fbReEH) && (re->fE_kevY[re->fIy] > fCorrelPar->fReElHigh) && (re->fE_kevY[re->fIy] < fCorrelPar->fReEhHigh)) 
	{
		//cout<<"E Re window ok |"<<fCorrelPar->fReElHigh<<" - "<<fCorrelPar->fReEhHigh<<endl;
		/*
		fhReTRH_True_Ex->Fill(re->fE_kevX[re->fIx]);
                fhReTRH_True_Ey->Fill(re->fE_kevY[re->fIy]);
                fhReTRH_True_X->Fill(re->fIDX[re->fIx]);
                fhReTRH_True_Y->Fill(re->fIDY[re->fIy]);
                fhReTRH_True_TOF->Fill(re->fTOF);

		fdReTRH_True_EyTOF->Fill(re->fTOF,re->fE_kevY[re->fIy]);*/
		return kTRUE;
	}
	else if ((fCorrelPar->fbReEHpoly) && (fCondEYvsTOF->Test(re->fTOF,re->fE_kevY[re->fIy])))
	{
		//cout<<"E Re poly ok"<<endl;
		/*fhReTRH_True_Ex->Fill(re->fE_kevX[re->fIx]);
                fhReTRH_True_Ey->Fill(re->fE_kevY[re->fIy]);
                fhReTRH_True_X->Fill(re->fIDX[re->fIx]);
                fhReTRH_True_Y->Fill(re->fIDY[re->fIy]);
                fhReTRH_True_TOF->Fill(re->fTOF);

		fdReTRH_True_EyTOF->Fill(re->fTOF,re->fE_kevY[re->fIy]);*/
		return kTRUE;
	}
	else{
		/*fhReTRH_False_Ex->Fill(re->fE_kevX[re->fIx]);
                fhReTRH_False_Ey->Fill(re->fE_kevY[re->fIy]);
                fhReTRH_False_X->Fill(re->fIDX[re->fIx]);
                fhReTRH_False_Y->Fill(re->fIDY[re->fIy]);
                fhReTRH_False_TOF->Fill(re->fTOF);	
		fdReTRH_False_EyTOF->Fill(re->fTOF,re->fE_kevY[re->fIy]);*/
		return kFALSE;
	}
}

// ==========================================================
// fill recoil spectra and set flag 'used' to 1

Int_t MoDSSCorrelProc::FillRecoilSpectra(MoDSSAnlEvent *re, MoDSSAnlEvent *nextMember) 
{
	//cout<<"Correl- FillRecoilSpectra"<<endl;

	// was the event already used ?
	//Question - in case of Re-al-al will be this filled when in the time difference
	//part also for the daughter alpha? And what will happen in case Re- gamma-alpha
	if (re->fUsed) return 0; 
	fhReTOF->Fill(re->fTOF);  // tof
	// -------- filling of low energy gammas in coinc with recoil
	//cout<<"Correl - Fill low energy gamma"<<endl;
	for(int i=0; i<20; i++){
		if (re->fegl[i] > 0) fhReLGaCoinc->Fill(re->fegl[i]);
	}


	//cout<<"Correl - Fill fIDX"<<endl;
	fhCountRe->Fill(re->fIDX[re->fIx]);  // counts for strips

	// ReGavsGa (Boris)
	/*if ((re->fegl1 > 0) && (re->fegl2 > 0)) 
	{
		fhReGavsGaCoinc->Fill(re->fegl1, re->fegl2);
		fhReGavsGaCoinc->Fill(re->fegl2, re->fegl1);
	}

	if ((re->fegl1 > 0) && (re->fegl3 > 0)) 
	{
		fhReGavsGaCoinc->Fill(re->fegl1, re->fegl3);
		fhReGavsGaCoinc->Fill(re->fegl3, re->fegl1);
	}

	if ((re->fegl1 > 0) && (re->fegl4 > 0)) 
	{
		fhReGavsGaCoinc->Fill(re->fegl1, re->fegl4);
		fhReGavsGaCoinc->Fill(re->fegl4, re->fegl1);
	}

	if ((re->fegl2 > 0) && (re->fegl3 > 0)) 
	{
		fhReGavsGaCoinc->Fill(re->fegl2, re->fegl3);
		fhReGavsGaCoinc->Fill(re->fegl3, re->fegl2);
	}

	if ((re->fegl2 > 0) && (re->fegl4 > 0)) 
	{
		fhReGavsGaCoinc->Fill(re->fegl2, re->fegl4);
		fhReGavsGaCoinc->Fill(re->fegl4, re->fegl2);
	}

	if ((re->fegl3 > 0) && (re->fegl4 > 0)) 
	{
		fhReGavsGaCoinc->Fill(re->fegl3, re->fegl4);
		fhReGavsGaCoinc->Fill(re->fegl4, re->fegl3);
	}*/

	// ReGavsTAC (Boris)
	//cout<<"Correl - Fill ReGa vs TAC"<<endl;
	if(fCorrelPar->fbReEL || fCorrelPar->fbReELpoly)
	{
		for(int i=0; i<20; i++){
			if (re->fegl[i] > 0){
				fhReGavsTAC->Fill(re->fegl[i], re->fTgamma[i]-re->fTimeX[re->fIx]);
			}
		}
	}
	if(fCorrelPar->fbReEH || fCorrelPar->fbReEHpoly)
	{
		for(int i=0; i<20; i++){
			if (re->fegl[i] > 0){
				fhReGavsTAC->Fill(re->fegl[i], re->fTgamma[i]-re->fTimeY[re->fIy]);
			}
		}
	}

	// ReGavsAlpha (Boris)

	//if(nextMember->fEventType == alpha)
	/*{
		for(int i=0; i<20; i++){
			if (re->fegl[i] > 0){
				fhReGavsAlpha->Fill(nextMember->fE_kevX[nextMember->fIx], re->fegl[i]);
			}
		}
	}*/



	// recoil 
	if (TestRecoilHigh(re)) {
		cout<<"FillRecoilSpectra - RecoilHigh valid"<<endl;
		fhEReX->Fill(re->fE_kevX[re->fIx]);  // energy
		fhTOFvsEReX->Fill(re->fTOF, re->fE_kevX[re->fIx]);   // tof vs. recoil energy
		fhEReY->Fill(re->fE_kevY[re->fIy]);   // energy
		fhTOFvsEReY->Fill(re->fTOF, re->fE_kevY[re->fIy]);   // tof vs. recoil energy
		// is the next chain member present ? if yes, fill dy and dt spectra and 2D energy spectra
		if (nextMember) {
			// next event is fission
			if (nextMember->fEventType == fission) {
				// 2D - recoil energy (low) vs fission energy
				fhEReXvsEFi->Fill(re->fE_kevX[re->fIx], nextMember->fE_kevY[nextMember->fIy]); 
				// dt re-fi
				fhDTReFi->Fill(nextMember->fTimeX[nextMember->fIx] - re->fTimeY[re->fIy]);
			} // if (nextMember->fEventType == fission) 

			// next member is an alpha
			if (nextMember->fEventType == alpha) {
				cout<<"FillRecoilSpectra - In Recoil-alpah"<<endl;
				// 2D - recoil energy (low) vs alpha energy
				fhEReXvsEAl->Fill(re->fE_kevX[re->fIx], nextMember->fE_kevX[nextMember->fIx]);
				fhEReYvsEAl->Fill(re->fE_kevY[re->fIy], nextMember->fE_kevX[nextMember->fIx]);
				fhReEAl->Fill(nextMember->fE_kevX[nextMember->fIx]);

				// dt re-al and 2D: dt(re-al) vs EAl
				if(!(fCorrelPar->fbEnXorY))
				{
					fhDTReAl->Fill(nextMember->fTimeX[nextMember->fIx] - re->fTimeX[re->fIx]);
					fhDTvsEAl->Fill(TMath::Log(nextMember->fTimeX[nextMember->fIx] - re->fTimeX[re->fIx])/TMath::Log(2), nextMember->fE_kevX[nextMember->fIx]);
				}
				else if(fCorrelPar->fbEnXorY && nextMember->fE_kevX[nextMember->fIx]>0)
				{
					fhDTReAl->Fill(nextMember->fTimeX[nextMember->fIx] - re->fTimeX[nextMember->fIx]);
					fhDTvsEAl->Fill(TMath::Log(nextMember->fTimeX[nextMember->fIx] - re->fTimeX[re->fIx])/TMath::Log(2), nextMember->fE_kevX[nextMember->fIx]);
				}
				else if(fCorrelPar->fbEnXorY && nextMember->fE_kevY[nextMember->fIy]>0)
				{
					fhDTReAl->Fill(nextMember->fTimeY[nextMember->fIy] - re->fTimeX[re->fIx]);
					fhDTvsEAl->Fill(TMath::Log(nextMember->fTimeX[nextMember->fIx] - re->fTimeX[re->fIx])/TMath::Log(2), nextMember->fE_kevY[nextMember->fIy]);
				}

			} // if (nextMember->fEventType == alpha) 
			// fill the dt(Re-gamma) spectrum
			if (nextMember->fEventType == evgamma) {
				for(int i=0; i<20; i++){
					if (nextMember->fegl[i] > 0)fhDTReGamma->Fill(nextMember->fTgamma[i] - re->fTimeX[re->fIx]);

					if (nextMember->fegl[i]){
						fhEGvsT->Fill(nextMember->fegl[i],(nextMember->fTgamma[i]-re->fTimeX[re->fIx]));
						fhEGvsT_RNDM->Fill(nextMember->fegl[i],(nextMember->fTgamma[i]-re->fTimeX[re->fIx]-fCorrelPar->fGaTl));
						fhEGvsT_Clover[0]->Fill(nextMember->fegl[i],(nextMember->fTgamma[i]-re->fTimeX[re->fIx]));
					}
				}
				// Search for the Re-Ga correlations in pulses which might be random
				// More effective would be to use div or mod functions
				for (Int_t PulseNumber = 0;PulseNumber<10;PulseNumber++) {
					if (((nextMember->fTimeX[nextMember->fIx]-re->fTimeX[re->fIx])>PulseNumber*20+10) && ((nextMember->fTimeX[nextMember->fIx]-re->fTimeX[re->fIx])<PulseNumber*20+30)){
						// filling "random" correlation position within a pulse
						fhDTReGammaBG->Fill((nextMember->fTimeX[nextMember->fIx] - re->fTimeX[re->fIx])-(PulseNumber+1)*20); 
						for(int i=0; i<20; i++){
							if (nextMember->fegl[i]) fhEGvsPulse->Fill(nextMember->fegl[i],(nextMember->fTimeX[nextMember->fIx]-re->fTimeX[re->fIx])-(PulseNumber+1)*20);
						}
						/*cout << "Filling with corr time" << nextMember->fTimeX[nextMember->fIx]-re->fTimeX[re->fIx]
						  << " the value " << (nextMember->fTimeX[nextMember->fIx]-re->fTimeX[re->fIx])-(PulseNumber+1)*20
						  << "and energies of " << nextMember->fegl1<< ", "<< nextMember->fegl2<<", "<< nextMember->fegl3
						  << ", "<<nextMember->fegl4<<"."<<endl;*/
					}
				}
			} //if (nextMember->fEventType == gamma) 
		} // if (nextMember) ...
	} // if (TestRecoilHigh(recoil)) ... 
//	cout<<"Correl - FillRecoilSpectra done"<<endl;
	re->fUsed = kTRUE;
	return 1;
}

// ==========================================================
// fill alpha spectra and set flag 'used' to 1

Int_t MoDSSCorrelProc::FillAlphaSpectra(MoDSSAnlEvent *al, MoDSSAnlEvent *nextMember, Int_t gen) 
{
	// was the event already used ?
	// One has to be careful here. Only the first correlation si considered for filling of spectra.
	// E.g. if there re two correlation with the same first gen. alpha but different
	// second gen. apha like Re-Al1_a-Al2_b and Re-Al1_a-Al2_c the latter one is not filled.
	// To avoid it there are spectra fhEAlMothervsEAlDaughter_AllChains and fhEAlMothervsEAlDaughter_BoxIncl
	// where even these events are filled. 
	// In spectra fhEAlMothervsEAlDaughter and fhEAlMothervsEAlDaughter only the first correlation
	// is filled.
	//cout<<"FillAlphaSpectra - is used ?"<<al->fUsed<<endl;

	if ((al->fUsed) && (nextMember) && !(nextMember->fUsed) && (nextMember->fEventType == alpha)) 
	{
		//cout<<"FillAlpha 1"<<endl;
		if(!(fCorrelPar->fbEnXorY))
			fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[al->fIx]);
		else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx] > 0 && nextMember->fE_kevX[nextMember->fIx] >0)
			fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
		else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx] > 0 && nextMember->fE_kevY[nextMember->fIy] >0)
			fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevY[nextMember->fIy]);
		else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy] > 0 && nextMember->fE_kevX[nextMember->fIx] >0)
			fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevY[al->fIy], nextMember->fE_kevX[nextMember->fIx]);
		else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy] > 0 && nextMember->fE_kevY[nextMember->fIy] >0)
			fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevY[al->fIy], nextMember->fE_kevY[nextMember->fIy]);


		if (fCorrelPar->fbBack)
		{ 
		//cout<<"FillAlpha 2"<<endl;
			//cout << "Values are: al->fE_kev " << al->fE_kev << " al->fEb_kev " << al->fEb_kev << " nextMember->fE_kev " << nextMember->fE_kev << " nextMember->fEb_kev " << nextMember->fEb_kev << endl;
			if ((al->fEb_kev[al->fIDB]) && (nextMember->fEb_kev[nextMember->fIDB]))
			{
				fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fEb_kev[al->fIDB], nextMember->fEb_kev[nextMember->fIDB]);
		//cout<<"FillAlpha 3"<<endl;
				//cout << "Filling al->fEb_kev " << al->fEb_kev << " and nextMember->fEb_kev " << nextMember->fEb_kev<<endl;
			}
			else if ((al->fEb_kev[al->fIDB]) && !(nextMember->fEb_kev[nextMember->fIDB]))
			{
				fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fEb_kev[al->fIDB], nextMember->fE_kevX[nextMember->fIx]); 
		//cout<<"FillAlpha 4"<<endl;
				//cout << "Filling al->fEb_kev " << al->fEb_kev << " and nextMember->fE_kev " << nextMember->fE_kev<<endl;
			}
			else if (!(al->fEb_kev[al->fIDB]) && (nextMember->fEb_kev[nextMember->fIDB]))
			{
				fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fEb_kev[nextMember->fIDB]); 
		//cout<<"FillAlpha 5"<<endl;
				//cout << "Filling al->fE_kev " << al->fE_kev << " and nextMember->fEb_kev " << nextMember->fEb_kev<<endl;
			}
			else if (!(al->fEb_kev[al->fIDB]) && !(nextMember->fEb_kev[nextMember->fIDB]))
			{
				fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
		//cout<<"FillAlpha 6"<<endl;
				//cout << "Filling al->fE_kev " << al->fE_kev << " and nextMember->fE_kev " << nextMember->fE_kev<<endl; 
			}
		} // if (fCorrelPar->fbBAck) ...
	} // if ((al->fUsed) && (nextMember) && !(nextMember->fUsed) ...

	if (al->fUsed) return 0;

	fhCountAl->Fill(al->fIx);  // counts for strips
	if(!(fCorrelPar->fbEnXorY))
	{
		fhEAlphaAll->Fill(al->fE_kevX[al->fIx]);  // energy
		//cout<<"FillAlpha 7"<<endl;
		fhEAlpha[gen-1]->Fill(al->fE_kevX[al->fIx]);  // energy for each generation separately

		//STOPvsBOX (Boris)
		if(al->fEb_kev[al->fIDB]>0)
			fhSTOPvsBOX->Fill(al->fE_kevX[al->fIx], al->fEb_kev[al->fIDB] - al->fE_kevX[al->fIx]);
	}
	else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0)
	{
		fhEAlphaAll->Fill(al->fE_kevX[al->fIx]);  // energy
		//cout<<"FillAlpha 8"<<endl;
		fhEAlpha[gen-1]->Fill(al->fE_kevX[al->fIx]);  // energy for each generation separately
		//STOPvsBOX (Boris)
		if(al->fEb_kev[al->fIDB]>0)
			fhSTOPvsBOX->Fill(al->fE_kevX[al->fIx], al->fEb_kev[al->fIDB] - al->fE_kevX[al->fIx]);
	}
	else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0)
	{
		fhEAlphaAll->Fill(al->fE_kevY[al->fIy]);  // energy
		//cout<<"FillAlpha 9"<<endl;
		fhEAlpha[gen-1]->Fill(al->fE_kevY[al->fIy]);  // energy for each generation separately
		//STOPvsBOX (Boris)
		if(al->fEb_kev[al->fIDB]>0)
			fhSTOPvsBOX->Fill(al->fE_kevY[al->fIy], al->fEb_kev[al->fIDB] - al->fE_kevY[al->fIy]);
	}


	if(!(fCorrelPar->fbEnXorY))
	{  
		// gamma coincident to alpha (for each generation separately)
		for(int i=0; i<20; i++){
			if (al->fegl[i] > 5.) 
			{
				fhGamAl[gen-1]->Fill(al->fegl[i]);
				if (gen == 1) fhEGvsEAl_1_corr->Fill(al->fE_kevX[al->fIx],al->fegl[i]);
			}
		}
	}	
	else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0)
	{  
		// gamma coincident to alpha (for each generation separately)
		for(int i=0; i<20; i++){
			if (al->fegl[i] > 5.) 
			{
				fhGamAl[gen-1]->Fill(al->fegl[i]);
				if (gen == 1) fhEGvsEAl_1_corr->Fill(al->fE_kevX[al->fIx],al->fegl[i]);
			}
		}
	}	
	else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0)
	{  
		// gamma coincident to alpha (for each generation separately)
		for(int i=0; i<20; i++){
			if (al->fegl[i] > 5.) 
			{
				fhGamAl[gen-1]->Fill(al->fegl[i]);
				if (gen == 1) fhEGvsEAl_1_corr->Fill(al->fE_kevY[al->fIy],al->fegl[i]);
			}
		}
	}  



	//STOPvsEG BOX gated (Boris)
	/*if((al->fEb_kev>0) && (al->fEb_kev<=1000)) {
	  if(al->fegl1 > 0)
	  fhSTOPvsEG_BOX->Fill(al->fE_kev, al->fegl1);	
	  if(al->fegl2 > 0)
	  fhSTOPvsEG_BOX->Fill(al->fE_kev, al->fegl2);		
	  if(al->fegl3 > 0)
	  fhSTOPvsEG_BOX->Fill(al->fE_kev, al->fegl3);		
	  if(al->fegl4 > 0)
	  fhSTOPvsEG_BOX->Fill(al->fE_kev, al->fegl4);	
	  }*/

	// is next chain member present ? if yes, fill dy and dt spectra and 2D energy spectra
	if (nextMember) {

		// next member is fission
		if (nextMember->fEventType == fission) {
			if(!(fCorrelPar->fbEnXorY))
			{ 
				// 2D - alpha energy vs fission energy
				fhEAlvsEFi->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevY[nextMember->fIy]);

				// dt al-fi 
				fhDTAlFi->Fill(nextMember->fTimeY[nextMember->fIy] - al->fTimeX[al->fIx]);
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0)
			{ 
				// 2D - alpha energy vs fission energy
				fhEAlvsEFi->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevY[nextMember->fIy]);

				// dt al-fi 
				fhDTAlFi->Fill(nextMember->fTimeY[nextMember->fIy] - al->fTimeX[al->fIx]);
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0)
			{ 
				// 2D - alpha energy vs fission energy
				fhEAlvsEFi->Fill(al->fE_kevY[al->fIy], nextMember->fE_kevY[nextMember->fIy]);

				// dt al-fi 
				fhDTAlFi->Fill(nextMember->fTimeY[nextMember->fIy] - al->fTimeY[al->fIy]);
			}        

		} // if (nextMember->fEventType == fission) ...

		if (nextMember->fEventType == evgamma) {
			if(!(fCorrelPar->fbEnXorY)) fhDTAlGamma->Fill(nextMember->fTimeTrig - al->fTimeX[al->fIx]);
			else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0) fhDTAlGamma->Fill(nextMember->fTimeTrig - al->fTimeX[al->fIx]);
			else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0) fhDTAlGamma->Fill(nextMember->fTimeTrig - al->fTimeY[al->fIy]);
			// This is for filling alpha-gamma delayed spectra when alpha is a mother
			if ((fCorrelPar->fbBack) && (al->fEb_kev[al->fIDB] > 10)){
				for(int i=0; i<20; i++)
					if (nextMember->fegl[i] > 0) fhEAlEGadelayed->Fill(al->fEb_kev[al->fIDB],nextMember->fegl[i]);
			}
			else if(!(fCorrelPar->fbEnXorY)) 
			{
				for(int i=0; i<20; i++)
					if (nextMember->fegl[i] > 0) fhEAlEGadelayed->Fill(al->fE_kevX[al->fIx],nextMember->fegl[i]);
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0)
			{
				for(int i=0; i<20; i++)
					if (nextMember->fegl[i] > 0) fhEAlEGadelayed->Fill(al->fE_kevX[al->fIx],nextMember->fegl[i]);
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0)
			{
				for(int i=0; i<20; i++)
					if (nextMember->fegl[i] > 0) fhEAlEGadelayed->Fill(al->fE_kevY[al->fIy],nextMember->fegl[i]);
			}
		}

		// next member is an alpha
		if (nextMember->fEventType == alpha) {
			if(!(fCorrelPar->fbEnXorY)) 
			{
				// 2D energy - alpha mother vs alpha daughter
				fhEAlMothervsEAlDaughter->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
				fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
				// dt al-al - for each generation (starting from 2nd. 1st = re-al) 
				fhDTAlAl[gen]->Fill(nextMember->fTimeX[nextMember->fIx] - al->fTimeX[al->fIx]);

				fhDTAl1Al2vsEAl2->Fill(nextMember->fTimeX[nextMember->fIx] - al->fTimeX[al->fIx], nextMember->fE_kevX[nextMember->fIx]); // pridala som tento riadok (Zdenka)
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0 && nextMember->fE_kevX[nextMember->fIx]>0)
			{
				// 2D energy - alpha mother vs alpha daughter
				fhEAlMothervsEAlDaughter->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
				fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
				// dt al-al - for each generation (starting from 2nd. 1st = re-al) 
				fhDTAlAl[gen]->Fill(nextMember->fTimeX[nextMember->fIx] - al->fTimeX[al->fIx]);

				fhDTAl1Al2vsEAl2->Fill(nextMember->fTimeX[nextMember->fIx] - al->fTimeX[al->fIx], nextMember->fE_kevX[nextMember->fIx]); // pridala som tento riadok (Zdenka)
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0 && nextMember->fE_kevX[nextMember->fIx]>0)
			{
				// 2D energy - alpha mother vs alpha daughter
				fhEAlMothervsEAlDaughter->Fill(al->fE_kevY[al->fIy], nextMember->fE_kevX[nextMember->fIx]);
				fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevY[al->fIy], nextMember->fE_kevX[nextMember->fIx]);
				// dt al-al - for each generation (starting from 2nd. 1st = re-al) 
				fhDTAlAl[gen]->Fill(nextMember->fTimeX[nextMember->fIx] - al->fTimeY[al->fIy]);

				fhDTAl1Al2vsEAl2->Fill(nextMember->fTimeX[nextMember->fIx] - al->fTimeY[al->fIy], nextMember->fE_kevX[nextMember->fIx]); // pridala som tento riadok (Zdenka)
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0 && nextMember->fE_kevY[nextMember->fIy]>0)
			{
				// 2D energy - alpha mother vs alpha daughter
				fhEAlMothervsEAlDaughter->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevY[nextMember->fIy]);
				fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevY[nextMember->fIy]);
				// dt al-al - for each generation (starting from 2nd. 1st = re-al) 
				fhDTAlAl[gen]->Fill(nextMember->fTimeY[nextMember->fIy] - al->fTimeX[al->fIx]);

				fhDTAl1Al2vsEAl2->Fill(nextMember->fTimeY[nextMember->fIy] - al->fTimeX[al->fIx], nextMember->fE_kevY[nextMember->fIy]); // pridala som tento riadok (Zdenka)
			}
			else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0 && nextMember->fE_kevY[nextMember->fIy]>0)
			{
				// 2D energy - alpha mother vs alpha daughter
				fhEAlMothervsEAlDaughter->Fill(al->fE_kevY[al->fIy], nextMember->fE_kevY[nextMember->fIy]);
				fhEAlMothervsEAlDaughter_AllChains->Fill(al->fE_kevY[al->fIy], nextMember->fE_kevY[nextMember->fIy]);
				// dt al-al - for each generation (starting from 2nd. 1st = re-al) 
				fhDTAlAl[gen]->Fill(nextMember->fTimeY[nextMember->fIy] - al->fTimeY[al->fIy]);

				fhDTAl1Al2vsEAl2->Fill(nextMember->fTimeY[nextMember->fIy] - al->fTimeY[al->fIy], nextMember->fE_kevY[nextMember->fIy]); // pridala som tento riadok (Zdenka)
			}

			if (fCorrelPar->fbBack)
			{ 
				//cout << "Values are: al->fE_kev " << al->fE_kev << " al->fEb_kev " << al->fEb_kev << " nextMember->fE_kev " << nextMember->fE_kev << " nextMember->fEb_kev " << nextMember->fEb_kev << endl;
				if ((al->fEb_kev[al->fIDB]) && (nextMember->fEb_kev[nextMember->fIDB]))
				{
					fhEAlMothervsEAlDaughter_BoxIncl->Fill(al->fEb_kev[al->fIDB], nextMember->fEb_kev[nextMember->fIDB]);
					fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fEb_kev[al->fIDB], nextMember->fEb_kev[nextMember->fIDB]);
					//cout << "Filling al->fEb_kev " << al->fEb_kev << " and nextMember->fEb_kev " << nextMember->fEb_kev<<endl;
				}
				else if ((al->fEb_kev[al->fIDB]) && !(nextMember->fEb_kev[nextMember->fIDB]))
				{
					fhEAlMothervsEAlDaughter_BoxIncl->Fill(al->fEb_kev[al->fIDB], nextMember->fE_kevX[nextMember->fIx]); 
					fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fEb_kev[al->fIDB], nextMember->fE_kevX[nextMember->fIx]);
					//cout << "Filling al->fEb_kev " << al->fEb_kev << " and nextMember->fE_kev " << nextMember->fE_kev<<endl;
				}
				else if (!(al->fEb_kev[al->fIDB]) && (nextMember->fEb_kev[nextMember->fIDB]))
				{
					fhEAlMothervsEAlDaughter_BoxIncl->Fill(al->fE_kevX[al->fIx], nextMember->fEb_kev[nextMember->fIDB]); 
					fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fEb_kev[nextMember->fIDB]);
					//cout << "Filling al->fE_kev " << al->fE_kev << " and nextMember->fEb_kev " << nextMember->fEb_kev<<endl;
				}
				else if (!(al->fEb_kev[al->fIDB]) && !(nextMember->fEb_kev[nextMember->fIDB]))
				{
					fhEAlMothervsEAlDaughter_BoxIncl->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
					fhEAlMothervsEAlDaughter_BoxIncl_AllChains->Fill(al->fE_kevX[al->fIx], nextMember->fE_kevX[nextMember->fIx]);
					//cout << "Filling al->fE_kev " << al->fE_kev << " and nextMember->fE_kev " << nextMember->fE_kev<<endl; 
				}
			} // if (fCorrelPar->fbBAck)



		} // if (nextMember->fEventType == alpha) 

	} // if (nextMember) ...

	al->fUsed = kTRUE;
	return 1;
}

// ==========================================================
// fill fission spectra and set flag 'used' to 1

Int_t MoDSSCorrelProc::FillFissionSpectra(MoDSSAnlEvent *fi) 
{
	// was the event already used ?
	if (fi->fUsed) return 0;

	fhEFissionStopDet->Fill(fi->fE_kevY[fi->fIy]);  // energy
	if (fi->fEb_kev[fi->fIDB] == 0) fhEFissionStopDetOnly->Fill(fi->fE_kevY[fi->fIy]);
	if (fi->fEb_kev[fi->fIDB]) fhEFissionStopBoxDet->Fill(fi->fEb_kev[fi->fIDB]);
	// all other spectra are filled in FillRecoilSpectra() and FillAlphaSpectra{}

	fi->fUsed = kTRUE;
	return 1;
}

Int_t MoDSSCorrelProc::FillGammaSpectra(MoDSSAnlEvent *gam, MoDSSAnlEvent *nextMember) 
{
	// was the event already used ?
	if (gam->fUsed) return 0;

	for(int i=0; i<20; i++){
		// Fill single spectra low energy gammas energy
		if (gam->fegl[i]) 
		{
			fhEGamma->Fill(gam->fegl[i]); 
			fhEGammaClo[i]->Fill(gam->fegl[i]);
		}

		// Fill single spectra for low energy gammas only from crystals 1,2 and 4 separately (due to the bad resolution of cyrstal 3)
		//if (gam->fegl[i]) fhEGammaBestRes->Fill(gam->fegl[i]);  // energy

		// Fill single spectra high energy gammas
		/* if ((gam->fegl[i]) && (gam->fTgamma[0]>0)) fhEGvsTAC->Fill(gam->fegl[i],gam->fTgamma[0]);  // energy
		   if ((gam->fegl2) && (gam->fTgamma[1]>0)) fhEGvsTAC->Fill(gam->fegl2,gam->fTgamma[1]);
		   if ((gam->fegl3) && (gam->fTgamma[2]>0)) fhEGvsTAC->Fill(gam->fegl3,gam->fTgamma[2]);
		   if ((gam->fegl4) && (gam->fTgamma[3]>0)) fhEGvsTAC->Fill(gam->fegl4,gam->fTgamma[3]);*/	

		// Fill electron - gamma1 coincidence spectra
		if ((gam->fegl[i]) && (gam->fE_kevX[gam->fIx])) fhElGa1Coinc->Fill(gam->fegl[i], gam->fE_kevX[gam->fIx]);

		// Fill electron - gamma 1 coincidence spectra for crystals with best resolution separately (crystals 1,2,4)
		//if ((gam->fegl[i]) && (gam->fE_kevX[gam->fIx])) fhElGa1CoincBestRes->Fill(gam->fegl[i], gam->fE_kevX[gam->fIx]);

		//multiplicity evaluation
		/*int dummi=0;
		  if (gam->fegl[i]) dummi++;
		  if (gam->fegl2) dummi++;
		  if (gam->fegl3) dummi++;
		  if (gam->fegl4) dummi++;
		// Filling of the gammas from one crystal in coincidence to sum of the gammas fromtwo another crystals
		// when 3 crystals gave a signal to cover the possibilities when one gamma is scatered and registred by
		// two another crystals
		if (dummi==3){
		if ((gam->fegl[i])  && (gam->fegl2) && (gam->fegl3)){
		fhEGCoincVsGaGaSum->Fill(gam->fegl[i]+gam->fegl2,gam->fegl3);
		fhEGCoincVsGaGaSum->Fill(gam->fegl2+gam->fegl3,gam->fegl[i]);
		fhEGCoincVsGaGaSum->Fill(gam->fegl[i]+gam->fegl3,gam->fegl2);
		}
		if ((gam->fegl[i])  && (gam->fegl2) && (gam->fegl4)){
		fhEGCoincVsGaGaSum->Fill(gam->fegl[i]+gam->fegl2,gam->fegl4);
		fhEGCoincVsGaGaSum->Fill(gam->fegl2+gam->fegl4,gam->fegl[i]);
		fhEGCoincVsGaGaSum->Fill(gam->fegl[i]+gam->fegl4,gam->fegl2);
		}
		if ((gam->fegl[i])  && (gam->fegl4) && (gam->fegl3)){
		fhEGCoincVsGaGaSum->Fill(gam->fegl[i]+gam->fegl4,gam->fegl3);
		fhEGCoincVsGaGaSum->Fill(gam->fegl4+gam->fegl3,gam->fegl[i]);
		fhEGCoincVsGaGaSum->Fill(gam->fegl[i]+gam->fegl3,gam->fegl4);
		}
		if ((gam->fegl4)  && (gam->fegl2) && (gam->fegl3)){
		fhEGCoincVsGaGaSum->Fill(gam->fegl4+gam->fegl2,gam->fegl3);
		fhEGCoincVsGaGaSum->Fill(gam->fegl2+gam->fegl3,gam->fegl4);
		fhEGCoincVsGaGaSum->Fill(gam->fegl4+gam->fegl3,gam->fegl2);
		}
		}*/
	}


	// Fill coincidence gamma gamma matrix

	/*if ((gam->fegl[i]!=0) && (gam->fegl2!=0)){
	  fhEGCoinc2D->Fill(gam->fegl[i],gam->fegl2);
	  fhEGCoinc2D->Fill(gam->fegl2,gam->fegl[i]);
	  }

	  if ((gam->fegl[i]!=0) && (gam->fegl3!=0)){
	  fhEGCoinc2D->Fill(gam->fegl[i],gam->fegl3);
	  fhEGCoinc2D->Fill(gam->fegl3,gam->fegl[i]);
	  }

	  if ((gam->fegl[i]!=0) && (gam->fegl4!=0)){
	  fhEGCoinc2D->Fill(gam->fegl[i],gam->fegl4);
	  fhEGCoinc2D->Fill(gam->fegl4,gam->fegl[i]);
	  }

	  if ((gam->fegl2!=0) && (gam->fegl3!=0)){
	  fhEGCoinc2D->Fill(gam->fegl2,gam->fegl3);
	  fhEGCoinc2D->Fill(gam->fegl3,gam->fegl2);
	  }

	  if ((gam->fegl2!=0) && (gam->fegl4!=0)){
	  fhEGCoinc2D->Fill(gam->fegl2,gam->fegl4);
	  fhEGCoinc2D->Fill(gam->fegl4,gam->fegl2);
	  }

	  if ((gam->fegl3!=0) && (gam->fegl4!=0)){
	  fhEGCoinc2D->Fill(gam->fegl3,gam->fegl4);
	  fhEGCoinc2D->Fill(gam->fegl4,gam->fegl3);
	  }*/

	// all other spectra are filled in FillRecoilSpectra() and FillAlphaSpectra{}
	// This is for gamma-alpha correlations (gamma as a mother!!!) however 
	// the setting of the histograms is the same as in the case of alpha gamma correlations
	if (nextMember){
		if ((nextMember->fEventType == alpha) && (fCorrelPar->fbCorGaAl)) {
			if ((fCorrelPar->fbBack) && (nextMember->fEb_kev[nextMember->fIDB]> 10)){
				for(int i=0; i<20; i++)
					if (gam->fegl[i] > 0) fhEGaEAldelayed->Fill(nextMember->fEb_kev[nextMember->fIDB],gam->fegl[i]);
			}
			else {
				for(int i=0; i<20; i++)
					if (gam->fegl[i] > 0) fhEGaEAldelayed->Fill(nextMember->fE_kevX[nextMember->fIx],gam->fegl[i]);
			}
		}

		// Case of Gamma - Gamma correlation
		if ((nextMember->fEventType == evgamma2) && (fCorrelPar->fbCorGaGa)) {

			// Fill gamma 2 - gamma 1 correlation spectra
			// mother gamma is on X axis, daughter on Y axis		
			for(int i=0; i<20; i++){
				for(int j=0; j<20; j++){
					if(gam->fegl[i]!=0 && nextMember->fegl[j]!=0)
						fhEG2EG1corr->Fill(gam->fegl[i],nextMember->fegl[j]);
				}
			}

			// Fill time difference of the mother and daughter gamma
			fhDTGa1Ga2->Fill(nextMember->fTimeTrig - gam->fTimeTrig);
			for(int i=0; i<20; i++){
				if(nextMember->fegl[i]!=0)
					fhDTGa1Ga2vsEGa2->Fill(nextMember->fTimeTrig - gam->fTimeTrig, nextMember->fegl[i]);  
			}

		} // if ((nextMember->fEventType == evgamma2) && (fCorrelPar->fbCorGaGa))
	}

	gam->fUsed = kTRUE;
	return 1;
}

Int_t MoDSSCorrelProc::FillGamma2Spectra(MoDSSAnlEvent *gam2) 
{
	// was the event already used ?
	if (gam2->fUsed) return 0;

	// Fill single spectra low energy gammas
	for(int i=0; i<20; i++){
		if (gam2->fegl[i]) fhEGamma2->Fill(gam2->fegl[i]);

		// Fill electron - gamma1 coincidence spectra
		if ((gam2->fegl[i]) && (gam2->fE_kevX[gam2->fIx])) fhElGa2Coinc->Fill(gam2->fE_kevX[gam2->fIx],gam2->fegl[i]);
	}

	// Fill gamma - gamma coinc matrix
	for(int i=0; i<20; i++){
		for(int j=i; j<20; j++){
			if(gam2->fegl[i]!=0 && gam2->fegl[j]!=0)
				fhEG2EG1corr->Fill(gam2->fegl[i],gam2->fegl[j]);
		}
	}
	gam2->fUsed = kTRUE;
	return 1;
}


// ==========================================================
// print chain 'Chain' and call histogram filling function
// 
// parameters from MoDSS-correl.dat :
// fCorrelPar->fbToScr  - print correlation to screen
// fCorrelPar->fbToFile - print correlation to file 
// global variable 'ofstream gPrtFile' is opened in the constructor
//
// here also histograms are filled 

void MoDSSCorrelProc::PrintThisChain(MoDSSAnlEvent *chain[MAXALPHA+1+1]) 
{
	Int_t i;					//4
	Int_t previous = 0;				//4
	Bool_t first = kTRUE;				//1
	static Int_t chain_no = 0;			//4

	// clear chain number if a new analysis have been submitted
	if (fbNewSubmit) {
		chain_no = 0;
		fbNewSubmit = kFALSE;
	}
	chain_no++;
	// ----- header -----
	// printing on screen activated ?
	if (fCorrelPar->fbToScr) {
		cout << endl;
		cout << "no.\tgen \t      timeX/s\t    dt/ms \t\t         energyX\tenergyY\tIDX\tIDY\tTOF/B_pulse\t Ga1\t Ga2\t Ga3\t Ga4\n";
	}
	// printing to file activated ? (print this only once)
	if ((fCorrelPar->fbToFile) && (chain_no == 1)) {
		gPrtFile << endl;
		gPrtFile << "no.\tgen \t timeX/ms \t pausebit \tdt/ms \t ekevX \t" 
			<< "   IDX     IDY \t  ekevY \t"
			<< "   tof \t tofbit\tboxdet\t"
			<< "  ebkev   egl1 \t  egl2 \t  egl3 \t  egl4 \tdtgx1/ms\tdtgx2/ms\tdtgx3/ms\tdtgx4/ms \n";
	}
	cout.flags(ios::fixed); // now 'precision' means number of digits after dot

	// loop over all posible generations
	for (i = 0; i < MAXALPHA+1; i++) {
		if (chain[i]) { // generation i exists

			// ----- FILL SPECTRA FOR EACH EVENT -----
			// 8.9.2014 Change (SA): the specific event type should be tested for recoil
			if (/*(i == 0)*/ chain[i]->fEventType == recoil){ 
				//cout<<"PrintThisChain - Calling FillRecoilSpectra"<<endl;
				//cout << "PrintThisChain: My event type is " << chain[i]->fEventType <<" num "<<chain[i]->fEventNum << " at time "<<chain[i]->fTimeX[chain[i]->fIx] <<" and filling chain on position "<<i<<"."<<endl;
				FillRecoilSpectra(chain[i], chain[i+1]);
			}
			else if (chain[i]->fEventType == alpha) {
				//cout<<"PrintThisChain - Calling FillAlphaSpectra"<<endl;
				//cout << "PrintThisChain: My event type is " << chain[i]->fEventType <<" num "<<chain[i]->fEventNum << " at time "<<chain[i]->fTimeX[chain[i]->fIx] <<" and filling chain on position "<<i<<"."<<endl;
				FillAlphaSpectra(chain[i], chain[i+1], i);
			}
			else if (chain[i]->fEventType == evgamma) 
				FillGammaSpectra(chain[i], chain[i+1]);
			else if (chain[i]->fEventType == evgamma2)
				FillGamma2Spectra(chain[i]);
			/*else if (chain[i]->fEventType == electron) 
			  FillElectronSpectra(chain[i]);*/
			else 
				FillFissionSpectra(chain[i]);

			// ----- PRINT EVENT -----
			// ----- first chain member -----
			if (first) { // print absolute positions for the first chain member
				first = kFALSE;

				// fill time spectrum (number of correlations in time)
				fhCorTIME->Fill(chain[i]->fTimeX[chain[i]->fIx]/10000.);  // time is in miliseconds -> 10s

				// print event to screen:
				if (fCorrelPar->fbToScr) {
					cout << chain_no << "\t";
					cout << std::setw(1) << i;
					if (chain[i]->fEventType == recoil) /*(i == 0)*/ cout << " (Re)\t"; // recoil
					else if (chain[i]->fEventType == evgamma) cout << " (Ga)\t"; // gamma
					else cout << " (Al)\t"; //alpha
					cout.precision(3);
					cout.width(14);
					cout << chain[i]->fTimeX[chain[i]->fIx]/1000. << "\t" << std::setw(14) << "--" << " \t\t";


					// first event is recoil: 2 posiblities - recoil in low or high branch (or both)
					if (chain[i]->fEventType == recoil) /* (i == 0)*/ {
						//if (TestRecoilLow(chain[i]))  // low energy branch
						cout.precision(1);
						cout << std::setw(12) << chain[i]->fE_kevX[chain[i]->fIx] << " \t" << chain[i]->fE_kevY[chain[i]->fIy] << " \t"
							<< std::setw(2) << chain[i]->fIDX[chain[i]->fIx] << "\t"
							<< std::setw(2) << chain[i]->fIDY[chain[i]->fIy] << "\t";

						cout << std::setw(12) << chain[i]->fTOF << " \t\t"; 
						cout.precision(1); 
						cout << std::setw(6) ;
						for(int j=0; j<20; j++){
							cout<<chain[i]->fegl[j] << std::setw(6);
						} 
						cout<< endl;
					} // if (i == 0) ...
					// first event is an alpha
					else {
						cout.precision(1);
						// escape alpha + box det.
						if ((fCorrelPar->fbBack) && (chain[i]->fEb_kev[chain[i]->fIDB] > 0))
							cout << "(b)" << chain[i]->fEb_kev[chain[i]->fIDB] << " \t";
						else  // normal alpha
							cout << "   " << std::setw(12) << chain[i]->fE_kevX[chain[i]->fIx] << " \t" << chain[i]->fE_kevY[chain[i]->fIy] << " \t";
						cout   << std::setw(2) << chain[i]->fIDX[chain[i]->fIx] << "\t"
							<< std::setw(2) << chain[i]->fIDY[chain[i]->fIy] << "\t";

						cout.precision(1); 
						for(int j=0; j<20; j++){
							cout<<chain[i]->fegl[j]<<"\t";
						} 
						cout << endl;
					}
				} // if (fCorrelPar->fbToScr) ...
				// print all info about event into file (no mather if it is alpha or recoil)
				if (fCorrelPar->fbToFile) {
					gPrtFile.flags(ios::fixed);
					gPrtFile << chain_no << "\t";
					gPrtFile << std::setw(1) << i;
					if (chain[i]->fEventType == recoil) /*(i == 0)*/ gPrtFile << " (Re)\t"; // recoil
					else if (chain[i]->fEventType == evgamma) gPrtFile << " (Ga)\t"; //gamma
					else gPrtFile << " (Al)\t"; //alpha
					gPrtFile.precision(3);
					gPrtFile << std::setw(11) << chain[i]->fTimeX[chain[i]->fIx] << "\t"
						<<"        -     \t" 
						<< std::setprecision(1) << std::setw(6) << chain[i]->fE_kevX[chain[i]->fIx] << "\t";
					gPrtFile.precision(2);
					gPrtFile << std::setw(5) << (chain[i]->fIDX[chain[i]->fIx]) << "\t"
						<< std::setw(5) << (chain[i]->fIDY[chain[i]->fIy]) << "\t";

					gPrtFile << std::setw(8) << chain[i]->fE_kevY[chain[i]->fIy] << "\t";

					gPrtFile.precision(1);
					gPrtFile << std::setw(6) << chain[i]->fTOF << "\t" 
						<< std::setw(4) << chain[i]->fB_tof << "\t" 
						//<< std::setw(4) << chain[i]->fB_hbg << "\t"
						<< std::setw(4) << chain[i]->fIDB << "\t"
						<< std::setw(6) << chain[i]->fEb_kev[chain[i]->fIDB] << "\t";
						//<< std::setw(6) << chain[i]->fEb_mev << "\t"
						//<< std::setw(4) << chain[i]->fB_veto << "\t"
						for(int j=0; j<20; j++){
							cout<<std::setw(6) << chain[i]->fegl[j]<<"\t";
						} 
					gPrtFile.precision(4);
					for(int j=0; j<20; j++){
						if(chain[i]->fegl[j]>0)              
							gPrtFile << std::setw(8) << (chain[i]->fTgamma[j]-chain[i]->fTimeX[chain[i]->fIx]) << "\t";
						else
							gPrtFile << std::setw(8) << " - " << "\t";
					} 
					if (fMBSfile) gPrtFile << "\t" << fMBSfile->GetCurrentFileName() << endl;
					else gPrtFile << endl;




					// For 1 line output please comment out
					// if (fMBSfile)... and else gPrtFile... 

				} // if (fCorrelPar->fbToFile) ...
			} // if (first) ... 
			// ----- other chain members -----
			// -> print also relative positions and time
			else {
				// print to screen:
				if (fCorrelPar->fbToScr) {
					cout << chain_no << "\t";
					cout << std::setw(1) << i;
					if (chain[i]->fEventType == alpha) 
						cout << " (Al)\t"; // alpha
					else if (chain[i]->fEventType == evgamma) 
						cout << " (Ga)\t"; // gamma
					else if (chain[i]->fEventType == evgamma2)
						cout << " (G2)\t"; // gamma2
					//else if (chain[i]->fEventType == electron)
					//  cout << " (El)\t"; // electron
					else 
						cout << " (Fi)\t"; // fission
					cout.precision(3);
					cout << std::setw(14) <<chain[i]->fTimeX[chain[i]->fIx]/1000. << "  \t"
						// time difference
						<< std::setw(14) << chain[i]->fTimeX[chain[i]->fIx] - chain[previous]->fTimeX[chain[i]->fIx] << "\t";
					// energy
					cout.precision(1);
					// alpha or fission ?
					if (chain[i]->fEventType == alpha) {
						// escape alpha + box det.
						if ((fCorrelPar->fbBack) && (chain[i]->fEb_kev[chain[i]->fIDB] > 0))
							cout << "(b)" << chain[i]->fEb_kev[chain[i]->fIDB] << " \t";
						else  // normal alpha
							cout << std::setw(12) << chain[i]->fE_kevX[chain[i]->fIx] << " \t" << chain[i]->fE_kevY[chain[i]->fIy] << " \t";
						// detector number
						cout << std::setw(2) << chain[i]->fIDX[chain[i]->fIx] << "\t";
						cout << std::setw(2) << chain[i]->fIDY[chain[i]->fIy] << "\t";
						cout.precision(1); 
						for(int j=0; j<20; j++){
							cout<<chain[i]->fegl[j]<<"\t";
						} 
						cout << endl;


					} // if (chain[i] ...
					else if (chain[i]->fEventType == evgamma){
						if (chain[i]->fEb_kev[chain[i]->fIDB] > 0) 
							cout << "(b)" << chain[i]->fEb_kev[chain[i]->fIDB] << " \t";
						else  // normal alpha     
							cout << "   " << std::setw(12) << chain[i]->fE_kevX[chain[i]->fIx] << " \t" << chain[i]->fE_kevY[chain[i]->fIy] << " \t";

						cout << std::setw(2) << chain[i]->fIDX[chain[i]->fIx] << "\t";
						cout << std::setw(2) << chain[i]->fIDY[chain[i]->fIy] << "\t";
						cout.precision(1); 
						for(int j=0; j<20; j++){
							cout<<chain[i]->fegl[j]<<"\t";
						} 
						cout << endl;
					}
					else if (chain[i]->fEventType == evgamma2){
						if (chain[i]->fEb_kev[chain[i]->fIDB] > 0) 
							cout << "(b)" << chain[i]->fEb_kev[chain[i]->fIDB] << " \t";
						else  // normal alpha     
							cout << "   " << std::setw(12) << chain[i]->fE_kevX[chain[i]->fIx] << " \t" << chain[i]->fE_kevY[chain[i]->fIy] << " \t";

						cout << std::setw(2) << chain[i]->fIDX[chain[i]->fIx] << "\t";
						cout << std::setw(2) << chain[i]->fIDY[chain[i]->fIy] << "\t";
						cout.precision(1); 
						for(int j=0; j<20; j++){
							cout<<chain[i]->fegl[j]<<"\t";
						} 
						cout << endl;
					}

					else {  // fission
						cout << "   " << std::setw(12) << chain[i]->fE_kevX[chain[i]->fIx] << " \t" << chain[i]->fE_kevY[chain[i]->fIy] << " \t"
							<< std::setw(2) << chain[i]->fIDX[chain[i]->fIx] << "\t"<< std::setw(2) << chain[i]->fIDY[chain[i]->fIy] << "\t";
							// position difference
						cout.precision(1); 
						for(int j=0; j<20; j++){
							cout<<chain[i]->fegl[j]<<"\t";
						} 
						cout << endl;

						// if (chain[previous]-> ...




					}
					cout.precision(1);
				} // if (fCorrelPar->fbToScr) ...
				// print all info about event into file (if requested)
				if (fCorrelPar->fbToFile) {
					// gPrtFile << "\t"; // remove comment for one line output.
					gPrtFile << chain_no << "\t";
					gPrtFile << std::setw(1) << i;
					if (chain[i]->fEventType == alpha) gPrtFile << " (Al)\t"; // alpha
					else if (chain[i]->fEventType == evgamma) gPrtFile << " (G)\t"; //gamma1
					else if (chain[i]->fEventType == evgamma2) gPrtFile << " (G2)\t"; //gamma2
					else gPrtFile << " (Fi)\t"; // fission
					gPrtFile.precision(3);
					gPrtFile << std::setw(11) << chain[i]->fTimeX[chain[i]->fIx] << "\t" 
						// time difference
						<< std::setw(13) << chain[i]->fTimeX[chain[i]->fIx] - chain[previous]->fTimeX[chain[i]->fIx] << "\t" 
						<< std::setprecision(1) << std::setw(6) << chain[i]->fE_kevX[chain[i]->fIx] << "\t";
					gPrtFile.precision(2);
					gPrtFile << std::setw(5) << (chain[i]->fIDX[chain[i]->fIx]) << "\t"
						<< std::setw(5) << (chain[i]->fIDY[chain[i]->fIy]) << "\t";  
					gPrtFile << std::setw(8) << chain[i]->fE_kevY[chain[i]->fIy] << "\t";                                     

					gPrtFile.precision(1);
					gPrtFile << std::setw(6) << chain[i]->fTOF << "\t" 
						<< std::setw(4) << chain[i]->fB_tof << "\t" 
						<< std::setw(4) << chain[i]->fIDB << "\t"
						<< std::setw(6) << chain[i]->fEb_kev[chain[i]->fIDB] << "\t" ;
						// << std::setw(6) << chain[i]->fEb_mev << "\t"
						//<< std::setw(4) << chain[i]->fB_veto << "\t"
						for(int j=0; j<20; j++){
							cout<<std::setw(6) << chain[i]->fegl[j]<<"\t";
						} 
					gPrtFile.precision(4);     
					for(int j=0; j<20; j++){
						if(chain[i]->fegl[j]>0)              
							gPrtFile << std::setw(8) << (chain[i]->fTgamma[j]-chain[i]->fTimeX[chain[i]->fIx]) << "\t";
						else
							gPrtFile << std::setw(8) << " - " << "\t";
					} 
					
				}// if (fCorrelPar->fbToFile) ...
			} // else
			previous = i;
		} // if (chain[i]) ...
	} // for ...
	if (fCorrelPar->fbToFile) gPrtFile << endl;
}

// ==========================================================
// Print found correlation. 
// RECURSIVE function !!!
// Prints all chains ending with 'event' and having "enough" members.
// Returns 1 if the last subchain was printed.
// Very complicated function, one should draw all posibilities to the paper to
// understand what's going on...
//
// event - current daughter event
// gen_last - generation from which the printing starts (last gen in the printed chain)
//          - when called recursively, it is the current event generation
//     - in Re-Fi gen_prt = 1
//     - in Al-Fi gen_prt = fNumAl + 1
// chnum - the number of chain members
// gen_to - important in the case when some chain members can miss. then we
//          look only at the gen_to mothers of current event. (up to gen_from of course)

Int_t MoDSSCorrelProc::PrintChain(MoDSSAnlEvent *event, Int_t gen_last, Int_t chnum, Int_t gen_to) 
{
	Int_t     rv      = 0;  // value returned from next recursion step
	Int_t     retval  = 0;  // return value
	Bool_t    ok      = kTRUE;
	Int_t     num     = 0;  // index of mothers for given daughter
	Int_t     gen_from  = 0;
	static 		MoDSSAnlEvent *chain[MAXALPHA+1+1];  // keeps the subchain to be printed
	Bool_t    bPrinted = kFALSE;
	Bool_t		bDebugPCH = kFALSE;

	if (bDebugPCH) cout << "PrintChain: Entering... Useful parameters  gen_from "<<gen_from<<" gen_last " <<gen_last<<" chnum "<<chnum<<" gen_to "<<gen_to<<" num "<<num<<" retval "<<retval<<endl;

	if (bDebugPCH) cout << "PrintChain: My event type is " << event->fEventType <<" num "<<event->fEventNum << " at time "<<event->fTimeX[event->fIx] <<" and filling chain on position "<<gen_last<<"."<<endl;

	// store this event into printchain
	chain[gen_last] = event;

	// recoil => print the chain (1st possible end of recursivity)
	if ((gen_last == 0) && (chnum >= fCorrelPar->fNumPrt)) {
		if (bDebugPCH) cout << "PrintChain: Parameter genlast " << gen_last <<endl;
		PrintThisChain(chain);
		//cout << "PrintChain: Have I printed a chain? \n";
		retval = 1;  // chain succesfully printed
	}
	// recoil => but not enought chain members (2nd possible end)
	else if ((gen_last == 0) && (chnum < fCorrelPar->fNumPrt)) {
		if (bDebugPCH) 
			cout << "PrintChain: Throwing away a chain since I have not enough chain members to print" <<endl;
		retval = 0;
	}

	// if there is mother gamma
	else if (event->fMama[MAXALPHA][0]){
		if (((fCorrelPar->fbCorReGa) && (gen_last == 2)) // This is a case of Re-Ga-present event
			|| ((fCorrelPar->fbCorAlGa) && (gen_last == 3)) // This is a Case of (Re)-Al-Ga-present event
			// 8.9.2014 Add (SA): Case for Ga-Fi only.
			|| (!(fCorrelPar->fbCorReGa) && !(fCorrelPar->fbCorAlGa) && ((fCorrelPar->fbCorGaFi) 
			|| (fCorrelPar->fbCorGaAl)) && (gen_last == 1))) // This should be for the case of Ga-Fi, i.e. Ga-(InpEvent)				
		{
			if (bDebugPCH) cout << "PrintChain: I entered GAMMA because ReGA = ";
			if (bDebugPCH) cout <<fCorrelPar->fbCorReGa<<" AlGa "<<fCorrelPar->fbCorAlGa<<" gen_last "<<gen_last<<endl;
			rv = 0; retval = 0; ok = kTRUE; num = 0;
			do {
				if (event->fMama[MAXALPHA][num]){
					if (bDebugPCH) cout <<"PrintChain: Callin' recursive PrintCHain in GAMMA with: num ";
					if (bDebugPCH) cout <<num<<" gen_to "<<gen_to<<" chnum+1 " <<chnum+1<<endl;
					rv = PrintChain(event->fMama[MAXALPHA][num], gen_to, chnum+1, gen_to-1);
					if (bDebugPCH) cout<<"PrintChain: Back from recursion in GAMMA with "<<rv<<endl;
				}
				else ok = kFALSE;
				if (rv == 1) retval = 1;
				if ((retval == 0) && (chnum >= fCorrelPar->fNumPrt)) {
					// do not print the same chain (when no mother in more previous gen was found)
					if (!bPrinted) {
						if (bDebugPCH) cout <<"PrintChain: The chain was not printed... forced print..."<<endl;
						PrintThisChain(chain);
						bPrinted = kTRUE;
					}
					retval = 1;
					ok = kFALSE;
				}
				// next index item 
				num++;
			} while ((ok) && (num < NUMMOTHER)); // = end do
		} // if (((fCorrelPar->fbCorReGa) && (gen_last == 2)) || ((fCorrelPar->fbCorAlGa) && (gen_last == 3)
	}		//else if (event->fMama[MAXALPHA][0]){

	// alpha => loop over index

	else {

		// in the case some chain members can miss, check all previous generations too.
		// fbAllAlphas in this case means that all members have to be present.
		if ((!fbAllAlphas) && (gen_last > 1))  gen_from = 0; 
		else gen_from = gen_last - 1;  // all members required -> test only previous gen

		for (Int_t g = gen_to; g >= gen_from; g--) {
			rv = 0; 
			retval = 0; 
			ok = kTRUE; 
			num = 0;

			if (bDebugPCH) cout <<"PrintChain: Now I will check if mother exists: num ";
			if (bDebugPCH) cout <<num<<" g "<<g<<" chnum+1 "<<chnum+1<<" gen_to "<<  gen_to<<endl;

			do {
				// check if previous generation exists.
				// if delayed gamma is expected then one should check one position lower member of
				// fMama array since before entering to the PrintChain it was called in AlphaAlpha
				// with the index one more to be sure that the complete chain together with gamma
				// will fit to the chain array other combination should work fine
				if ((g>0) && (event->fMama[g-1][num]) && ((fCorrelPar->fbCorReGa) && (fCorrelPar->fbCorAlAl))){
					if (bDebugPCH) cout << "PrintChain: Callin' recursive PrintChain in AL switch GA with param: num ";
					if (bDebugPCH) cout <<num<<" g "<<g<<" chnum+1 "<<chnum+1<<" g-1 "<<g-1<<endl;

					// if the mother alpha is found once again is called with usual counters (g)
					// however (g-1) event is passed 
					rv = PrintChain(event->fMama[g-1][num], g, chnum+1, g-1);
					if (bDebugPCH) cout<<"PrintChain: Back from recursion in AL switch GA with "<<rv<<endl;
				}

				// check if the previous generation exists.
				// if no delayed gamma is part of the chain then search on expected position
				else if ((event->fMama[g][num])) /* && !(fCorrelPar->fbCorAlGa)*/ {
					if (bDebugPCH) cout <<"PrintChain: Callin' recursive PrintChain in AL switch AL with param: num ";
					if (bDebugPCH) cout <<num<<" g "<<g<<" chnum+1 "<<chnum+1<<" g-1 "<<g-1<<endl; 
					rv = PrintChain(event->fMama[g][num], g, chnum+1, g-1);
					if (bDebugPCH) cout<<"PrintChain: Back from recursion in AL switch AL with "<<rv<<endl;
				}

				// if no mother, exit loop
				else ok = kFALSE;

				// chain printed => do not print subchains
				if (rv == 1) retval = 1;
				if ((retval == 0) && (chnum >= fCorrelPar->fNumPrt)) {
					// do not print the same chain (when no mother in more previous gen was found)
					if (!bPrinted) {
						//cout<<"PrintChain - Calling PrintThisChain"<<endl;
						PrintThisChain(chain);
						bPrinted = kTRUE;
					}
					retval = 1;
					ok = kFALSE;
				}
				// next index item 
				num++;
			} while ((ok) && (num < NUMMOTHER)); // = end do
		} // for ...
	} // else // alpha ...

	// erase event from the printchain
	chain[gen_last] = 0;
	return retval;
}

// ==========================================================
// look for alpha mother with current (fInpEvent) alpha event (daughter)
// all found mothers are stored in daughter's pointer array.
// gen - previous (mother) alpha generation; the generation whic will be checked now...
// dtmin - low time limit for correlation
// dtmax - high time limit for correlation
// gen_prt - generation from which the printing starts (last gen in the printed chain); position of fInpEvent based on the comparing with energy gates in ProcessAlpha

Int_t MoDSSCorrelProc::AlphaAlpha(Int_t gen, Double_t dtmin, Double_t dtmax, Int_t gen_prt)
{
	Double_t dt, mother_energy;
	Int_t retval = 0;
	Int_t rv   = 0;
	MoDSSAnlEvent *alm = 0;            // mother alpha
	Int_t     num      = 0;            // index of found alpha-mothers for a given alpha-daughter
	Int_t     counter  = fAlCounter;   // number of alphas in the table
	Int_t     pos;                     // position of previous alpha in the table
	Bool_t		debugAA = kFALSE;

	//if gen=0 (recoil) search for re-al or ga-al corr. (if activated). otherwise exit.
	if ((debugAA) && (fInpEvent->fE_kevX[fInpEvent->fIx]>8775) && (fInpEvent->fE_kevX[fInpEvent->fIx]<8780)&& (fInpEvent->fIx==12))  cout << "alphaAlpha: Entering AlphaAlpha with gen = "<<gen<<" and alpha energy "<<fInpEvent->fE_kevX[fInpEvent->fIx]<<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<" and gen_prt "<<gen_prt<<endl;
	if (gen == 0){ 
		if ((fCorrelPar->fbCorReAl)||(fCorrelPar->fbCorGaAl)){
			if (fCorrelPar->fbCorReAl){ 
				rv=RecoilAlpha(dtmin, dtmax, gen_prt);
				fhEAlphaReAl->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
			}
			if ((fCorrelPar->fbCorGaAl) && (fCorrelPar->fbCorReGa)) {
				//if ((fCorrelPar->fbCorReGa && gen_prt==1) || (fCorrelPar->fbCorAlGa && gen_prt==2)) 
				if ((debugAA) && (fInpEvent->fE_kevX[fInpEvent->fIx]>8775) && (fInpEvent->fE_kevX[fInpEvent->fIx]<8780) && (fInpEvent->fIx==12)) cout<< "AlphaAlpha: I'm going to search Gamma-Alpha"<<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
				rv=GammaAlpha(dtmin, dtmax, gen_prt);
				if ((debugAA) && (fInpEvent->fE_kevX[fInpEvent->fIx]>8775) && (fInpEvent->fE_kevX[fInpEvent->fIx]<8780) && (fInpEvent->fIx==12)) cout<< "AlphaAlpha: I'm back from Gamma-Alpha with return value "<<rv<<" and num = "<<num<<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
			}
			return rv;
		}
	}
	if ((gen == 0) && !(fCorrelPar->fbCorReAl)&& !(fCorrelPar->fbCorGaAl)) return 0;
	if ((gen == 1) && (fCorrelPar->fbCorGaAl) && (fCorrelPar->fbCorAlGa)){
		if ((debugAA) && (fInpEvent->fE_kevX[fInpEvent->fIx]>8775) && (fInpEvent->fE_kevX[fInpEvent->fIx]<8780) && (fInpEvent->fIx==12)) cout<< "AlphaAlpha: I'm going to search Gamma-Alpha with gen = 1"<<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
		rv=GammaAlpha(dtmin, dtmax, gen_prt); 
		if ((debugAA) && (fInpEvent->fE_kevX[fInpEvent->fIx]>8775) && (fInpEvent->fE_kevX[fInpEvent->fIx]<8780) && (fInpEvent->fIx==12)) cout<< "AlphaAlpha: I'm back from Gamma-Alpha with return value "<<rv<<" and num = "<<num<<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
		return rv;
	}
	// check position in the table
	//if ((gen == 1) && 
	if (fAlLast == 0) pos = fCorrelPar->fAlTableSize - 1;
	else pos = fAlLast - 1;

	// loop over alpha table
	if (fCorrelPar->fbCorAlAl) {
		while (counter > 0) {
			if (debugAA) cout << "AlphaAlpha: I'm in the while cycle with counter: "<<counter<<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
			alm = &fAlTable[pos];

			if ((fCorrelPar->fbBack) && (alm->fEb_kev[alm->fIDB] > 0)) 
			{
				mother_energy = alm->fEb_kev[alm->fIDB];
				dt = fInpEvent->fTimeX[fInpEvent->fIx] - alm->fTimeX[alm->fIx];  // time difference in ms
			}
			else if(!(fCorrelPar->fbEnXorY))
			{
				mother_energy = alm->fE_kevX[alm->fIx];	
				dt = fInpEvent->fTimeX[fInpEvent->fIx] - alm->fTimeX[alm->fIx];  // time difference in ms
			}
			else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevX[fInpEvent->fIx]>0 && alm->fE_kevX[alm->fIx]>0)
			{
				mother_energy = alm->fE_kevX[alm->fIx];
				dt = fInpEvent->fTimeX[fInpEvent->fIx] - alm->fTimeX[alm->fIx];  // time difference in ms
			}
			else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevX[fInpEvent->fIx]>0 && alm->fE_kevY[alm->fIy]>0)
			{
				mother_energy = alm->fE_kevY[alm->fIy];
				dt = fInpEvent->fTimeX[fInpEvent->fIx] - alm->fTimeY[alm->fIy];  // time difference in ms
			}
			else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevY[fInpEvent->fIy]>0 && alm->fE_kevX[alm->fIx]>0)
			{
				mother_energy = alm->fE_kevX[alm->fIx];
				dt = fInpEvent->fTimeY[fInpEvent->fIy] - alm->fTimeX[alm->fIx];
			}
			else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevY[fInpEvent->fIy]>0 && alm->fE_kevY[alm->fIy]>0)
			{
				mother_energy = alm->fE_kevY[alm->fIy];
				dt = fInpEvent->fTimeY[fInpEvent->fIy] - alm->fTimeY[alm->fIy];
			}
			else break;

			//if (dt < 0) cout << "!!! Time difference (al-al) < 0 !!! (" << dt << ") Td " << fInpEvent->fTimeX[fInpEvent->fIx] << " Tm " << alm->fTimeX[alm->fIx] << " Em " << mother_energy<< "\n";
			// skip events with too big time difference
			if (dt > dtmax) break;
			// dtmin condition and strip condition
			//  !!!!!!!!!!!!!!!!!!!!!!!!!1
			// TO DO (3.2.2006) Due to some reason test of the low energy was removed? it's missing

			if (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],alm->fIDX[alm->fIx],alm->fIDY[alm->fIy])) {
				// energy condition for mother alpha. use back det (escape alpha) if allowed.

				if ((mother_energy > fCorrelPar->fAlEl[gen]) && (mother_energy < fCorrelPar->fAlEh[gen])) {
					if(gen == 1){
						fhXAl->Fill(alm->fIDX[alm->fIx]);
						fhYAl->Fill(alm->fIDY[alm->fIy]);
					}
					// a correlation is found, create link.
					if (num < NUMMOTHER) {
						fInpEvent->fMama[gen][num++] = alm;
					}
					else {
						cout << " ---------------------------\n";
						cout << "(Al-Al) Number of possible alpha mothers is too large (> " << NUMMOTHER << ") !!!\n";
						cout << "Chains are being lost !\n";
						cout << "Change ";
						cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
						cout << " ---------------------------\n";
					}
				}  // if ((mother_energy >  ... 
			}  // if (TestPositions ...
			// next loop, update counters
			counter--;
			pos--;
			if (pos < 0) pos = fCorrelPar->fAlTableSize - 1;
		}  // while
	} //if (fCorrelPar->fbCorAlAl)
	// if at least one correlation has been found, try to print them
	if (num > 0) {
		if ((fCorrelPar->fbCorReGa) || (fCorrelPar->fbCorAlGa)){
			//cout << "AlphaAlpha: Passing to PrintChain from GAMMA part with gen_prt "<<gen_prt<<" gen "<<gen<<endl;
			retval = PrintChain(fInpEvent, gen_prt+1, 1, gen+1); // here also the histograms are filled.
			//cout << "AlphaAlpha: Back from Printchain in GAMMA with return value "<<retval<<endl;
		}
		else {
			//cout << "AlphaAlpha: Passing to PrintChain from ALPHA part with gen_prt "<<gen_prt<<" gen "<<gen<<endl;		
			retval = PrintChain(fInpEvent, gen_prt, 1, gen); // here also the histograms are filled.
			//cout << "AlphaAlpha: Back from Printchain in ALPHA with return value "<<retval<<endl;
		}
	}
	// in the case when some alphas can absent in the chain, search for correlation
	// with original alpha in the next generation
	if (!fbAllAlphas) {
		// fNumCorrHelp is the last generation, from which it has sense to look for
		// corr up to first chain member.
		if (gen_prt <= fNumCorrHelp) 
			retval = AlphaAlpha(gen-1, dtmin, dtmax + fCorrelPar->fAlTh[gen], gen_prt);
		// if the generation is higher, continue only if there is enough members left.
		else if ((gen_prt - gen) <= (fGenTotal - fCorrelPar->fNumPrt))
			retval = AlphaAlpha(gen-1, dtmin, dtmax + fCorrelPar->fAlTh[gen], gen_prt);
	}
	return retval;
}

// ==========================================================
// look for re-al correlation with the current alpha event.
// energy and tof of the recoil doesn't have to be tested, while it was
// done in ProcessRecoil().
// dtmin, dtmax - time window for the correlation
// gen_prt - generation from which the printing starts (last gen in the printed chain)

Int_t MoDSSCorrelProc::RecoilAlpha(Double_t dtmin, Double_t dtmax, Int_t gen_prt)
{
	Double_t  dt;
	Int_t     retval  = 0;           // return value
	MoDSSAnlEvent *re = 0;           // recoil from the recoil table
	Int_t     num     = 0;           // index of found recoils for a given alpha
	Int_t     counter = fReCounter;  // number of recoils in the table
	Int_t     pos;                   // position of previous recoil in the table

	//cout<<"In RecoilAlpha :"<<endl;
	if (fReLast == 0) pos = fCorrelPar->fReTableSize - 1;
	else pos = fReLast - 1;

	//cout<<"RecoilAlpha - al "<<fInpEvent->fEventNum<<"\t"<<fInpEvent->fIDX[fInpEvent->fIx]<<"\t"<<fInpEvent->fIDY[fInpEvent->fIy]<<"\t"<<fInpEvent->fE_kevX[fInpEvent->fIx]<<"\t"<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;

	//cout<<"counter = "<<counter<<endl;
	//for (int i=counter; i>0; i--){
	//	re = &fReTable[counter-1]; 
	//	cout<<counter-1<<"\t"<<re->fEventNum<<"\t"<<re->fIDX[re->fIx]<<"\t"<<re->fIDY[re->fIy]<<"\t"<<re->fE_kevY[re->fIy]<<"\t"<<re->fTimeY[re->fIy]<<endl;
	//	re = &fReTable[0]; 
	//	cout<<0<<"\t"<<re->fEventNum<<"\t"<<re->fIDX[re->fIx]<<"\t"<<re->fIDY[re->fIy]<<"\t"<<re->fE_kevY[re->fIy]<<"\t"<<re->fTimeY[re->fIy]<<endl;
	//}
	
	// loop over recoil table
	//if(fInpEvent->fEventNum>443007350 && fInpEvent->fEventNum<445000000 )Log<<"Corr  Al "<<fInpEvent->fEventNum<<setprecision(25)<<" - "<<hex<<fInpEvent->fTimeX[fInpEvent->fIx]<<dec<<endl;
	while (counter > 0) {
		//cout<<"loop recoil #"<<counter<<endl;
		re = &fReTable[pos]; 
		//Log<<"\tRead  Re "<<re->fEventNum<<endl;
	//	cout<<"\tRead  Re "<<re->fEventNum<<"\t"<<re->fIDX[re->fIx]<<"\t"<<re->fIDY[re->fIy]<<endl;

		if(!(fCorrelPar->fbEnXorY) && (fCorrelPar->fbReEL || fCorrelPar->fbReELpoly)){
			dt = fInpEvent->fTimeX[fInpEvent->fIx] - re->fTimeX[re->fIx];
			//cout<<"cond1"<<endl;
		}
		else if(!(fCorrelPar->fbEnXorY) && (fCorrelPar->fbReEH || fCorrelPar->fbReEHpoly)){
			dt = fInpEvent->fTimeX[fInpEvent->fIx] - re->fTimeY[re->fIy];
			//cout<<"cond2 : "<<dt<<" ms"<<endl;
			//if(fInpEvent->fEventNum>443007350 && fInpEvent->fEventNum<450000000 ) Log<<"Cond2 -"<<fInpEvent->fEventNum<<" : "<<setprecision(25)<<hex<<fInpEvent->fTimeX[fInpEvent->fIx]<<" - "<<re->fTimeY[re->fIy]<<" = "<<dt<<dec<<" = "<<dt<<endl;
		}
		else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevX[fInpEvent->fIx]>0 && (fCorrelPar->fbReEL || fCorrelPar->fbReELpoly)){
			dt = fInpEvent->fTimeX[fInpEvent->fIx] - re->fTimeX[re->fIx];
			//cout<<"cond3"<<endl;
		}
		else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevX[fInpEvent->fIx]>0 && (fCorrelPar->fbReEH || fCorrelPar->fbReEHpoly)){
			dt = fInpEvent->fTimeX[fInpEvent->fIx] - re->fTimeY[re->fIy];
			//cout<<"cond4"<<endl;
		}
		else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevY[fInpEvent->fIy]>0 && (fCorrelPar->fbReEL || fCorrelPar->fbReELpoly)){
			dt = fInpEvent->fTimeY[fInpEvent->fIy] - re->fTimeX[re->fIx];
			//cout<<"cond5"<<endl;
		}
		else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevY[fInpEvent->fIy]>0 && (fCorrelPar->fbReEH || fCorrelPar->fbReEHpoly)){
			dt = fInpEvent->fTimeY[fInpEvent->fIy] - re->fTimeY[re->fIy];
			//cout<<"cond6"<<endl;
		}
		else{
			//cout<<"No cond"<<endl;
			break;
		}
		//if (dt < 0) cout << "!!! Time difference (re-al) < 0 !!! (" << dt << ")\n";
		if (dt > dtmax){ 
			//cout<<"Going out of the window : "<<dt<<" / "<<dtmax<<endl;
			break;  // skip events with too big time difference
		}
		if(dt <= dtmax && dt >=0){
			fhEAlphaTimeOK->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
		}
		//cout<<"This is a Recoil :"<<re->fIDX[re->fIx]<<" \t"<<re->fIDY[re->fIy]<<" \t"<<re->fTimeX[re->fIx]<<" \t pos = "<<pos<<endl;
		// dtmin condition and strip condition
		if(TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],re->fIDX[re->fIx],re->fIDY[fInpEvent->fIy]) && dt >= dtmin)

		{
			fhEAlphaTimePosOK->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
			//cout<<"position ok : "<<num<<" : "<<NUMMOTHER<<endl;
			// a correlation is found, create link.
			if (num < NUMMOTHER) {
				fInpEvent->fMama[0][num++] = re;  // generation is 0
				cout<<"we have a correlation<<"<<endl;
				cout<<"al "<<fInpEvent->fEventNum<<"\t"<<fInpEvent->fIDX[fInpEvent->fIx]<<"\t"<<fInpEvent->fIDY[fInpEvent->fIy]<<"\t"<<fInpEvent->fE_kevX[fInpEvent->fIx]<<"\t"<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
				cout<<"Re "<<re->fEventNum<<"\t"<<re->fIDX[re->fIx]<<"\t"<<re->fIDY[re->fIy]<<"\t"<<re->fE_kevY[re->fIy]<<"\t"<<re->fTimeY[re->fIy]<<endl;
				cout<<"++++++++++++++++++"<<endl;
				fhEAlphaCorred->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
			}
			else {
				cout << " ---------------------------\n";
				cout << "(Re-Al) Number of possible recoil mothers is too large (> " << NUMMOTHER << ") !!!\n";
				cout << "Chains are being lost !\n";
				cout << "Change ";
				cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
				cout << " ---------------------------\n";
			}
		} // if ((dt > dtmin)  ...
		// next loop, update counters
		counter--;
		pos--;
		if (pos < 0) pos = fCorrelPar->fReTableSize - 1;
	} // while ...
	//cout<<"RecoilAlpha - out while"<<endl;
	// if at least one correlation has been found, try to print them
	if (num > 0){ 
		//	cout<<"RecoilAlpha - Calling PrintChain"<<endl;
		fhEAlphaPrint->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
		retval = PrintChain(fInpEvent, gen_prt, 1, 0);  // here also the histograms are filled.
		return retval;
	}
}//End RecoilAlpha

// ==========================================================
// look for ga-al correlation with the current alpha event.
// gamma energy doesn't have to be tested, while it was
// done in MoDSSCorrelationSearch().
// dtmin, dtmax - time window for the correlation
// gen_prt - generation from which the printing starts (last gen in the printed chain)

Int_t MoDSSCorrelProc::GammaAlpha(Double_t dtmin, Double_t dtmax, Int_t gen_prt)
{
	Double_t  dt;
	Int_t     retval  = 0;           // return value
	MoDSSAnlEvent *ga = 0;           // recoil from the gamma table
	Int_t     num     = 0;           // index of found gammas for a given alpha
	Int_t     counter = fGaCounter;  // number of gammas in the table
	Int_t     pos;                   // position of previous gamma in the table
	Bool_t    posOK;
	Bool_t		debugGA = kFALSE;

	if (fGaLast == 0) pos = fCorrelPar->fGaTableSize - 1;
	else pos = fGaLast - 1;
	if (debugGA) cout << "GammaAlpha: Entering... counter = "<<counter<<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
	// loop over recoil table
	while (counter > 0) {
		if (debugGA) cout << "GammaAlpha: In the while with counter = " << counter<< " and pos = "<<pos<<endl;
		ga = &fGaTable[pos];
		if(!(fCorrelPar->fbEnXorY)) dt = fInpEvent->fTimeX[fInpEvent->fIx] - ga->fTimeX[ga->fIx];  // time difference in ms
		else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevX[fInpEvent->fIx]>0) dt = fInpEvent->fTimeX[fInpEvent->fIx] - ga->fTimeX[ga->fIx];
		else if(fCorrelPar->fbEnXorY && fInpEvent->fE_kevY[fInpEvent->fIy]>0) dt = fInpEvent->fTimeY[fInpEvent->fIy] - ga->fTimeX[ga->fIx];
		else break;
		if (dt < 0) cout << "!!! Time difference (re-al) < 0 !!! (" << dt << ")\n";
		if (dt > dtmax) {
			if (debugGA) cout << "GammaAlpha: Throwing away with dt = "<<dt<<endl;
			break;  // skip events with too big time difference
		}
		// dtmin condition and strip condition
		if (dt > dtmin){
			if (debugGA) cout << "GammaAlpha: Passing time test..."<<endl;
			if (((fCorrelPar->fbElectronReq) && (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],ga->fIDX[ga->fIx],ga->fIDY[ga->fIy]))) ||
					(!fCorrelPar->fbElectronReq)) {
				if (debugGA) cout << "GammaAlpha: Passing test of the electron..."<<endl;
				// position condition :
				posOK = kFALSE;

				// --- test positions if Recoil - Gamma - alpha correlation is searched
				// --- recoil position in LOW energy branch (alpha is always in LOW branch)
				if ((fCorrelPar->fbCorReGa) && !(fCorrelPar->fbCorAlGa)){
					if (TestRecoilLow(ga->fMama[0][0])) {
						if (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],ga->fMama[0][0]->fIDX[ga->fMama[0][0]->fIx],ga->fMama[0][0]->fIDY[ga->fMama[0][0]->fIy])) posOK = kTRUE;
					} 
				}
				/*else if (!(fCorrelPar->fbCorReGa) && (fCorrelPar->fbCorAlGa))
				  posOK = kTRUE;
				  else if (!(fCorrelPar->fbCorReGa) && !(fCorrelPar->fbCorAlGa)){
				  posOK = kTRUE;
				  cout <<"GammaAlpha: Why I'm in Gamma-Alpha when no gamma mother was searched for?"<<endl;
				  }*/
				else if ((fCorrelPar->fbCorGaAl) && !(fCorrelPar->fbCorAlGa) && !(fCorrelPar->fbCorReGa))
				{ 
					posOK = kTRUE; 
					// suppose that we are running Gamma-Alpha without preceding mother 
					// and therefore the position is not relevant
				}	


				if (posOK) {
					// a correlation is found, create link.
					if (num < NUMMOTHER) {
						if (debugGA) cout << "GammaAlpha: filling gamma with sum energy "<<
							SumGamma()<<" as a mother on position "
								<<num<<endl;
						fInpEvent->fMama[MAXALPHA][num++] = ga;
					}
					else {
						cout << " ---------------------------\n";
						cout << "(Ga-Al) Number of possible gamma mothers is too large (> " << NUMMOTHER << ") !!!\n";
						cout << "Chains are being lost !\n";
						cout << "Change ";
						cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
						cout << " ---------------------------\n";
					}
				} // if (posOK) 
			} // if (((fCorrelPar->fbElectronReq) && (fCorrelPar->fElectronEh > 0)...
		} // 	if (dt > dtmin)...
		// next loop, update counters
		counter--;
		pos--;
		if (pos < 0) pos = fCorrelPar->fGaTableSize - 1;
	} // while ...
	// if at least one correlation has been found, try to print them
	if (num > 0){ 

		if  (fCorrelPar->fbCorReGa) {
			//cout << "Gammaalpha: Callin' PrintChain in part w/o alpha" << endl;
			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 8 "<<endl;
			retval = PrintChain(fInpEvent, 2, 1, 1);  // Re-Ga-Al - case;here also the histograms are filled.
			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 9 "<<endl;
		}
		else if (fCorrelPar->fbCorAlGa){
			if (debugGA) cout << "GammaAlpha: Callin' PrintChain in part with alpha" << endl;		
			retval = PrintChain(fInpEvent, fCorrelPar->fNumAl+1, 1, fCorrelPar->fNumAl);  // Re-Al-Ga-Al case;here also the histograms are filled.
		}
		else if ((fCorrelPar->fbCorGaAl) && !(fCorrelPar->fbCorAlGa) && !(fCorrelPar->fbCorReGa))
		{
			if (debugGA) cout << "GammaAlpha: Callin' PrintChain in part with GaAl" << endl;
			retval = PrintChain(fInpEvent, 1, 1, 0); // This should work for Ga-Al only
		}
		else cout <<"GammaAlpha: Why I'm in Gamma-Alpha when no gamma mother was searched for?"<<endl;
	}

	//retval = PrintChain(fInpEvent, gen_prt, 1, 0);  // here also the histograms are filled.
	return retval;
}//End GammaAlpha

// ==========================================================
// look for re-fi correlation with current fission event

Int_t MoDSSCorrelProc::RecoilFission()
{
	Double_t dt;
	Int_t     retval  = 0;           // return value
	MoDSSAnlEvent *re = 0;           // recoil from the recoil table
	Int_t     num     = 0;           // index of found recoils for a given fission
	Int_t     counter = fReCounter;  // number of recoils in the table
	Int_t     pos;                   // position of previous recoil in the table

	if (fReLast == 0) pos = fCorrelPar->fReTableSize - 1;
	else pos = fReLast - 1;

	// loop over recoil table
	while (counter > 0) {
		re = &fReTable[pos];
		if(fInpEvent->fE_kevX[fInpEvent->fIx]>0 && (fCorrelPar->fbReEL || fCorrelPar->fbReELpoly)){
			dt = fInpEvent->fTimeX[fInpEvent->fIx] - re->fTimeX[re->fIx];  // time difference in ms
		}
		else if(fInpEvent->fE_kevY[fInpEvent->fIy]>0 && (fCorrelPar->fbReEH || fCorrelPar->fbReEHpoly)){ 
			dt = fInpEvent->fTimeY[fInpEvent->fIy] - re->fTimeY[re->fIy];
		}
		else break;
		if (dt < 0) cout << "!!! Time difference (re-fi) < 0 !!! (" << dt << ")\n";
		if (dt > fCorrelPar->fFiTh) break;  // skip events with too big time difference
		// dtmin condition and strip condition
		if (dt > fCorrelPar->fFiTl) {
			// position condition :
			Bool_t posOK = kFALSE;

			// --- recoil position in LOW energy branch (fission is always in HIGH branch)
			if (TestRecoilHigh(re)) {

				if (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],re->fIDX[re->fIx],re->fIDY[re->fIy])) posOK = kTRUE;
			}
			// --- test requested positions
			if (posOK) {
				// a correlation is found, create link.
				if (num < NUMMOTHER) {
					fInpEvent->fMama[0][num++] = re;  // generation is 0
				}
				else {
					cout << " ---------------------------\n";
					cout << "(Re-Fi) Number of possible recoil mothers is too large (> " << NUMMOTHER << ") !!!\n";
					cout << "Chains are being lost !\n";
					cout << "Change ";
					cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
					cout << " ---------------------------\n";
				}
			} // if ((TestPositions( ...
		}  // if ((dt > fCorrelPar-> ...
		// next loop, update counters
		counter--;
		pos--;
		if (pos < 0) pos = fCorrelPar->fReTableSize - 1;
	} // while ...
	// if at least one correlation has been found, try to print them
	if (num > 0){
		//cout <<"volam PrintChain v recoil fission s num "<<num<<endl; 
		retval = PrintChain(fInpEvent, 1, 1, 0);  // here also the histograms are filled.
		//cout <<"som naspat v recoil fission s navratom "<<retval<<endl;
	}
	return retval;
}


Int_t MoDSSCorrelProc::GammaFission()
{
	Double_t	dt			= 0;						//time between events.
	Int_t     retval  = 0;           // return value
	MoDSSAnlEvent *ga = 0;           // gamma from the gamma table
	Int_t     num     = 0;           // index of found gammas for a given fission
	Int_t     counter = fGaCounter;  // number of gammas in the table
	Int_t     pos;                   // position of previous gamma in the table

	if (fGaLast == 0) pos = fCorrelPar->fGaTableSize - 1;
	else pos = fGaLast - 1;

	// loop over gamma table
	//
	//might be stopped when 
	//1. all gammas are checked (counter is zero), 
	//2. when the more then NUMMOTHER of correlation is found (not implemented yet)
	//3. when the dt> then the limit (fCorrelPar->fGaTh)
	//
	//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 1 "<<endl;
	while (counter > 0) { // do while there are some gammas avaiable in the table.. possible end if all gammas are used
		//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 2 "<<endl;
		ga = &fGaTable[pos];
		dt = fInpEvent->fTimeY[fInpEvent->fIy] - ga->fTimeTrig;  // time difference in ms
		//cout << "GammaFission: Checking gamma on position... " << pos << " and ";
		if (dt < 0) cout << "!!! Time difference (ga-fi) < 0 !!! (" << dt << ")\n";
		if (dt > fCorrelPar->fFiTh){
			//cout << "thrown away due to the long dt "<<dt<<endl;
			break;  // skip events with too big time difference; possible end of "while"
		}
		// dtmin condition and strip condition
		if (dt > fCorrelPar->fFiTl) {
			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 3 "<<endl;
			//cout << "it is passed since the dt is "<<dt<<endl;
			if (((fCorrelPar->fbElectronReq) && (fCorrelPar->fElectronEh > 0) && TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],ga->fIDX[ga->fIx],ga->fIDY[ga->fIy])) ||
					(!fCorrelPar->fbElectronReq)) {
				//if (fInpEvent->fDet_index == ga->fDet_index)
				//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 4 "<<endl;
				//cout << "GammaFission: Passed through detector test... "<<endl;
				//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 5 "<<endl;
				//cout << "GammaFission: Passed pause test..." <<endl;
				// a correlation is found, create link.
				if (num < NUMMOTHER) {
					//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 6 "<<endl;
					//cout << "GammaFission: All conditions are fine and I'm filling gamma as a mama on position " << num <<endl;
					fInpEvent->fMama[MAXALPHA][num++] = ga;  // gamma is strored behind alphas
				}
				else {
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					// TO DO (30.1.2006): 
					//  Here should be a break possible... to avoid additional test and outputs To be test later
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					cout << " ---------------------------\n";
					cout << "(Ga-Fi) Number of possible recoil mothers is too large (> " << NUMMOTHER << ") !!!\n";
					cout << "Chains are being lost !\n";
					cout << "Change ";
					cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
					cout << " ---------------------------\n";
				}
				//else cout << "GammaFission: Failed pause test..." <<endl;
			} // if (((fCorrelPar->fbElectronReq) && (fCorrelPar->fElectronEh > 0...
			//else cout << "GammaFission: Failed detector test..." <<endl;
		}  // if ((dt > fCorrelPar-> ...
		// next loop, update counters
		counter--; // take next gamma from a table.
		pos--;
		//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 7 "<<endl;
		if (pos < 0) pos = fCorrelPar->fGaTableSize - 1;
	} // while ...
	// if at least one correlation has been found, try to print them
	if (num > 0){
		//cout <<"volam PrintChain v recoil fission s num "<<num<<endl;
		if  (fCorrelPar->fbCorReGa) {
			//cout << "GammaFission: Callin' PrintChain in part w/o alpha" << endl;
			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 8 "<<endl;
			retval = PrintChain(fInpEvent, 2, 1, 1);  // Ga-SF - no alpha case;here also the histograms are filled.
			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 9 "<<endl;
		}
		else if (fCorrelPar->fbCorAlGa){
			//cout << "GammaFission: Callin' PrintChain in part with alpha" << endl;		
			retval = PrintChain(fInpEvent, fCorrelPar->fNumAl+1+1, 1, fCorrelPar->fNumAl+1);  // Ga-SF - ALpha case;here also the histograms are filled.
		}
		else if ((fCorrelPar->fbCorGaFi) && !(fCorrelPar->fbCorReGa) && !(fCorrelPar->fbCorReGa) && !(fCorrelPar->fbCorGaGa)) {
			// 8.9.2014 Note (SA): Parameters were "guessed" from Re-Fi and Re-El.
			retval = PrintChain(fInpEvent, 1, 1, 0);
		}
		else cout <<"GammaFission: Why I'm in Gamma fission when no gamma mother was searched for?"<<endl;
	}
	//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 10 "<<endl;
	return retval;
}


Int_t MoDSSCorrelProc::GammaGamma()
{
	Double_t	dt			= 0;						//time between events.
	Int_t     retval  = 0;           // return value
	MoDSSAnlEvent *ga = 0;           // gamma from the gamma table
	Int_t     num     = 0;           // index of found gammas for a given fission
	Int_t     counter = fGaCounter;  // number of gammas in the table
	Int_t     pos;                   // position of previous gamma in the table

	if (fGaLast == 0) pos = fCorrelPar->fGaTableSize - 1;
	else pos = fGaLast - 1;

	// loop over gamma table
	//
	//might be stopped when 
	//1. all gammas are checked (counter is zero), 
	//2. when the more then NUMMOTHER of correlation is found (not implemented yet)
	//3. when the dt> then the limit (fCorrelPar->fGaTh)
	//
	// cout << counter <<" ";
	while (counter > 0) { // do while there are some gammas avaiable in the table.. 
		// This is possible end if all gammas are used
		ga = &fGaTable[pos];
		dt = fInpEvent->fTimeTrig - ga->fTimeTrig;  // time difference in ms
		//cout << "GammaGamma: Checking gamma on position... " << pos << " and ";

		if (dt < 0) cout << "!!! Time difference Ga-Ga) < 0 !!! (" << dt << ")\n";

		if (dt > fCorrelPar->fGa2Th){
			//cout << "thrown away due to the long dt "<<dt<<endl;
			break;  // skip events with too big time difference; possible end of "while"
		}

		// dtmin condition and strip condition
		if (dt > fCorrelPar->fGa2Tl) {
			//cout << "it is passed since the dt is "<<dt<<endl;
			if (((fCorrelPar->fbElectron2Req) && (fCorrelPar->fElectron2Eh > 0) 
						&& (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],ga->fIDX[ga->fIx],ga->fIDY[ga->fIy]))) 
					||	(!fCorrelPar->fbElectron2Req))
			{
				//if (fInpEvent->fDet_index == ga->fDet_index)
				//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 4 "<<endl;
				//cout << "GammaGamma: Passed through detector test... "<<endl;
				//cout << "GammaGamma: Passed pause test..." <<endl;
				// a correlation is found, create link.
				if (num < NUMMOTHER) {
					//cout << "GammaGamma: All conditions are fine and I'm filling gamma as a mama on position " << num <<endl;
					fInpEvent->fMama[MAXALPHA][num++] = ga;  // gamma is strored behind alphas

					//							!!!!!!!!!!!!!!!!!
					//							HERE MIGHT BE PROBLEM
					//							WHAT IF GAMMA2 WILL FULFILL ALSO THE CONDITIONS
					//							FOR GAMMA1 AND MEANWHILE THE POSITIONS FOR MOTHER
					//							WILL BE FILLED?
					//							
					//							ONE SOLUTION: GAMMA2 NEEDS TO BE TESTED BEFORE GAMMA1
					//							THEN CORRELATIONS FOR GAMMA2 WILL BE PRINTED OUT AND IT CAN BE TESTED
					//							AS AN CANDIDATE FOR GAMMA1 AND STORED IN THE GAMMA TABLE WITH PROPER MOTHER.
					//							!!! DO NOT FORGET TO CLEAR MOTHERS IN THIS CASE !!!
					//							(This I implemented)
					//							
					//							ANOTHER SOLUTION: GAMMA2 NEEDS TO HAVE SEPARATED FIELDS FOR
					//							MOTHERS
					//							!!!!!!!!!!!!!!!

				}
				else {
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					// TO DO (30.1.2006): 
					//  Here should be a break possible... to avoid additional test and outputs To be test later
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					cout << " ---------------------------\n";
					cout << "(Ga-Ga) Number of possible recoil mothers is too large (> " << NUMMOTHER << ") !!!\n";
					cout << "Chains are being lost !\n";
					cout << "Change ";
					cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
					cout << " ---------------------------\n";
				}
				//else cout << "GammaGamma: Failed pause test..." <<endl;
			} // if (((fCorrelPar->fbElectronReq) && (fCorrelPar->fElectronEh > 0...
			//else cout << "GammaGamma: Failed detector test..." <<endl;
		}  // if ((dt > fCorrelPar-> ...
		// next loop, update counters
		counter--; // take next gamma from a table.
		pos--;
		//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 7 "<<endl;
		if (pos < 0) pos = fCorrelPar->fGaTableSize - 1;
	} // while ...
	// if at least one correlation has been found, try to print them
	if (num > 0){
		//cout <<"volam PrintChain v recoil fission s num "<<num<<endl;
		if  (fCorrelPar->fbCorReGa) {
			//cout << "GammaGamma: Callin' PrintChain in part w/o alpha" << endl;
			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 8 "<<endl;
			retval = PrintChain(fInpEvent, 2, 1, 1);  // Ga-Ga - no alpha case;here also the histograms are filled.
			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "GammaFission: Test SF event... step 9 "<<endl;
		}
		else if (fCorrelPar->fbCorAlGa){
			//cout << "GammaGamma: Callin' PrintChain in part with alpha" << endl;		
			retval = PrintChain(fInpEvent, fCorrelPar->fNumAl+1+1, 1, fCorrelPar->fNumAl+1);  // Ga-SF - ALpha case;here also the histograms are filled.
		}
		else cout <<"GammaGamma: Why I'm in Gamma Gamma when no gamma mother was searched for?"<<endl;
	}
	return retval;
}


// ==========================================================
// look for re-gamma correlation with current gamma event

Int_t MoDSSCorrelProc::RecoilGamma()
{
	Double_t dt;
	Int_t     retval  = 0;           // return value
	MoDSSAnlEvent *re = 0;           // recoil from the recoil table
	Int_t     num     = 0;           // index of found recoils for a given fission
	Int_t     counter = fReCounter;  // number of recoils in the table
	Int_t     pos;                   // position of previous recoil in the table
	Bool_t		debugRG = kFALSE;

	//cout << counter <<" ";
	if (fReLast == 0) pos = fCorrelPar->fReTableSize - 1;
	else pos = fReLast - 1;
	if ((debugRG) && (counter>0) && (fInpEvent->fE_kevX[fInpEvent->fIx]>0)) cout << "RecoilGamma: Entering... some usefull parameters: fReLast "<<fReLast<< " counter " << counter << " pos " << pos << endl;



	// loop over recoil table  
	while (counter > 0) {
		if ((debugRG) && (fInpEvent->fE_kevX[fInpEvent->fIx]>0)) cout << "RecoilGamma: I have counter "<<counter<<"Checking Recoil on position... " << pos << " and ";
		re = &fReTable[pos];
		if((fCorrelPar->fbReEL || fCorrelPar->fbReELpoly)) dt = fInpEvent->fTimeTrig - re->fTimeX[re->fIx];  // time difference in ms
		else if((fCorrelPar->fbReEH || fCorrelPar->fbReEHpoly)) dt = fInpEvent->fTimeTrig - re->fTimeY[re->fIy];
		else break;
		//if (dt < 0) cout << "!!! Time difference (re-gamma) < 0 !!! (" << dt << ")\n";
		if (dt > fCorrelPar->fGaTh) {
			if ((debugRG) && (fInpEvent->fE_kevX[fInpEvent->fIx]>0)) cout << "thrown away due to the long dt "<<dt<<endl;
			break;  // skip events with too big time difference - temporary fixed test condition
		}
		// dtmin condition and 
		// !!! next if temporary disabled due to a fixed test condition
		//if ((dt > fCorrelPar->fFiTl) && (fInpEvent->fDet_index == re->fDet_index)) 
		if (dt > fCorrelPar->fGaTl){
			if ((debugRG)&& (fInpEvent->fE_kevX[fInpEvent->fIx]>0)) cout << "RecoilGamma: passing time condition since the dt is "<<dt<<endl;
			//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
			//cout << "dt condition crossed" << endl;
			// if electron is required test the detector strip otherwise continue without the test
			if (((fCorrelPar->fbElectronReq) && (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],re->fIDX[re->fIx],re->fIDY[re->fIy]))) ||
					(!fCorrelPar->fbElectronReq)) {
				if (debugRG) cout << "RecoilGamma: Passed through detector test..." << endl; 
				// position condition not tested 
				//cout << "RecoilGamma: passed through pause test..."<<endl;
				// a correlation is found, create link.
				if (num < NUMMOTHER) {
					if (debugRG) cout << "RecoilGamma: All conditions are fine and I'm filling recoil with an energy "<<re->fE_kevX[re->fIx]<<" as a mama on position " << num <<endl;
					//cout<< "Sum high energy: " <<
					//fInpEvent->fegh1+fInpEvent->fegh2+fInpEvent->fegh3+fInpEvent->fegh4 << 
					//"Sum low energy "<<
					//fInpEvent->fegl1+fInpEvent->fegl2+fInpEvent->fegl3+fInpEvent->fegl4<<endl;
					fInpEvent->fMama[0][num++] = re;  // generation is 0
				}
				else {
					cout << " ---------------------------\n";
					cout << "(Re-Gamma) Number of possible recoil mothers is too large (> " << NUMMOTHER << ") !!!\n";
					cout << "Chains are being lost !\n";
					cout << "Change ";
					cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
					cout << " ---------------------------\n";
				}
			} // if ((fCorrelPar->fElectronEh > 0...
			// else cout << "not agreement of strips... strip of recoil" << re->fDet_index << " strip of gamma " << fInpEvent->fDet_index << endl; 
		}  // if ((dt > fCorrelPar-> ...
		// next loop, update counters
		counter--;
		pos--;
		if (pos < 0) pos = fCorrelPar->fReTableSize - 1;
	} // while ...
	// if at least one correlation has been found, try to print them
	// TO DO (30.1.2006):
	// Here maybe should be tested if the required number of chain memmbers is 2
	// otherwise it has no sense to try to print the chain since form Re-Ga we have only
	// two members
	if ((num > 0) && (fCorrelPar->fNumPrt == 2)){
		//cout <<"Calling a PrintChain in RecoilGamma using num "<<num<<endl; 

		retval = PrintChain(fInpEvent, 1, 1, 0);  // Re-Ga print chain; here also the histograms are filled.
		//cout <<"I am back in the RecoilGamma with a return value "<<retval<<endl;
	}
	return retval;
}

// ==========================================================
// look for al-gamma correlation with current fission event
// all found mothers are stored in gammas's pointer array.
// gen - previous (mother) alpha generation
// dtmin - low time limit for correlation
// dtmax - high time limit for correlation
// gen_prt - generation from which the printing starts (last gen in the 
// printed chain - in this case the fission gen = fNumAl+1)

Int_t MoDSSCorrelProc::AlphaGamma(Int_t gen, Double_t dtmin, Double_t dtmax)
{
	Double_t dt, mother_energy;
	Int_t    retval   = 0;
	MoDSSAnlEvent *al = 0;           // alpha from the alpha table
	Int_t     num     = 0;           // index of found alphas for a given gammas?
	Int_t     counter = fAlCounter;  // number of alphas in the table
	Int_t     pos;                   // position of previous alpha in the table
	Bool_t		debugAG = kFALSE;

	// if gen=0 (recoil) then exit. (re-gamma correlation search is done in RecoilGamma)
	// in fact this can't happen, but one never knows. 
	if (gen == 0) return 0;

	// check position in the table
	if (fAlLast == 0) pos = fCorrelPar->fAlTableSize - 1;
	else pos = fAlLast - 1;

	// loop over alpha table
	while (counter > 0) {
		al = &fAlTable[pos];
		if ((fCorrelPar->fbBack) && (al->fEb_kev[al->fIDB] > 0)) 
		{
			mother_energy = al->fEb_kev[al->fIDB];
			dt = fInpEvent->fTimeTrig - al->fTimeX[al->fIx];  // time difference in ms
		}
		else if(!(fCorrelPar->fbEnXorY))
		{
			mother_energy = al->fE_kevX[al->fIx];	
			dt = fInpEvent->fTimeTrig - al->fTimeX[al->fIx];  // time difference in ms
		}
		else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0)
		{
			mother_energy = al->fE_kevX[al->fIx];
			dt = fInpEvent->fTimeTrig - al->fTimeX[al->fIx];  // time difference in ms
		}
		else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0)
		{
			mother_energy = al->fE_kevY[al->fIy];
			dt = fInpEvent->fTimeTrig - al->fTimeY[al->fIy];  // time difference in ms
		}
		else break;

		//if (dt < 0) cout << "!!! Time difference (al-fi) < 0 !!! (" << dt << ")\n";
		// skip events with too big time difference
		if (dt > dtmax) break;
		//cout <<"dt "<<dt<<endl;Destr
		// dtmin condition and strip condition
		if (dt>dtmin) {
			if (((fCorrelPar->fbElectronReq) && (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],al->fIDX[al->fIx],al->fIDY[al->fIy]))) ||
					(!fCorrelPar->fbElectronReq)) {

				// If Gamma Alpha will be searched for, then the gamma is expected only after first generation alpha
				// If Gamma Alpha will NOT be searched for then the gamma might be after any generation
				// TO DO (14.3.2006)) It doesn't look very nice here... seems that also unnecessary things are tested.
				if (((fCorrelPar->fbCorGaAl) && (mother_energy > fCorrelPar->fAlEl[1]) && (mother_energy < fCorrelPar->fAlEh[1])) ||
						(!(fCorrelPar->fbCorGaAl) && (mother_energy > fCorrelPar->fAlEl[gen]) && (mother_energy < fCorrelPar->fAlEh[gen]))) {
					if (debugAG) cout << "Alpha Gamma: Passed energy test..."<<endl;
					// a correlation is found, create link 
					//cout << "I found a mother" <<endl;
					if (num < NUMMOTHER) {
						if (!fCorrelPar->fbCorGaAl) fInpEvent->fMama[gen][num++] = al;
						if (fCorrelPar->fbCorGaAl) fInpEvent->fMama[1][num++] = al;
					}
					else {
						cout << " ---------------------------\n";
						cout << "(Al-Ga) Number of possible alpha mothers is too large (> " << NUMMOTHER << ") !!!\n";
						cout << "Chains are being lost ! \n";
						cout << "Change ";
						cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
						cout << " ---------------------------\n";
					}
				} // if ((mother_energy > ...
			} // if (((fCorrelPar->fbElectronReq) ...
		} // if ((dt > dtmin) ...
		// next loop, update counters
		counter--;
		pos--;
		if (pos < 0) pos = fCorrelPar->fAlTableSize - 1;
	} // while (counter ...
	// if at least one correlation has been found, try to print them
	if (num > 0) 
		retval = PrintChain(fInpEvent, fCorrelPar->fNumAl+1, 1, gen); // here also the histograms are filled.

	// in the case when some alphas can absent in the chain, search for correlation
	// with the next alpha generation
	if (!fbAllAlphas) 
		retval = AlphaGamma(gen-1, dtmin, dtmax + fCorrelPar->fAlTh[gen]);

	return retval;
}


// ==========================================================
// look for al-fi correlation with current fission event
// all found mothers are stored in fission's pointer array.
// gen - previous (mother) alpha generation (what should be the mother?)
// dtmin - low time limit for correlation
// dtmax - high time limit for correlation
// gen_prt - generation from which the printing starts (last gen in the 
// printed chain - in this case the fission gen = fNumAl+1)

Int_t MoDSSCorrelProc::AlphaFission(Int_t gen, Double_t dtmin, Double_t dtmax)
{
	Double_t dt, mother_energy;
	Int_t    retval   = 0;
	MoDSSAnlEvent *al = 0;           // alpha from the alpha table
	Int_t     num     = 0;           // index of found alphas for a given fission
	Int_t     counter = fAlCounter;  // number of alphas in the table
	Int_t     pos;                   // position of previous alpha in the table

	// if gen=0 (recoil) then exit. (re-fi correlation search is done in RecoilFission)
	// in fact this can't happen, but one never knows. 
	if (gen == 0) return 0;

	// check position in the table
	if (fAlLast == 0) pos = fCorrelPar->fAlTableSize - 1;
	else pos = fAlLast - 1;

	// loop over alpha table
	while (counter > 0) {
		al = &fAlTable[pos];
		if ((fCorrelPar->fbBack) && (al->fEb_kev[al->fIDB] > 0)) 
		{
			mother_energy = al->fEb_kev[al->fIDB];
			dt = fInpEvent->fTimeY[fInpEvent->fIy] - al->fTimeX[al->fIx];  // time difference in ms
		}
		else if(!(fCorrelPar->fbEnXorY))
		{
			mother_energy = al->fE_kevX[al->fIx];	
			dt = fInpEvent->fTimeY[fInpEvent->fIy] - al->fTimeX[al->fIx];  // time difference in ms
		}
		else if(fCorrelPar->fbEnXorY && al->fE_kevX[al->fIx]>0)
		{
			mother_energy = al->fE_kevX[fInpEvent->fIx];
			dt = fInpEvent->fTimeY[fInpEvent->fIy] - al->fTimeX[al->fIx];  // time difference in ms
		}
		else if(fCorrelPar->fbEnXorY && al->fE_kevY[al->fIy]>0)
		{
			mother_energy = al->fE_kevY[al->fIy];
			dt = fInpEvent->fTimeY[fInpEvent->fIy] - al->fTimeY[al->fIy];  // time difference in ms
		}
		else break;
		if (dt < 0) cout << "!!! Time difference (al-fi) < 0 !!! (" << dt << ")\n";
		// skip events with too big time difference
		if (dt > dtmax) break;
		// dtmin condition and strip condition
		if ((dt > dtmin)) {
			if (TestPositions(fInpEvent->fIDX[fInpEvent->fIx],fInpEvent->fIDY[fInpEvent->fIy],al->fIDX[al->fIx],al->fIDY[al->fIy])) {
				// energy condition for mother alpha. use back det (escape alpha) if allowed.
				if ((mother_energy > fCorrelPar->fAlEl[gen]) && (mother_energy < fCorrelPar->fAlEh[gen])) {
					// a correlation is found, create link 
					if (num < NUMMOTHER) {
						fInpEvent->fMama[gen][num++] = al;
					}
					else {
						cout << " ---------------------------\n";
						cout << "(Al-Fi) Number of possible alpha mothers is too large (> " << NUMMOTHER << ") !!!\n";
						cout << "Chains are being lost !\n";
						cout << "Change ";
						cout << "correlation settings.(smaller time window). HIGH BACKGROUND POSSIBLE ?\n";
						cout << " ---------------------------\n";
					}
				} // if ((mother_energy > ...
			} // if (TestPositions ...
		} // if ((dt > dtmin) ...
		// next loop, update counters
		counter--;
		pos--;
		if (pos < 0) pos = fCorrelPar->fAlTableSize - 1;
	} // while (counter ...
	// if at least one correlation has been found, try to print them
	if (num > 0) 
		retval = PrintChain(fInpEvent, fCorrelPar->fNumAl+1, 1, gen); // Al-Ga printchain; here also the histograms are filled.

	// in the case when some alphas can absent in the chain, search for correlation
	// with the next alpha generation
	if (!fbAllAlphas) 
		retval = AlphaFission(gen-1, dtmin, dtmax + fCorrelPar->fAlTh[gen]);

	return retval;
}


// ==========================================================
// process input event as recoil

void MoDSSCorrelProc::ProcessRecoil() 
{
	Bool_t ok = kFALSE;
	MoDSSAnlEvent *test;
	// re-al or re-fi correlation activated ? 
	/*cout<<"Processrecoil - fCorrelPar->fbCorReAl = "<<fCorrelPar->fbCorReAl<<endl<<
			"----- For TestRecoilHigh : "<<endl<<
			"\t\tfCorrelPar->fbReEHpoly = "<<fCorrelPar->fbReEHpoly<<endl<<
			"\t\tfCorrelPar->fbReEH = "<<fCorrelPar->fbReEH<<endl<<
			"\t\tfCorrelPar->fReElHigh = "<<fCorrelPar->fReElHigh<<endl<<
			"\t\tfCorrelPar->fReEhHigh = "<<fCorrelPar->fReEhHigh<<endl<<
			"----- For Table :"<<endl<<
			"\t\tfCorrelPar->fbReELpoly = "<<fCorrelPar->fbReELpoly<<endl<<
			"\t\tfCorrelPar->fReTOFLow = "<<fCorrelPar->fReTOFLow<<endl<<
			"\t\tfCorrelPar->fReTOFHigh = "<<fCorrelPar->fReTOFHigh<<endl<<endl;*/

	if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorReFi) || (fCorrelPar->fbCorReGa)) {
		// recoil in LOW energy part ? also at least one position > 0 !
		if (TestRecoilHigh(fInpEvent)) ok = kTRUE;

		//cout << "Accepted recoil in the time " << fInpEvent->fTimeY[fInpEvent->fIy] << " with energy of " << fInpEvent->fE_kevY[fInpEvent->fIy] <<endl;
		// energy and position ok ?
		if (ok) {
			// tof condition
			//cout << "energy " << fInpEvent->fE_kevY[fInpEvent->fIy]<< " tof "<< fInpEvent->fTOF<<endl;
			//if (( !(fCorrelPar->fbReEHpoly) && !(fCorrelPar->fbReELpoly) && (fInpEvent->fTOF >= fCorrelPar->fReTOFLow) && (fInpEvent->fTOF < fCorrelPar->fReTOFHigh)) || (fCorrelPar->fbReEHpoly) || (fCorrelPar->fbReELpoly))  {
				// event fulfils all conditions for searched recoil event.
				// set recoil flag
				fInpEvent->fEventType = recoil;
				//cout<<"ProcessRecoil - This is a recoil"<<endl;
				//cout<<"This is a recoil :"<<fInpEvent->fE_kevX[fInpEvent->fIx]<<"\t"<<fInpEvent->fIx<<" \t"<<fInpEvent->fIy<<" \t"<<fInpEvent->fTimeY[fInpEvent->fIy]<<endl;
				// store it in the recoil table. When top of the table is reached
				// continue from the beginning.
				fReTable[fReLast] = *fInpEvent;
				/*fhReTable_True_Ex->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
                                fhReTable_True_Ey->Fill(fInpEvent->fE_kevY[fInpEvent->fIy]);
                                fhReTable_True_X->Fill(fInpEvent->fIDX[fInpEvent->fIx]);
                                fhReTable_True_Y->Fill(fInpEvent->fIDY[fInpEvent->fIy]);
                                fhReTable_True_TOF->Fill(fInpEvent->fTOF);
				fdReTable_True_EyTOF->Fill(fInpEvent->fTOF,fInpEvent->fE_kevY[fInpEvent->fIy]);*/
				test = &fReTable[fReLast];
				//Log<<"store Re "<<fInpEvent->fEventNum<<endl;
				//cout<<"store Re "<<test->fEventNum<<"\t X="<<test->fIx<<"\t"<<test->fIy<<endl;
				fReLast++;
				//cout << "Filled recoil in the time " << fInpEvent->fTimeY[fInpEvent->fIy] << " with energy of " << fInpEvent->fE_kevY[fInpEvent->fIy]<< " at the position "<< fReLast<<" - X : "<<fInpEvent->fIx<<" - Y : "<<fInpEvent->fIy<<endl;
				if (fReLast == fCorrelPar->fReTableSize) fReLast = 0;
				if (fReCounter < fCorrelPar->fReTableSize) fReCounter++;
				// In this place also check if the table is long enought
				// to store recoils up to the given correlation time. (only in the case
				// the recoil table is full.)
				// fReLast - the next position in recoil table to be filled = the oldest recoil
				// last - last filled position in the recoil table = newest recoil
				if (fReCounter == fCorrelPar->fReTableSize) {
					Int_t last;
					Double_t dtTable = 0;
					if (fReLast == 0) last = fCorrelPar->fReTableSize - 1;
					else last = fReLast - 1;
					dtTable = (fReTable[last].fTimeY[fReTable[last].fIy] - fReTable[fReLast].fTimeY[fReTable[fReLast].fIy])*0.00002;//in ms for everyone. 
					if (dtTable < fReTableTimeLimit) {
						cout << "\n!!! Recoil Table too small !!! INCREASE TABLE SIZE !!!\n";
						cout << "Time needed: " << fReTableTimeLimit << " ms \t time within ReTable: "
							<< dtTable << " ms\n";
						cout<<"last = "<<last<<" - fReLast = "<<fReLast<<" - size = "<<fCorrelPar->fReTableSize<<endl;
					}
				} // if (fReCounter == ...
			//} // tof condition for rectangular
		} // if (ok) 
		else{
			/*fhReTable_False_Ex->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
			fhReTable_False_Ey->Fill(fInpEvent->fE_kevY[fInpEvent->fIy]);
			fhReTable_False_X->Fill(fInpEvent->fIDX[fInpEvent->fIx]);
			fhReTable_False_Y->Fill(fInpEvent->fIDY[fInpEvent->fIy]);
			fhReTable_False_TOF->Fill(fInpEvent->fTOF);
			fdReTable_False_EyTOF->Fill(fInpEvent->fTOF,fInpEvent->fE_kevY[fInpEvent->fIy]);*/
		}
	} // if ((fCorrelPar->fbCorReAl ...
}

// ==========================================================
// process input event as alpha
// alpha generations are counting from 1
// 0-th generation is reserved for recoil

void MoDSSCorrelProc::ProcessAlpha() 
{
	//cout<<"Correl - ProcessAlpha()"<<endl;
	Double_t energy;
	Int_t retval = 0;
	Bool_t	debugPA = kFALSE;

	//cout << "ProcesAlpha: entering..."<<endl;
	// re-al, al-al or al-fi correlation activated ? 
	if ((fCorrelPar->fbCorReAl) || (fCorrelPar->fbCorAlAl) || (fCorrelPar->fbCorAlFi) || (fCorrelPar->fbCorAlGa) || (fCorrelPar->fbCorGaAl)) {
		//cout << "ProcesAlpha: Passing correlation definitions..."<<endl;
		// at least one position > 0
		if ((fInpEvent->fIDX[fInpEvent->fIx] > 0) || (fInpEvent->fIDY[fInpEvent->fIy] > 0)) { 
			// use alpha escapes with back detector ? (back det. activated ?)
			// if yes and summed energy > 50 then use it instead of ekev. 
			// (ekev is > 50, this was tested in MoDSSCorrelationSearch() 
			if ((fCorrelPar->fbBack) && (fInpEvent->fEb_kev[fInpEvent->fIDB] > 50))
				energy = fInpEvent->fEb_kev[fInpEvent->fIDB];
			else if(!(fCorrelPar->fbEnXorY)) energy = fInpEvent->fE_kevX[fInpEvent->fIx];
			else if((fCorrelPar->fbEnXorY) && (fInpEvent->fE_kevX[fInpEvent->fIx] > 0)) energy = fInpEvent->fE_kevX[fInpEvent->fIx];
			else if((fCorrelPar->fbEnXorY) && (fInpEvent->fE_kevY[fInpEvent->fIy] > 0)) energy = fInpEvent->fE_kevY[fInpEvent->fIy];
			else energy = 0;

			/*fhAlTRH_True_Ex->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
			fhAlTRH_True_Ey->Fill(fInpEvent->fE_kevY[fInpEvent->fIy]);
			fhAlTRH_True_X->Fill(fInpEvent->fIDX[fInpEvent->fIx]);
			fhAlTRH_True_Y->Fill(fInpEvent->fIDY[fInpEvent->fIy]);
			fhAlTRH_True_TOF->Fill(fInpEvent->fTOF);*/
			// set flag
			fInpEvent->fEventType = alpha;
			//if(energy>7200 && energy<10000)cout<<"This could be a 216Th :"<<fInpEvent->fIDX[fInpEvent->fIx]<<" \t"<<fInpEvent->fIDY[fInpEvent-<>fIy]<<" \t"<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;

			// loop over alpha generations; test energy; look for correlations
			for (Int_t gen = 1; gen <= fCorrelPar->fNumAl; gen++)
			{
				//if (debugPA) cout << "ProcesAlpha: Checking generation "<<gen<<endl;
				// 2.6.2015 Added check for Al-Al correlation search to avoid
				// unnecessary search for Alpha-Alpha correlations
				if (((fCorrelPar->fbCorAlAl) || (fCorrelPar->fbCorReAl) || ((fCorrelPar->fbCorGaAl) && (fCorrelPar->fbCorReGa))) && (energy > fCorrelPar->fAlEl[gen]) && (energy < fCorrelPar->fAlEh[gen]))
				{
					if (debugPA) cout << "ProcessAlpha: Entering AlphaAlpha with generation no. :"<<gen<<endl;
					/*fhAlTable_True_Ex->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
					fhAlTable_True_Ey->Fill(fInpEvent->fE_kevY[fInpEvent->fIy]);
					fhAlTable_True_X->Fill(fInpEvent->fIDX[fInpEvent->fIx]);
					fhAlTable_True_Y->Fill(fInpEvent->fIDY[fInpEvent->fIy]);
					fhAlTable_True_TOF->Fill(fInpEvent->fTOF);*/
					retval = AlphaAlpha(gen-1,fCorrelPar->fAlTl[gen],fCorrelPar->fAlTh[gen],gen);
					if (debugPA) cout << "ProcessAlpha: Returned from AlphaAlpha with return value "<<retval<<endl;
				}
				else{
					//Julien - For debug purposes
					/*fhAlTable_False_Ex->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
					fhAlTable_False_Ey->Fill(fInpEvent->fE_kevY[fInpEvent->fIy]);
					fhAlTable_False_X->Fill(fInpEvent->fIDX[fInpEvent->fIx]);
					fhAlTable_False_Y->Fill(fInpEvent->fIDY[fInpEvent->fIy]);
					fhAlTable_False_TOF->Fill(fInpEvent->fTOF);*/

				}
				if ((fCorrelPar->fbCorGaAl) && !(fCorrelPar->fbCorReGa) && !(fCorrelPar->fbCorAlGa)) // assumed simple GaAl case, doesn't work somehow
				{
					// test for alpha-decay energy
					if ((energy > fCorrelPar->fAlEl[gen]) && (energy < fCorrelPar->fAlEh[gen]))
					{
						if (debugPA) cout << "ProcessAlpha: Entering GammaAlpha with generation no. :"<<gen<<endl;
						retval = GammaAlpha(fCorrelPar->fAlTl[gen],fCorrelPar->fAlTh[gen],1);
						if (debugPA) cout << "ProcessAlpha: Returned from GammaAlpha with return value "<<retval<<endl;
					}
				}
			} // for (...) 	
			// if event fulfils energy condition for any searched alpha event.
			// store it in the alpha table. When the top of the table is reached 
			// continue from the beginning.
			if ((energy > fAlEnMin) && (energy < fAlEnMax)) {
				fAlTable[fAlLast] = *fInpEvent;
				fhEAlphaTable->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
				//Log<<"store Al "<<fInpEvent->fEventNum<<endl;
				//cout<<"\nstore Al "<<fInpEvent->fEventNum<<endl;
				//cout << "Filling alpha table on position "<<fAlLast<<endl;
				fAlLast++;
				if (fAlLast == fCorrelPar->fAlTableSize) fAlLast = 0;
				if (fAlCounter < fCorrelPar->fAlTableSize) fAlCounter++;
				// In this place also check if the table is long enought
				// to store alphas up to the given correlation time. (only in the case
				// the alpha table is full.)
				// fAlLast - the next position in alpha table to be filled = the oldest alpha
				// last - last filled position in the alpha table = the newest alpha
				if (fAlCounter == fCorrelPar->fAlTableSize) {
					Int_t last;
					Double_t dtTable = 0;
					if (fAlLast == 0) last = fCorrelPar->fAlTableSize - 1;
					else last = fAlLast - 1;
					dtTable = fAlTable[last].fTimeX[fAlTable[last].fIx] - fAlTable[fAlLast].fTimeX[fAlTable[fAlLast].fIx];
					if (dtTable < fAlTableTimeLimit) {
						cout << "\n!!! Alpha Table too small !!! INCREASE TABLE SIZE !!!\n";
						cout << "Time needed: " << fAlTableTimeLimit << " ms \t time within AlTable: "
							<< dtTable << " ms\n";
					}
				}
			}  // if ((energy > fAlEnMin)  ...
		} // if ((fInpEvent->fXY_lt > 0) ...
		else{
			//Julien - For debug purposes
			/*
			fhAlTRH_False_Ex->Fill(fInpEvent->fE_kevX[fInpEvent->fIx]);
			fhAlTRH_False_Ey->Fill(fInpEvent->fE_kevY[fInpEvent->fIy]);
			fhAlTRH_False_X->Fill(fInpEvent->fIDX[fInpEvent->fIx]);
			fhAlTRH_False_Y->Fill(fInpEvent->fIDY[fInpEvent->fIy]);
			fhAlTRH_False_TOF->Fill(fInpEvent->fTOF);
			*/
		}
	} // if ((fCorrelPar->fbCorReAl) ...
}


// ==========================================================
// process input event as fission

void MoDSSCorrelProc::ProcessFission() 
{
	Int_t retval = 0;
	// re-fi or al-fi correlation activated ? 
	if ((fCorrelPar->fbCorReFi) || (fCorrelPar->fbCorAlFi) || (fCorrelPar->fbCorGaFi)) {
		// at least one position > 0
		//if ((fInpEvent->fTimeY[fInpEvent->fIy] > 1404796.65E3) && (fInpEvent->fTimeY[fInpEvent->fIy] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "ProcessFission: Test SF event... step 1 "<<endl;
		if ((fInpEvent->fIDX[fInpEvent->fIx] > 0) || (fInpEvent->fIDY[fInpEvent->fIy] > 0)) {
			//if ((fInpEvent->fTimeY[fInpEvent->fIy] > 1404796.65E3) && (fInpEvent->fTimeY[fInpEvent->fIy] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "ProcessFission: Test SF event... step 2 "<<endl;
			// energy condition
			if ((fInpEvent->fE_kevY[fInpEvent->fIy] > fCorrelPar->fFiEl) && (fInpEvent->fE_kevY[fInpEvent->fIy] < fCorrelPar->fFiEh)) {
				// all conditions fulfilled - set fission flag
				fInpEvent->fEventType = fission;
				//if ((fInpEvent->fTimeY[fInpEvent->fIy] > 1404796.65E3) && (fInpEvent->fTimeY[fInpEvent->fIy] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "ProcessFission: Test SF event... step 3 "<<endl;
				// cout << "Based on correlation setting and existing positions event assigned as "<< fInpEvent->fEventType <<endl;
				// Re-Fi correlation
				if (fCorrelPar->fbCorReFi) {
					// cout << "Now I will check Recoil-Fission since I have fbCorReFi: "<< fCorrelPar->fbCorReFi<<endl;
					retval = RecoilFission();
					//cout << "Recoil-Fission checked with returned value: "<< retval<< endl;
				}
				// Ga-Fi correlation
				if (fCorrelPar->fbCorAlFi) {
					//cout << "Now I will check Alpha-Fission since I have fbCorGaFi: "<< fCorrelPar->fbCorAlFi<<endl;
					retval = AlphaFission(fCorrelPar->fNumAl,fCorrelPar->fFiTl, fCorrelPar->fFiTh);
					//cout << "Alpha-Fission checked with returned value: "<< retval<< endl;
				}
				// Ga-Fi correlation
				if (fCorrelPar->fbCorGaFi) {
					//cout << "Now I will check Gamma-Fission since I have fbCorGaFi: "<< fCorrelPar->fbCorGaFi<<endl;
					//if ((fInpEvent->fTimeY[fInpEvent->fIy] > 1404796.65E3) && (fInpEvent->fTimeY[fInpEvent->fIy] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "ProcessFission: Test SF event... step 4 "<<endl;
					retval = GammaFission();
					//if ((fInpEvent->fTimeY[fInpEvent->fIy] > 1404796.65E3) && (fInpEvent->fTimeY[fInpEvent->fIy] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "ProcessFission: Test SF event... step 5 "<<endl;
					//cout << "Gamma-Fission checked with returned value: "<< retval << endl;
				}
			} //if ((fInpEvent->fE_mev > fCorrelPar->fFiEl) &&...
		} //if ((fInpEvent->fXY_ht > 0) || (fInpEvent->fXY_hb > 0))
	} //if ((fCorrelPar->fbCorReFi) ||...
}	//void MoDSSCorrelProc::ProcessFission() 

void MoDSSCorrelProc::ProcessGamma() 
{
	Int_t retval = 0;
	Bool_t MotherFound = kFALSE;
	Bool_t debugPG = kFALSE;

	// re-fi or al-fi correlation activated ? 
	// next if temporary disabled due to a fixed test condition
	if ((fCorrelPar->fbCorGaAl) || (fCorrelPar->fbCorGaFi) || (fCorrelPar->fbCorReGa) || (fCorrelPar->fbCorAlGa) || (fCorrelPar->fbCorGaGa)) {	  
		//if ((fInpEvent->fE_kevX[0]>0) && (fInpEvent->fE_kevX[0]<1000)) cout << "Testing  gamma: E_keV "<< fInpEvent->fE_kevX[0] << " Eb_keV " << fInpEvent->fEb_kev[0] << "Clovers: " << fInpEvent->fegl[0] << "\t"<< fInpEvent->fegl[1]<< "\t"<<fInpEvent->fegl[2]<< "\t"<< fInpEvent->fegl[3]<<"\t fElectronEl "<<fCorrelPar->fElectronEl<<"\t fElectronEh "<<fCorrelPar->fElectronEh<<  endl;       
		// energy condition      
		//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404792.1E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404792.4E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "ProcessGamma: Test gamma event... step 1 "<<endl;



		// ------------- PROCESSING SECOND GAMMA -----------------------------
		// testing and checking the "second" generation gamma 
		// This needs to be before first gamma check because
		// once the first gamma is stored it needs to remember
		// also its mothers

		if ((fCorrelPar->fbCorGaGa) && // test if we look for Ga-Ga
				((fInpEvent->fE_kevX[fInpEvent->fIx] < 10000) && // throwing away fissions
				 (fInpEvent->fE_kevX[fInpEvent->fIx]<=fCorrelPar->fElectron2Eh) &&//test electron in DSSD X
				 (fInpEvent->fEb_kev[fInpEvent->fIDB]<=fCorrelPar->fElectron2Eh)) && //Test electrons in box
				((fInpEvent->fE_kevX[fInpEvent->fIx]>=fCorrelPar->fElectron2El) ||//test electron in DSSD X
				 (fInpEvent->fEb_kev[fInpEvent->fIDB]>=fCorrelPar->fElectron2El) || //Test electrons in Box
				 (!fCorrelPar->fbElectron2Req))) 
		{ //testing electron in back
			/*if ((debugPG) && (fInpEvent->fegl[3]>218.0) && (fInpEvent->fegl[3]<219.0)) 
				cout <<"ProcessGamma: I have a gamma with sum energy of " <<
					SumGamma()<<
					" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;*/

			// all conditions fulfilled - set gamma flag
			fInpEvent->fEventType = evgamma2;

			//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404792.1E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404792.4E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl[3] > 880))
			//cout << "ProcessGamma: Test gamma event... step 2 "<<endl;
			retval = GammaGamma();
			/*if ((debugPG) && (retval >0)) cout << "Gamma - Gamma checked... Returned value: " 
				<< retval <<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;*/
			// now all chains ended with gamma 2 should be printed already
			// and found mothers will be deleted, since also first gamma
			// which will be checked now and will be stored in the gamma table
			// might fulfill the same conditions
			for(int num=0; num<NUMMOTHER;num++)
			{
				fInpEvent->fMama[MAXALPHA][num] = 0;	
			}
		}	//  if ((fInpEvent->fE_mev == 0) &&
		// -------------------END OF PROCESSING OF SECOND GAMMA -------------------



		// ------------- PROCESSING FIRST GAMMA -----------------------------
		// testing and checking the "first" generation gamma 
		// This should cover all situation except of gamma-gamma correlation

		if (((fInpEvent->fE_kevY[fInpEvent->fIy] < 30000) //throwing away fissions
			&& (fInpEvent->fE_kevX[fInpEvent->fIx]<=fCorrelPar->fElectronEh) 
			&& (fInpEvent->fEb_kev[fInpEvent->fIDB]<=fCorrelPar->fElectronEh)) 
		&& ((fInpEvent->fE_kevX[fInpEvent->fIx]>=fCorrelPar->fElectronEl) //testing electron in stop
			|| (fInpEvent->fEb_kev[fInpEvent->fIDB]>=fCorrelPar->fElectronEl) 
			|| (!fCorrelPar->fbElectronReq))) //testing electron in back
				{ 

				/*if ((debugPG) && (fInpEvent->fegl[3]>218.0) && (fInpEvent->fegl[3]<219.0)) 
				cout <<"ProcessGamma: I have a gamma with sum energy of " <<
				SumGamma()<<
				" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;*/

				// all conditions fulfilled - set gamma flag
				fInpEvent->fEventType = evgamma;

				//------- Look for Recoil-Gamma correlations
				if (fCorrelPar->fbCorReGa) {
					retval = RecoilGamma();
					/*if ((debugPG) && (retval >0)) cout << "Recoil - Gamma checked... Returned value: " << retval <<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;*/
				}

				//------ Look for Alpha Gamma correlations
				if (fCorrelPar->fbCorAlGa) {
					retval = AlphaGamma(fCorrelPar->fNumAl,fCorrelPar->fGaTl, fCorrelPar->fGaTh);
					/*if ((debugPG) && (retval >0)) cout << "ProcessGamma: Alpha - Gamma checked... Return value: " << retval <<" at time "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;*/
				}

				//------ 
				for (Int_t CheckGen = 0; CheckGen <MAXALPHA; CheckGen++){
					if (fInpEvent->fMama[CheckGen][0]) {

						/*if (debugPG) cout << "ProcessGamma: This Gamma has a mother with generation..." << CheckGen << " and event type "<< fInpEvent->fMama[CheckGen][0]->fEventType <<" at time "<<fInpEvent->fMama[CheckGen][0]->fTimeX[fInpEvent->fMama[CheckGen][0]->fIx]<< endl;*/
						MotherFound=kTRUE;
						for (Int_t CheckMotherGen = 0; CheckMotherGen <MAXALPHA; CheckMotherGen++)
							if (fInpEvent->fMama[CheckGen][0]->fMama[CheckMotherGen][0]) 
								if (debugPG) cout << "ProcessGamma: ...... and its mother is generation..." << CheckMotherGen << " and event type "<< fInpEvent->fMama[CheckGen][0]->fMama[CheckMotherGen][0]->fEventType <<" at time " <<fInpEvent->fMama[CheckGen][0]->fMama[CheckMotherGen][0]->fTimeX[fInpEvent->fMama[CheckGen][0]->fMama[CheckMotherGen][0]->fIx] << endl;
					}
				//else cout << "This Gamma has NO mother at generation..." << CheckGen <<endl;
				}

				if (((fCorrelPar->fbCorReGa) && (fCorrelPar->fbCorGaAl) && MotherFound) || ((fCorrelPar->fbCorGaGa) && MotherFound) || (((fCorrelPar->fbCorGaAl) || (fCorrelPar->fbCorGaFi)) && !(fCorrelPar->fbCorReGa))) {
					//cout << "Now I will fill the gamma to the table... fbCorGaAl " << fCorrelPar->fbCorGaAl << " fbCorGaFi " << fCorrelPar->fbCorGaFi << " retval " << retval << " MotherFound " << MotherFound<<endl;
					//if ((energy > fGaEnMin) && (energy < fGaEnMax)) {
					// fGaLast - position of the presently evaluated gamma
					// fGaCounter - top of the gamma heap
					fGaTable[fGaLast] = *fInpEvent;
					//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404792.1E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404792.4E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl[3] > 880)) cout << "ProcessGamma: Test gamma event... step 5 "<<endl;
					if (debugPG) cout << "ProcessGamma: Filling gamma table on position "<<fGaLast<<". The fGaCounter = " << fGaCounter << endl;
					fGaLast++;
					if (fGaLast == fCorrelPar->fGaTableSize) fGaLast = 0; // if we are writing to the end of the GaTable - go back to begin
					if (fGaCounter < fCorrelPar->fGaTableSize) fGaCounter++; //if the GaTable is not full jut increase the max position of the heap
					// In this place also check if the table is long enought
					// to store alphas up to the given correlation time. (only in the case
					// the alpha table is full.)
					// fGaLast - the next position in gamma table to be filled = the oldest gamma
					// last - last filled position in the gamma table = the newest gamma
					if (fGaCounter == fCorrelPar->fGaTableSize) { // what to do if we are at the end of the GaTable
						Int_t last;
						Double_t dtTable = 0;
						// if we are accidentially at the botom of the table, take as a last written one top of it
						// else took the last (previous) one
						if (fGaLast == 0) last = fCorrelPar->fGaTableSize - 1; 
						else last = fGaLast - 1;
						// Evaluate time within gamma table as a difference of the new one and last one
						dtTable = fGaTable[last].fTimeX[fGaTable[last].fIx] - fGaTable[fGaLast].fTimeX[fGaTable[fGaLast].fIx]; 
						if (dtTable < fGaTableTimeLimit) {
							cout << "\n!!! Gamma Table too small !!! INCREASE TABLE SIZE !!!\n";
							cout << "Time needed: " << fGaTableTimeLimit << " ms \t time within GaTable: " << dtTable << " ms\n";
						}
					}
				}  //if ((fCorrelPar->fbCorGaAl) ||...
				}	//  if ((fInpEvent->fE_mev == 0) &&
				// -------------------END OF PROCESSING OF FIRST GAMMA -------------------



				//else {cout << "Thrown gamma: E_mev " << fInpEvent->fE_mev <<" E_keV "<< fInpEvent->fE_kev << " Eb_keV " << fInpEvent->fEb_kev << " Clovers: " << fInpEvent->fegl1 << " "<< fInpEvent->fegl2<< " "<<fInpEvent->fegl3<< " "<< fInpEvent->fegl4<<endl;}
				// }
	} // if ((fCorrelPar->fbCorGaAl) || (fCorrelPar->fbCorGaFi) ||
}

// ==========================================================
// here the input event is being analysed and output event (which
// comes as a parameter) is filled

void MoDSSCorrelProc::MoDSSCorrelationSearch(MoDSSCorrelEvent* pOutEvent) 
{

	
	Bool_t debugCP = kFALSE;
	// get input event from the previous (ANL) step
	// values are then accessible as: fInpEvent->fxxxx
	//cout << "I have these parameters: AlFi "<<fCorrelPar->fbCorAlFi<< " ReGa: "<<fCorrelPar->fbCorReGa << " AlGa: " << fCorrelPar->fbCorAlGa << " SF pause " << fCorrelPar->fbFiPause << " Ga pause " << fCorrelPar->fbGaPause<< " GaEl: " << fCorrelPar->fGaEl << " GaEh: "<< fCorrelPar->fGaEh << " ElectronEl " << fCorrelPar->fElectronEl << " ElectronEh "<< fCorrelPar->fElectronEh << " GaTl: "<< fCorrelPar->fGaTl << " GaTh: "<<fCorrelPar->fGaTh <<endl;

	// output events are not stored until kTRUE is set
	fInpEvent  = (MoDSSAnlEvent*) GetInputEvent();
	if(!fInpEvent -> IsValid()){
		pOutEvent->SetValid(kFALSE); // set the invalidity for the further steps. In correlation routine it's not necessary but anyway...
		//cout << "Correl part: Rejecting event"<<endl;
		return;
	}
	else {
		pOutEvent->SetValid(kTRUE);
	}
	// --------------------- process event ---------------------

	//Debug cut for time (be careful that it's commented ;-)
	//if ((fInpEvent->fTimeX[fInpEvent->fIx]>297692.5) && (fInpEvent->fTimeX[fInpEvent->fIx]<303410) && ((fInpEvent->fDet_index==0)||(fInpEvent->fDet_index==13))) 

	// ----- 1. Recoil (tof signal present, antiveto, energy > 50 keV, energy MeV > 10 MeV)
	// Be carefull!!! This contition is for calibrated MeV range only
	// In other case it may cause the overflow of the recoil.

	//cout<<fInpEvent->fB_tof<<"\t"<<fInpEvent->fE_kevX[fInpEvent->fIx]<<"\t"<<fInpEvent->fE_kevY[fInpEvent->fIy]<<endl;
	ix=0;
	iy=0;
	fepsT = 0.000603821; //See on fhdTXY the peak is centered at 0.00012662 and drops in the noise at 0.000603821
	fepsE = 400;
	fThresEx = 400;
	fThresEy = 1300;
		

	//if(fInpEvent->fE_kevX[fInpEvent->fIx]>5500 && !fInpEvent->fB_tof)cout<<"Enter Correl - "<<fInpEvent->fEventNum<<" - "<<fInpEvent->fE_kevX[fInpEvent->fIx]<<" - "<<fInpEvent->fB_tof<<" - "<<fInpEvent->fTimeX[fInpEvent->fIx]<<endl;
	//cout<<"Enter Correl - "<<fInpEvent->fEventNum<<" - mx "<<fInpEvent->fMultX<<" - EX "<<fInpEvent->fE_kevX[0]<<" - tof "<<fInpEvent->fB_tof<<" - Tx "<<fInpEvent->fTimeX[0]<<endl;
	//cout<<"Enter Correl - "<<fInpEvent->fEventNum<<" - my "<<fInpEvent->fMultY<<" - EY "<<fInpEvent->fE_kevY[0]<<" - tof "<<fInpEvent->fB_tof<<" - TY "<<fInpEvent->fTimeY[0]<<endl;
	// ----- 1. Recoil (tof, energy > Threshold)
	if (fInpEvent->fB_tof){

		if(fInpEvent->fMultX == 1 &&  fInpEvent->fMultY == 1){

			//if(TMath::Abs(fInpEvent->fE_kevY[0]-fInpEvent->fE_kevX[0])<fepsE){
				fhEY_Single->Fill(fInpEvent->fE_kevY[0]);
				fhEX_Single->Fill(fInpEvent->fE_kevX[0]);
				fhdTXY->Fill(TMath::Abs(fInpEvent->fTimeX[0]-fInpEvent->fTimeY[0]));
				fInpEvent->fIx=0;
				fInpEvent->fIy=0;
				fhExEy->Fill(fInpEvent->fE_kevX[0],fInpEvent->fE_kevY[0]);
				//fdReInput_EyTOF->Fill(fInpEvent->fTOF,fInpEvent->fE_kevY[0]);
				ProcessRecoil();
			//}
		}//End if multiplicity 1
	}//End if Recoil
	// ----- 2. Alpha (antitof,energy > 10 keV)
	if (!fInpEvent->fB_tof){

		if(fInpEvent->fMultX == 1 &&  fInpEvent->fMultY == 1){
			//if(TMath::Abs(fInpEvent->fE_kevY[0]-fInpEvent->fE_kevX[0])<fepsE){
				fhEY_Single->Fill(fInpEvent->fE_kevY[0]);
				fhEX_Single->Fill(fInpEvent->fE_kevX[0]);
				fhdTXY->Fill(TMath::Abs(fInpEvent->fTimeX[0]-fInpEvent->fTimeY[0]));
				fInpEvent->fIx=0;
				fInpEvent->fIy=0;
				fhExEy->Fill(fInpEvent->fE_kevX[0],fInpEvent->fE_kevY[0]);
				ProcessAlpha();
			//}
		}//End if multiplicity 1
	}//End if alpha

	// ----- 3. Gamma (antitof, antiveto, not during TMP[4] condition, energy_gamma > 10 keV)
	//General conditions
	if ((!(fCorrelPar->fbGaPause) && !(fInpEvent->fB_tof)) ) {
		//at least one crystal fires (coincidence allowed)
		if (!fCorrelPar->fbGammaHigh){
			if ((fCorrelPar->fbDetAtLeastOne) && GammaSingleFired()){
				if ((debugCP) && (fInpEvent->fE_kevX[fInpEvent->fIx]>30)){
					cout << "MoDSSCorrelProc: processing gamma-electron with a gamma energy sum "
					<<SumGamma()
					<<" in part DetAtLeastOne"<<endl;
				}
				for(int i=0; i<20; i++){
					if(fInpEvent->fegl[i]>0) fInpEvent->fMultGe++;
				}
				fhGammaMult->Fill(fInpEvent->fMultGe);

				ProcessGamma();

			}
			// only one crystal fires (no coincidence allowed)
			if ((fCorrelPar->fbDetOnlyOne) && GammaOnlySingleFired()){
				if ((debugCP) && (fInpEvent->fE_kevX[fInpEvent->fIx]>30)){ 
					cout << "MoDSSCorrelProc: Processing gamma-electron with a gamma energy sum "
					<<SumGamma()<<
					" in part DetOnlyOne"<<endl;
				}
					ProcessGamma();
				
			}

			// only coincidences of diagonal crystals are allowed
			if ((fCorrelPar->fbDetDiagonal) && GammaDiagFired()){
				if ((debugCP) && (fInpEvent->fE_kevX[fInpEvent->fIx]>30)){
					cout << "MoDSSCorrelProc: Processing gamma-electron with a gamma energy sum "
					<<SumGamma()
					<<" in part DetDiagonal"<<endl;
				}
					ProcessGamma();
				
			}
		} //if (!fbGammaHigh)
	}

	//else{cout << "gamma out" << fInpEvent->fegl1 << "  "<<  fInpEvent->fegl2 << "  "<<  fInpEvent->fegl3 << "  "<<  fInpEvent->fegl4 <<endl;} 

	// ----- 4. Fission (antitof, antiveto, not during TMP[4] condition, energy_mev > 20 MeV)
	// see comments few lines above about the calibration.
	if (!(fInpEvent->fB_tof) && (fInpEvent->fE_kevY[fInpEvent->fIy]> 20000)) {
		//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "MoDSSCorrelationSearch: Test SF event... step 1 "<<endl;
		ProcessFission();
		//if ((fInpEvent->fTimeX[fInpEvent->fIx] > 1404796.65E3) && (fInpEvent->fTimeX[fInpEvent->fIx] < 1404796.9E3) && (fInpEvent->fDet_index==15) && (fInpEvent->fegl4 > 880)) cout << "MoDSSCorrelationSearch: Test SF event... step 2 "<<endl;
	}
} // MoDSSCorrelationSearch()

Bool_t 	MoDSSCorrelProc::GammaSingleFired(){
	for(int i=0; i<20; i++){
		if((fInpEvent->fegl[i] > fCorrelPar->fGaEl) && (fInpEvent->fegl[i] < fCorrelPar->fGaEh)){ 
			return kTRUE;
		}
	}
	return kFALSE;
}


Double_t MoDSSCorrelProc::SumGamma(){
	Double_t Sum=0;	
	for(int i=0; i<20; i++){
		if((fInpEvent->fegl[i] > fCorrelPar->fGaEl) && (fInpEvent->fegl[i] < fCorrelPar->fGaEh)) {
			Sum+=fInpEvent->fegl[i];
		}
	}
	return Sum;
}	

Bool_t 	MoDSSCorrelProc::GammaOnlySingleFired(){
	Bool_t Hit=false;
	for(int i=0; i<20; i++){
		if((fInpEvent->fegl[i] > fCorrelPar->fGaEl) && (fInpEvent->fegl[i] < fCorrelPar->fGaEh)){
			if(Hit) return true;
			else Hit = true;
		}
	}
	return false;
}

Bool_t 	MoDSSCorrelProc::GammaDiagFired(){
	for(int i=0; i<5; i++){
		if((fInpEvent->fegl[i*4] > fCorrelPar->fGaEl) && (fInpEvent->fegl[i*4] < fCorrelPar->fGaEh) && (fInpEvent->fegl[i*4+2] > fCorrelPar->fGaEl) && (fInpEvent->fegl[i*4+2] < fCorrelPar->fGaEh)) return true;
		if((fInpEvent->fegl[i*4+1] > fCorrelPar->fGaEl) && (fInpEvent->fegl[i*4+1] < fCorrelPar->fGaEh) && (fInpEvent->fegl[i*4+3] > fCorrelPar->fGaEl) && (fInpEvent->fegl[i*4+3] < fCorrelPar->fGaEh)) return true;
	}
	return false;
}


// =================================================================
ClassImp(MoDSSCorrelProc)

