#include "MoDSSDSSDEvent.h"

MoDSSDSSDEvent::MoDSSDSSDEvent() 
	:TGo4EventElement("MoDSSDSSDEvent")
{

	l_EnX.clear();
	l_EnY.clear();
	l_t_trigX.clear();
	l_t_trigY.clear();
	l_X.clear();
	l_Y.clear();
	
	l_Tof = -1;
	l_used = false;
	l_part = 'a';

}
//-------------
MoDSSDSSDEvent::MoDSSDSSDEvent(Text_t * name) 
	:TGo4EventElement(name)
{
	l_EnX.clear();
	l_EnY.clear();
	l_t_trigX.clear();
	l_t_trigY.clear();
	l_X.clear();
	l_Y.clear();
	
	l_Tof = -1;
	l_used = false;
	l_part = 'a';

}
//-----------------------
MoDSSDSSDEvent::~MoDSSDSSDEvent() 
{
}
//----------------------------
// assignment operator '='

MoDSSDSSDEvent MoDSSDSSDEvent::operator = (const MoDSSDSSDEvent &from)
{
	if (this == &from) return *this; // selfassignment
	l_EnX	=from.l_EnX;
	l_EnY	=from.l_EnY;
	l_t_trigX = from.l_t_trigX;
	l_t_trigY = from.l_t_trigY;
	l_X 	= from.l_X;
	l_Y 	= from.l_Y;
	l_Tof 	= from.l_Tof;
	l_used 	= from.l_used;
	l_part 	= from.l_part;

	return *this;
}
//--------------------------
Int_t MoDSSDSSDEvent::Init()
{
	Int_t rev = 0;
	Clear();
	return rev;
}
//------------------------
Int_t MoDSSDSSDEvent::Fill()
{
	Int_t rev = 0;
	Clear();
	return rev;
}
//-----------------------------
void MoDSSDSSDEvent::Clear(Option_t *t)
{ 
	l_EnX.clear();
	l_EnY.clear();
	l_t_trigX.clear();
	l_t_trigY.clear();
	l_X.clear();
	l_Y.clear();
	
	l_Tof = -1;
	l_used = false;
	l_part = 'a';
}
void MoDSSDSSDEvent::ClearEvent()
{ 
	l_EnX.clear();
	l_EnY.clear();
	l_t_trigX.clear();
	l_t_trigY.clear();
	l_X.clear();
	l_Y.clear();
	
	l_Tof = -1;
	l_used = false;
	l_part = 'a';
}
Int_t MoDSSDSSDEvent::Dump(){
	cout<<"l_EnX:\t";
	for(std::vector<Double_t>::iterator it = l_EnX.begin();it<l_EnX.end(); it++) cout<<"\t"<<*it<<endl; 

	cout<<"l_EnX:\t";
	for(std::vector<Double_t>::iterator it = l_EnY.begin();it<l_EnY.end(); it++) cout<<"\t"<<*it<<endl; 

	cout<<"l_t_trigX:\t";
	for(std::vector<Long64_t>::iterator it = l_t_trigX.begin();it<l_t_trigX.end(); it++) cout<<"\t"<<*it<<endl; 

	cout<<"l_t_trigY:\t";
	for(std::vector<Long64_t>::iterator it = l_t_trigY.begin();it<l_t_trigY.end(); it++) cout<<"\t"<<*it<<endl; 

	cout<<"l_X:\t";
	for(std::vector<Int_t>::iterator it = l_X.begin();it<l_X.end(); it++) cout<<"\t"<<*it<<endl; 

	cout<<"l_Y:\t";
	for(std::vector<Int_t>::iterator it = l_Y.begin();it<l_Y.end(); it++) cout<<"\t"<<*it<<endl; 

	cout<<"l_Tof:\t"<<l_Tof<<endl;
	cout<<"l_used:\t"<<l_used<<endl;
	cout<<"l_part:\t"<<l_part<<endl;
	
	return 1;
}

//Return the Detector number

Int_t	MoDSSDSSDEvent::CalcStripNum(Int_t det, Int_t cha){
	if(det == 0)return cha;
	if(det == 1)return cha+16;
	if(det == 2)return cha+32;
	if(det == 3)return cha+46;
	if(det == 4)return cha;
	if(det == 5)return cha+16;
	if(det == 6)return cha+32;
	if(det == 7)return cha+46;
	else return -1;
}
//--------- Setters ------------------------
Int_t 	MoDSSDSSDEvent::Set_l_EnX(Double_t nrj)
{
	l_EnX.push_back(nrj);
	return 1;
}	
Int_t 	MoDSSDSSDEvent::Set_l_EnY(Double_t nrj)
{
	l_EnY.push_back(nrj);
	return 1;
}	
Int_t 	MoDSSDSSDEvent::Set_l_t_trigX(Long64_t time)
{
	l_t_trigX.push_back(time);
	return 1;
}
Int_t 	MoDSSDSSDEvent::Set_l_t_trigY(Long64_t time)
{
	l_t_trigY.push_back(time);
	return 1;
}
Int_t 	MoDSSDSSDEvent::Set_l_X(Int_t X)
{
	l_X.push_back(X);
	return 1;
}
Int_t 	MoDSSDSSDEvent::Set_l_Y(Int_t Y)
{
	l_Y.push_back(Y);
	return 1;
}
Int_t 	MoDSSDSSDEvent::Set_l_Tof(Double_t tof)
{
	l_Tof = tof;
	return 1;
}
Int_t 	MoDSSDSSDEvent::Set_l_used(Bool_t used)
{
	l_used = used;
	return 1;
}
Int_t 	MoDSSDSSDEvent::Set_l_part(TString part)
{
	l_part = part;
	return 1;
}
//--------- getters ------------------------
//Returns the iterator to the first element of the vector or the value for a single value.

