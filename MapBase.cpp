#include <iostream>
#include "MapBase.h"
#define NEED 100

using namespace std;

//constructor
MapBase::MapBase() : MapElement()
{
	setPALL(NEED);
	setIRID(NEED);
	setPLAT(NEED);
}
//set- functions
void MapBase::setPALL(int x)
{
	PALL_NEED = x;
}

void MapBase::setIRID(int x)
{
	IRID_NEED = x;
}

void MapBase::setPLAT(int x)
{
	PLAT_NEED = x;
}

//get- functions
int MapBase::getPALL(void)
{
	return PALL_NEED;
}

int MapBase::getIRID(void)
{
	return IRID_NEED;
}

int MapBase::getPLAT(void)
{
	return PLAT_NEED;
}

//print information
void MapBase::MapInfo(void)
{
	MapElement::MapInfo();

	cout<<"Palladium Need: "<<getPALL()<<endl;
	cout<<"Iridium Need: "<<getIRID()<<endl;
	cout<<"Platinum Need: "<<getPLAT()<<endl;
}
