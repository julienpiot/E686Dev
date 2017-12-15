
// MoDSS UNPACK FACTORY - definition
// management of UNPACK step

#ifndef MoDSSUNPACKFACTORY_H
#define MoDSSUNPACKFACTORY_H

#include "Go4EventServer/Go4EventServer.h"
#include "MoDSSUnpackEvent.h"
#include "MoDSSUnpackProc.h"
#include "Go4EventServer/TGo4EventServerFactory.h"

class MoDSSUnpackEvent;

class MoDSSUnpackFact : public TGo4EventServerFactory {
   public:
      virtual TGo4EventElement   * CreateOutputEvent();
      virtual TGo4EventProcessor * CreateEventProcessor(TGo4EventProcessorParameter* par);
      MoDSSUnpackFact();
      MoDSSUnpackFact(Text_t * name);
      virtual ~MoDSSUnpackFact();

ClassDef(MoDSSUnpackFact,1)     
};
#endif //MoDSSUNPACKFACTORY_H

