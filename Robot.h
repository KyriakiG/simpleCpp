#ifndef ROBOT_H
#define ROBOT_H

#include "MapBox.h"

class Robot
{
	friend class MapBox;

	public:
		// constructor
		Robot(int N, MapElement ***p);

		// set- methods
		void setSpeed(int);
		void setCond(bool);
		void setAccAbility(float);
		void setPosx(int);
		void setPosy(int);
		void setOrder(bool);
		void setRounds(int);

		// get- methods
		int getSpeed(void);
		bool getCond(void);
		float getAccAbility(void);
		int getPosx(void);
		int getPosy(void);
		bool getOrder(void);
		int getRounds(void);

		// print information
		void RobInfo(void);

		// motion method
		int Move(int N);

		// operation method
		void Oper(void);

		// damage probability method
		bool Damage(float);

		// plus rounds method
		void PlusRound(void);

	private:
		int speed; 			// speed: speed of operation and motion, 1-5
		bool cond; 			// condition: 1->injured, 0->healthy
		float accability; 	// access ability: values 0.1-1, 1->most likely get injured
		int posx; 			// position: x,y map coordinates
		int posy;
		bool order;			// order: 1->operate, 0->move
        int roundsInjured;  // how many rounds robot is injured

    protected:
        int damages;        // how many times got ingured
        int moves;          // how many moves
	    bool ok, go;        /* motion vars */
	    int rounds;         /*ok->next position chosen | go->able to go to next position | rounds->count rounds to get there */

};
#endif