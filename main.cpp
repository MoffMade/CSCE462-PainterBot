#include "forwardKinematics.h"

int main(){
	cout<<"Building the DH Parameters:: "<<endl;
	DH_Param L1(0,40,0,70);
	DH_Param L2(0,150,0,50);
	DH_Param L3(0,100,0,130);

	f_kin_solver_3dof solver(L1,L2,L3);
	solver.printSolver();

	char hs=' ';
	cout<<"Press any key + Enter to Exit:: ";
	cin>>hs;
	return 0;
};