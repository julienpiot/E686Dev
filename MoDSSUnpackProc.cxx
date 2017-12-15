// N.Kurz, EE, GSI, 15-Jan-2010

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
#include "MoDSSUnpackProc.h"
#include "MoDSSUnpackEvent.h"
#include "MoDSSFilterParameter.h"
string inputfilename_marija;

#ifdef USE_MBS_PARAM
static UInt_t    l_tr    [MAX_TRACE_SIZE];
static Double_t  f_tr_blr[MAX_TRACE_SIZE];
static UInt_t    l_sfp_slaves[MAX_SFP] = {MAX_SLAVE, MAX_SLAVE, MAX_SLAVE, MAX_SLAVE};
static UInt_t    l_slaves=0;
static UInt_t    l_trace=0;
static UInt_t    l_e_filt=0;
#else
static UInt_t    l_tr    [TRACE_SIZE];
static Double_t  f_tr_blr[TRACE_SIZE];
static UInt_t    l_sfp_slaves  [MAX_SFP] = NR_SLAVES;
static UInt_t    l_sfp_adc_type[MAX_SFP] = ADC_TYPE;
#endif //

static UInt_t    l_adc_type;

static UInt_t    l_evt_ct=0;
static UInt_t    l_evt_ct_phys=0;
static UInt_t    l_good_energy_ct=0;
static UInt_t    l_1_hit_ct=0;
static UInt_t    l_more_1_hit_ct=0;
static UInt_t    l_more_1_hit_first_energy_ok_ct=0;
static UInt_t    l_eeeeee_ct=0;
static UInt_t    l_eeeee1_ct=0;
static UInt_t    l_eeeee2_ct=0;
static UInt_t    l_eeeee3_ct=0;
static UInt_t    l_1_hit_and_eeeeee_ct=0;
static UInt_t    l_more_1_hit_and_eeeeee_ct=0;

static UInt_t    l_e_filt_out_of_trig_wind_ct=0;

static UInt_t    l_first=0;

