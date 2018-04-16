#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "MapElement.h"	//map classes
#include "MapBox.h"
#include "MapBase.h"

#include "Robot.h"		//rob classes
#include "ExpRob.h"
#include "ResRob.h"
#include "AnaRob.h"

using namespace std;

int main()
{
	int StartMenu();
	bool Menu(int N, MapElement ***p, int *E, vector<ExpRob> *exp, int *R, vector<ResRob> *res, int *A, vector<AnaRob> *ana);

	srand(time(NULL));
	bool keepGoing = true;
	int choiceA;


	while (keepGoing)
	{
		// print start me
		choiceA = StartMenu();

		if (choiceA==0 || choiceA==2)
		{
			keepGoing = false;
		}
		else
		{
			bool keepGoingSim = true;
			int i, j;

			/* INITIALIZATION */
			/* map size NxN */
			int N;
			cout<<endl<<"Give size of map (4...10, else random.)"<<endl<<"Size : ";

			cin>>N;

			if (N>10 || N<4)
			{
				N = rand()%5 + 6; // size = [4-10]
			}

			cout<<"Size of map: "<<N<<endl;
			cout<<endl;

			/* map initialization */
			MapElement ***p;
			p = new MapElement**[N];

			for(i=0 ; i<N ; i++)
			{
				p[i] = new MapElement*[N];
			}

			p[0][0] = new MapBase();
			for(i=0 ; i<N ; i++)
				for(j=0 ; j<N ; j++)
				{
					if(i!=0 || j!=0)
					{
						p[i][j] = new MapBox();
					}
				}


			/* robot army size */
			int Nexp, Nres, Nana;

			Nexp = (N-1)/2;
			Nres = (N-1)/2;
			Nana = (N-1)/2;


			/* initialization of robots */
			vector<ExpRob> exploreTeam;
			vector<ResRob> rescueTeam;
			vector<AnaRob> analysisTeam;

			for (i=0 ; i<Nexp ; i++)
			{
				exploreTeam.push_back(ExpRob(N, p));
			}

			for (i=0 ; i<Nres ; i++)
			{
				rescueTeam.push_back(ResRob(N, p));
			}

			for (i=0 ; i<Nana ; i++)
			{
				analysisTeam.push_back(AnaRob(N, p));
			}

			cout<<"Initialization is done. Simulation is running..."<<endl;
			cout<<endl;

            /* END OF INITIALIZATION */

            /* START RUNNING SIMULATION */
            // simulation vars for robots' condition
            int erasedRobs=0; // !!! in next block, use i-erasedRobs instead of i
            bool allInjured=true;

			do {
                /* 1 - Exploration Robots Action */
                cout<<endl<<"Exploration robots :"<<endl;

                for (i=0 ; i<Nexp ; i++)
                {
                    cout<<"\t"<<(i+1)<<". ";

                    // check condition
                    if(exploreTeam[i-erasedRobs].getCond())
                    {
                        // if injured for less than 10 rounds, just add a round if not repaired, else erase it!
                        if(exploreTeam[i-erasedRobs].getRounds()!=10)
                        {
                            // if injured and rounds==0 means that robot is getting repaired
                            if (exploreTeam[i-erasedRobs].getRounds()!=0)
                            {
                                exploreTeam[i-erasedRobs].PlusRound();
                                cout<<"Robot is injured. (for "<<exploreTeam[i-erasedRobs].getRounds()<<" rounds)";
                            }
                            else
                            {
                                cout<<"Robot is getting repaired";
                                dynamic_cast <MapBox*> (p[exploreTeam[i-erasedRobs].getPosx()][exploreTeam[i-erasedRobs].getPosy()])->setInjured(false);
                            }
                        }
                        // if injured for 10 rounds, erase robot
                        else
                        {
                            // get out of map
                            dynamic_cast <MapBox*> (p[exploreTeam[i-erasedRobs].getPosx()][exploreTeam[i-erasedRobs].getPosy()])->setExist(false);
                            dynamic_cast <MapBox*> (p[exploreTeam[i-erasedRobs].getPosx()][exploreTeam[i-erasedRobs].getPosy()])->setInjured(false);

                            // get out of robot vector
                            exploreTeam.erase(exploreTeam.begin() + i - erasedRobs);
                            erasedRobs++;

                            cout<<"Robot is injured for too long. It has just been erased.";
                        }
                        cout<<endl;
                    }
                    // if not injured
                    else
                    {
                        // update flag
                        allInjured = false;

                        /* ACTION! */
                        // operation
                        if (exploreTeam[i-erasedRobs].getOrder())
                        {
                            exploreTeam[i-erasedRobs].Oper(dynamic_cast <MapBox*> (p[exploreTeam[i-erasedRobs].getPosx()][exploreTeam[i-erasedRobs].getPosy()]));
                        }
                        // motion
                        else
                        {
                            exploreTeam[i-erasedRobs].Move(N, p);
                        }
                    }

                }

                // update exploration robots number
                Nexp = Nexp - erasedRobs;

                erasedRobs=0; // clear var


                /* 2 - Rescue Robots Action */
                cout<<endl<<"Rescue robots :"<<endl;

                for (i=0 ; i<Nres ; i++)
                {
                    cout<<"\t"<<(i+1)<<". ";

                    // check condition
                    if(rescueTeam[i-erasedRobs].getCond())
                    {
                        // if injured for less than 10 rounds, just add a round if not repaired, else erase it!
                        if(rescueTeam[i-erasedRobs].getRounds()!=10)
                        {
                            // if injured and rounds==0 means that robot is getting repaired
                            if (rescueTeam[i-erasedRobs].getRounds()!=0)
                            {
                                rescueTeam[i-erasedRobs].PlusRound();
                                cout<<"Robot is injured. (for "<<rescueTeam[i-erasedRobs].getRounds()<<" rounds)";
                            }
                            else
                            {
                                cout<<"Robot is getting repaired";
                                dynamic_cast <MapBox*> (p[rescueTeam[i-erasedRobs].getPosx()][rescueTeam[i-erasedRobs].getPosy()])->setInjured(false);
                            }
                        }
                        // if injured for 10 rounds, erase robot
                        else
                        {
                            // get out of map
                            dynamic_cast <MapBox*> (p[rescueTeam[i-erasedRobs].getPosx()][rescueTeam[i-erasedRobs].getPosy()])->setExist(false);
                            dynamic_cast <MapBox*> (p[rescueTeam[i-erasedRobs].getPosx()][rescueTeam[i-erasedRobs].getPosy()])->setInjured(false);

                            // get out of robot vector
                            rescueTeam.erase(rescueTeam.begin() + i - erasedRobs);
                            erasedRobs++;

                            cout<<"Robot is injured for too long. It has just been erased.";
                        }
                        cout<<endl;
                    }
                    // if not injured
                    else
                    {
                        // update flag
                        allInjured = false;

                        /* ACTION! */
                        // operation
                        if (rescueTeam[i-erasedRobs].getOrder())
                        {
                            int k;

                            // if a RescueRobot is in operation mode, that means at the same coordinates, there is an injured robot, search, find and repair it
                            for (k=0 ; k<Nexp ; k++)
                            {
                                if (exploreTeam[k].getPosx()==rescueTeam[i-erasedRobs].getPosx() && exploreTeam[k].getPosy()==rescueTeam[i-erasedRobs].getPosy())
                                    rescueTeam[i-erasedRobs].Oper(&exploreTeam[k]);
                            }
                            for (k=0 ; k<(Nres-erasedRobs) ; k++)
                            {
                                if (k!=(i-erasedRobs) && rescueTeam[k].getPosx()==rescueTeam[i-erasedRobs].getPosx() && rescueTeam[k].getPosy()==rescueTeam[i-erasedRobs].getPosy())
                                    rescueTeam[i-erasedRobs].Oper(&rescueTeam[k]);
                            }
                            for (k=0 ; k<Nana ; k++)
                            {
                                if (analysisTeam[k].getPosx()==rescueTeam[i-erasedRobs].getPosx() && analysisTeam[k].getPosy()==rescueTeam[i-erasedRobs].getPosy())
                                    rescueTeam[i-erasedRobs].Oper(&analysisTeam[k]);
                            }
                        }
                        // motion
                        else
                        {
                            rescueTeam[i-erasedRobs].Move(N, p);
                        }
                    }
                }

                // update rescue robots number
                Nres = Nres - erasedRobs;

                erasedRobs=0; // clear var


                /* 3 - Analysis Robots Action */
                cout<<endl<<"Analysis robots :"<<endl;

                for (i=0 ; i<Nana ; i++)
                {
                    cout<<"\t"<<(i+1)<<". ";

                    // check condition
                    if(analysisTeam[i-erasedRobs].getCond())
                    {
                        // if injured for less than 10 rounds, just add a round if not repaired, else erase it!
                        if(analysisTeam[i-erasedRobs].getRounds()!=10)
                        {
                            // if injured and rounds==0 means that robot is getting repaired
                            if (analysisTeam[i-erasedRobs].getRounds()!=0)
                            {
                                analysisTeam[i-erasedRobs].PlusRound();
                                cout<<"Robot is injured. (for "<<analysisTeam[i-erasedRobs].getRounds()<<" rounds)";
                            }
                            else
                            {
                                cout<<"Robot is getting repaired";
                                dynamic_cast <MapBox*> (p[analysisTeam[i-erasedRobs].getPosx()][analysisTeam[i-erasedRobs].getPosy()])->setInjured(false);
                            }
                        }
                        // if injured for 10 rounds, erase robot
                        else
                        {
                            // get out of map
                            dynamic_cast <MapBox*> (p[analysisTeam[i-erasedRobs].getPosx()][analysisTeam[i-erasedRobs].getPosy()])->setExist(false);
                            dynamic_cast <MapBox*> (p[analysisTeam[i-erasedRobs].getPosx()][analysisTeam[i-erasedRobs].getPosy()])->setInjured(false);

                            // get out of robot vector
                            analysisTeam.erase(analysisTeam.begin() + i - erasedRobs);
                            erasedRobs++;

                            cout<<"Robot is injured for too long. It has just been erased.";
                        }
                        cout<<endl;
                    }
                    // if not injured
                    else
                    {
                        // update flag
                        allInjured = false;

                        /* ACTION! */
                        // case: Got to Base!!! Unload carry...
                        if(analysisTeam[i-erasedRobs].getPosx()==0 && analysisTeam[i-erasedRobs].getPosy()==0)
                        {
                            // palladium
                            if ((dynamic_cast<MapBase*> (p[0][0]))->getPALL() - analysisTeam[i-erasedRobs].getCarP()>=0)
                                (dynamic_cast<MapBase*> (p[0][0]))->setPALL((dynamic_cast<MapBase*> (p[0][0]))->getPALL() - analysisTeam[i-erasedRobs].getCarP());
                            else
                                (dynamic_cast<MapBase*> (p[0][0]))->setPALL(0);

                            // iridium
                            if ((dynamic_cast<MapBase*> (p[0][0]))->getIRID() - analysisTeam[i-erasedRobs].getCarI()>=0)
                                (dynamic_cast<MapBase*> (p[0][0]))->setIRID((dynamic_cast<MapBase*> (p[0][0]))->getIRID() - analysisTeam[i-erasedRobs].getCarI());
                            else
                                (dynamic_cast<MapBase*> (p[0][0]))->setIRID(0);

                            // platinum
                            if ((dynamic_cast<MapBase*> (p[0][0]))->getPLAT() - analysisTeam[i-erasedRobs].getCarL()>=0)
                                (dynamic_cast<MapBase*> (p[0][0]))->setPLAT((dynamic_cast<MapBase*> (p[0][0]))->getPLAT() - analysisTeam[i-erasedRobs].getCarL());
                            else
                                (dynamic_cast<MapBase*> (p[0][0]))->setPLAT(0);

                            // destroy robot!!!
                            cout<<"ROBOT HAS REACHED BASE SUCCESFULLY!!! Now it will be distroyed."<<endl;
                            analysisTeam.erase(analysisTeam.begin() + i - erasedRobs);
                            erasedRobs++;
                        }
                        // case: operation
                        else if (analysisTeam[i-erasedRobs].getOrder())
                        {
                            analysisTeam[i-erasedRobs].Oper(dynamic_cast <MapBox*> (p[analysisTeam[i-erasedRobs].getPosx()][analysisTeam[i-erasedRobs].getPosy()]) , (dynamic_cast<MapBase*> (p[0][0]))->getPALL() , (dynamic_cast<MapBase*> (p[0][0]))->getIRID(), (dynamic_cast<MapBase*> (p[0][0]))->getPLAT());
                        }
                        // case: motion
                        else
                        {
                            analysisTeam[i-erasedRobs].Move(N, p);
                        }
                    }

                }

                // update analysis robots number
                Nana = Nana - erasedRobs;

                erasedRobs=0; // clear var

                cout<<endl<<endl;

                // call simulation menu
				keepGoingSim = Menu(N, p, &Nexp, &exploreTeam, &Nres, &rescueTeam, &Nana, &analysisTeam);

                // if asked amount is reached, END OF SIMULATION
				if ((dynamic_cast<MapBase*> (p[0][0]))->getPALL()==0 && (dynamic_cast<MapBase*> (p[0][0]))->getIRID()==0 && (dynamic_cast<MapBase*> (p[0][0]))->getPLAT()==0)
                {
                    keepGoingSim = false;
                    cout<<"ALL PALLADIUM, IRIDIUM AND PLATINUM NEEDED HAS BEEN COLLECTED!"<<endl;
                    cout<<"Simulation is terminated succesfully!"<<endl;
                }

                // if all robots are injured, END OF SIMULATION
                if (allInjured)
                {
                    keepGoingSim = false;
                    cout<<"ALL ROBOTS ARE OUT OF ORDER!"<<endl;
                    cout<<"Simulation is terminated unsuccesfully..."<<endl;
                }
                else
                    allInjured = true; // clear var


			} while (keepGoingSim);


			cout<<"Simulation is stopped and closed."<<endl;
			cout<<endl;

			/* END OF SIMULATION */
		}
	}

    /* EXIT */
	if (choiceA==0)
	{
		cout<<"Program is stopped."<<endl<<endl;
	}

	cout<<"Program has been closed. Simulation stopped.";

	cout<<endl;
	return 0;
}

