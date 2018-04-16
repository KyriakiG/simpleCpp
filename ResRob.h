#ifndef RESROB_H
#define RESROB_H

#include "Robot.h"

class ResRob: public Robot
{
	public:
		// constructor
		ResRob(int N, MapElement ***p);
		// ~

		// print information
		void RobInfo(void);

		// motion method
		void Move(int N, MapElement ***p);

		// operation method
		void Oper(Robot *r);

	private:
	    int sumRescued; // all rescued robots by this robot
};

#endif