//***********************************************************
MoDSSUnpackProc::MoDSSUnpackProc() : TGo4EventProcessor("Proc")
{
	cout << "**** MoDSSUnpackProc: Create instance " << endl;

	//MWD Filter parameter
	pFil = (MoDSSFilterParameter*)  GetParameter("MoDSSFilterPar");
	if(!pFil)
	{
		pFil = new MoDSSFilterParameter("MoDSSFilterPar");
		AddParameter(pFil);
		// if Filter parameter set exists - print it out, otherwise warn the user.
		if (pFil) pFil->PrintParameter(0,0);
		else cout << "***** ERR - FilterParam doesn't exist - program will crash.\n";
	}
	if (pFil) pFil->PrintParameter(0,0);
	else cout << "***** ERR - FilterParam doesn't exist - program will crash.\n";


}
//***********************************************************
MoDSSUnpackProc::~MoDSSUnpackProc()
{
	l_first=0;
	cout << "**** Setting l_first = 0 " << endl;
	cout << "**** MoDSSUnpackProc: Delete instance " << endl;
}
//***********************************************************
// this one is used in standard factory
MoDSSUnpackProc::MoDSSUnpackProc(const char* name) : TGo4EventProcessor(name)
{
	//MWD Filter parameter
	pFil = (MoDSSFilterParameter*)  GetParameter("MoDSSFilterPar");
	if(!pFil)
	{
		pFil = new MoDSSFilterParameter("MoDSSFilterPar");
		AddParameter(pFil);
		// if Filter parameter set exists - print it out, otherwise warn the user.
		if (pFil) pFil->PrintParameter(0,0);
		else cout << "***** ERR - FilterParam doesn't exist - program will crash.\n";
	}
	if (pFil) pFil->PrintParameter(0,0);

	cout << "**** MoDSSUnpackProc: Create instance " << name << endl;

	//printf ("Histograms created \n");  fflush (stdout);
}
//-----------------------------------------------------------
// event function
Bool_t MoDSSUnpackProc::BuildEvent(TGo4EventElement* target)
{  // called by framework. We dont fill any output event here at all

	//output event for the next step
	//cout<<"+++++++++++++++++++++++++++++++++++++++++++++"<<endl<<"Begin BuildEvent()"<<endl;
	fOutput=dynamic_cast<MoDSSUnpackEvent*>(target);
	if(fOutput==0){
		cout << "wrong event type in buildEvent!!!!!!!!!!!!!";
		return false;
	}
	//cout<<"Unpack.BuildEvent() - GetInputEvent()"<<endl;
	fInput = (TGo4MbsEvent* ) GetInputEvent();

	//Reset X and Y hits counters
	c_X=0;
	c_Y=0;
	UInt_t         l_dat_fir;
	UInt_t         l_dat_sec;
	//Initialize values for MWD
	k = 60;
	m = 60;
	c = k+m;
	Sum = 0;
	dX = 0;
	//Initialize variables for bl restoration
	l_bls_start = BASE_LINE_SUBT_START;
	l_bls_stop  = BASE_LINE_SUBT_START + BASE_LINE_SUBT_SIZE;

	//reset data variables
	l_hit_cha_id = 0;
	ll_trg_time = 0.;
	f_bls_val=0.;

	Long64_t Dll_time = 0.;
	//
	// calibration parameter - contains all calibration values == COPIED from ANL
	//uCal = new MoDSSCalibParameter("CalibPar");
	//AddParameter(uCal);
	// fCal = (MoDSSCalibParameter*) GetParameter("CalibPar");
	//if (uCal) uCal->PrintParameter(0,0);
	//else cout << "**** ERRR - CalibPar doesn't exist - program will crash.\n";

	if(fInput == 0)
	{
		cout << "UnpackProc: no input event !"<< endl;
		return kFALSE;
	}
	//cout<<"Read InputEvent"<<endl;

	l_trig_type_triva = fInput->GetTrigger();
	//cout<<"Unpack.BuildEvent() - Trigger = "<<l_trig_type_triva<<endl;
	if (l_trig_type_triva == 1)
	{
		l_evt_ct_phys++;
	}

	l_evt_ct++;

	//JULIEN - Check the number of subevents

	/*fInput->ResetIterator();
	  subcount=0;
	  fInput->ResetIterator();
	  while((psubevt = fInput->NextSubEvent())!=0){ // only one subevent
	  subcount++;
	  }
	  std::cout<<"Found "<<subcount<<" subevents"<<endl;*/
	//



	fInput->ResetIterator();

	//cout<<"Unpack.BuildEvent() - fInput->NextSubEvent()"<<endl;
	psubevt = fInput->NextSubEvent(); // only one subevent
	fOutput->event_number=fInput->GetCount();
	//cout<<"- Event Number: "<<fOutput->event_number<<endl;
	//cout<<"Unpack.BuildEvent() - fInput->GetDataField()"<<endl;
	//cout<<"psubevt = "<<psubevt<<endl;
	if(psubevt != 0){
		//cout<<"DataField not NULL"<<endl;
		pl_se_dat = (uint32_t *)psubevt->GetDataField();
		//cout<<"------------ "<<endl<<"psubevt = "<<psubevt<<endl;
		//cout<<"DataField = "<<psubevt->GetDataField()<<endl;
		//cout<<"pl_se_dat = "<<pl_se_dat<<endl;
		//psubevt->PrintEvent();
	}else{
		cout<<"There is nothing there ! Break !"<<endl;
		goto bad_event;
	}
	//cout<<hex<<"Unpack.BuildEvent() - pl_se_dat :"<<endl<<pl_se_dat<<endl;
	l_dat_len = psubevt->GetDlen();
	l_dat_len_byte = (l_dat_len - 2) * 2;

	pl_tmp = pl_se_dat;

	if (pl_se_dat == (UInt_t*)0)
	{
		printf ("ERROR>> ");
		printf ("pl_se_dat: 0x%x, ", pl_se_dat);
		printf ("l_dat_len: 0x%x, ", (UInt_t)l_dat_len);
		printf ("l_trig_type_triva: 0x%x \n", (UInt_t)l_trig_type_triva); fflush (stdout);
		goto bad_event;
	}

	if ( (*pl_tmp) == 0xbad00bad)
	{
		printf ("ERROR>> found bad event (0xbad00bad) \n");
		goto bad_event;
	}

#ifdef WR_TIME_STAMP
	// 4 first 32 bits must be TITRIS time stamp
	l_dat = *pl_tmp++;
	if (l_dat != SUB_SYSTEM_ID)
	{
		printf ("ERROR>> 1. data word is not sub-system id: %d \n");
		printf ("should be: 0x%x, but is: 0x%x\n", SUB_SYSTEM_ID, l_dat);
	}

	if (l_dat != 0x100)
	{
		cout<<"why am I here ?"<<endl;
		goto bad_event;
	}

	l_dat = (*pl_tmp++) >> 16;
	//printf("TS_ID_low %dx \n ", l_dat);
	if (l_dat != TS__ID_L16)
	{
		printf ("ERROR>> 2. data word does not contain low 16bit identifier: %d \n");
		printf ("should be: 0x%x, but is: 0x%x\n", TS__ID_L16, l_dat);
	}
	l_dat = (*pl_tmp++) >> 16;
	//printf("TS_ID_middle %dx \n ", l_dat);
	if (l_dat != TS__ID_M16)
	{
		printf ("ERROR>> 3. data word does not contain low 16bit identifier: %d \n");
		printf ("should be: 0x%x, but is: 0x%x\n", TS__ID_M16, l_dat);
	}
	l_dat = (*pl_tmp++) >> 16;
	if (l_dat != TS__ID_H16)
	{
		printf ("ERROR>> 4. data word does not contain low 16bit identifier: %d \n");
		printf ("should be: 0x%x, but is: 0x%x\n", TS__ID_H16, l_dat);
	}
#endif // WR_TIME_STAMP

	// extract analysis parameters from MBS data
	// ATTENTION:  these data is only present if WRITE_ANALYSIS_PARAM
	//             is enabled in corresponding f_user.c
	// WRITE_ANALYSIS_PARAM (in mbs) and USE_MBS_PARAM (in go4) must be used always together

#ifdef USE_MBS_PARAM
	l_slaves = *pl_tmp++;
	l_trace  = *pl_tmp++;
	l_e_filt = *pl_tmp++;
	pl_tmp  += 4;
#endif

	if (l_first == 0)
	{
		l_first = 1;
#ifdef USE_MBS_PARAM
		//printf ("debug: 0x%x, 0x%x, 0x%x \n", l_slaves, l_trace, l_e_filt);
		//fflush (stdout);
#endif
		f_make_histo (0);
	}

	//printf ("                     - debug: 0x%x, 0x%x, 0x%x \n", l_slaves, l_trace, l_e_filt);
	//fflush (stdout);
	for (l_i=0; l_i<MAX_SFP; l_i++)
	{
		if (l_sfp_slaves[l_i] != 0)
		{
			for (l_j=0; l_j<l_sfp_slaves[l_i]; l_j++)
			{
				for (l_k=0; l_k<N_CHA; l_k++)
				{
					//h_trace       [l_i][l_j][l_k]->Reset ("");
					//h_trace_blr   [l_i][l_j][l_k]->Reset ("");
				}
			}
		}
	}

	while ( (pl_tmp - pl_se_dat) < (l_dat_len_byte/4) )
	{
		//sleep (1);
		//printf ("Unpack.BuildEvent() - begin while loop \n");  fflush (stdout);
		l_dat = *pl_tmp++;   // must be padding word or channel header
		//printf ("                    - l_dat 0x%x \n", l_dat);
		//cout<<hex<<"                    - l_dat 0x"<<l_dat<<endl;
		if ( (l_dat & 0xfff00000) == 0xadd00000 ) // begin of padding 4 byte words
		{
			//printf ("padding found \n");
			l_dat = (l_dat & 0xff00) >> 8;
			pl_tmp += l_dat - 1;  // increment by pointer by nr. of padding  4byte words
		}
		else if ( (l_dat & 0xff) == 0x34) //channel header
		{
			l_cha_head = l_dat;
			//printf ("Unpack.BuildEvent() - l_cha_head: 0x%x \n", l_cha_head);


			l_trig_type = (l_cha_head & 0xf00)      >>  8;
			l_sfp_id    = (l_cha_head & 0xf000)     >> 12;
			l_feb_id    = (l_cha_head & 0xff0000)   >> 16;
			l_cha_id    = (l_cha_head & 0xff000000) >> 24;


			if ((l_sfp_id > (MAX_SFP-1)) || (l_sfp_id < 0))
			{
				printf ("ERROR>> l_spf_id: %d \n", l_sfp_id);  fflush (stdout);
				goto bad_event;
			}
			if ((l_feb_id > (MAX_SLAVE-1)) || (l_feb_id < 0))
			{
				printf ("ERROR>> l_feb_id: %d \n", l_feb_id); fflush (stdout);
				goto bad_event;
			}
			if ((l_cha_id > (N_CHA-1)) || (l_cha_id < 0))
			{
				if (l_cha_id != 0xff)
				{
					printf ("ERROR>> l_cha_id: %d \n", l_cha_id); fflush (stdout);
					goto bad_event;
				}
			}

			if ( ((l_cha_head & 0xff) >> 0) != 0x34 )
			{
				printf ("ERROR>> channel header type is not 0x34 \n");
				goto bad_event;
			}

			if ( (l_cha_head & 0xff000000) == 0xff000000) // special channel 0xff for E,t from fpga
			{
				//printf ("                   - summary channel \n");
				// special channel data size
				l_cha_size = *pl_tmp++;
				//printf ("l_cha_head: 0x%x \n", l_cha_head); sleep (1);
				//printf ("l_cha_size: 0x%x \n", l_cha_size);

				l_spec_head = *pl_tmp++;
				if ( (l_spec_head & 0xff000000) != 0xaf000000)
				{
					printf ("ERROR>> E,t summary: wrong header is 0x%x, must be: 0x%x\n",
							(l_spec_head & 0xff000000)>>24, 0xaf);
					goto bad_event;
					//sleep (1);
				}




				//Baatar time
				ll_trg_time  = (Long64_t)*pl_tmp++;
				//cout << " trig_time " << ll_trg_time << endl;
				ll_time      = (Long64_t)*pl_tmp++;
				ll_time += ((ll_trg_time & 0xffffff) << 32);
				//cout << " sample time " << ll_time << endl;
				//	ll_trg_time += ((ll_time & 0xffffff) << 32);
				//	cout << " added time " << ll_trg_time << endl;
				//Bataar correction of time gives negative time output
				/* ll_trg_time  = (Long64_t)*pl_tmp++;
				   ll_time      = (Long64_t)*pl_tmp++;
				//                     printf(" ll_time %llx dec %lld \n ", ll_time, ll_time);
				ll_time += ((ll_trg_time & 0xffffff) << 32);
				//                   printf("TS_ID_low %llx and ll_time %llx \n ", ll_trg_time, ll_time);*/


				l_n_hit = (l_cha_size - 16) >> 3;
				//printf ("#hits: %d \n", l_n_hit);

				//Physics Event
				if (l_trig_type_triva == 1)
				{
					//cout<<"Physics Event trigger"<<endl;
					// h_hitpat[l_sfp_id][l_feb_id]->Fill (-1, 1);

					//Loop on the number of hits
					for (l_i=0; l_i<l_n_hit; l_i++)
					{
						//cout<<"Loop on hits"<<endl;
						l_dat = *pl_tmp++;      // hit time from fpga (+ other info)
						l_hit_cha_id              = (l_dat & 0xf0000000) >> 28;
						l_n_hit_in_cha            = (l_dat & 0xf000000)  >> 24;

						l_more_than_1_hit_in_cha  = (l_dat & 0x400000)   >> 22;
						l_e_filt_out_of_trig_wind = (l_dat & 0x200000)   >> 21;
						l_only_one_hit_in_cha     = (l_dat & 0x100000)   >> 20;

						l_ch_hitpat[l_sfp_id][l_feb_id][l_hit_cha_id] = l_n_hit_in_cha;

						if (l_only_one_hit_in_cha == 1)
						{
							l_1_hit_ct++;
						}

						if (l_more_than_1_hit_in_cha == 1)
						{
							l_more_1_hit_ct++;
							//printf ("%d More than 1 hit found for SFP: %d FEBEX: %d CHA: %d:: %d \n",
							//l_more_1_hit_ct, l_sfp_id, l_feb_id, l_hit_cha_id, l_n_hit_in_cha);
							//fflush (stdout);
						}
						if (l_e_filt_out_of_trig_wind == 1)
						{
							l_e_filt_out_of_trig_wind_ct++;
							printf ("%d energy filter out of trigger window <<<<<<<<<<<<<<<<<<<<<<\n", l_e_filt_out_of_trig_wind_ct);
						}

						if ((l_more_than_1_hit_in_cha == 1) && (l_only_one_hit_in_cha == 1))
						{
							printf ("ERROR>> haeh? \n"); fflush (stdout);
						}

						if (l_only_one_hit_in_cha == 1)
						{

							l_hit_time_sign[l_sfp_id][l_feb_id][l_hit_cha_id] = (l_dat & 0x8000) >> 15;
							l_hit_time[l_sfp_id][l_feb_id][l_hit_cha_id] = l_dat & 0x7ff;     // positive := AFTER  trigger, relative to trigger time
							if (l_hit_time_sign[l_sfp_id][l_feb_id][l_hit_cha_id] == 1)       // negative sign
							{
								l_hit_time[l_sfp_id][l_feb_id][l_hit_cha_id] = l_hit_time[l_sfp_id][l_feb_id][l_hit_cha_id] * (-1); // negative := BEFORE trigger, relative to trigger time
							}
							//printf ("cha: %d, hit fpga time:  %d \n", l_hit_cha_id,  l_hit_time);
							//  h_trgti_hitti[l_sfp_id][l_feb_id][l_hit_cha_id]->Fill (l_hit_time);
							//h_hitpat[l_sfp_id][l_feb_id]->Fill (l_hit_cha_id, 1);
						}
						// h_hitpat[l_sfp_id][l_feb_id]->Fill (l_hit_cha_id, l_n_hit_in_cha);

						l_dat = *pl_tmp++;      // energy from fpga (+ other info)
						l_hit_cha_id2  = (l_dat & 0xf0000000) >> 28;

						if ( (l_dat & 0xfffff0) != 0xeeeee0)
						{
							//cout<<"0xeeeee0"<<endl;
							if (l_hit_cha_id != l_hit_cha_id2)
							{
								printf ("ERROR>> hit channel ids differ in energy and time data word\n");
								goto bad_event;
							}
							if ((l_hit_cha_id > (N_CHA-1)) || (l_hit_cha_id < 0))
							{
								printf ("ERROR>> hit channel id: %d \n", l_hit_cha_id); fflush (stdout);
								goto bad_event;
							}

							if (l_more_than_1_hit_in_cha == 1)
							{
								l_more_1_hit_first_energy_ok_ct++;
							}

							if ( (l_only_one_hit_in_cha == 1) || (l_more_than_1_hit_in_cha == 1) )
							{
								//cout<<"on one hit"<<endl;
								l_good_energy_ct++;
								l_fpga_energy_sign = (l_dat & 0x800000) >> 23;
								//l_fpga_energy      =  l_dat & 0x7ffff;      // positiv
								l_fpga_energy      =  l_dat & 0x3fffff;     // positiv
								if (l_fpga_energy_sign == 1)                // negative sign
								{
									l_fpga_energy = l_fpga_energy * (-1);     // negative
								}
								//printf ("cha: %d, hit fpga energy: %d \n", l_hit_cha_id2,  l_fpga_energy);
								//printf ("sfp: %d, feb: %d, cha: %d \n", l_sfp_id, l_feb_id, l_hit_cha_id);

								//FPGA energy extraction positive polarity signals
								//DSSD X
								if(l_feb_id==0 || l_feb_id==1 || l_feb_id==2 || l_feb_id==3 ) {

									h_fpga_e[l_sfp_id][l_feb_id][l_hit_cha_id]->Fill (l_fpga_energy/60);
									//fOutput->l_E[l_feb_id][l_hit_cha_id] =(l_fpga_energy/60);
									//if(l_feb_id==6){
									//  cout << "Energy " << -l_fpga_energy/60 <<endl;
									//}
								}

								//FPGA energy extraction negative polarity signals
								//DSSD Y
								if(l_feb_id==4 || l_feb_id==5 || l_feb_id==6 || l_feb_id==7 ){

									h_fpga_e[l_sfp_id][l_feb_id][l_hit_cha_id]->Fill (-l_fpga_energy/60);
									//fOutput->l_E[l_feb_id][l_hit_cha_id] =(-l_fpga_energy/60);
								}
								//FPGA energy extraction positive polarity signals
								//Box
								if( l_feb_id ==8 || l_feb_id == 9  ){

									h_fpga_e[l_sfp_id][l_feb_id][l_hit_cha_id]->Fill (l_fpga_energy/60);
									//fOutput->l_E[l_feb_id][l_hit_cha_id] =(l_fpga_energy/60);
								}


								//FPGA energy extraction negative polarity signals
								//Germanium
								if(  (l_feb_id==11 && l_hit_cha_id<4) || l_feb_id==10){

									h_fpga_e[l_sfp_id][l_feb_id][l_hit_cha_id]->Fill (-l_fpga_energy/20);
									//fOutput->l_E[l_feb_id][l_hit_cha_id] =(-l_fpga_energy/20);
								}

								//Timing and diagnostics - NOT TREATED HERE - SEE BELOW
								/*if(l_feb_id==11 && l_hit_cha_id>4) {
								  h_fpga_e[l_sfp_id][l_feb_id][l_hit_cha_id]->Fill (-l_fpga_energy/80);
								  fOutput->l_E[l_feb_id][l_hit_cha_id] = (-l_fpga_energy/80);
								  }*/




								if (l_fpga_energy < -1000000)
								{
									//printf ("l_dat: 0x%x, fpga energy: 0x%x \n", l_dat, l_fpga_energy);
								}

								l_fpga_e_found [l_sfp_id][l_feb_id][l_hit_cha_id] = 1;
								l_fpga_e[l_sfp_id][l_feb_id][l_hit_cha_id] = l_fpga_energy;
							}
						}
						else
						{
							l_eeeeee_ct++;
							if (l_dat == 0xeeeee1) {l_eeeee1_ct++;}
							if (l_dat == 0xeeeee2) {l_eeeee2_ct++;}
							if (l_dat == 0xeeeee3) {l_eeeee3_ct++;}

							if (l_only_one_hit_in_cha    == 1) {l_1_hit_and_eeeeee_ct++;}
							if (l_more_than_1_hit_in_cha == 1) {l_more_1_hit_and_eeeeee_ct++;}

							//printf ("%d 0xeeeeee found \n", l_eeeeee_ct);
						}
					}
					//cout<<"out loop on hits"<<endl;
				}
				l_spec_trail = *pl_tmp++;
				if ( (l_spec_trail & 0xff000000) != 0xbf000000)
				{
					printf ("ERROR>> E,t summary: wrong header is 0x%x, must be: 0x%x\n",
							(l_spec_trail & 0xff000000)>>24, 0xbf);
					goto bad_event;
					//sleep (1);
				}
			}
			else // real channel
			{

				//printf ("                    - real channel \n");
				// channel data size
				l_cha_size = *pl_tmp++;

				// trace header
				l_trace_head = *pl_tmp++;
				//printf ("trace header \n");
				if ( ((l_trace_head & 0xff000000) >> 24) != 0xaa)
				{
					printf ("ERROR>> trace header id is not 0xaa \n");
					goto bad_event;
				}

				l_fpga_filt_on_off = (l_trace_head & 0x80000) >> 19;
				l_fpga_filt_mode   = (l_trace_head & 0x40000) >> 18;
				//printf ("fpga filter on bit: %d, fpga filter mode: %d \n", l_fpga_filt_on_off, l_fpga_filt_mode);
				fflush (stdout);
				//sleep (1);

				if (l_trig_type == 1) // physics event
				{
					//cout<<"Physics Event"<<endl;
					if (l_first_trace[l_sfp_id][l_feb_id] == 0)
					{
						l_first_trace[l_sfp_id][l_feb_id] = 1;
						//h_hitpat_tr[l_sfp_id][l_feb_id]->Fill (-1, 1);
					}
					//  h_hitpat_tr[l_sfp_id][l_feb_id]->Fill (l_cha_id, 1);
					l_ch_hitpat_tr[l_sfp_id][l_feb_id][l_cha_id]++;
					//cout<<"Get trace"<<endl;
					// now trace
					l_trace_size = (l_cha_size/4) - 2;     // in longs/32bit

					if (l_fpga_filt_on_off == 0) // only trace. no fpga filter trace data
					{
						//cout<<"only trace, no fpga"<<endl;
						for (l_l=0; l_l<l_trace_size; l_l++)   // loop over traces
						{
							//cout<<"in loop over trace size"<<endl;
							// disentangle data
							l_dat_fir = *pl_tmp++;
							l_dat_sec = l_dat_fir;

#ifdef USE_MBS_PARAM
							l_adc_type = (l_trace_head & 0x800000) >> 23;
#else
							l_adc_type = (l_sfp_adc_type[l_sfp_id] >> l_feb_id) & 0x1;
#endif

							if (l_adc_type == 0) // 12 bit
							{
								l_dat_fir =  l_dat_fir        & 0xfff;
								l_dat_sec = (l_dat_sec >> 16) & 0xfff;
							}

							if (l_adc_type == 1)  // 14 bit
							{
								l_dat_fir =  l_dat_fir        & 0x3fff;
								l_dat_sec = (l_dat_sec >> 16) & 0x3fff;
							}
							// if(fOutput->event_number==1272628){
							h_trace[l_sfp_id][l_feb_id][l_cha_id]->SetBinContent (l_l*2  +1, l_dat_fir);
							h_trace[l_sfp_id][l_feb_id][l_cha_id]->SetBinContent (l_l*2+1+1, l_dat_sec);
							// }

							l_tr[l_l*2]   = l_dat_fir;
							l_tr[l_l*2+1] = l_dat_sec;
						}
						//cout<<"loop done"<<endl;
						l_trace_size = l_trace_size * 2;
					}
					if (l_fpga_filt_on_off == 1) // trace AND fpga filter data (for use if FPGA trapezoids are activated)
					{
						//cout<<"traces & fpga"<<endl;
						for (l_l=0; l_l<(l_trace_size>>1); l_l++)   // loop over traces
						{
							// disentangle data
							l_dat_trace = *pl_tmp++;
							l_dat_filt  = *pl_tmp++;
							l_filt_sign  =  (l_dat_filt & 0x800000) >> 23;

#ifdef USE_MBS_PARAM
							l_adc_type = (l_trace_head & 0x800000) >> 23;
#else
							l_adc_type = (l_sfp_adc_type[l_sfp_id] >> l_feb_id) & 0x1;
#endif

							if (l_adc_type == 0) // 12 bit
							{
								l_dat_trace = l_dat_trace  & 0xfff;
							}

							if (l_adc_type == 1)  // 14 bit
							{
								l_dat_trace = l_dat_trace  & 0x3fff;
							}

							l_dat_filt  = l_dat_filt   & 0x7fffff;
							if (l_filt_sign == 1) {l_dat_filt = l_dat_filt * -1;}

							// if(fOutput->event_number==5089636)
							h_trace_filt      [l_sfp_id][l_feb_id][l_cha_id]->SetBinContent (l_l, l_dat_trace);
							h_trapez_fpga[l_sfp_id][l_feb_id][l_cha_id]->SetBinContent (l_l, l_dat_filt);

							l_tr[l_l] = l_dat_trace;
						}
						l_trace_size = l_trace_size >> 1;
					}
					// find base line value of trace and correct it to baseline 0
					//cout<<"Unpack - Find Baseline"<<endl;
					f_bls_val = 0.;
					for (l_l=l_bls_start; l_l<l_bls_stop; l_l++)
					{
						f_bls_val += (Double_t)l_tr[l_l];
					}
					f_bls_val = f_bls_val / (Double_t)(l_bls_stop - l_bls_start);

					// 					if (h_trace[l_sfp_id][l_feb_id][l_cha_id]->GetBinContent(l_l) > 0 && (l_feb_id ==8 || l_feb_id == 9)){
					// 					cout << "Box is here!!!"<< endl;
					// 					}
					// create baseline restored trace
					for (l_l=0; l_l<l_trace_size; l_l++)
					{
						//Julien - Create trace for positive polarity signal
						if(l_feb_id==0 || l_feb_id==1 || l_feb_id==2 ||l_feb_id==3  || l_feb_id==8 || l_feb_id==9){
							f_tr_blr[l_l] =  (Double_t)l_tr[l_l] - f_bls_val;
							h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->SetBinContent (l_l+1, f_tr_blr[l_l]);
						}
						//Julien - Create trace for negative polarity signal
						if(l_feb_id==4 || l_feb_id==5 || l_feb_id==6 ||l_feb_id==7 || l_feb_id ==10 || l_feb_id ==11 ){
							f_tr_blr[l_l] =  (Double_t)l_tr[l_l] - f_bls_val;
							h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->SetBinContent (l_l+1, - f_tr_blr[l_l]);
						}
					}


					if(l_feb_id==0 ||l_feb_id==1 || l_feb_id==2 || l_feb_id==3){
						fOutput->l_E[l_feb_id][l_cha_id]=h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ();
						h_peak[l_sfp_id][l_feb_id][l_cha_id]->Fill(h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ());
					}
					if(l_feb_id==4 ||l_feb_id==5 || l_feb_id==6 || l_feb_id==7){
						fOutput->l_E[l_feb_id][l_cha_id]=h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ();
						h_peak_back[l_sfp_id][l_feb_id][l_cha_id]->Fill(h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ());
					}
					if(l_feb_id==8 ||l_feb_id==9){
						//fOutput->l_E[l_feb_id][l_cha_id]=h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ();
						h_peak_box[l_sfp_id][l_feb_id][l_cha_id]->Fill(h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ());
					}
					if(l_feb_id==10 ||l_feb_id==11){
						h_peak_gamma[l_sfp_id][l_feb_id][l_cha_id]->Fill(h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ());
					}

					//TOF signals & Diagnostics

					if (l_feb_id == 11 && l_cha_id>=4 && l_cha_id <10){
						fOutput->l_E[l_feb_id][l_cha_id]=h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ();

						h_fpga_e[l_sfp_id][l_feb_id][l_cha_id]->Fill(h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ());
						//fOutput->l_E[l_feb_id][l_cha_id]=h_trace_blr[l_sfp_id][l_feb_id][l_cha_id]->GetMaximum ();
					}

					//cout<<"Unpack - Send data to Event - sfp: "<<l_sfp_id<<" - feb: "<<l_feb_id<<" - l_cha_id: "<<l_cha_id<<endl;

					if(l_feb_id==0){h_hits->Fill(l_cha_id);}
					else if(l_feb_id==1){h_hits->Fill(l_cha_id+16);}
					else if(l_feb_id==2){h_hits->Fill(l_cha_id+32);}
					else if(l_feb_id==3){h_hits->Fill(l_cha_id+48);}
					else if(l_feb_id==4){h_hits->Fill(l_cha_id+64);}
					else if(l_feb_id==5){h_hits->Fill(l_cha_id+80);}
					else if(l_feb_id==6){h_hits->Fill(l_cha_id+96);}
					else if(l_feb_id==7){h_hits->Fill(l_cha_id+112);}
					else if(l_feb_id==8){h_hits->Fill(l_cha_id+130);}
					else if(l_feb_id==9){h_hits->Fill(l_cha_id+146);}
					else if(l_feb_id==10){h_hits->Fill(l_cha_id+164);}
					else if(l_feb_id==11){h_hits->Fill(l_cha_id+180);}


					//Julien - Send data to Unpack Event
					//Commented on 4/1/2017 because l_hit_cha_id should not be used here. It is only defined in the special events !
					//fOutput->l_T[l_feb_id][l_hit_cha_id] =  (Long64_t)l_hit_time[l_sfp_id][l_feb_id][l_hit_cha_id];
					fOutput->l_T[l_feb_id][l_cha_id] =  (Long64_t)l_hit_time[l_sfp_id][l_feb_id][l_cha_id];
					fOutput->l_t_trig = ll_time;

					//Fill the frequency analysis histograms
					Dll_time = ll_time -ll_time_sav;
					fhD_ll_time->Fill(Dll_time);
					if(Dll_time !=0) fhD_Inv_ll_time->Fill(1/Dll_time);
					fhD_ll_trg_time->Fill(ll_trg_time -ll_trg_time_sav);
					fhD_l_hit_time->Fill(l_cha_id+16*l_feb_id,l_hit_time[l_sfp_id][l_feb_id][l_cha_id] -l_hit_time_sav);
					fh_l_hit_time->Fill(l_hit_time[l_sfp_id][l_feb_id][l_cha_id]);
					ll_time_sav = ll_time;
					ll_trg_time_sav = ll_trg_time;
					l_hit_time_sav = (Long64_t)l_hit_time[l_sfp_id][l_feb_id][l_cha_id];

					l_feb_id_sav = l_feb_id;
					l_hit_cha_id_sav = l_hit_cha_id;
					l_cha_id_sav = l_cha_id;
					Ener_sav = Ener;

					if(b_first_event){
						l_hit_time_first =(Long64_t)l_hit_time[l_sfp_id][l_feb_id][l_hit_cha_id];
						b_first_event = kFALSE;
					}

					//	Energy channels - With MWD
					//	Karl Hauschild and Kseniia Rezynkina MWD Code (Mistry edit)

					if(l_sfp_id ==0 && l_feb_id <12 && l_cha_id <16){
						//cout<<"cleanup"<<endl;
						for (l_l=0; l_l<l_trace_size; l_l++) {     //clean up before a new trace
							d[l_l]=0;
							S2[l_l]=0;
							r2[l_l]=0;
							dX=0;

							//copy the trace for the MWD algorithm
							if(l_l<l_trace_size) X[l_l] = h_trace[l_sfp_id][l_feb_id][l_cha_id]->GetBinContent(l_l);
						}

						Sum=0;
						BL=0;
						h_mwd_traps[l_sfp_id][l_feb_id][l_cha_id]->Reset("");
						h_mwd_traps2[l_sfp_id][l_feb_id][l_cha_id]->Reset("");

						if(l_feb_id < 4){//DSSD Front
							//cout<<"MWD Front"<<endl;
							adj=60;
							//Z = X[275]<8500;
							Z=1;
							c= pFil->fk+pFil->fm;
							//cout<<"back var def"<<endl;

							for(l_l=c+pFil->fk+1; l_l<(l_trace_size) && Z; l_l++){     //loop over osc points
								d[l_l] = (X[l_l] - X[l_l-pFil->fk]) - (X[l_l-c] - X[l_l-c-pFil->fk]);
								r2[l_l] = r2[l_l-1] + d[l_l] - exp(-20./pFil->ftau[l_feb_id][l_cha_id])*d[l_l-1];    //e^(-t_sampling/tau)
								S2[l_l] = S2[l_l-1] + r2[l_l]/adj;

								h_mwd_traps[l_sfp_id][l_feb_id][l_cha_id]->Fill(l_l, S2[l_l]);

								//in case you want to average between several points (say 710..720)
								if (l_l>=pFil->fSum_Start && l_l<=(pFil->fSum_Start+pFil->fSum_Av)){
									Sum += S2[l_l];
								}
								if (l_l>=pFil->fBL_Start && l_l<=(pFil->fBL_Start+pFil->fBL_Av)){
									BL += S2[l_l];
									//cout<<"Sum_incre"<<" "<<Sum<<endl;
								}
							}
							//cout<<"back loop done"<<endl;

							Sum=Sum/pFil->fSum_Av;
							//cout<<"X BL= "<<BL<<" pFil->fBL_Av= "<<pFil->fBL_Av<<endl;
							BL=BL/pFil->fBL_Av;
							//cout<<"X - result mwd 1 : "<<Sum-BL<<"Sum= "<<Sum<<" BL= "<<BL<<endl;
							h_mwd_E[l_sfp_id][l_feb_id][l_cha_id]->Fill((Sum-BL));
							//fOutput->l_E[l_feb_id][l_cha_id]= (Sum - BL);
							//cout<<"X - fOutput->l_E["<<l_feb_id<<"]["<<l_cha_id<<"] = "<<fOutput->l_E[l_feb_id][l_cha_id]<<endl;
							//cout<<"back mwd out"<<endl;


												}
						if(l_feb_id > 3 &&l_feb_id <8){//DSSD Back
							//cout<<"MWD Back"<<endl;

							adj=7;
							//Z = X[275]>8500;
							Z=1;
							c= pFil->fk+pFil->fm;
							//cout<<"back var def"<<endl;

							for(l_l=c+pFil->fk+1; l_l<(l_trace_size) && Z; l_l++){     //loop over osc points
								d[l_l] = (X[l_l] - X[l_l-pFil->fk]) - (X[l_l-c] - X[l_l-c-pFil->fk]);
								r2[l_l] = r2[l_l-1] + d[l_l] - exp(-20./pFil->ftau[l_feb_id][l_cha_id])*d[l_l-1];    //e^(-t_sampling/tau)
								S2[l_l] = S2[l_l-1] + r2[l_l]/adj;

								h_mwd_traps[l_sfp_id][l_feb_id][l_cha_id]->Fill(l_l, S2[l_l]);

								//in case you want to average between several points (say 710..720)
								if (l_l>=pFil->fSum_Start && l_l<=(pFil->fSum_Start+pFil->fSum_Av)){
									Sum += S2[l_l];
								}
								if (l_l>=pFil->fBL_Start && l_l<=(pFil->fBL_Start+pFil->fBL_Av)){
									BL += S2[l_l];
									//cout<<"Sum_incre"<<" "<<Sum<<endl;
								}
							}
							//cout<<"back loop done"<<endl;

							Sum=Sum/pFil->fSum_Av;
							BL=BL/pFil->fBL_Av;
							//cout<<"Y - result mwd 1 : "<<Sum-BL<<endl;
							//cout<<"result mwd 1"<<endl;
							//fOutput->l_E[l_feb_id][l_cha_id] =(-1*Sum);/
							//if((S2[350]<-50) && (S2[183]>-10) &&(S2[500]>-10)){
							h_mwd_E[l_sfp_id][l_feb_id][l_cha_id]->Fill(-1*(Sum-BL));
							//fOutput->l_E[l_feb_id][l_cha_id]= -1*(Sum - BL);
							//cout<<"Y - fOutput->l_E["<<l_feb_id<<"]["<<l_cha_id<<"] = "<<fOutput->l_E[l_feb_id][l_cha_id]<<endl;
							//}
							//cout<<"back mwd out"<<endl;

						}
						if(l_feb_id > 7 &&l_feb_id <10){//Box
							//cout<<"MWD Box"<<endl;

							adj=60;
							Z = X[275]<8500;
							c= pFil->fk+pFil->fm;

							for(l_l=c+pFil->fk+1; l_l<(l_trace_size) && Z; l_l++){     //loop over osc points
								d[l_l] = (X[l_l] - X[l_l-pFil->fk]) - (X[l_l-c] - X[l_l-c-pFil->fk]);
								r2[l_l] = r2[l_l-1] + d[l_l] - exp(-20./pFil->ftau[l_feb_id][l_cha_id])*d[l_l-1];    //e^(-t_sampling/tau)
								S2[l_l] = S2[l_l-1] + r2[l_l]/adj;

								h_mwd_traps[l_sfp_id][l_feb_id][l_cha_id]->Fill(l_l, S2[l_l]);

								//in case you want to average between several points (say 710..720)
								if (l_l>=pFil->fSum_Start && l_l<=(pFil->fSum_Start+pFil->fSum_Av)){
									Sum += S2[l_l];
								}
								if (l_l>=pFil->fBL_Start && l_l<=(pFil->fBL_Start+pFil->fBL_Av)){
									BL += S2[l_l];
									//cout<<"Sum_incre"<<" "<<Sum<<endl;
								}
							}

							Sum=Sum/pFil->fSum_Av;
							BL=BL/pFil->fBL_Av;
							//cout<<"result mwd 1"<<endl;
							//fOutput->l_E[l_feb_id][l_cha_id] =(-1*Sum);/
							if((S2[350]<-50) && (S2[183]>-10) &&(S2[500]>-10)){
								h_mwd_E[l_sfp_id][l_feb_id][l_cha_id]->Fill(-1*(Sum-BL));
								fOutput->l_E[l_feb_id][l_cha_id]= -1*(Sum - BL);
							}

						}
						if(l_feb_id==10||(l_feb_id==11 && l_cha_id<4)){//
							//cout<<"MWD Ge"<<endl;

							adj=50;
							Z = 1;
							c= pFil->fk+pFil->fm;

							for(l_l=c+pFil->fk+1; l_l<(l_trace_size) && Z; l_l++){     //loop over osc points
								d[l_l] = (X[l_l] - X[l_l-pFil->fk]) - (X[l_l-c] - X[l_l-c-pFil->fk]);
								r2[l_l] = r2[l_l-1] + d[l_l] - exp(-20./pFil->ftau[l_feb_id][l_cha_id])*d[l_l-1];    //e^(-t_sampling/tau)
								S2[l_l] = S2[l_l-1] + r2[l_l]/adj;

								h_mwd_traps[l_sfp_id][l_feb_id][l_cha_id]->Fill(l_l, S2[l_l]);

								//in case you want to average between several points (say 710..720)
								if (l_l>=pFil->fSum_Start && l_l<=(pFil->fSum_Start+pFil->fSum_Av)){
									Sum += S2[l_l];
								}
								if (l_l>=pFil->fBL_Start && l_l<=(pFil->fBL_Start+pFil->fBL_Av)){
									BL += S2[l_l];
									//cout<<"Sum_incre"<<" "<<Sum<<endl;
								}
							}

							Sum=Sum/pFil->fSum_Av;
							BL=BL/pFil->fBL_Av;
							//cout<<"result mwd 1"<<endl;
							//fOutput->l_E[l_feb_id][l_cha_id] =(-1*Sum);/
							if((S2[350]<-50) && (S2[183]>-10) &&(S2[500]>-10)){
								h_mwd_E[l_sfp_id][l_feb_id][l_cha_id]->Fill(-1*(Sum-BL));
								fOutput->l_E[l_feb_id][l_cha_id]= -1*(Sum - BL);
							}

						}
					}//end valid channel number
				}//end Physics event

				l_trace_trail = *pl_tmp++;
				if ( ((l_trace_trail & 0xff000000) >> 24) != 0xbb)
				{
					printf ("ERROR>> trace trailer id is not 0xbb, ");
					printf ("SFP: %d, FEB: %d, CHA: %d \n", l_sfp_id, l_feb_id, l_cha_id);
					goto bad_event;
				}
				//cout<<"Unpack - End real channel"<<endl;
			}//end real channel
		}//end Channel header
		else
		{
			printf ("ERROR>> data word neither channel header nor padding word \n");
		}
	}


bad_event:
	{
		/*cout<<"We have a bad event :"<<endl;
		  cout<<"Unpack.BuildEvent() - fOutput->l_E["<<l_sfp_id<<"]["<<l_feb_id<<"]["<<l_cha_id<<"]= " <<
		  fOutput->l_E[l_feb_id][l_cha_id] <<" - l_T["<<l_sfp_id<<"]["<<l_feb_id<<"]["<<l_cha_id<<"]= " <<
		  fOutput->l_T[l_feb_id][l_cha_id] << endl;
		  cout<<"Unpack.BuildEvent() - l_t_trig = "<<fOutput->l_t_trig<<endl;*/
	}
	inputfilename_marija=(fInput->GetMbsSourceHeader())->filhe_file;
	if ( (l_evt_ct % 500000) == 0)
	{
		printf ("------------------------------------------------------\n");
		printf ("nr of events processed:                  %10u \n", l_evt_ct);
		/*  printf ("nr of good_energies found:               %10u \n", l_good_energy_ct);
		    printf ("single hits found in trigger window:     %10u \n", l_1_hit_ct);
		    printf ("multi hits found in trigger window:      %10u \n", l_more_1_hit_ct);
		    printf ("multi hits found, first hit energy ok:   %10u \n", l_more_1_hit_first_energy_ok_ct);
		    printf ("nr of events with failed fpga energy:    %10u \n", l_eeeeee_ct);
		    printf ("nr of events with eeeee1:                %10u \n", l_eeeee1_ct);
		    printf ("nr of events with eeeee2:                %10u \n", l_eeeee2_ct);
		    printf ("nr of events with eeeee3:                %10u \n", l_eeeee3_ct);
		    printf ("nr of events with failed fpga energy - \n");
		    printf ("and a single hit found in trigger window %10u \n", l_1_hit_and_eeeeee_ct);
		    printf ("nr of events with failed fpga energy - \n");
		    printf ("and multi hits found in trigger window   %10u \n", l_more_1_hit_and_eeeeee_ct);*/
		printf ("------------------------------------------------------\n");
	}
	//cout<<c_Y<<" front events - "<<c_X<<" back events"<<endl;
	//fOutput->Clear();
	return kTRUE;
}

