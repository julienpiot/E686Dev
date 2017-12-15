/* ===================================================================
  MoDSS analysis program
  Peter Cagarda, 2003

  --------------------------------------------------
  Adjusted to Go4 v3 for usage with analysis server
    05.July 2006 J"orn Adamczewski
  --------------------------------------------------

  Licence policy according to GO4 (go4.gsi.de) 
=================================================================== */

#include "MoDSSAnalysis.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"


//***********************************************************
MoDSSAnalysis::MoDSSAnalysis() 
  : fMbsEvent(0),fRawEvent(0),fCalEvent(0)
{
  cout << "Wrong constructor MoDSSAnalysis()!" << endl;
}

//***********************************************************
// this constructor is used
// lmd: input file name (*.lmd)

MoDSSAnalysis::MoDSSAnalysis(Text_t * input,  Int_t type, Int_t port) 
  : fMbsEvent(0),
    fRawEvent(0), 
    fCalEvent(0),
    fEvents(0),
    fLastEvent(0),
    fTime(0)
  
{ 
  cout << "**** MoDSSAnalysis: Create" << endl;

// the step definitions can be changed in the GUI

// first step definitions: --------- UNPACK ----------
// the name of the step can be used later to get event objects

  MoDSSUnpackFact* factory1 = new MoDSSUnpackFact("Unpack-factory");
  TGo4EventSourceParameter* source1=0;
  TGo4EventStoreParameter* store1=0;
   //  TGo4EventStoreParameter* store1   = new TGo4FileStoreParameter("MoDSS_UPA");

   switch (type){
    case GO4EV_MBS_FILE:
       source1   = new TGo4MbsFileParameter(input);
       cout << "**** Unpack: Create file input " << input << endl;
       break;
    case GO4EV_MBS_STREAM:
       source1 = new TGo4MbsStreamParameter(input);
       cout << "**** Unpack: Create stream input "  << input << endl;
       break;
    case GO4EV_MBS_TRANSPORT:
       source1  = new TGo4MbsTransportParameter(input);
       cout << "**** Unpack: Create transport input " << input  << endl;
       break;
    case GO4EV_MBS_REVSERV:
       source1    = new TGo4RevServParameter(input);
       ((TGo4RevServParameter*) source1)->SetPort(port);
       cout << "**** Unpack: Create remote event server input " << input << " port " << port <<endl;
       break;   
    case GO4EV_MBS_EVENTSERVER:
       source1  = new TGo4MbsEventServerParameter(input);
       cout << "**** Unpack: Create mbs event server input "  << input << endl;
       break;
    case GO4EV_MBS_RANDOM:
       source1 = new TGo4MbsRandomParameter(input);
       cout << "**** Unpack: Create mbs random input "  << input << endl;
       break;
    default:
       cout << "**** Unpack: unknown input type!! undefined"  << input << endl;
       break;
  }
  TGo4AnalysisStep*       step1    = new TGo4AnalysisStep("Unpack",factory1,source1,store1,0);
//  store1->SetOverwriteMode(kTRUE);
  step1->SetSourceEnabled(kTRUE);
  step1->SetStoreEnabled(kFALSE);  // disable output
  step1->SetProcessEnabled(kTRUE);
  step1->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step1);


// second step definitions: ---------- ANL ------------
// If source is enabled, take output file from step 1 as input.
// otherwise we use output event from step 1 (set in the factory)

  MoDSSAnlFact*            factory2 = new MoDSSAnlFact("Anl-factory");
//  TGo4FileSourceParameter* source2  = new TGo4FileSourceParameter(out1);
//  TGo4FileStoreParameter*  store2   = new TGo4FileStoreParameter(out2);
  TGo4AnalysisStep*        step2    = new TGo4AnalysisStep("Analysis",factory2,0,0,0);
//  store2->SetOverwriteMode(kTRUE);
  step2->SetSourceEnabled(kFALSE);  // disable file input (output file of step 1)
  step2->SetStoreEnabled(kFALSE);    // disable output
  step2->SetProcessEnabled(kTRUE);
  step2->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step2);

  
// third step definitions: ---------- CORREL ------------
  
  MoDSSCorrelFact*        factory3 = new MoDSSCorrelFact("Correl-factory");
  TGo4AnalysisStep*       step3    = new TGo4AnalysisStep("Correl",factory3,0,0,0);
  step3->SetSourceEnabled(kFALSE);   // disable file input (output file of step 2)
  step3->SetStoreEnabled(kFALSE);    // disable output
  step3->SetProcessEnabled(kTRUE);
  step3->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step3);

}

//***********************************************************
MoDSSAnalysis::~MoDSSAnalysis() 
{
  cout << "**** MoDSSAnalysis: Delete" << endl;
}
//***********************************************************

//-----------------------------------------------------------
Int_t MoDSSAnalysis::UserPreLoop()
{ 
  cout << "**** MoDSSAnalysis: PreLoop" << endl;
   // we update the pointers to the current event structures here:
   fMbsEvent = dynamic_cast<TGo4MbsEvent*>     (GetInputEvent("Unpack"));   // of step "Unpack"
   fRawEvent = dynamic_cast<MoDSSUnpackEvent*> (GetOutputEvent("Unpack"));
   fCalEvent = dynamic_cast<MoDSSAnlEvent*>    (GetOutputEvent("Analysis"));
   fEvents = 0;
   fLastEvent = 0;
   fTime = 0;
  

   return 0;
}

//-----------------------------------------------------------

Int_t MoDSSAnalysis::UserPostLoop()
{ 
  cout << "\n**** MoDSSAnalysis: PostLoop" << endl;
  cout << "Last event: " << fLastEvent << " Total events: " << fEvents << endl;
  cout << "RUNING TIME: " << fTime << endl;
  
 
  fMbsEvent = 0; // reset to avoid invalid pointer if analysis is changed in between
  fRawEvent = 0;
  fCalEvent = 0;
  fEvents = 0;
  fLastEvent = 0;
  fTime = 0;

  
  return 0;
}

//-----------------------------------------------------------
Int_t MoDSSAnalysis::UserEventFunc()
{ 

 // all this is optional:
   // This function is called once for each event after all steps.
   if(fMbsEvent) {
      fEvents++;
      fLastEvent=fMbsEvent->GetCount();
   }
   if(fEvents == 1 || IsNewInputFile()) {
      cout << "First event #: " << fLastEvent  << endl;
      SetNewInputFile(kFALSE); // we have to reset the newfile flag
   }
   
   return 0;
}
//-----------------------------------------------------------

ClassImp(MoDSSAnalysis)

