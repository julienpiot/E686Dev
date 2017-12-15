//---------------------------------------------------------------
//        Go4 Analysis for Shiptrap experiment
//
//        18-Feburary-2010 (Joern Adamczewski-Musch, GSI)
//                         - first version with adjustments for Go4 4.4
//
//---------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//       Experiment Data Processing at EE department, GSI
//---------------------------------------------------------------
//
//Copyright (C) 2000- Gesellschaft f. Schwerionenforschung, GSI
//                    Planckstr. 1, 64291 Darmstadt, Germany
//Contact:            http://go4.gsi.de
//----------------------------------------------------------------
//This software can be used under the license agreements as stated
//in Go4License.txt file which is part of the distribution.
//----------------------------------------------------------------

#include "MoDSSUnpackEvent.h"

#include "Riostream.h"


//***********************************************************
MoDSSUnpackEvent::MoDSSUnpackEvent() :
        TGo4EventElement()
{
        std::cout << "**** MoDSSUnpackEvent: Create instance" << std::endl;
}
//***********************************************************
MoDSSUnpackEvent::MoDSSUnpackEvent(const char* name) :
        TGo4EventElement(name)
{
        std::cout << "**** MoDSSUnpackEvent: Create instance " << name << std::endl;
}
//***********************************************************
MoDSSUnpackEvent::~MoDSSUnpackEvent()
{
        std::cout << "**** MoDSSUnpackEvent: Delete instance " << std::endl;
}

//-----------------------------------------------------------
void  MoDSSUnpackEvent::Clear(Option_t *t)
{
        //Julien output event of unpack
        for(int i=0; i<12; i++){
                for(int j=0;j<16;j++){
                        l_E[i][j]=0;
                        l_T[i][j]=0;
                }
        }
        l_sfp = -1;
        l_feb = -1;
        l_cha = -1;
        l_t_trig = -1;

        //      fPatternUnit = 0;
}
void  MoDSSUnpackEvent::ClearEvent()

{
        //Julien output event of unpack
        for(int i=0; i<12; i++){
                for(int j=0;j<16;j++){
                        l_E[i][j]=0;
                        l_T[i][j]=0;
                }
        }
        l_sfp = -1;
        l_feb = -1;
        l_cha = -1;
        l_t_trig = -1;

        //      fPatternUnit = 0;
}

Int_t   MoDSSUnpackEvent::Dump(){
//         Int_t chit_dssdY = 0;
//         Int_t chit_dssdX = 0;
//         std::cout<<"========================================"<<std::endl<<"UnpackEvent Dump : "<<std::endl;
//         for(int i=0; i<12; i++){
//                 for(int j=0;j<16;j++){
//                         //if(l_E[0][i][j]>0){
//                                 if( i <4)chit_dssdY++;
//                                 if( i>=4 && i<8) chit_dssdX++;
//                                 std::cout<<"feb "<<i<<" cha "<<j<<" = "<<l_E[0][i][j]<<" - "<<l_T[0][i][j]<<std::endl;
//                         //}
//                 }
//         }
//         std::cout<<"trig time = "<<l_t_trig<<std::endl;
//         std::cout<<chit_dssdY<<" Anl front - "<<chit_dssdX<<" Anl back"<<std::endl;
//         std::cout<<"========================================"<<std::endl;
         return 1;

}
//----------------------------END OF GO4 SOURCE FILE ---------------------
