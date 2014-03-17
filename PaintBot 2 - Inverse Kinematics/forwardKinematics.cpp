#include "forwardKinematics.h"

armLink::armLink(){
	start_position.push_back(0);
	start_position.push_back(0);
	start_position.push_back(0);
	start_position.push_back(1);
	end_position.push_back(0);
	end_position.push_back(0);
	end_position.push_back(0);
	end_position.push_back(1);
};
armLink::armLink(vector<double> start, vector<double> end){
	start_position=start;
	end_position=end;
};
T_Matrix::T_Matrix(){
	origin_frame=-1;
	destination_frame=-1;
	for(int i=0;i<4;i++)
		for(int j=0; j<4; j++)
			matrix[i][j]=-1;
};
T_Matrix::T_Matrix(int org, int des, DH_Param dh){
	origin_frame=org;
	destination_frame=des;
	//Init basic T-Matrix using DH Parameter values (alpha, a, d, theta)
	//Angles are in RADIANS
	calcMatrix(dh);
}
void T_Matrix::calcMatrix(DH_Param dh){
	//Calculates a transformation matrix based upon the provided DH parameters (alpha, a, d, theta)
	//Using the formula based upon the lectures and online sources. Generates a 4x4 matrix that 
	//describes the rotation of the frame as well as the translation from origin_frame to destination_frame
	matrix[0][0]=(cos(dh.theta));
	matrix[0][1]=(-sin(dh.theta)*cos(dh.alpha));
	matrix[0][2]=(sin(dh.theta)*sin(dh.alpha));
	matrix[0][3]=(dh.a*cos(dh.theta));

	matrix[1][0]=(sin(dh.theta));
	matrix[1][1]=(cos(dh.theta)*cos(dh.alpha));
	matrix[1][2]=(-cos(dh.theta)*sin(dh.alpha));
	matrix[1][3]=(dh.a*sin(dh.theta));

	matrix[2][0]=0;
	matrix[2][1]=(sin(dh.alpha));
	matrix[2][2]=(cos(dh.alpha));
	matrix[2][3]=(dh.d);

	matrix[3][0]=0;
	matrix[3][1]=0;
	matrix[3][2]=0;
	matrix[3][3]=1;
};
double T_Matrix::get(int r, int c) const{
	return matrix[r][c];
};
int T_Matrix::getOrg() const{
	return origin_frame;
};
int T_Matrix::getDes() const{
	return destination_frame;
};
int T_Matrix::set(double val, int r, int c){
	if(r>=0 && r<4 && c>=0 && c<4){
		matrix[r][c]=val;
		return 0;
	}
	else
		return 1;
};
void T_Matrix::T_Mat_multiply(const T_Matrix A, const T_Matrix B){
	//Performs 4x4 * 4x4 matrix multiplication A*B
	double temp=0;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			temp=0;
			for(int k=0; k<4; k++){
				temp+=(A.get(i,k)*B.get(k,j));
			};
			matrix[i][j]=(temp);
		};
	};
	origin_frame=A.getOrg();
	destination_frame=B.getDes();
};
vector<double> T_Matrix::pointTransform(vector<double> org_pos){
	//Function takes a column matrix vector of 4 values org_pos = [X, Y, Z, 1] and determines result=T*org_pos using matrix multiplication
	//Performs 4x4 * 4x1 matrix multiplication T*P
	vector<double> result;
	double temp=0;
	for(int i=0; i<4; i++){
		temp=0;
		for(int k=0; k<4; k++){
			temp+=matrix[i][k]*org_pos[k];
		}
		result.push_back((temp));
	}
	return result;
};
void T_Matrix::printMatrix(){
	cout<<"Org: "<<origin_frame<<" Des: "<<destination_frame<<endl;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
};
f_kin_solver::f_kin_solver(DH_Param l0, DH_Param l1, DH_Param l2, DH_Param l3){
	DH_Parameters[0]=l0;	
	DH_Parameters[1]=l1;
	DH_Parameters[2]=l2;
	DH_Parameters[3]=l3;
	T01.setOrg(0);
	T01.setDes(1);
	T12.setOrg(1);
	T12.setDes(2);
	T23.setOrg(2);
	T23.setDes(3);
	T34.setOrg(3);
	T34.setDes(4);
	T02.setOrg(0);
	T02.setDes(2);
	T03.setOrg(0);
	T03.setDes(3);
	T04.setOrg(0);
	T04.setDes(4);
	initLinkPoints();
	updateTMatrices();	
	updateArmLinks();
};