// Start Menu - Run or Exit
int StartMenu(void)
{
	int choice, count=0;

	cout<<"Start Menu :"<<endl;
	cout<<endl<<"\t1. Run simulation";
	cout<<endl<<"\t2. Exit"<<endl;

	cout<<endl<<"Your choice : ";

	do{
		cin>>choice;
		if (choice!=1 && choice!=2 && count!=3)
		{
			count++;
			cout<<"This choice is not available."<<endl;
			cout<<endl<<"Your choice : ";
		}
	} while (choice!=1 && choice!=2 && choice!=3 && count!=3);

	if(count!=3)
	{
		return choice;
	}
	else
	{
		return 0;
	}

	cout<<endl;
}

// Simulation Menu
bool Menu(int N, MapElement ***p, int *E, vector<ExpRob> *exp, int *R, vector<ResRob> *res, int *A, vector<AnaRob> *ana)
{
	int choice, count=0;
    int e, r, a;

    vector<ExpRob> exploreTeam = *exp;
    vector<ResRob> rescueTeam = *res;
    vector<AnaRob> analysisTeam = *ana;

    e = *E;
    r = *R;
    a = *A;

	cout<<"Simulation Menu :"<<endl;
	cout<<endl<<"\t1. Continue...";
	cout<<endl<<"\t2. See informations.";
	cout<<endl<<"\t3. Change values.";
	cout<<endl<<"\t4. Add/Remove robot.";
	cout<<endl<<"\t5. Exit"<<endl;

	cout<<endl<<"Your choice : ";

	do{
		cin>>choice;
		if (choice!=1 && choice!=2 && choice!=3 && choice!=4 && choice!=5 && count!=3)
		{
			count++;
			cout<<"This choice is not available."<<endl;
			cout<<endl<<"Your choice : ";
		}
	} while (choice!=1 && choice!=2 && choice!=3 && choice!=4 && choice!=5 && count!=3);

	if (count==3)
	{
		choice = 5;
	}

	switch (choice){

		case 1 :
		//Simulation will continue
			return true;
			break;

		case 2 :
        //Show Information Menu and Printing Info
		{
				void ShowMap(int N, MapElement ***p);
				void ShowExpRobots(int N, vector<ExpRob> exploreTeam);
				void ShowResRobots(int N, vector<ResRob> rescueTeam);
				void ShowAnaRobots(int N, vector<AnaRob> analysisTeam);
				void ShowAllRobots(int e, vector<ExpRob> exploreTeam, int r, vector<ResRob> rescueTeam, int a, vector<AnaRob> analysisTeam);

				int count1 = 0;
				int choice1 = 0;

				cout<<"Show Menu :"<<endl;
				cout<<endl<<"\t1. Show map base";
				cout<<endl<<"\t2. Show a map box";
				cout<<endl<<"\t3. Show the whole map";
				cout<<endl<<"\t4. Show a robot";
				cout<<endl<<"\t5. Show some robots";
				cout<<endl<<"\t6. Show all robots";
                cout<<endl<<"\t7. Show average access difficulity";
                cout<<endl<<"\t8. Show all the amount of matirials other the world";
                cout<<endl<<"\t9. Show how many flags are on the map";
                cout<<endl<<"\t10. Show the amount of the matirials robots have";
				cout<<endl<<"\t11. Go Back";

				cout<<endl<<"Your choice : ";

				do{
					cin>>choice1;
					if (choice1!=1 && choice1!=2 && choice1!=3 && choice1!=4 && choice1!=5 && choice1!=6 && choice1!=7 && choice1!=8 && choice1!=9 && choice1!=10 && choice1!=11 && count1!=3)
					{
						count1++;
						cout<<"This choice is not available."<<endl;
						cout<<endl<<"Your choice : ";
					}
				} while (choice1!=1 && choice1!=2 && choice1!=3 && choice1!=4 && choice1!=5 && choice1!=6 && choice1!=7 && choice1!=8 && choice1!=9 && choice1!=10 && choice1!=11 && count1!=3);

				if (count1==3)
				{
					choice1 = 11;
				}

				switch (choice1) {
					case 1 :
                    //prints map base
						(dynamic_cast<MapBase*> (p[0][0]))->MapInfo();
						break;

					case 2 :
                    //prints one mapbox
						{
							int x, y;
							cout<<"Give coordinates :"<<endl;
							cout<<"\tx : ";
							cin>>x;
							cout<<"\ty : ";
							cin>>y;

							if (x<0 || y<0 || x>=N || y>=N)
							{
								cout<<"This box doesn't exist."<<endl;
							}
							else
							{
								(dynamic_cast<MapBox*> (p[x][y]))->MapInfo();
							}
							cout<<endl;
						}
						break;

					case 3 :
                    //prints all the map
						ShowMap(N, p);
						break;

					case 4 :
                    //prints a robot
						{
							int cat, num;

							cout<<"In the map right now there are :"<<endl;
							cout<<"\t1. ("<<e<<") Explore Robots"<<endl;
							cout<<"\t2. ("<<r<<") Rescue Robots"<<endl;
							cout<<"\t3. ("<<a<<") Analysis Robots"<<endl;

							cout<<endl<<"Choose category(1, 2 or 3) : ";
							cin>>cat;
							cout<<endl<<"Choose which(1..."<<e<<" , 1..."<<r<<" or 1..."<<a<<" ) : ";
							cin>>num;

							if (cat==1)
							{
								if(num<1 || num>e)
								{
									cout<<"This robot doesn't exist."<<endl;
								}
								else
								{
									exploreTeam[num-1].RobInfo();
								}
							}
							else if (cat==2)
							{
								if(num<1 || num>r)
								{
									cout<<"This robot doesn't exist."<<endl;
								}
								else
								{
									rescueTeam[num-1].RobInfo();
								}
							}
							else if (cat==3)
							{
								if(num<1 || num>a)
								{
									cout<<"This robot doesn't exist."<<endl;
								}
								else
								{
									analysisTeam[num-1].RobInfo();
								}
							}
							else
							{
								cout<<"This robot doesn't exist."<<endl;
							}
						}
						break;

					case 5 :
                    //pirnts some robots(exploration, rescue, analysis and injured)
						{
							int choice1;
							cout<<"You can see :"<<endl;
							cout<<endl<<"\t1. All Explore Robots";
							cout<<endl<<"\t2. All Rescue Robots";
							cout<<endl<<"\t3. All Analysis Robots";
							cout<<endl<<"\t4. All injured Robots"<<endl;

							cout<<endl<<"Your choice : ";
							cin>>choice1;

							if (choice1==1)
							{
								ShowExpRobots(e, exploreTeam);
							}
							else if (choice1==2)
							{
								ShowResRobots(r, rescueTeam);
							}
							else if (choice1==3)
							{
								ShowAnaRobots(a, analysisTeam);
							}
							else if (choice1==4)
							{
								int i;
								for (i=0 ; i<e ; i++)
								{
									if (exploreTeam[i].getCond())
										exploreTeam[i].RobInfo();
								}
								for (i=0 ; i<r ; i++)
								{
									if (rescueTeam[i].getCond())
										rescueTeam[i].RobInfo();
								}
								for (i=0 ; i<a ; i++)
								{
									if (analysisTeam[i].getCond())
										analysisTeam[i].RobInfo();
								}
							}
							else
							{
								cout<<"This choice is not available."<<endl;
							}
						}
						break;

					case 6 :
                    //prints all reobots
						ShowAllRobots(e, exploreTeam, r, rescueTeam, a, analysisTeam);
						break;
                    case 7 :
                    //prints average access difficulity
                        {
                            int ii, jj;
                            double av = 0;

                            for(ii=0 ; ii<N ; ii++)
                         		for(jj=0 ; jj<N ; jj++)
                         		{
			                         if(ii!=0 || jj!=0)
                         			 {
                                        av = av + (dynamic_cast<MapBox*> (p[ii][jj]))->getAccess();
			                         }
		                         }

                            av = av/(N*N - 1); // final calculation of average value

                            cout<<"Average access difficulity: "<<av<<endl;
                        }
                        break;
                    case 8 :
                    //prints the materials all over
                        {
                            int ii, jj;
                            int pAll=0, iAll=0, lAll=0;

                            for(ii=0 ; ii<N ; ii++)
                         		for(jj=0 ; jj<N ; jj++)
                         		{
			                         if(ii!=0 || jj!=0)
                         			 {
                         			     pAll += (dynamic_cast<MapBox*> (p[ii][jj]))->getPall();
                         			     iAll += (dynamic_cast<MapBox*> (p[ii][jj]))->getIrid();
                         			     lAll += (dynamic_cast<MapBox*> (p[ii][jj]))->getPlat();
			                         }
		                         }

                            cout<<endl;
                            cout<<"All palladium around the world: "<<pAll<<endl;
                            cout<<"All iridium around the world: "<<iAll<<endl;
                            cout<<"All platinum around the world: "<<lAll<<endl;
                        }
                        break;
                    case 9 :
                    //show how many flags
                        exploreTeam[0].printAllFlags();
                        break;
                    case 10 :
                    //show materials we 've got
                        analysisTeam[0].printMat();
                        break;
				}

				return Menu(N, p, E, exp, R, res, A, ana);
		}
			break;

		case 3 :
		//Make Changes Menu and Changes
			{
				int count2 = 0;
				int choice2 = 0;

				cout<<"Change Menu :"<<endl;
				cout<<endl<<"\t1. Change map base needs";
				cout<<endl<<"\t2. Change a map box's variables";
				cout<<endl<<"\t3. Change an exploration robot's variables";
				cout<<endl<<"\t4. Change a rescue robot's variables";
				cout<<endl<<"\t5. Change an analysis robot's variables";
				cout<<endl<<"\t6. Go Back";

				cout<<endl<<"Your choice : ";

				do{
					cin>>choice2;
					if (choice2!=1 && choice2!=2 && choice2!=3 && choice2!=4 && choice2!=5 && choice2!=6 && count2!=3)
					{
						count2++;
						cout<<"This choice is not available."<<endl;
						cout<<endl<<"Your choice : ";
					}
				} while (choice2!=1 && choice2!=2 && choice2!=3 && choice2!=4 && choice2!=5 && choice2!=6 && count2!=3);

				if (count2==3)
				{
					choice2 = 6;
				}

				switch (choice2) {

					case 1 :
					//map base
						{
							int baseCh;

							cout<<"You want to change base's"<<endl;
							cout<<endl<<"\t1. palladium necessity?";
							cout<<endl<<"\t2. iridium necessity?";
							cout<<endl<<"\t3. platinum necessity?";

							cout<<endl<<"Your choice : ";
							cin>>baseCh;
							cout<<endl;

							if (baseCh==1)
							{
								int pall;
								cout<<"Current Palladium Necessity : "<<(dynamic_cast <MapBase*> (p[0][0]))->getPALL();
								cout<<endl<<"Enter new necessity : ";
								cin>>pall;

								if (pall>=0 && pall<=100000)
								{
									(dynamic_cast <MapBase*> (p[0][0]))->setPALL(pall);
								}

								cout<<"New Palladium Necessity : "<<(dynamic_cast <MapBase*> (p[0][0]))->getPALL();
								cout<<endl;
							}
							else if (baseCh==2)
							{
								int irid;
								cout<<"Current Iridium Necessity : "<<(dynamic_cast <MapBase*> (p[0][0]))->getIRID();
								cout<<endl<<"Enter new necessity : ";
								cin>>irid;

								if (irid>=0 && irid<=100000)
								{
									(dynamic_cast <MapBase*> (p[0][0]))->setIRID(irid);
								}

								cout<<"New Iridium Necessity : "<<(dynamic_cast <MapBase*> (p[0][0]))->getIRID();
								cout<<endl;
							}
							else if (baseCh==3)
							{
								int plat;
								cout<<"Current Platinum Necessity : "<<(dynamic_cast <MapBase*> (p[0][0]))->getPLAT();
								cout<<endl<<"Enter new necessity : ";
								cin>>plat;

								if (plat>=0 && plat<=100000)
								{
									(dynamic_cast <MapBase*> (p[0][0]))->setPLAT(plat);
								}

								cout<<"New Platinum Necessity : "<<(dynamic_cast <MapBase*> (p[0][0]))->getPLAT();
								cout<<endl;
							}
							else
							{
								cout<<endl<<"There is not such choice. Back to menu."<<endl;
							}
						}
						break;

					case 2 :
					//map box
						{
							int tempx, tempy;

							cout<<"Give coordinates"<<endl;
							cout<<"Give x (0..."<<(N-1)<<") : ";
							cin>>tempx;
							cout<<endl<<"Give y (0..."<<(N-1)<<") : ";
							cin>>tempy;

							if (!(tempx==0 && tempy==0) && tempx>=0 && tempx<N && tempy>=0 && tempy<N)
							{
								int boxCh;

								cout<<"You want to change box's"<<endl;
								cout<<endl<<"\t1. palladium amount?";
								cout<<endl<<"\t2. iridium amount?";
								cout<<endl<<"\t3. platinum amount?";
								cout<<endl<<"\t4. danger access?";

								cout<<endl<<"Your choice : ";
								cin>>boxCh;
								cout<<endl;

								if (boxCh==1)
								{
									int pall;
									cout<<"Current Palladium Amount : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getPall();
									cout<<endl<<"Enter new amount : ";
									cin>>pall;

									if (pall>=0 && pall<=100)
									{
										(dynamic_cast <MapBox*> (p[tempx][tempy]))->setPall(pall);
									}

									cout<<"New Palladium Amount : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getPall();
									cout<<endl;
								}
								else if (boxCh==2)
								{
									int irid;
									cout<<"Current Iridium Amount : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getIrid();
									cout<<endl<<"Enter new amount : ";
									cin>>irid;

									if (irid>=0 && irid<=100)
									{
										(dynamic_cast <MapBox*> (p[tempx][tempy]))->setIrid(irid);
									}

									cout<<"New Iridium Amount : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getIrid();
									cout<<endl;
								}
								else if (boxCh==3)
								{
									int plat;
									cout<<"Current Platinum Amount : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getPlat();
									cout<<endl<<"Enter new amount : ";
									cin>>plat;

									if (plat>=0 && plat<=100)
									{
										(dynamic_cast <MapBox*> (p[tempx][tempy]))->setPlat(plat);
									}

									cout<<"New Platinum Amount : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getPlat();
									cout<<endl;
								}
								else if (boxCh==4)
								{
									int acc;

									cout<<"Current Danger Access : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getAccess();
									cout<<endl<<"Enter new access(0, 1, ... 9) : ";
									cin>>acc;

									if (acc>=0 && acc<=9)
									{
										(dynamic_cast <MapBox*> (p[tempx][tempy]))->setAccess(acc/(float)10);
									}

									cout<<"New Danger Access : "<<(dynamic_cast <MapBox*> (p[tempx][tempy]))->getAccess();
								}
								else
								{
									cout<<endl<<"There is not such choice. Back to menu."<<endl;
								}
							}
							else if ((tempx==0 && tempy==0))
							{
								cout<<"You cannot change base's characteristics in this scope. Go back to menu."<<endl;
							}
							else
							{
								cout<<"This box doesn't exist. Go back to menu."<<endl;
							}
						}
						break;

					case 3 :
					//exploration rob
						{
							int wh=0;

							cout<<"Choose which exploration robot you want to make changes to (1..."<<e<<") : ";
							cin>>wh;
							if(wh>=1 && wh<=e)
							{
								int el;

								cout<<"You want to change robot's"<<endl;
								cout<<endl<<"\t1. speed?";
								cout<<endl<<"\t2. access ability?";
								cout<<endl<<"\t3. condition?";
								cout<<endl<<"\t4. position?";
								cout<<endl<<"\t5. order?";


								cout<<endl<<"Your choice : ";
								cin>>el;
								cout<<endl;

								if (el==1)
								{
									int temp;

									cout<<"Robot's speed : "<<exploreTeam[wh-1].getSpeed()<<endl;
									cout<<"Give new speed (1...5) : ";
									cin>>temp;

									if (temp==1 || temp==2 || temp==3 || temp==4 || temp==5)
									{
										exploreTeam[wh-1].setSpeed(temp);
									}

									cout<<"Robot's new speed : "<<exploreTeam[wh-1].getSpeed()<<endl;
								}
								else if (el==2)
								{
									int temp;

									cout<<"Robot's access ability : "<<exploreTeam[wh-1].getAccAbility()<<endl;
									cout<<"Give new access ability (1, 2, ... 10) : ";
									cin>>temp;

									if (temp>=1 && temp<=10)
									{
										exploreTeam[wh-1].setAccAbility(temp/(float)10);
									}

									cout<<"Robot's new access ability : "<<exploreTeam[wh-1].getAccAbility()<<endl;

								}
								else if (el==3)
								{
									int temp;

									cout<<"Robot's condition : "<<exploreTeam[wh-1].getCond()<<endl;
									cout<<"Give new condition (0 or 1) : ";
									cin>>temp;

									if (temp==1)
									{
										exploreTeam[wh-1].setCond(true);
										(dynamic_cast <MapBox*> (p[exploreTeam[wh-1].getPosx()][exploreTeam[wh-1].getPosy()]))->setInjured(true);
									}
									else if (temp==0)
									{
										exploreTeam[wh-1].setCond(false);
										(dynamic_cast <MapBox*> (p[exploreTeam[wh-1].getPosx()][exploreTeam[wh-1].getPosy()]))->setInjured(false);
									}

									cout<<"Robot's new condition : "<<exploreTeam[wh-1].getCond()<<endl;

								}
								else if (el==4)
								{
									int tempx, tempy;

									cout<<"Robot's position : "<<exploreTeam[wh-1].getPosx()<<", "<<exploreTeam[wh-1].getPosy()<<endl;
									cout<<"\tGive new x (0..."<<(N-1)<<") : ";
									cin>>tempx;
									cout<<endl<<"\tGive new y (0..."<<(N-1)<<") : ";
									cin>>tempy;

									if ((tempx==0 && tempy==0) || (tempx>=0 && tempx<N && tempy>=0 && tempy<N && !(dynamic_cast <MapBox*> (p[tempx][tempy]))->getExist()))
									{
										(dynamic_cast <MapBox*> (p[exploreTeam[wh-1].getPosx()][exploreTeam[wh-1].getPosy()]))->setExist(false);
										(dynamic_cast <MapBox*> (p[exploreTeam[wh-1].getPosx()][exploreTeam[wh-1].getPosy()]))->setInjured(false);

										exploreTeam[wh-1].setPosx(tempx);
										exploreTeam[wh-1].setPosy(tempy);

										(dynamic_cast <MapBox*> (p[tempx][tempy]))->setExist(true);
										if (exploreTeam[wh-1].getCond())
                                            (dynamic_cast <MapBox*> (p[tempx][tempy]))->setInjured(true);
									}

									cout<<endl<<"Robot's new position : "<<exploreTeam[wh-1].getPosx()<<", "<<exploreTeam[wh-1].getPosy()<<endl;

								}
								else if (el==5)
								{
									int temp;

									cout<<"Robot's order : "<<exploreTeam[wh-1].getCond()<<endl;
									cout<<"Give new order (move->0 or operation->1) : ";
									cin>>temp;

									if (temp==1)
									{
										exploreTeam[wh-1].setOrder(true);
									}
									else if (temp==0)
									{
										exploreTeam[wh-1].setOrder(false);
									}

									cout<<"Robot's new order : "<<exploreTeam[wh-1].getOrder()<<endl;

								}
								else
								{
									cout<<endl<<"There is not such choice. Back to menu."<<endl;
								}

							}
							else
							{
								cout<<"This robot doesn't exist."<<endl;
							}

							// save changes
							*exp = exploreTeam;
						}
						break;

					case 4 :
					//rescue rob
						{
							int wh=0;

							cout<<"Choose which rescue robot you want to make changes to (1..."<<r<<") : ";
							cin>>wh;
							if(wh>=1 && wh<=r)
							{
								int el;

								cout<<"You want to change robot's"<<endl;
								cout<<endl<<"\t1. speed?";
								cout<<endl<<"\t2. access ability?";
								cout<<endl<<"\t3. condition?";
								cout<<endl<<"\t4. position?";
								cout<<endl<<"\t5. order?";


								cout<<endl<<"Your choice : ";
								cin>>el;
								cout<<endl;

								if (el==1)
								{
									int temp;

									cout<<"Robot's speed : "<<rescueTeam[wh-1].getSpeed()<<endl;
									cout<<"Give new speed (1...5) : ";
									cin>>temp;

									if (temp==1 || temp==2 || temp==3 || temp==4 || temp==5)
									{
										rescueTeam[wh-1].setSpeed(temp);
									}

									cout<<"Robot's new speed : "<<rescueTeam[wh-1].getSpeed()<<endl;
								}
								else if (el==2)
								{
									int temp;

									cout<<"Robot's access ability : "<<rescueTeam[wh-1].getAccAbility()<<endl;
									cout<<"Give new access ability (1, 2, ... 10) : ";
									cin>>temp;

									if (temp>=1 && temp<=10)
									{
										rescueTeam[wh-1].setAccAbility(temp/(float)10);
									}

									cout<<"Robot's new access ability : "<<rescueTeam[wh-1].getAccAbility()<<endl;

								}
								else if (el==3)
								{
									int temp;

									cout<<"Robot's condition : "<<rescueTeam[wh-1].getCond()<<endl;
									cout<<"Give new condition (0 or 1) : ";
									cin>>temp;

									if (temp==1)
									{
										rescueTeam[wh-1].setCond(true);
										(dynamic_cast <MapBox*> (p[rescueTeam[wh-1].getPosx()][rescueTeam[wh-1].getPosy()]))->setInjured(true);
									}
									else if (temp==0)
									{
										rescueTeam[wh-1].setCond(false);
										(dynamic_cast <MapBox*> (p[rescueTeam[wh-1].getPosx()][rescueTeam[wh-1].getPosy()]))->setInjured(false);
									}

									cout<<"Robot's new condition : "<<rescueTeam[wh-1].getCond()<<endl;

								}
								else if (el==4)
								{
									int tempx, tempy;

									cout<<"Robot's position : "<<rescueTeam[wh-1].getPosx()<<", "<<rescueTeam[wh-1].getPosy()<<endl;
									cout<<"\tGive new x (0..."<<(N-1)<<") : ";
									cin>>tempx;
									cout<<endl<<"\tGive new y (0..."<<(N-1)<<") : ";
									cin>>tempy;

									if ((tempx==0 && tempy==0) || (tempx>=0 && tempx<N && tempy>=0 && tempy<N && !(dynamic_cast <MapBox*> (p[tempx][tempy]))->getExist()))
									{
										(dynamic_cast <MapBox*> (p[rescueTeam[wh-1].getPosx()][rescueTeam[wh-1].getPosy()]))->setExist(false);
										(dynamic_cast <MapBox*> (p[rescueTeam[wh-1].getPosx()][rescueTeam[wh-1].getPosy()]))->setInjured(false);

										rescueTeam[wh-1].setPosx(tempx);
										rescueTeam[wh-1].setPosy(tempy);

										(dynamic_cast <MapBox*> (p[tempx][tempy]))->setExist(true);
										if (rescueTeam[wh-1].getCond())
                                            (dynamic_cast <MapBox*> (p[tempx][tempy]))->setInjured(true);
									}
									cout<<endl<<"Robot's new position : "<<rescueTeam[wh-1].getPosx()<<", "<<rescueTeam[wh-1].getPosy()<<endl;

								}
								else if (el==5)
								{
									int temp;

									cout<<"Robot's order : "<<rescueTeam[wh-1].getCond()<<endl;
									cout<<"Give new order (move->0 or operation->1) : ";
									cin>>temp;

									if (temp==1)
									{
										rescueTeam[wh-1].setOrder(true);
									}
									else if (temp==0)
									{
										rescueTeam[wh-1].setOrder(false);
									}

									cout<<"Robot's new order : "<<rescueTeam[wh-1].getOrder()<<endl;

								}
								else
								{
									cout<<endl<<"There is not such choice. Back to menu."<<endl;
								}

							}
							else
							{
								cout<<"This robot doesn't exist."<<endl;
							}

							// save changes
							*res = rescueTeam;
						}
						break;

					case 5 :
					//analysis rob
						{
							int wh=0;

							cout<<"Choose whitch analysis robot you want to make changes to (1..."<<a<<") : ";
							cin>>wh;
							if(wh>=1 && wh<=a)
							{
								int el;

								cout<<"You want to change robot's"<<endl;
								cout<<endl<<"\t1. speed?";
								cout<<endl<<"\t2. access ability?";
								cout<<endl<<"\t3. condition?";
								cout<<endl<<"\t4. position?";
								cout<<endl<<"\t5. order?";
								cout<<endl<<"\t6. maximum carry?";
								cout<<endl<<"\t7. current carry of palladium?";
								cout<<endl<<"\t8. current carry of iridium?";
								cout<<endl<<"\t9. current carry of platinum?";

								cout<<endl<<"Your choice : ";
								cin>>el;
								cout<<endl;

								if (el==1)
								{
									int temp;

									cout<<"Robot's speed : "<<analysisTeam[wh-1].getSpeed()<<endl;
									cout<<"Give new speed (1...5) : ";
									cin>>temp;

									if (temp==1 || temp==2 || temp==3 || temp==4 || temp==5)
									{
										analysisTeam[wh-1].setSpeed(temp);
									}

									cout<<"Robot's new speed : "<<analysisTeam[wh-1].getSpeed()<<endl;
								}
								else if (el==2)
								{
									int temp;

									cout<<"Robot's access ability : "<<analysisTeam[wh-1].getAccAbility()<<endl;
									cout<<"Give new access ability (1, 2, ... 10) : ";
									cin>>temp;

									if (temp>=1 && temp<=10)
									{
										analysisTeam[wh-1].setAccAbility(temp/(float)10);
									}

									cout<<"Robot's new access ability : "<<analysisTeam[wh-1].getAccAbility()<<endl;

								}
								else if (el==3)
								{
									int temp;

									cout<<"Robot's condition : "<<analysisTeam[wh-1].getCond()<<endl;
									cout<<"Give new condition (0 or 1) : ";
									cin>>temp;

									if (temp==1)
									{
										analysisTeam[wh-1].setCond(true);
										(dynamic_cast <MapBox*> (p[analysisTeam[wh-1].getPosx()][analysisTeam[wh-1].getPosy()]))->setInjured(true);
									}
									else if (temp==0)
									{
										analysisTeam[wh-1].setCond(false);
										(dynamic_cast <MapBox*> (p[analysisTeam[wh-1].getPosx()][analysisTeam[wh-1].getPosy()]))->setInjured(false);
									}

									cout<<"Robot's new condition : "<<analysisTeam[wh-1].getCond()<<endl;

								}
								else if (el==4)
								{
									int tempx, tempy;

									cout<<"Robot's position : "<<analysisTeam[wh-1].getPosx()<<", "<<analysisTeam[wh-1].getPosy()<<endl;
									cout<<"\tGive new x (0..."<<(N-1)<<") : ";
									cin>>tempx;
									cout<<endl<<"\tGive new y (0..."<<(N-1)<<") : ";
									cin>>tempy;

									if ((tempx==0 && tempy==0) || (tempx>=0 && tempx<N && tempy>=0 && tempy<N && !(dynamic_cast <MapBox*> (p[tempx][tempy]))->getExist()))
									{
										(dynamic_cast <MapBox*> (p[analysisTeam[wh-1].getPosx()][analysisTeam[wh-1].getPosy()]))->setExist(false);
										(dynamic_cast <MapBox*> (p[analysisTeam[wh-1].getPosx()][analysisTeam[wh-1].getPosy()]))->setInjured(false);

										analysisTeam[wh-1].setPosx(tempx);
										analysisTeam[wh-1].setPosy(tempy);

										(dynamic_cast <MapBox*> (p[tempx][tempy]))->setExist(true);
										if (analysisTeam[wh-1].getCond())
                                            (dynamic_cast <MapBox*> (p[tempx][tempy]))->setInjured(true);
									}

									cout<<endl<<"Robot's new position : "<<analysisTeam[wh-1].getPosx()<<", "<<analysisTeam[wh-1].getPosy()<<endl;

								}
								else if (el==5)
								{
									int temp;

									cout<<"Robot's order : "<<analysisTeam[wh-1].getCond()<<endl;
									cout<<"Give new order (move->0 or operation->1) : ";
									cin>>temp;

									if (temp==1)
									{
										analysisTeam[wh-1].setOrder(true);
									}
									else if (temp==0)
									{
										analysisTeam[wh-1].setOrder(false);
									}

									cout<<"Robot's new order : "<<analysisTeam[wh-1].getOrder()<<endl;

								}
								else if (el==6)
								{
									int temp;

									cout<<"Robot's maximum carry : "<<analysisTeam[wh-1].getMaxCar()<<endl;
									cout<<"Give new maximum carry (25...100) : ";
									cin>>temp;

									if (temp>=25 && temp<=100)
									{
										analysisTeam[wh-1].setMaxCar(temp);
									}

									cout<<"Robot's new maximum carry : "<<analysisTeam[wh-1].getMaxCar()<<endl;
								}
								else if (el==7)
								{
									int temp;

									cout<<"Robot's current carry of palladium : "<<analysisTeam[wh-1].getCarP()<<endl;
									cout<<"Give new current carry of palladium (0..."<<(analysisTeam[wh-1].getMaxCar() - analysisTeam[wh-1].getCarI() - analysisTeam[wh-1].getCarL())<<") : ";
									cin>>temp;

									if (temp>=0 && temp<=(analysisTeam[wh-1].getMaxCar() - analysisTeam[wh-1].getCarI() - analysisTeam[wh-1].getCarL()))
									{
										analysisTeam[wh-1].setCarP(temp);
									}

									cout<<"Robot's new current carry of palladium : "<<analysisTeam[wh-1].getCarP()<<endl;
								}
								else if (el==8)
								{
									int temp;

									cout<<"Robot's current carry of iridium : "<<analysisTeam[wh-1].getCarI()<<endl;
									cout<<"Give new current carry of iridium (0..."<<(analysisTeam[wh-1].getMaxCar() - analysisTeam[wh-1].getCarP() - analysisTeam[wh-1].getCarL())<<") : ";
									cin>>temp;

									if (temp>=0 && temp<=(analysisTeam[wh-1].getMaxCar() - analysisTeam[wh-1].getCarP() - analysisTeam[wh-1].getCarL()))
									{
										analysisTeam[wh-1].setCarI(temp);
									}

									cout<<"Robot's new current carry of iridium : "<<analysisTeam[wh-1].getCarI()<<endl;
								}
								else if (el==9)
								{
									int temp;

									cout<<"Robot's current carry of platinum : "<<analysisTeam[wh-1].getCarL()<<endl;
									cout<<"Give new current carry of platinum (0..."<<(analysisTeam[wh-1].getMaxCar() - analysisTeam[wh-1].getCarP() - analysisTeam[wh-1].getCarI())<<") : ";
									cin>>temp;

									if (temp>=0 && temp<=(analysisTeam[wh-1].getMaxCar() - analysisTeam[wh-1].getCarP() - analysisTeam[wh-1].getCarI()))
									{
										analysisTeam[wh-1].setCarL(temp);
									}

									cout<<"Robot's new current carry of platinum : "<<analysisTeam[wh-1].getCarL()<<endl;
								}
								else
								{
									cout<<endl<<"There is not such choice. Back to menu."<<endl;
								}
							}
							else
							{
								cout<<"This robot doesn't exist."<<endl;
							}

							// save changes
							*ana = analysisTeam;
						}
						break;
				}

				cout<<endl;

				return Menu(N, p, E, exp, R, res, A, ana);
		    }
			break;

		case 4 :
		//Add/Remove Robot Menu and Add/Remove
			{
				int count1 = 0, count2 = 0;
				int choice1 = 0;
				int add = 0;

				cout<<"Type of robot you want to add/remove :"<<endl;
				cout<<endl<<"\t1. Exploration Robot";
				cout<<endl<<"\t2. Rescue Robot";
				cout<<endl<<"\t3. Analysis Robot";
				cout<<endl<<"\t4. Go Back"<<endl;

				cout<<endl<<"Your choice : ";

				do{
					cin>>choice1;
					if (choice1!=1 && choice1!=2 && choice1!=3 && choice1!=4 && count1!=3)
					{
						count1++;
						cout<<"This choice is not available."<<endl;
						cout<<endl<<"Your choice : ";
					}
				} while (choice1!=1 && choice1!=2 && choice1!=3 && choice1!=4 && count1!=3);

				if (count1==3)
				{
					choice1 = 4;
				}
				else if (choice1!=4)
				{
					cout<<"Do you want to :";
					cout<<endl<<"\t1. add a Robot?";
					cout<<endl<<"\t2. remove a Robot?"<<endl;

                    cout<<endl<<"Your choice : ";

					do{
						cin>>add;
						if (add!=1 && add!=2 && count2!=3)
						{
							count2++;
							cout<<"This choice is not available."<<endl;
							cout<<endl<<"Your choice : ";
						}
					} while (add!=1 && add!=2 && count2!=3);
				}

				if (add!=1 && add!=2)
				{
					choice1 = 4;
				}

				switch (choice1){
					case 1 :
                    //add/remove explore robot
						{
							if (add==1)
							{
								exploreTeam.push_back(ExpRob(N, p));
								*E = *E + 1;
								e = *E;
								cout<<"An exploration robot has been created. If you want to change its values, choose to change at the initial menu."<<endl;
							}
							else
							{
								int rem=0;

								cout<<"Choose whitch robot you want to remove (1..."<<e<<") : ";
								cin>>rem;
								if(rem>=1 && rem<=e)
								{
									exploreTeam.erase(exploreTeam.begin() + rem - 1);
                                    *E = *E - 1;
                                    e = *E;
									cout<<"Explore robot has been removed."<<endl;
								}
								else
								{
									cout<<"This robot doesn't exist."<<endl;
								}
							}

							// save changes
							*exp = exploreTeam;
						}
						break;

					case 2 :
                    //add/remove rescue robot
						{
							if (add==1)
							{
								rescueTeam.push_back(ResRob(N, p));
								*R = *R + 1;
								r = *R;
								cout<<"A rescue robot has been created. If you want to change its values, choose to change at the initial menu."<<endl;
							}
							else
							{
								int rem=0;

								cout<<"Choose which robot you want to remove (1..."<<r<<") : ";
								cin>>rem;
								if(rem>=1 && rem<=r)
								{
									rescueTeam.erase(rescueTeam.begin() + rem - 1);
									*R = *R - 1;
									r = *R;
									cout<<"Rescue robot has been removed."<<endl;
								}
								else
								{
									cout<<"This robot doesn't exist."<<endl;
								}
							}

							//pernane oi allages
                            *res = rescueTeam;
						}
						break;

					case 3 :
                    //add/remove analysis robot
						{
							if (add==1)
							{
								analysisTeam.push_back(AnaRob(N, p));
								*A = *A + 1;
								a = *A;
								cout<<"An analysis robot has been created. If you want to change its values, choose to change at the initial menu."<<endl;
							}
							else
							{
								int rem=0;

								cout<<"Choose which robot you want to remove (1..."<<a<<") : ";
								cin>>rem;
								if(rem>=1 && rem<=a)
								{
									analysisTeam.erase(analysisTeam.begin() + rem - 1);
									*A = *A - 1;
									a = *A;
									cout<<"Analysis robot has been removed."<<endl;
								}
								else
								{
									cout<<"This robot doesn't exist."<<endl;
								}
							}

							// save changes
							*ana = analysisTeam;
						}
						break;
				}

				cout<<endl;

				return Menu(N, p, E, exp, R, res, A, ana);
			}
			break;

		case 5 :
		//Closes program
			return false;
			break;
	}
}

