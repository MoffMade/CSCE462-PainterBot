#include "forwardKinematics.h"

double degree2rad(double degrees){
	return (degrees*PI)/180;
};
double rad2degree(double rads){
	return (rads*180)/PI;
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
};
void T_Matrix::calcMatrix(DH_Param dh){
	matrix[0][0]=cos(dh.theta);
	matrix[0][1]=-sin(dh.theta);
	matrix[0][2]=0;
	matrix[0][3]=dh.a;

	matrix[1][0]=sin(dh.theta)*cos(dh.alpha);
	matrix[1][1]=cos(dh.theta)*cos(dh.alpha);
	matrix[1][2]=-sin(dh.alpha);
	matrix[1][3]=-sin(dh.alpha)*dh.d;

	matrix[2][0]=sin(dh.theta)*sin(dh.alpha);
	matrix[2][1]=cos(dh.theta)*sin(dh.alpha);
	matrix[2][2]=cos(dh.alpha);
	matrix[2][3]=cos(dh.alpha)*dh.d;

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
	double temp=0;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			temp=0;
			for(int k=0; k<4; k++){
				temp+=(A.get(i,k)*B.get(k,j));
			};
			matrix[i][j]=temp;
		};
	};
	origin_frame=A.getOrg();
	destination_frame=B.getDes();
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
f_kin_solver_3dof::f_kin_solver_3dof(DH_Param l1, DH_Param l2, DH_Param l3){
	DH_Parameters[0]=l1;	
	DH_Parameters[1]=l2;
	DH_Parameters[2]=l3;
	T01 = *(new T_Matrix(0,1,l1));
	T12 = *(new T_Matrix(1,2,l2));
	T23 = *(new T_Matrix(2,3,l3));

	T_Matrix T02;
	T02.T_Mat_multiply(T01,T12);
	FK_Transform.T_Mat_multiply(T02,T23);

};
void f_kin_solver_3dof::printSolver(){
	DH_Parameters[0].printDH();
	DH_Parameters[1].printDH();
	DH_Parameters[2].printDH();
	cout<<endl;
	T01.printMatrix();
	T12.printMatrix();
	T23.printMatrix();
	FK_Transform.printMatrix();
};

void f_kin_solver_3dof::updateTMatrices(){
	T01.calcMatrix(DH_Parameters[0]);
	T12.calcMatrix(DH_Parameters[1]);
	T23.calcMatrix(DH_Parameters[2]);

	T_Matrix T02;
	T02.T_Mat_multiply(T01,T12);
	FK_Transform.T_Mat_multiply(T02,T23);
};
int f_kin_solver_3dof::updateTheta(int link, double newTheta){
	if(link<=0||link>=4){
		//force link to 1, 2, or 3
		return 1;
	}
	else{
		DH_Parameters[link-1].theta=degree2rad(newTheta);
		updateTMatrices();
		return 0;
	}
};
int f_kin_solver_3dof::updateBaseSlider(double newSliderPos){
	if(newSliderPos<0||newSliderPos>300){
		//force link to 1, 2, or 3
		return 1;
	}
	else{
		DH_Parameters[0].a=newSliderPos;
		updateTMatrices();
		return 0;
	}
};