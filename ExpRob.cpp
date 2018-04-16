#include "ExpRob.h"

#include <iostream>

using namespace std;

// ExpRob constructor
ExpRob::ExpRob(int N, MapElement ***p) : Robot(N, p)
{
    sumFlags = 0;
    rounds = getSpeed();
	//cout<<"An exploration robot has been created."<<endl;
}

// ExpRob motion method
void ExpRob::Move(int N, MapElement ***p)
{
	int newPos;
	int count=0;

	if (!ok)
	{
		do {
            count++;

			newPos = Robot::Move(N);

            // if new position has no robot and not very dangerous, ok --- try 10 times
			if (!(dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getExist() && !(dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getDanger())
			{
				ok = true;
			}

            if (!ok && count==10)
            {
                ok = true;
                count = 0;
            }

		} while (!ok);

		if (count==0)
        {
            cout<<"This robot is not able to move now."<<endl;
            ok = false; // try again at next round
        }
        else
        {
            // leaving...
            (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setExist(false);

            // coming...
            setPosx(newPos%N);
            setPosy(newPos/N);
            (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setExist(true);

            // go to next position
            go = true;
        }
	}

	// in motion
	if (go)
    {
        // if there
        if (rounds==0)
        {
            // run damage probability method
            setCond(Damage((dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->getAccess()));

            cout<<"Just got in Position: "<<getPosx()<<", "<<getPosy()<<". ";
            moves++;

            // change to operation mode
            setOrder(true);

            // set clear motion vars
            ok = false; rounds = getSpeed(); go = false;

            // if ExpRob injured
            if (getCond())
            {
                (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setInjured(true);
                setRounds(1);

                cout<<"DAMAGE!!! Robot out of order.";
                damages++;
            }

            cout<<endl;
        }
        else
        {
            cout<<"New position: "<<getPosx()<<", "<<getPosy()<<". Get there in "<<--rounds<<" more rounds."<<endl;
        }
    }
}

// ExpRob operation method
void ExpRob::Oper(MapBox *m)
{
	Robot::Oper();

	// check how dangerous MapBox is
	if(m->getAccess()>=0.6)
	{
		cout<<"Dangerous area!"<<endl;
		m->setDanger(true); // set flag

		sumFlags++;
        addFlag();
	}
	else
	{
		cout<<"Not so dangerous area."<<endl;
	}

	// change to motion mode
	setOrder(false);
}

// print information
void ExpRob::RobInfo(void)
{
	Robot::RobInfo();
	cout<<"|Flags: "<<sumFlags<<endl;
}

// methods for all flags on the map
void ExpRob::printAllFlags(void)
{
    cout<<"All flags: "<<addF(false)<<endl;
}

void ExpRob::addFlag(void)
{
    // just to be used as void method by main with no problem
    int temp ;

    temp = addF(true);
}

int ExpRob::addF(bool x)
{
    static int allFlags = 0; // initialize counter once for all robots

    if (x)
        allFlags++;

    return allFlags;
}