// print map info
void ShowMap(int N, MapElement ***p)
{
	int i, j;

	cout<<"Base : "<<endl;
	(dynamic_cast<MapBase*> (p[0][0]))->MapInfo();

	for(i=0 ; i<N ; i++)
		for(j=0 ; j<N ; j++)
		{
			if(i!=0 || j!=0)
			{
			    cout<<"Box["<<i<<"]["<<j<<"] : ";
				(dynamic_cast<MapBox*> (p[i][j]))->MapInfo();
			}
		}
}

// print all exploration robots info
void ShowExpRobots(int N, vector<ExpRob> exploreTeam)
{
	int i;

    cout<<"All exploration robots:"<<endl;
	for (i=0 ; i<N ; i++)
	{
	    cout<<i<<". ";
		exploreTeam[i].RobInfo();
	}
}

// print all rescue robots info
void ShowResRobots(int N, vector<ResRob> rescueTeam)
{
	int i;

    cout<<"All rescue robots:"<<endl;
	for (i=0 ; i<N ; i++)
	{
	    cout<<i<<". ";
		rescueTeam[i].RobInfo();
	}
}

// print all analysis robots info
void ShowAnaRobots(int N, vector<AnaRob> analysisTeam)
{
	int i;

    cout<<"All analysis robots:"<<endl;
	for (i=0 ; i<N ; i++)
	{
	    cout<<i<<". ";
		analysisTeam[i].RobInfo();
	}
}

// print all robots info
void ShowAllRobots(int e, vector<ExpRob> exploreTeam, int r, vector<ResRob> rescueTeam, int a, vector<AnaRob> analysisTeam)
{
	ShowExpRobots(e, exploreTeam);
	ShowResRobots(r, rescueTeam);
	ShowAnaRobots(a, analysisTeam);
}
