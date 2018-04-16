#include <iostream>
#include "Robot.h"
#include "MapElement.h"
#include "MapBox.h"
#include "MapBase.h"
#include <stdlib.h> /* srand, rand */

using namespace std;

// Robot constructor
Robot::Robot(int N, MapElement ***p)
{
    damages = 0;
    moves = 0;
	setSpeed(rand()%5 + 1);
	setCond(false);
	setAccAbility((rand()%10+1)/(float)10);
	setOrder(false);
	setRounds(0);

	bool ok=false;
	int x, y;
	do{
		x = rand()%N;
		y = rand()%N;
		if ((x!=0 || y!=0) && (dynamic_cast<MapBox*> (p[x][y]))->getExist())
		{
			ok = false;
		}
		else if (x==0 && y==0)
		{
			ok = false;
		}
		else
		{
			ok = true;
			setPosx(x);
			setPosy(y);
			(dynamic_cast<MapBox*> (p[x][y]))->setExist(true);
		}
	} while (!ok);

}


// set- functions
void Robot::setSpeed(int x)
{
	speed = x;
}

void Robot::setCond(bool x)
{
	cond = x;
}

void Robot::setAccAbility(float x)
{
	accability = x;
}

void Robot::setPosx(int x)
{
	posx = x;
}

void Robot::setPosy(int x)
{
	posy = x;
}

void Robot::setOrder(bool x)
{
	order = x;
}

void Robot::setRounds(int x)
{
	roundsInjured = x;
}

// get- functions
int Robot::getSpeed(void)
{
	return speed;
}

bool Robot::getCond(void)
{
	return cond;
}

float Robot::getAccAbility(void)
{
	return accability;
}

int Robot::getPosx(void)
{
	return posx;
}

int Robot::getPosy(void)
{
	return posy;
}

bool Robot::getOrder(void)
{
	return order;
}

int Robot::getRounds(void)
{
	return roundsInjured;
}

// print information
void Robot::RobInfo(void)
{
	cout<<"RobInfo :"<<"|";
	cout<<"Position: "<<getPosx()<<", "<<getPosy()<<"|";
	cout<<"Speed: "<<getSpeed()<<"|";
	cout<<"Condition: "<<getCond()<<"|";
	cout<<"Access: "<<getAccAbility()<<"|Moves: "<<moves<<"|Damages: "<<damages;
}


// motion function
int Robot::Move(int N)
{
	static int rounds = speed;

	int x, y, dx=0, dy=0;

	x = getPosx();
	y = getPosy();
	// cout<<"Current position: "<<x<<", "<<y;
	while (dx==0 && dy==0 || (x+dx)==0 && (y+dy)==0)
	{
		if (x!=0 && x!=(N-1))
		{
			dx = rand()%3-1;
		}
		else if (x==0)
		{
			dx = rand()%2;
		}
		else
		{
			dx = rand()%2-1;
		}

		if (y!=0 && y!=(N-1))
		{
			dy = rand()%3-1;
		}
		else if (y==0)
		{
			dy = rand()%2;
		}
		else
		{
			dy = rand()%2-1;
		}
	}

	x += dx;
	y += dy;

	return x + N * y;
}

// operation function
void Robot::Oper(void)
{
	// empty...
}

bool Robot::Damage(float danger)
{
    float probability;
    int temp;

    // initialize probability
    probability = danger * (1 - getAccAbility());
    // random number
    temp = rand()%101;

    // see/set if injured
    if (temp <= (probability*100))
        return true;
    else
        return false;
}

void Robot::PlusRound(void)
{
    setRounds(getRounds()+1);
}
