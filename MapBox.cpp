#include <iostream>
#include <stdlib.h> /* srand, rand */
#include "MapBox.h"

using namespace std;

// MapBox contructor
MapBox::MapBox() : MapElement()
{
	setPall(rand()%100 + 1);
	setIrid(rand()%100 + 1);
	setPlat(rand()%100 + 1);
	setAccess(rand()%10/(float)10);
	setDanger(false);
	setExist(false);
	setInjured(false);
}

// set- functions
void MapBox::setPall(int x)
{
	pallCont = x;
}

void MapBox::setIrid(int x)
{
	iridCont = x;
}

void MapBox::setPlat(int x)
{
	platCont = x;
}

void MapBox::setAccess(float x)
{
	access = x;
}

void MapBox::setDanger(bool x)
{
	danger = x;
}

void MapBox::setExist(bool x)
{
	robExist = x;
}

void MapBox::setInjured(bool x)
{
	injured = x;
}

// get- functions
int MapBox::getPall(void)
{
	return pallCont;
}

int MapBox::getIrid(void)
{
	return iridCont;
}

int MapBox::getPlat(void)
{
	return platCont;
}

float MapBox::getAccess(void)
{
	return access;
}

bool MapBox::getDanger(void)
{
	return danger;
}

bool MapBox::getExist(void)
{
	return robExist;
}

bool MapBox::getInjured(void)
{
	return injured;
}

// print iformation
void MapBox::MapInfo(void)
{
	MapElement::MapInfo();

	cout<<"Palladium: "<<getPall()<<"|";
	cout<<"Iridium: "<<getIrid()<<"|";
	cout<<"Platinum: "<<getPlat()<<"|";
	cout<<"Access: "<<getAccess()<<"|";
	cout<<"Flag: "<<getDanger()<<"|";
	if (!getExist())
	{
		cout<<"No ";
	}
	cout<<"Robot Here"<<endl;
}