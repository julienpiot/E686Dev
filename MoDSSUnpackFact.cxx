
// MoDSS UNPACK FACTORY - implementation
// management of UNPACK step

#include "MoDSSUnpackFact.h"

//***********************************************************
// Constructor

MoDSSUnpackFact::MoDSSUnpackFact(Text_t * name)
: TGo4EventServerFactory(name)
{
}

//***********************************************************
// Default constructor

MoDSSUnpackFact::MoDSSUnpackFact() 
: TGo4EventServerFactory("MoDSS Event Fact")
{ 
}

//***********************************************************
// Default destructor

MoDSSUnpackFact::~MoDSSUnpackFact() 
{ 
}
//***********************************************************

//-----------------------------------------------------------

TGo4EventProcessor * MoDSSUnpackFact::CreateEventProcessor(TGo4EventProcessorParameter* par)
{ 
   MoDSSUnpackProc* proc = new MoDSSUnpackProc("MoDSSUnpackProc");
   return proc;
}

//-----------------------------------------------------------

TGo4EventElement * MoDSSUnpackFact::CreateOutputEvent()
{ // name is given in MoDSSUnpackEvent
   MoDSSUnpackEvent * event = new MoDSSUnpackEvent();
   SetOutputEvent(event); // important for some event store implementations!!!
   return event;
}

//-----------------------------------------------------------
ClassImp(MoDSSUnpackFact)

