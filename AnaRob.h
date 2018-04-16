#ifndef ANAROB_H
#define ANAROB_H

#include "Robot.h"

class AnaRob: public Robot
{
	public:

		// constructor
		AnaRob(int N, MapElement ***p);
		// ~

		// set- methods
		void setMaxCar(int);
		void setCarP(int);
		void setCarI(int);
		void setCarL(int);

		// get- methods
		int getMaxCar(void);
		int getCarP(void);
		int getCarI(void);
		int getCarL(void);

		// print information
		void RobInfo(void);

		// motion method
		void Move(int, MapElement ***p);

		// operation method
		void Oper(MapBox *m, int, int, int);

        // sum carry methods
        void addMat(int, int);
        int addM(int, int, bool);
		void printMat(void);

	private:

		int maxCar;	// max amount that can carry
		int carP;	// amount of Palladium
		int carI;	// amount of Iridium
		int carL;	// amount of PLatinum
};

#endif