#include "forwardKinematics.h"

int main(){
	cout<<"Building the DH Parameters:: "<<endl;
	DH_Param L1(0,0,0,90);
	DH_Param L2(0,150,0,0);
	DH_Param L3(0,100,0,0);

	f_kin_solver_3dof solver(L1,L2,L3);
	solver.printSolver();

	cout<<"Changing Base Position and Recalculating:: "<<endl;
	solver.updateBaseSlider(20);
	solver.printSolver();

	cout<<"Changing Theta2 and Recalculating:: "<<endl;
	solver.updateTheta(2, 20);
	solver.printSolver();
	char hs=' ';
	cout<<"Press any key + Enter to Exit:: ";
	cin>>hs;
	return 0;
};