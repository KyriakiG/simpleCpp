#ifndef EXPROB_H
#define EXPROB_H

#include "Robot.h"

class ExpRob: public Robot
{
	public:
		// constructor
		ExpRob(int N, MapElement ***p);
		// ~

		// print information
		void RobInfo(void);

		// motion method
		void Move(int N, MapElement ***p);

		// operation method
		void Oper(MapBox *m);
		
		// danger flag methods
		void addFlag(void);
        int addF(bool);
		void printAllFlags(void);

	private:
        int sumFlags;

};

#endif