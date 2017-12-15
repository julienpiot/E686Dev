#include "MoDSSAnlFact.h"

//***********************************************************

MoDSSAnlFact::MoDSSAnlFact(Text_t * name)
: TGo4EventServerFactory(name)
{}

//***********************************************************

MoDSSAnlFact::MoDSSAnlFact() 
: TGo4EventServerFactory("MoDSS Analysis Fact")
{}

//***********************************************************

MoDSSAnlFact::~MoDSSAnlFact() 
{}

//***********************************************************

//-----------------------------------------------------------

TGo4EventProcessor * MoDSSAnlFact::CreateEventProcessor(TGo4EventProcessorParameter* par)
{ 
   MoDSSAnlProc* anlproc = new MoDSSAnlProc("MoDSSAnlProc");
   return anlproc;
}

//-----------------------------------------------------------

TGo4EventElement * MoDSSAnlFact::CreateOutputEvent()
{ 
   MoDSSAnlEvent* anlevent = new MoDSSAnlEvent("MoDSSAnlEvent");
   SetOutputEvent(anlevent); // important for some event store implementations!!!
   return anlevent;
}

//-----------------------------------------------------------

TGo4EventElement * MoDSSAnlFact::CreateInputEvent()
{  // name is given in MoDSSUnpackEvent
   MoDSSUnpackEvent* event = new MoDSSUnpackEvent();
   return event;
}

//-----------------------------------------------------------
ClassImp(MoDSSAnlFact)

//----------------------------END OF GO4 SOURCE FILE ---------------------
