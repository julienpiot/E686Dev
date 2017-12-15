#ifndef MoDSSDSSDVENT_H
#define MoDSSDSSDVENT_H

//#include "MoDSSGlobals.h"

#include "Go4Event/TGo4EventElement.h"
#include "Go4EventServer/TGo4TreeSource.h"
#include "Go4EventServer/TGo4FileSource.h"

#include "TString.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
class MoDSSDSSDEvent : public TGo4EventElement
{
	public:
		MoDSSDSSDEvent() ;
		MoDSSDSSDEvent(Text_t * name) ;
		~MoDSSDSSDEvent() ;
		// operator '=' ; only for the use in correlation processor,
		// do not use in AnlProcessor unless you know what you are doing.
		MoDSSDSSDEvent operator = (const MoDSSDSSDEvent &from);
		
		Int_t 		Set_l_EnX(Double_t nrj);
		Int_t 		Set_l_EnY(Double_t nrj);
		Int_t 		Set_l_t_trigX(Long64_t time);
		Int_t 		Set_l_t_trigY(Long64_t time);
		Int_t 		Set_l_X(Int_t X);
		Int_t 		Set_l_Y(Int_t Y);
		Int_t 		Set_l_Tof(Double_t tof);
		Int_t 		Set_l_used(Bool_t used);
		Int_t 		Set_l_part(TString part);
		
		Int_t 		Get_l_EnX(std::vector<Double_t>::iterator it);
		Int_t 		Get_l_EnY(std::vector<Double_t>::iterator it);
		Int_t  		Get_l_t_trigX(std::vector<Long64_t>::iterator it);
		Int_t  		Get_l_t_trigY(std::vector<Long64_t>::iterator it);
		Int_t 		Get_l_X(std::vector<Int_t>::iterator it);
		Int_t 		Get_l_Y(std::vector<Int_t>::iterator it);
		Double_t	Get_l_Tof();
		Bool_t		Get_l_used();
		TString		Get_l_part();


		virtual Int_t Init();
		virtual Int_t Fill();
		virtual void  Clear(Option_t *t="");  
		virtual void  ClearEvent();  
		Int_t 	Dump();
		Int_t 	CalcStripNum(Int_t feb, Int_t cha);

		Int_t 		Get_max_EnX(std::vector<Double_t>::iterator it);
		Int_t 		Get_max_EnY(std::vector<Double_t>::iterator it);
		Int_t 		Get_max_EnX();
		Int_t 		Get_max_EnY();
		Int_t 		Get_X_max();
		Int_t 		Get_Y_max();
		Long64_t 	Get_t_trigX_max();
		Long64_t 	Get_t_trigY_max();
		Int_t		Get_EnX_size();
		Int_t		Get_EnY_size();

	private:
		// output event from second (analysis) step - contains variables
		// needed for correlations
		std::vector<Double_t>	l_EnX;
		std::vector<Double_t>	l_EnY;
		std::vector<Long64_t>	l_t_trigX;
		std::vector<Long64_t>	l_t_trigY;
		std::vector<Int_t>	l_X;
		std::vector<Int_t>	l_Y;

		Double_t		l_Tof;
		Bool_t			l_used;
		TString			l_part;
	
		ClassDef(MoDSSDSSDEvent,1)         
};
#endif //MoDSSDSSDEVENT_H
