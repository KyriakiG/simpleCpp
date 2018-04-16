#include "AnaRob.h"

#include <iostream>
#include <stdlib.h> /* srand, rand */

using namespace std;

// AnaRob constructor
AnaRob::AnaRob(int N, MapElement ***p) : Robot(N, p)
{
    rounds = getSpeed();

	setMaxCar(rand()%76 + 25);
	setCarP(0);
	setCarI(0);
	setCarL(0);
	//cout<<"An analysis robot has been created."<<endl;
}

// AnaRob motion method
void AnaRob::Move(int N, MapElement ***p)
{
	int newPos;
	int count=0;

    if (!ok)
    {
        // if AnaRob cannot carry anything else go towards base!
        if (getCarP() + getCarI() + getCarL() == getMaxCar())
        {
            int tempdx=0, tempdy=0;

            // go towards base
            if ((getPosx()==0 || getPosx()==1) && (getPosy()==0 || getPosy()==1))
            {
                if (getPosx()==1)
                    tempdx = -1;

                if (getPosy()==1)
                    tempdy = -1;

                ok = true;
            }
            else if (getPosx()!=0)
                if (getPosy()!=0)
                {
                    if (!(dynamic_cast<MapBox*> (p[getPosx()-1][getPosy()-1]))->getExist())
                    {
                        tempdx = -1;
                        tempdy = -1;
                        ok = true;
                    }
                    else if (!(dynamic_cast<MapBox*> (p[getPosx()][getPosy()-1]))->getExist())
                    {
                        tempdx = 0;
                        tempdy = -1;
                        ok = true;
                    }
                    else if (!(dynamic_cast<MapBox*> (p[getPosx()-1][getPosy()]))->getExist())
                    {
                        tempdx = -1;
                        tempdy = 0;
                        ok = true;
                    }
                    else // not able to move
                    {
                        cout<<"This robot is not able to move now."<<endl;
                        ok = false; // try again at next round
                    }
                }
                else
                {
                    if (!(dynamic_cast<MapBox*> (p[getPosx()-1][0]))->getExist())
                    {
                        tempdx = -1;
                        tempdy = 0;
                        ok = true;
                    }
                    else // not able to move
                    {
                        cout<<"This robot is not able to move now."<<endl;
                        ok = false; // try again at next round
                    }
                }
            else
                if (getPosy()!=0)
                {
                    if (!(dynamic_cast<MapBox*> (p[0][getPosy()-1]))->getExist())
                    {
                        tempdx = 0;
                        tempdy = -1;
                        ok = true;
                    }
                    else // not able to move
                    {
                        cout<<"This robot is not able to move now."<<endl;
                        ok = false; // try again at next round
                    }
                }

            // if able to move towards base
            if (ok)
            {
                // leaving...
                (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setExist(false);

                // coming...
                setPosx(getPosx()+tempdx);
                setPosy(getPosy()+tempdy);
                // when robot reaches base, is destroyed
                if (getPosx()!=0 || getPosy()!=0)
                    (dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->setExist(true);

                // go to next position
                go = true;
            }

        }
        // if can carry, choose a random next position
        else
        {
            do {
                count++;

                newPos = Robot::Move(N);

        	    // if new position has no other robot, not very dangerous and there is at least half of the amount this AnaRob can carry, ok --- try 10 times
                if ((!(dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getExist() &&
                    !(dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getDanger() &&
                    ((dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getPall() + (dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getIrid() + (dynamic_cast<MapBox*> (p[newPos%N][newPos/N]))->getPlat())>=getMaxCar()/2))
                {
                    ok = true;
                }

                // exit after 10 tries
                if (!ok && count==10)
                {
                    ok = true;
                    count = 0;
                }

            } while (!ok);

            // if didn't found a satisfying next position
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
    }

    // in motion...
    if (go)
	{
	    // if reached base
	    if (rounds==0 && getPosx()==0 && getPosy()==0)
        {
            cout<<"Just got in BASE!";
        }
        // if next position is base
        else if (getPosx()==0 && getPosy()==0)
        {
            cout<<"New position: BASE. Get there at the next round."<<endl;
        }
        // if reached next position
	    else if (rounds==0)
        {
            // run damage probability method
            setCond(Damage((dynamic_cast<MapBox*> (p[getPosx()][getPosy()]))->getAccess()));

            cout<<"Just got in position: "<<getPosx()<<", "<<getPosy()<<". ";
            moves++;

            // if can carry more, change to operation mode
            if (getCarP() + getCarI() + getCarL() != getMaxCar())
                setOrder(true);

            ok = false; rounds = getSpeed(); go = false; // clear motion vars

            // if AnaRob injured
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

// AnaRob operation method
void AnaRob::Oper(MapBox *m, int PA, int IR, int PL)
{
    int need[3] = {PA, IR, PL};

	Robot::Oper();

	int carperround = getMaxCar() / getSpeed();	// amount can get in every round

	int amount[3], sum;

	// set how much can carry in this round
	if ((getCarP() + getCarI() + getCarL() + carperround) <= getMaxCar())
		sum = carperround;
	else
		sum = getMaxCar() - getCarP() - getCarI() - getCarL();

	// if can carry more and there is at least one unit of material in the position box, get materials
	if (sum!=0 && (m->getPall()!=0 || m->getIrid()!=0 || m->getPlat()!=0))
	{
		cout<<"Palladium needed : "<<need[0];
		cout<<"|Iridium needed   : "<<need[1];
		cout<<"|Platinum needed  : "<<need[2];
		cout<<endl;

		cout<<"\t   Can carry in every round : "<<carperround;
		cout<<" | Can carry in this round  : "<<sum<<endl;

		if (need[0]>=need[1])
		{
			if(need[0]>=need[2])
			{
				if(need[1]>=need[2])	//pall >= irid >= plat // 0,1,2
				{
				if(m->getPall()<=sum)						//palladio sthn perioxh ligotero/iso ap' auto pou mporei na e3oryx8ei
				{
					if(m->getPall()!=0)						//palladio yparxei
					{
						amount[0] = m->getPall();			//programmatismos e3ory3hs olou tou palladiou
						sum -= m->getPall();					//enhmerwsh metrhth gyrou
					}
					else									//palladio den yparxei
					{
						amount[0] = 0;						//programmatismos mh e3ory3hs
					}

					if(m->getIrid()<=sum)					//iridio sthn perioxh ligotero/iso ap' auto pou mporei na e3oryx8ei
					{
						if (m->getIrid()!=0)				//iridio yparxei
						{
							amount[1] = m->getIrid();		//programmatismos e3ory3hs olou tou iridiou
							sum -= m->getIrid();			//enhmerwsh metrhth gyrou
						}
						else								//iridio den yparxei
						{
							amount[1] = 0;					//programmatismos mh e3ory3hs
						}

						if(m->getPlat()<=sum)				//leukoxrysos sthn perioxh ligotero/iso ap' auto pou mporei na e3oryx8ei
						{
							if (m->getPlat()!=0)			//leykoxrysos yparxei
							{
								amount[2] = m->getPlat();	//programmatismos e3ory3hs olou tou leukoxrysou
								sum -= m->getPlat();		//enhmerwsh metrhth gyrou
							}
							else							//leykoxrysos den yparxei
							{
								amount[2] = 0;				//programmatismos mh e3ory3hs
							}
						}
						else								//leukoxrysos sthn perioxh parapanw ap' auto pou mporei na e3oryx8ei
						{
							amount[2] = sum;				//programmatismos e3ory3hs
							sum = 0;						//enhmerwsh metrhth gyrou
						}
					}

					else									//iridio sthn perioxh parapanw ap' auto pou mporei na e3oryx8ei
					{
						amount[1] = sum;					//programmatismos e3ory3hs
						amount[2] = 0;
						sum = 0;							//enhmerwsh metrhth gyrou
					}

				}
				else										//palladio sthn perioxh parapanw ap' auto pou mporei na e3oryx8ei
				{
					amount[0] = sum;						//programmatismos e3ory3hs
					amount[1] = 0;
					amount[2] = 0;
					sum = 0;								//enhmerwsh metrhth gyrou
				}
			}
				else					//pall >= plat >= irid // 0,2,1 (antistoixws)
				{
				if(m->getPall()<=sum)
				{
					if(m->getPall()!=0)
					{
						amount[0] = m->getPall();
						sum -= m->getPall();
					}
					else
					{
						amount[0] = 0;
					}

					if(m->getPlat()<=sum)
					{
						if (m->getPlat()!=0)
						{
							amount[2] = m->getPlat();
							sum -= m->getPlat();
						}
						else
						{
							amount[2] = 0;
						}

						if(m->getIrid()<=sum)
						{
							if (m->getIrid()!=0)
							{
								amount[1] = m->getIrid();
								sum -= m->getIrid();
							}
							else
							{
								amount[1] = 0;
							}
						}
						else
						{
							amount[1] = sum;
							sum = 0;
						}
					}

					else
					{
						amount[2] = sum;
						amount[1] = 0;
						sum = 0;
					}

				}
				else
				{
					amount[0] = sum;
					amount[2] = 0;
					amount[1] = 0;
					sum = 0;
				}

			}
			}
			else						//plat >= pall >= irid // 2,0,1 (antistoixws)
			{
				if(m->getPlat()<=sum)
				{
					if(m->getPlat()!=0)
					{
						amount[2] = m->getPlat();
						sum -= m->getPlat();
					}
					else
					{
						amount[2] = 0;
					}

					if(m->getPall()<=sum)
					{
						if (m->getPall()!=0)
						{
							amount[0] = m->getPall();
							sum -= m->getPall();
						}
						else
						{
							amount[0] = 0;
						}

						if(m->getIrid()<=sum)
						{
							if (m->getIrid()!=0)
							{
								amount[1] = m->getIrid();
								sum -= m->getIrid();
							}
							else
							{
								amount[0] = 0;
							}
						}
						else
						{
							amount[1] = sum;
							sum = 0;
						}
					}

					else
					{
						amount[0] = sum;
						amount[1] = 0;
						sum = 0;
					}

				}
				else
				{
					amount[2] = sum;
					amount[0] = 0;
					amount[1] = 0;
					sum = 0;
				}
            }
		}
		else
		{
			if (need[1]>=need[2])
			{
				if (need[0]>=need[2])	//irid >= pall >= plat // 1,0,2 (antistoixws)
				{
				if(m->getIrid()<=sum)
				{
					if(m->getIrid()!=0)
					{
						amount[1] = m->getIrid();
						sum -= m->getIrid();
					}
					else
					{
						amount[1] = 0;
					}

					if(m->getPall()<=sum)
					{
						if (m->getPall()!=0)
						{
							amount[0] = m->getPall();
							sum -= m->getPall();
						}
						else
						{
							amount[0] = 0;
						}

						if(m->getPlat()<=sum)
						{
							if (m->getPlat()!=0)
							{
								amount[2] = m->getPlat();
								sum -= m->getPlat();
							}
							else
							{
								amount[2] = 0;
							}
						}
						else
						{
							amount[2] = sum;
							sum = 0;
						}
					}

					else
					{
						amount[0] = sum;
						amount[2] = 0;
						sum = 0;
					}

				}
				else
				{
					amount[1] = sum;
					amount[0] = 0;
					amount[2] = 0;
					sum = 0;
				}
			}
				else					//irid >= plat >= pall // 1,2,0 (antistoixws)
				{
				if(m->getIrid()<=sum)
				{
					if(m->getIrid()!=0)
					{
						amount[1] = m->getIrid();
						sum -= m->getIrid();
					}
					else
					{
						amount[1] = 0;
					}

					if(m->getPlat()<=sum)
					{
						if (m->getPlat()!=0)
						{
							amount[2] = m->getPlat();
							sum -= m->getPlat();
						}
						else
						{
							amount[2] = 0;
						}

						if(m->getPall()<=sum)
						{
							if (m->getPall()!=0)
							{
								amount[0] = m->getPall();
								sum -= m->getPall();
							}
							else
							{
								amount[0] = 0;
							}
						}
						else
						{
							amount[0] = sum;
							sum = 0;
						}
					}

					else
					{
						amount[2] = sum;
						amount[0] = 0;
						sum = 0;
					}

				}
				else
				{
					amount[1] = sum;
					amount[2] = 0;
					amount[0] = 0;
					sum = 0;
				}
			}
			}
			else						//plat >= irid >= pall // 2,1,0 (antistoixws)
			{
				if(m->getPlat()<=sum)
				{
					if(m->getPlat()!=0)
					{
						amount[2] = m->getPlat();
						sum -= m->getPlat();
					}
					else
					{
						amount[2] = 0;
					}

					if(m->getIrid()<=sum)
					{
						if (m->getIrid()!=0)
						{
							amount[1] = m->getIrid();
							sum -= m->getIrid();
						}
						else
						{
							amount[1] = 0;
						}

						if(m->getPall()<=sum)
						{
							if (m->getPall()!=0)
							{
								amount[0] = m->getPall();
								sum -= m->getPall();
							}
							else
							{
								amount[0] = 0;
							}
						}
						else
						{
							amount[0] = sum;
							sum = 0;
						}
					}

					else
					{
						amount[1] = sum;
						amount[0] = 0;
						sum = 0;
					}

				}
				else
				{
					amount[2] = sum;
					amount[1] = 0;
					amount[0] = 0;
					sum = 0;
				}
		}
		}

		cout<<"\t   Pall get : "<<amount[0];
		cout<<"|Irid get : "<<amount[1];
		cout<<"|Plat get : "<<amount[2];

		cout<<endl;

		setCarP(getCarP() + amount[0]);
		m->setPall(m->getPall() - amount[0]);
		addMat(0, amount[0]);

		setCarI(getCarI() + amount[1]);
		m->setIrid(m->getIrid() - amount[1]);
		addMat(1, amount[1]);

		setCarL(getCarL() + amount[2]);
		m->setPlat(m->getPlat() - amount[2]);
        addMat(2, amount[2]);

		cout<<endl;
	}
	else if (sum == 0)
	{
		cout<<"Robot cannot carry anything else."<<endl;
        // change to motion mode
		setOrder(false);
	}
	else
	{
		cout<<"This area is empty."<<endl;
        // change to motion mode
		setOrder(false);
	}
}


void AnaRob::setMaxCar(int m)
{
	maxCar = m;
}

void AnaRob::setCarP(int p)
{
	carP = p;
}

void AnaRob::setCarI(int i)
{
	carI = i;
}

void AnaRob::setCarL(int l)
{
	carL = l;
}

//get- methods
int AnaRob::getMaxCar(void)
{
	return maxCar;
}

int AnaRob::getCarP(void)
{
	return carP;
}

int AnaRob::getCarI(void)
{
	return carI;
}

int AnaRob::getCarL(void)
{
	return carL;
}

// print information
void AnaRob::RobInfo(void)
{
	//klhsh ths Robot print info
	Robot::RobInfo();
	cout<<"   Max carry: "<<maxCar<<"|";
	cout<<"Palladium carry: "<<getCarP()<<"|";
	cout<<"Iridium carry: "<<getCarI()<<"|";
	cout<<"Platinum carry: "<<getCarL();
	cout<<endl;
}

// sum carry methods
// print total amount of all materials
void AnaRob::printMat(void)
{
    cout<<"All Palladium: "<<addM(0, 0, false)<<endl;
    cout<<"All Iridium: "<<addM(1, 0, false)<<endl;
    cout<<"All Platinum: "<<addM(2, 0, false)<<endl;
}

void AnaRob::addMat(int which, int howmuch)
{
	// just to be used as void method by main with no problem
    int temp ;

    temp = addM(which, howmuch, true);
}

int AnaRob::addM(int which, int howmuch, bool x)
{
    static int allamount[3] {0, 0, 0}; // initialize counters once for all robots

    if (x)
        allamount[which] += howmuch;

    return allamount[which];
}