//--------------------------------------------------------------------------------------------------------

void MoDSSUnpackProc:: f_make_histo (Int_t l_mode)
{
	Text_t chis[256];
	Text_t chead[256];
	Text_t chis1[256];
	Text_t chead1[256];
	UInt_t l_i, l_j, l_k;
	UInt_t l_tra_size;
	UInt_t l_trap_n_avg;

	//traces_Th.open ("traces_Th_old.txt");

#ifdef USE_MBS_PARAM
	l_tra_size   = l_trace & 0xffff;
	l_trap_n_avg = l_e_filt >> 21;
	//printf ("f_make_histo: trace size: %d, avg size %d \n", l_tra_size, l_trap_n_avg);
	fflush (stdout);
	l_sfp_slaves[0] =  l_slaves & 0xff;
	l_sfp_slaves[1] = (l_slaves & 0xff00)     >>  8;
	l_sfp_slaves[2] = (l_slaves & 0xff0000)   >> 16;
	l_sfp_slaves[3] = (l_slaves & 0xff000000) >> 24;
	//printf ("f_make_histo: # of sfp slaves: 3:%d, 2:%d, 1: %d, 0: %d \n",
	//	l_sfp_slaves[3], l_sfp_slaves[2], l_sfp_slaves[1], l_sfp_slaves[0]);
	fflush (stdout);
#else
	l_tra_size   = TRACE_SIZE;
	l_trap_n_avg = TRAPEZ_N_AVG;
#endif // USE_MBS_PARAM
	sprintf(chis1,"DT_ll_time");
	sprintf(chead1,"Difference between 2 successive ll_time");
	RemoveHistogram(chis1);
	fhD_ll_time = new TH1I(chis1,chead1,50001,-1000,50000);
	AddHistogram(fhD_ll_time, "Unpack/");

	sprintf(chis1,"DT_Inv_ll_time");
	sprintf(chead1,"Frequency spectrum of events arrival (based on 1/D(ll_time);Fequency (xHz)");
	RemoveHistogram(chis1);
	fhD_Inv_ll_time = new TH1I(chis1,chead1,200000,0,2);
	AddHistogram(fhD_Inv_ll_time, "Unpack/");

	sprintf(chis1,"DT_ll_trg_time");
	sprintf(chead1,"Difference between 2 successive ll_trg_time");
	RemoveHistogram(chis1);
	fhD_ll_trg_time = new TH1I(chis1,chead1,50001,-1000,50000);
	AddHistogram(fhD_ll_trg_time, "Unpack/");

	sprintf(chis1,"l_hit_time");
	sprintf(chead1,"l_hit_time;Clock Cycles (20ns);Hits");
	RemoveHistogram(chis1);
	fh_l_hit_time = new TH1I(chis1,chead1,10001,-5000,5000);
	AddHistogram(fh_l_hit_time, "Unpack/");

	sprintf(chis1,"DT_l_hit_time");
	sprintf(chead1,"Difference between 2 successive l_hit_time");
	RemoveHistogram(chis1);
	fhD_l_hit_time = new TH2I(chis1,chead1,200,0,200,3001,-1500,1500);
	AddHistogram(fhD_l_hit_time, "Unpack/");

	sprintf(chis1,"X1vsX2_l_hit_time_zero");
	sprintf(chead1,"X1 vs X2 for Difference between 2 successive l_hit_time in [-5-6];Current;Previous");
	RemoveHistogram(chis1);
	fh_X1vsX2_hit_time_zero= new TH2I(chis1,chead1,63,-1,62,63,-1,62);
	AddHistogram(fh_X1vsX2_hit_time_zero, "Unpack/");

	sprintf(chis1,"X1vsX2_l_hit_time_281");
	sprintf(chead1,"X1 vs X2 for Difference between 2 successive l_hit_time in [264-288];Current;Previous");
	RemoveHistogram(chis1);
	fh_X1vsX2_hit_time_281= new TH2I(chis1,chead1,63,-1,62,63,-1,62);
	AddHistogram(fh_X1vsX2_hit_time_281, "Unpack/");

	sprintf(chis1,"E1vsE2_l_hit_time_zero");
	sprintf(chead1,"E1 vs E2 for Difference between 2 successive l_hit_time in [-5-6];Current;Previous");
	RemoveHistogram(chis1);
	fh_E1vsE2_hit_time_zero= new TH2I(chis1,chead1,4000,0,16000,4000,0,16000);
	AddHistogram(fh_E1vsE2_hit_time_zero, "Unpack/");

	sprintf(chis1,"E1vsE2_l_hit_time_281");
	sprintf(chead1,"E1 vs E2 for Difference between 2 successive l_hit_time in [264-288];Current;Previous");
	RemoveHistogram(chis1);
	fh_E1vsE2_hit_time_281= new TH2I(chis1,chead1,4000,0,16000,4000,0,16000);
	AddHistogram(fh_E1vsE2_hit_time_281, "Unpack/");

	sprintf(chis1,"Unpack/Hits");
	sprintf(chead1,"Number of hits per strip in all the channels");
	h_hits= MakeTH1('D',chis1,chead1,200,0,200);

	for (l_i=0; l_i<MAX_SFP; l_i++)
	{
		if (l_sfp_slaves[l_i] != 0)
		{
			for (l_j=0; l_j<l_sfp_slaves[l_i]; l_j++)
			{

				for (l_k=0; l_k<N_CHA; l_k++)
				{
					sprintf(chis,"Unpack/Raw/Traces/FEB: %2d CH: %2d", l_j, l_k);
					sprintf(chead,"Trace");
					h_trace[l_i][l_j][l_k] = MakeTH1('I', chis,chead,l_tra_size,0,l_tra_size);

					sprintf(chis,"Unpack/Raw/Traces/FEB_filt: %2d CH: %2d", l_j, l_k);
					sprintf(chead,"Trace filt");
					h_trace_filt[l_i][l_j][l_k] = MakeTH1('I', chis,chead,l_tra_size,0,l_tra_size);

					sprintf(chis,"Unpack/Raw/Traces BLR/TRACE, base line restored SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
					sprintf(chead,"Trace, base line restored");
					h_trace_blr[l_i][l_j][l_k] = MakeTH1('F', chis,chead,l_tra_size,0,l_tra_size);

					sprintf(chis,"Unpack/FPGA/Trapez/FPGA Trapez SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
					sprintf(chead,"FPGA Trapez");
					h_trapez_fpga[l_i][l_j][l_k] = MakeTH1('F', chis,chead,l_tra_size,0,l_tra_size);

				}

				if(l_j==0 ||l_j==1 || l_j==2 || l_j==3){
					for (l_k=0; l_k<N_CHA; l_k++)
					{
						sprintf(chis,"Unpack/Raw/FrontX/SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"FrontX");
						h_peak[l_i][l_j][l_k] = MakeTH1('I', chis,chead,4096,0,8192);

						sprintf(chis,"MWD/EnergyX/MWDE_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Energy");
						h_mwd_E[l_i][l_j][l_k] = MakeTH1('D', chis,chead,8192,0,16384);

						sprintf(chis,"Unpack/FPGA/FPGA Energy/FPGA Energy SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"FPGA Energy");
						h_fpga_e[l_i][l_j][l_k] = MakeTH1('F', chis,chead,20000,0,20000);

						sprintf(chis,"MWD/Trapezoid/MWDTRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid");
						h_mwd_traps[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);

						sprintf(chis,"MWD/Trapezoid2/MWD2TRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid2");
						h_mwd_traps2[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);
						
					}}

				if(l_j==4 ||l_j==5 || l_j==6 || l_j==7){
					for (l_k=0; l_k<N_CHA; l_k++)
					{
						sprintf(chis,"Unpack/Raw/BackY/SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"BackY");
						h_peak_back[l_i][l_j][l_k] = MakeTH1('I', chis,chead,4096,0,8192);

						sprintf(chis,"MWD/EnergyY/MWDE_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Energy");
						h_mwd_E[l_i][l_j][l_k] = MakeTH1('D', chis,chead,4096,0,16384);

						sprintf(chis,"Unpack/FPGA/FPGA Energy/FPGA Energy SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"FPGA Energy");
						h_fpga_e[l_i][l_j][l_k] = MakeTH1('F', chis,chead,20000,0,20000);

						sprintf(chis,"MWD/Trapezoid/MWDTRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid");
						h_mwd_traps[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);

						sprintf(chis,"MWD/Trapezoid2/MWD2TRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid2");
						h_mwd_traps2[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);
					}}

				if(l_j==8 || l_j==9){
					for (l_k=0; l_k<N_CHA; l_k++)
					{
						sprintf(chis,"Unpack/Raw/Box/SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"Box");
						h_peak_box[l_i][l_j][l_k] = MakeTH1('I', chis,chead,4096,0,8192);

						sprintf(chis,"MWD/EnergyBox/MWDE_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Energy");
						h_mwd_E[l_i][l_j][l_k] = MakeTH1('D', chis,chead,8192,0,16384);

						sprintf(chis,"Unpack/FPGA/FPGA Energy/FPGA Energy SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"FPGA Energy");
						h_fpga_e[l_i][l_j][l_k] = MakeTH1('F', chis,chead,20000,0,20000);

						sprintf(chis,"MWD/Trapezoid/MWDTRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid");
						h_mwd_traps[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);

						sprintf(chis,"MWD/Trapezoid2/MWD2TRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid2");
						h_mwd_traps2[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);
					}}

				if(l_j==10){
					for (l_k=0; l_k<N_CHA; l_k++)
					{
						sprintf(chis,"Unpack/Raw/Gamma/SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"Gamma");
						h_peak_gamma[l_i][l_j][l_k] = MakeTH1('I', chis,chead,8192,0,16384);

						sprintf(chis,"MWD/EnergyGe/MWDE_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Energy");
						h_mwd_E[l_i][l_j][l_k] = MakeTH1('D', chis,chead,8192,0,8192);

						sprintf(chis,"Unpack/FPGA/FPGA Energy/FPGA Energy SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"FPGA Energy");
						h_fpga_e[l_i][l_j][l_k] = MakeTH1('F', chis,chead,20000,0,20000);

						sprintf(chis,"MWD/Trapezoid/MWDTRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid");
						h_mwd_traps[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);

						sprintf(chis,"MWD/Trapezoid2/MWD2TRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid2");
						h_mwd_traps2[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);
					}}

				if(l_j==11){
					for (l_k=0; l_k<N_CHA; l_k++)
					{
						sprintf(chis,"Unpack/Raw/Gamma/SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"Gamma");
						h_peak_gamma[l_i][l_j][l_k] = MakeTH1('I', chis,chead,8192,0,8192);

						sprintf(chis,"MWD/EnergyGe/MWDE_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Energy");
						h_mwd_E[l_i][l_j][l_k] = MakeTH1('D', chis,chead,8192,0,16384);

						sprintf(chis,"MWD/Trapezoid/MWDTRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid");
						h_mwd_traps[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);

						sprintf(chis,"MWD/Trapezoid2/MWD2TRAP_SFP:_%2d_FEBEX:_%2d_CHAN:_%2d", l_i, l_j, l_k);
						sprintf(chead,"MWD Trapezoid2");
						h_mwd_traps2[l_i][l_j][l_k] = MakeTH1('D', chis,chead,l_tra_size,0,l_tra_size);

						sprintf(chis,"Unpack/FPGA/FPGA Energy/FPGA Energy SFP: %2d FEBEX: %2d CHAN: %2d", l_i, l_j, l_k);
						sprintf(chead,"FPGA Energy");
						h_fpga_e[l_i][l_j][l_k] = MakeTH1('F', chis,chead,20000,0,20000);
					}
				}
			}
		}
	}

	b_first_event = kTRUE;
	//cout<<"make_histos done"<<endl;
}

//----------------------------END OF GO4 SOURCE FILE ---------------------