void f_kin_solver::initLinkPoints(){
	vector<double> org(3,0);
	org.push_back(1);
	vector<double> end1(3,0);
	end1.push_back(1);
	vector<double> end2(3,0);
	end2.push_back(1);
	vector<double> end3(3,0);
	end3.push_back(1);
	end1[0]=150;
	end2[0]=100;
	end3[0]=75;
	Link0=*(new armLink(org, org));
	Link1=*(new armLink(org, end1));
	Link2=*(new armLink(org, end2));
	Link3=*(new armLink(org, end3));
};
void f_kin_solver::printSolver(){
	DH_Parameters[0].printDH();
	DH_Parameters[1].printDH();
	DH_Parameters[2].printDH();
	DH_Parameters[3].printDH();
	cout<<endl;
	T01.printMatrix();
	T02.printMatrix();
	T03.printMatrix();
	T04.printMatrix();
};

void f_kin_solver::updateTMatrices(){
	//Recalculates each matrix for the solver using the DH parameters, then matrix multiplication.
	T01.calcMatrix(DH_Parameters[0]);
	T12.calcMatrix(DH_Parameters[1]);
	T23.calcMatrix(DH_Parameters[2]);
	T34.calcMatrix(DH_Parameters[3]);
	T02.T_Mat_multiply(T01,T12);
	T03.T_Mat_multiply(T02,T23);
	T04.T_Mat_multiply(T03,T34);
};
vector<vector<double> > f_kin_solver::updateArmLinks(){
	vector<vector<double> > linkPositions;
	vector<double> org(3,0);
	org.push_back(1);
	linkPositions.push_back(T01.pointTransform(org));
	linkPositions.push_back(T02.pointTransform(org));
	linkPositions.push_back(T03.pointTransform(org));
	linkPositions.push_back(T04.pointTransform(org));
	Link0.setStart(linkPositions[0]);
	Link1.setStart(linkPositions[1]);
	Link2.setStart(linkPositions[2]);
	Link3.setStart(linkPositions[3]);
	return linkPositions;
};
vector<vector<double> > f_kin_solver::updateTheta(double newTheta, int sub){
	vector<vector<double> > linkPositions, XYpositions;
	if(sub==2||sub==3){
		DH_Parameters[sub].theta=degree2rad(newTheta);
		updateTMatrices();
		linkPositions=updateArmLinks();		//Get (X,Y,Z) positions for link points.
		vector<double> temp;
		//Change the points from (X,Y,Z) to (Z, X) points for use in the GUI XY plane
		for(int i=0; i<linkPositions.size(); i++){
			temp.clear();
			temp.push_back(linkPositions[i][2]);	//Z value of position to first parameter
			temp.push_back(linkPositions[i][0]);	//X value of position to second parameter
			XYpositions.push_back(temp);			//Store (Z,X) position to XYpositions to return to the GUI
		}
	}
	else{
		cout<<"Invalid link number, must be Link 2 or Link 3; no change in position."<<endl;
		linkPositions.push_back(Link0.getStart());
		linkPositions.push_back(Link1.getStart());
		linkPositions.push_back(Link2.getStart());
		linkPositions.push_back(Link3.getStart());
		XYpositions=linkPositions;
	}
	return XYpositions;
};
vector<vector<double> >  f_kin_solver::updateBaseSlider(double newSliderPos){
	vector<vector<double> > linkPositions, XYpositions;
	if(newSliderPos>=0&&newSliderPos<=300){
		DH_Parameters[0].d=newSliderPos;
		updateTMatrices();
		linkPositions=updateArmLinks();		//Get (X,Y,Z) positions for link points.
		vector<double> temp;
		//Change the points from (X,Y,Z) to (Z, X) points for use in the GUI XY plane
		for(int i=0; i<linkPositions.size(); i++){
			temp.clear();
			temp.push_back(linkPositions[i][2]);	//Z value of position to first parameter
			temp.push_back(linkPositions[i][0]);	//X value of position to second parameter
			XYpositions.push_back(temp);			//Store (Z,X) position to XYpositions to return to the GUI
		}
	}
	else{
		cout<<"Invalid base position, must be between 0 and 300 inclusive; no change in position."<<endl;
		linkPositions.push_back(Link0.getStart());
		linkPositions.push_back(Link1.getStart());
		linkPositions.push_back(Link2.getStart());
		linkPositions.push_back(Link3.getStart());
		XYpositions=linkPositions;
	}
	return XYpositions;
};

void f_kin_solver::printLinkPoints(){
	cout<<"Link Points"<<endl;
	cout<<"Point 0: "<<Link0.getStart()[0]<<", "<<Link0.getStart()[1]<<", "<<Link0.getStart()[2]<<endl;
	cout<<"Point 1: "<<Link1.getStart()[0]<<", "<<Link1.getStart()[1]<<", "<<Link1.getStart()[2]<<endl;
	cout<<"Point 2: "<<Link2.getStart()[0]<<", "<<Link2.getStart()[1]<<", "<<Link2.getStart()[2]<<endl;
	cout<<"Point 3: "<<Link3.getStart()[0]<<", "<<Link3.getStart()[1]<<", "<<Link3.getStart()[2]<<endl;
}

double degree2rad(double degrees){
	return (degrees*PI)/180;
};
double rad2degree(double rads){
	return (rads*180)/PI;
};
