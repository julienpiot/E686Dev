/* ===================================================================
  MoDSS analysis program
  Peter Cagarda, 2003
  
  modified
  Stanislav Antalic, 2004
  
  Licence policy according to GO4 (go4.gsi.de)
=================================================================== */

#ifndef MoDSSANALYSIS_H
#define MoDSSANALYSIS_H

#include "TH1.h"
#include "TFile.h"

#include "Go4Analysis/TGo4Analysis.h"
#include "Go4DynamicList/TGo4HistoFillAction.h"

#include "MoDSSUnpackFact.h"
#include "MoDSSAnlFact.h"
#include "MoDSSCorrelFact.h"

class MoDSSAnalysis : public TGo4Analysis
   {
   public:   
     MoDSSAnalysis();
     MoDSSAnalysis(Text_t* input, Int_t type, Int_t port);
     virtual ~MoDSSAnalysis() ;
     virtual Int_t UserPreLoop();
     virtual Int_t UserEventFunc();
     virtual Int_t UserPostLoop();
   private:
     TGo4MbsEvent     *fMbsEvent;   // source input event from MBS
     MoDSSUnpackEvent *fRawEvent;   // output from the first step (raw event)
     MoDSSAnlEvent    *fCalEvent;   // output from the second step (calibrated event) - only parameters needed for correlation search
  //   MoDSSCorrelEvent *fCorEvent;   // output from the 3rd step (correlation search)
     
     Int_t     fEvents;        // no. of events
     Int_t     fLastEvent;
     
     Int_t     fTime;    // running time of experiment (file) in seconds
    

ClassDef(MoDSSAnalysis,1)
};
#endif //MoDSSANALYSIS_H

