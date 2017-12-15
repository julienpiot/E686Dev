#ifndef MoDSSSINGLEVENT_H
#define MoDSSSINGLEVENT_H

//#include "MoDSSGlobals.h"
#include <iostream>
#include <algorithm>
#include "Go4Event/TGo4EventElement.h"
#include "Go4EventServer/TGo4TreeSource.h"
#include "Go4EventServer/TGo4FileSource.h"

using namespace std;
class MoDSSSingleEvent : public TGo4EventElement
{
	public:
		MoDSSSingleEvent() ;
		MoDSSSingleEvent(Text_t * name) ;
		~MoDSSSingleEvent() ;
		// operator '=' ; only for the use in correlation processor,
		// do not use in AnlProcessor unless you know what you are doing.
		MoDSSSingleEvent operator = (const MoDSSSingleEvent &from);
		
		Int_t 		Set_l_En(Double_t nrj);
		Int_t 		Set_l_sfp(Int_t sfp);
		Int_t 		Set_l_feb(Int_t feb);
		Int_t 		Set_l_cha(Int_t cha);
		Int_t 		Set_l_t_trig(Long64_t time);
		
		Int_t 		Get_l_En(std::vector<Double_t>::iterator it);
		Int_t 		Get_l_sfp(std::vector<Int_t>::iterator it);
		Int_t 		Get_l_feb(std::vector<Int_t>::iterator it);
		Int_t 		Get_l_cha(std::vector<Int_t>::iterator it);
		Int_t	 	Get_l_t_trig(std::vector<Long64_t>::iterator it);

		Int_t 		Get_max_En(std::vector<Double_t>::iterator it);
		Int_t 		Get_max_En();

		vector<Double_t> 	Get_l_En();
		vector<Int_t> 		Get_l_sfp();
		vector<Int_t> 		Get_l_feb();
		vector<Int_t> 		Get_l_cha();
		vector<Long64_t> 	Get_l_t_trig();

		virtual Int_t Init();
		virtual Int_t Fill();
		virtual void  Clear(Option_t *t="");  
		virtual void  ClearEvent();  

		Int_t 	CalcDetNum(Int_t det, Int_t cha);
		Int_t	CalcCryNum(Int_t det,Int_t cha);

		Int_t 	Dump();

		// output event from second (analysis) step - contains variables
		// needed for correlations
		std::vector<Double_t>	l_En;
		std::vector<Int_t>	l_sfp;
		std::vector<Int_t>	l_feb;
		std::vector<Int_t>	l_cha;
		std::vector<Long64_t>	l_t_trig;
		Bool_t			b_used;	
		string			l_part;
		std::vector<Double_t>	l_Tof;

	private:


		ClassDef(MoDSSSingleEvent,1)         
};
#endif //MoDSSSINGLEEVENT_H
