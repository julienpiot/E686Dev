
#include "MoDSSCorrelFact.h"

//***********************************************************

MoDSSCorrelFact::MoDSSCorrelFact(const char* name)
: TGo4EventServerFactory(name)
{}

//***********************************************************

MoDSSCorrelFact::MoDSSCorrelFact() 
: TGo4EventServerFactory("MoDSS Correlation Fact")
{}

//***********************************************************

MoDSSCorrelFact::~MoDSSCorrelFact() 
{}

//***********************************************************

//-----------------------------------------------------------

TGo4EventProcessor * MoDSSCorrelFact::CreateEventProcessor(TGo4EventProcessorParameter* par)
{ 
   MoDSSCorrelProc* corproc = new MoDSSCorrelProc("MoDSSCorrelProc");
   return corproc;
}

//-----------------------------------------------------------

TGo4EventElement * MoDSSCorrelFact::CreateOutputEvent()
{ 
   MoDSSCorrelEvent* corevent = new MoDSSCorrelEvent("MoDSSCorrelEvent");
   SetOutputEvent(corevent); // important for some event store implementations!!!
   return corevent;
}

//-----------------------------------------------------------

TGo4EventElement * MoDSSCorrelFact::CreateInputEvent()
{  // name is given in MoDSSAnlEvent
   MoDSSAnlEvent* event = new MoDSSAnlEvent();
   return event;
}

//-----------------------------------------------------------
ClassImp(MoDSSCorrelFact)

//----------------------------END OF GO4 SOURCE FILE ---------------------
