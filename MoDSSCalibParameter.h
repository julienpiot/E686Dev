#ifndef MoDSSCALIBPAR_H
#define MoDSSCALIBPAR_H

#include "Go4StatusBase/TGo4Parameter.h"
#include <iomanip>
//#include "TRandom3.h"

class MoDSSCalibParameter : public TGo4Parameter {

	public:
		MoDSSCalibParameter();
		MoDSSCalibParameter(Text_t *name);
		virtual ~MoDSSCalibParameter();    
		Int_t   PrintParameter(Text_t *buf, Int_t);
		Bool_t  UpdateFrom(TGo4Parameter *);

		Double_t Ax[60], Bx[60];    
		Double_t Ay[60], By[60];    
		Double_t Ab[32], Bb[32];    
                Double_t Ag[20], Bg[20], Cg[20];
		//Double_t AXref, BXref;
		//Double_t Abuf, Bbuf;

		//TRandom3 *R3;

		ClassDef(MoDSSCalibParameter,6)
};

#endif
