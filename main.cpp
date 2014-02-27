#include "forwardKinematics.h"

int main(){

//----------------------------------------------------------------START INITIALIZE SOLVER-------------

// USE THIS BLOCK TO INIT THE SYSTEM FOR USE IN GUI
	//Initializing the DH Parameters for each link
	// (alpha, a, d, theta)
	DH_Param L0(0,0,0,0);		// d is variable (prismatic)
	DH_Param L1(90,150,0,0);	// non-variable transform
	DH_Param L2(0,100,0,0);		// theta is variable (revolute)
	DH_Param L3(0,75,0,0);		// theta is variable (revolute)

	//Creating solver object and initializing positions
	f_kin_solver solver(L0,L1,L2,L3);
//------------------------------------------------------------------END INITIALIZE SOLVER-------------

//-------------------------------------------------------------------START TESTING SOLVER-------------

//USE updateBaseSlider(double) AND updateTheta(double, int) FUNCTIONS FOR JOINT CONTROL INTERFACE

	solver.printLinkPoints();
	//Testing movement of arm positions
	cout<<"\nChanging Base Position and Recalculating:: \n";
	solver.updateBaseSlider(75);
	solver.printLinkPoints();
	
	cout<<"\nChanging Theta 2 and Recalculating:: \n";
	solver.updateTheta(60, 2);
	solver.printLinkPoints();
	
	cout<<"\nReseting Base Position and Recalculating:: \n";
	solver.updateBaseSlider(0);
	solver.printLinkPoints();

	cout<<"\nReseting Theta 2 and Recalculating:: \n";
	solver.updateTheta(0, 2);
	solver.printLinkPoints();

	cout<<"\nChanging Theta 3 and Recalculating:: \n";
	solver.updateTheta(80, 3);
	solver.printLinkPoints();

//---------------------------------------------------------------------END TESTING SOLVER-------------
	char hs=' ';
	cout<<"\nPress any key + Enter to Exit:: ";
	cin>>hs;
	return 0;
};