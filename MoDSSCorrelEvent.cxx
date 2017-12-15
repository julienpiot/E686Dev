
#include "MoDSSCorrelEvent.h"

//***********************************************************

MoDSSCorrelEvent::MoDSSCorrelEvent() 
  :TGo4EventElement("MoDSSCorrelEvent"),fxMoDSSCP(0),fxMoDSSFS(0)
{ 
}

//***********************************************************

MoDSSCorrelEvent::MoDSSCorrelEvent(const char* name) 
  :TGo4EventElement(name),fxMoDSSCP(0),fxMoDSSFS(0)
{ 
}

//***********************************************************

MoDSSCorrelEvent::~MoDSSCorrelEvent() 
{ 
}

//***********************************************************


//-----------------------------------------------------------

Int_t MoDSSCorrelEvent::Init()
{ 
  // check for different source types
  Int_t rev = 0;
  Clear();
  // is it used by Correl step as output ?
  if(CheckEventSource("MoDSSCorrelProc")){
    fxMoDSSCP = (MoDSSCorrelProc*)GetEventSource();
    cout << "**** MoDSSCorrelEvent init for Correlation step" << endl;
  }
  // This event is never used as input from file
  //   else if(CheckEventSource("TGo4FileSource")) fxMoDSSFS = (TGo4FileSource*)GetEventSource();
  else rev = 1;
  return rev;
}

//-----------------------------------------------------------

Int_t MoDSSCorrelEvent::Fill()
{ 
   Int_t rev = 0;
   Clear();
   // check for different source types
   // call CorrelationSearch function in event processor (this fills the event)
   if (fxMoDSSCP) fxMoDSSCP->MoDSSCorrelationSearch(this);

   // This event is never used as input from file
   //   if(fxMoDSSFS)fxMoDSSFS->BuildEvent(this);
   return rev;

}
//-----------------------------------------------------------
void MoDSSCorrelEvent::Clear(Option_t *t)
{ 
/*  
  fTime    = 0;
  fTMP     = 0;
  fB_pulse = kFALSE;
  fB_tof   = kFALSE;
  fTOF     = 0;
  fDet_index = 0;
  fE_kev   = 0;
  fXY_lt   = 0;
  fXY_lb   = 0;
  fE_mev   = 0;
  fXY_ht   = 0;
  fXY_hb   = 0;
  fDetb_index = 0;
  fEb_kev  = 0;
  fEb_mev  = 0;
  fB_veto  = kFALSE;
  fegl1   = 0;
  fegl2   = 0;
  fegl3   = 0;
  fegl4   = 0;
  fTgamma  = 0;
*/
}

//-----------------------------------------------------------
ClassImp(MoDSSCorrelEvent)