Int_t 	MoDSSDSSDEvent::Get_l_EnX(std::vector<Double_t>::iterator it){

	it = l_EnX.begin();
	return 1;
}
Int_t 		MoDSSDSSDEvent::Get_l_EnY(std::vector<Double_t>::iterator it){
	it = l_EnY.begin();
	return 1;
}
Int_t  		MoDSSDSSDEvent::Get_l_t_trigX(std::vector<Long64_t>::iterator it){
	it = l_t_trigX.begin();
	return 1;
}
Int_t  		MoDSSDSSDEvent::Get_l_t_trigY(std::vector<Long64_t>::iterator it){
	it = l_t_trigY.begin();
	return 1;
}

Int_t 		MoDSSDSSDEvent::Get_l_X(std::vector<Int_t>::iterator it){
	it = l_X.begin();
	return 1;
}

Int_t 		MoDSSDSSDEvent::Get_l_Y(std::vector<Int_t>::iterator it){
	it = l_Y.begin();
	return 1;
}

Double_t	MoDSSDSSDEvent::Get_l_Tof(){
	return l_Tof;
}
Bool_t		MoDSSDSSDEvent::Get_l_used(){
	return l_used;
}
TString		MoDSSDSSDEvent::Get_l_part(){
	return l_part;
}

//Returns the iterator with the position of the Max energy of X side
Int_t		MoDSSDSSDEvent::Get_max_EnX(std::vector<Double_t>::iterator it){
	it = max_element(l_EnX.begin(),l_EnX.end());	
	return 1;
}
Int_t		MoDSSDSSDEvent::Get_max_EnX(){
	std::vector<Double_t>::iterator it;
	it = max_element(l_EnX.begin(),l_EnX.end());	
	return *it;
}	
//Returns the iterator with the position of the Max energy of Y side
Int_t		MoDSSDSSDEvent::Get_max_EnY(std::vector<Double_t>::iterator it){
	it = max_element(l_EnY.begin(),l_EnY.end());	
	return 1;
}	
Int_t		MoDSSDSSDEvent::Get_max_EnY(){
	std::vector<Double_t>::iterator it;
	it = max_element(l_EnY.begin(),l_EnY.end());	
	return *it;
}	
//Get_X_max
//Returns the X strip for the maximum energy in the event
Int_t 		MoDSSDSSDEvent::Get_X_max(){
	std::vector<Double_t>::iterator it_EnX = max_element(l_EnX.begin(),l_EnX.end());
	Int_t 	pos = it_EnX - l_EnX.begin();
	std::vector<Int_t>::iterator it_X = l_X.begin()+pos;	
	return *it_X;	
}

//Get_Y_max
//Returns the Y strip for the maximum energy in the event
Int_t 		MoDSSDSSDEvent::Get_Y_max(){
	std::vector<Double_t>::iterator it_EnY = max_element(l_EnY.begin(),l_EnY.end());
	Int_t 	pos = it_EnY - l_EnY.begin();
	std::vector<Int_t>::iterator it_Y = l_Y.begin()+pos;	
	return *it_Y;
}	

//Get_t_trig_max
//Returns the t_trig for the maximum X energy in the event
Long64_t	MoDSSDSSDEvent::Get_t_trigX_max(){

	std::vector<Double_t>::iterator it_EnX = max_element(l_EnX.begin(),l_EnX.end());
	Int_t 	pos = it_EnX - l_EnX.begin();
	std::vector<Long64_t>::iterator it_X = l_t_trigX.begin()+pos;	
	return *it_X;
}

//Get_t_trig_max
//Returns the t_trig for the maximum Y energy in the event
Long64_t	MoDSSDSSDEvent::Get_t_trigY_max(){

	std::vector<Double_t>::iterator it_EnY = max_element(l_EnY.begin(),l_EnY.end());
	Int_t 	pos = it_EnY - l_EnY.begin();
	std::vector<Long64_t>::iterator it_Y = l_t_trigY.begin()+pos;	
	return *it_Y;

}
Int_t	MoDSSDSSDEvent::Get_EnX_size(){
	return l_EnX.size();
}
Int_t	MoDSSDSSDEvent::Get_EnY_size(){
	return l_EnY.size();
}

//-----------------------------
ClassImp(MoDSSDSSDEvent)
