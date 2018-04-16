#ifndef MAPBOX_H
#define MAPBOX_H

#include "MapElement.h"

class MapBox: public MapElement
{
	friend class rob_test;

	public:
		// constructor
		MapBox();
		// ~

		// set- functions
		void setPall(int);
		void setIrid(int);
		void setPlat(int);
		void setAccess(float);
		void setDanger(bool);
		void setExist(bool);
		void setInjured(bool);

		// get- functions
		int getPall(void);
		int getIrid(void);
		int getPlat(void);
		float getAccess(void);
		bool getDanger(void);
		bool getExist(void);
		bool getInjured(void);

		// print information
		virtual void MapInfo(void);

	private:
		int pallCont; 	// palladium 0-100
		int iridCont; 	// iridium 0-100
		int platCont; 	// platinum 0-100
		float access; 	// access danger 0-0.9
		bool danger; 	// flag for danger 1-> very dangerous
		bool robExist;  // robot on box
		bool injured;   // injured robot on box
};
#endif

