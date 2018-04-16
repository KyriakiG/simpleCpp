#ifndef MAPBASE_H
#define MAPBASE_H

#include "MapElement.h"

class MapBase: public MapElement
{
	friend class Robot;
	
	public:
		//constructor
		MapBase();
		//~

		//set- functions
		void setPALL(int);
		void setIRID(int);
		void setPLAT(int);

		//get- functions
		int getPALL(void);
		int getIRID(void);
		int getPLAT(void);

		//print information
		virtual void MapInfo(void);

	private:
		int PALL_NEED; 	//palladium
		int IRID_NEED; 	//iridium
		int PLAT_NEED; 	//platinum
};

#endif