#include "MoDSSCalibParameter.h"
#include <iostream>
//#include "TRandom3.h"

using namespace std;


MoDSSCalibParameter::MoDSSCalibParameter()
	: TGo4Parameter("AnaOnlineCalPar")
{ Int_t i;

	// DSSD X alpha
	for (i = 0; i < 60; i++){
		Ax[i] = 0;
		Bx[i] = 0;
	}
	//Abuf = 0;
	//Bbuf = 0;
	//AXref = 0;
        //BXref = 0;

	for (i = 0; i < 60; i++){
		Ay[i] = 0;
		By[i] = 0;
	}
	//box
	for (i = 0; i < 32; i++){
		Ab[i] = 0;
		Bb[i] = 0;
	}
	//Ge
	for (i = 0; i < 20; i++)
	{
		Ag[i] = 0;
                Bg[i] = 0;
                Cg[i] = 0;
	}

	//Create the Random3 generator
	//R3 = new TRandom3();

}

//----------------------------------------------
	MoDSSCalibParameter::MoDSSCalibParameter(Text_t *name)
: TGo4Parameter(name)
{
	Int_t i;
	ifstream    file;

	//AXref = 1.4622; //Calibration parameter for Strip 19 from 216Th
	//BXref = -44.68;	//Calibration parameter for Strip 19 from 216Th
	// read alpha energy calibration for X DSSD
	//file.open("Align_DSSDX_24-5-17.dat");
	file.open("./Calibration/Calib_frontX.dat");
	if (file.fail()) {
		cout << "ERROR: MoDSSCalibParameter - Could not open file: Calib_frontX.dat ! (params set to 1 and 0)\n";
		for (i = 0; i < 60; i++){
			Ax[i] = 1.;
			Bx[i] = 0.;
		}
	}
	else {
		cout << "MoDSSCalibParameter - reading alpha calibration from: Calib_frontX.dat\n";
		for (i = 0; i < 60; i++){
			file >> Ax[i]>> Bx[i] ;
			//Abuf = AXref*Ax[i];
			//Bbuf = AXref*Bx[i]+BXref;
			//cout<<"Cal X - Abuf = "<<Abuf<<" - Ax["<<i<<"] = "<<Ax[i]<<" - Bbuf = "<<Bbuf<<" - Bx["<<i<<"] = "<<Bx[i]<<endl;
			//Ax[i] = Abuf;
			//Bx[i] = Bbuf;
			if (file.fail()) cout << "ERROR reading Calib_frontX.dat\n";
		}
	}
	file.close();

	// read alpha energy calibration for Y DSSD
	file.open("./Calibration/Calib_backY.dat");
	if (file.fail()) {
		cout << "ERROR: MoDSSCalibParameter - Could not open file: Calib_backY.dat ! (params set to 1 and 0)\n";
		for (i = 0; i < 60; i++){
			Ay[i] = 1.;
			By[i] = 0.;
		}
	}
	else {
		cout << "MoDSSCalibParameter - reading alpha calibration from: Calib_backY.dat\n";
		for (i = 0; i < 60; i++){
			file >> Ay[i]>> By[i] ;
			if (file.fail()) cout << "ERROR reading Calib_backY.dat\n";
		}
	}
	file.close();
	//
	// read energy calibration for Box
	file.open("./Calibration/Calib_box.dat");
	if (file.fail()) {
		cout << "ERROR: MoDSSCalibParameter - Could not open file: Calib_Box.dat ! (params set to 1 and 0)\n";
		for (i = 0; i < 32; i++){
			Ab[i] = 1.;
			Bb[i] = 0.;
		}
	}
	else {
		cout << "MoDSSCalibParameter - reading alpha calibration from: Calib_Box.dat\n";
		for (i = 0; i < 32; i++){
			file  >> Ab[i]>> Bb[i];
			if (file.fail()) cout << "ERROR reading Calib_Box.dat\n";
		}
	}
	file.close();

	// read energy calibration for Germanium
	file.open("./Calibration/Calib_Ge.dat");
	if (file.fail()) {
		cout << "ERROR: MoDSSCalibParameter - Could not open file: Calib_Ge.dat ! (params set to 1 and 0)\n";
		for (i = 0; i < 20; i++){
			Ag[i] = 1.;
			Bg[i] = 0.;
                        Cg[i] = 0.;
		}
	}
	else {
		cout << "MoDSSCalibParameter - reading alpha calibration from: Calib_Ge.dat\n";
		for (i = 0; i < 20; i++){
                        file >> Ag[i]>> " ">>Bg[i] >> " ">>Cg[i];
			if (file.fail()) cout << "ERROR reading Calib_Ge.dat\n";
		}
	}
	file.close();

	//Create the Random3 generator
	//R3 = new TRandom3();
}

//-----------------------------------------------
// destructor
MoDSSCalibParameter::~MoDSSCalibParameter()
{}
//-------------------------------
Int_t MoDSSCalibParameter::PrintParameter(Text_t *buf, Int_t)
{
	Int_t  i;
	cout << "\n AnaOnline Calibration Parameter: " << GetName() << endl;

	//X dssd
	cout << "DSSD X calibration:\n";
	for (i = 0; i < 60; i++){
		cout << "strip " << i << "  \t A = " << Ax[i] << "\t B = " << Bx[i] << endl;
	}

	cout << "DSSD Y calibration:\n";
	for (i = 0; i < 60; i++){
		cout << "strip " << i << "  \t A = " << Ay[i] << "\t B = " << By[i] << endl;
	}

	cout << "Box calibration:\n";
	for (i = 0; i < 32; i++){
		cout << "strip " << i << "  \t A = " << Ab[i] << "\t B = " << Bb[i] << endl;
	}
	cout << "Ge calibration:\n";
	for (i = 0; i < 20; i++){
                cout << "strip " << i << "  \t A = " << Ag[i] << "\t B = " << Bg[i] << "\t C = " << Cg[i]<< endl;
	}

	//Delete the Random3 generator
	//delete R3;
	return 1;
}
//---------------------------------------------------
Bool_t MoDSSCalibParameter::UpdateFrom(TGo4Parameter *pp)
{
	if(pp->InheritsFrom("MoDSSCalibParameter"))
	{
		Int_t i;
		MoDSSCalibParameter *from = (MoDSSCalibParameter *) pp;

		//X alpha DSSD
		for (i = 0; i < 60; i++){
			Ax[i]  = from->Ax[i];
			Bx[i]  = from->Bx[i];

			cout << "MoDSSCalibParameter - Parameter : " << GetName() << " UPDATED\n";
		}

		//Y alpha DSSD
		for (i = 0; i < 60; i++){
			Ay[i]  = from->Ay[i];
			By[i]  = from->By[i];

			cout << "MoDSSCalibParameter - Parameter : " << GetName() << " UPDATED\n";
		}

		//box
		for (i = 0; i < 32; i++){
			Ab[i] = from->Ab[i];
			Bb[i] = from->Bb[i];
		}
		//Ge
		for (i = 0; i < 20; i++)
		{
			Ag[i] = from->Ag[i];
			Bg[i] = from->Bg[i];
                        Cg[i] = from->Cg[i];
		}

	}
	else {
		cout << "ERROR: MoDSSCalibParameter - Wrong parameter object: " << pp->ClassName() << endl;}


	return kTRUE;
}

ClassImp(MoDSSCalibParameter)
