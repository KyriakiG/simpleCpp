#include "ResRob.h"

#include <iostream>

using namespace std;

// ResRob constructor
ResRob::ResRob(int N, MapElement ***p) : Robot(N, p)
{
    sumRescued = 0;
    rounds = getSpeed();
	//cout<<"A rescue robot has been created."<<endl;
}

// ResRob motion method
void ResRob::Move(int N, MapElement ***p)
{
	int newPos;
	int count=0;

	if (!ok)
	{
		do {
            count++;

			newPos = Robot::Move(N);

            // if new position has injured robot and not very dangerous, ok --- try 10 times
			if ((dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getInjured() && !(dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getDanger())
			{
				ok = true;
			}

            // if injured robot not found nearby
			if (!ok && count==10)
			{
			    do{
                    count++;

                    newPos = Robot::Move(N);

                    // if new position not very dangerous, ok --- try 10 times
                    if (!(dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getDanger())
                    {
                        ok = true;
                    }

                    // if not able to move after 20 tries, exit
                    if (!ok && count==20)
                    {
                        ok = true;
                        count = 0;
                    }
			    } while (!ok);
			}
		} while (!ok);

        // not able to move
		if (count==0)
        {
            cout<<"This robot is not able to move now."<<endl;
            ok = false; // try again at next round
        }
        else
        {
            // leaving...
            (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setInjured(false);
            (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setExist(false);

            // coming...
            setPosx(newPos%N);
            setPosy(newPos/N);
            (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setExist(true);

            // go to next position
            go = true;
        }

	}

	// in motion...
	if (go)
    {
        // if there
        if (rounds==0)
        {
            // run damage probability method
            setCond(Damage((dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->getAccess()));

            cout<<"Just got in position: "<<getPosx()<<", "<<getPosy()<<". ";
            moves++;

            // if injured robot exists, change to operation mode
            if ((dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->getInjured())
            {
                setOrder(true);
                rounds = getSpeed() - 1;
            }
            else
                rounds = getSpeed();

            ok = false; go = false; // clear motion vars

            // if ResRob injured
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

// ResRob operation method
/* injured robot found, start repairing */
void ResRob::Oper(Robot *r)
{
    Robot::Oper();

	// if got repaired
	if(rounds == 0)
	{
		r->setCond(false);
        cout<<"Robot has been rescued!"<<endl;

        sumRescued++; // update counter of rescued robots

        // change to motion mode
		setOrder(false);
		rounds = getSpeed() - 1;
	}
	else
	{
	    if (rounds==getSpeed() - 1){
            r->setRounds(0); // stop counting injured rounds
        }

		cout<<"Robot will be rescued in "<<rounds<<" more rounds"<<endl;
		rounds--;
	}
}

// print information
void ResRob::RobInfo(void)
{
	Robot::RobInfo();
	cout<<"|Rescued: "<<sumRescued;
}
