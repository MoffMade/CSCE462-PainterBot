#include <cmath>
#include <vector>
#include <iostream>
#include <stdlib.h>
#define PI 3.141592653589793238463

using namespace std;

double degree2rad(double degrees);
double rad2degree(double rads);

class DH_Param {
	//Angles are in RAD
	// RAD = (DEGREE*PI)/180
	// DEGREE = (RAD*180)/PI
public:
	double alpha;
	double a;
	double d;
	double theta;
	DH_Param(): alpha(0), a(0), d(0), theta(0){};
	DH_Param(double al, double ai, double di, double th): alpha(al), a(ai), d(di), theta(degree2rad(th)) {};
	void printDH(){
		cout<<"("<<rad2degree(alpha)<<", "<<a<<", "<<d<<", "<<rad2degree(theta)<<") "<<endl;
	};
};

class T_Matrix {
private:
	int origin_frame, destination_frame;
	double matrix[4][4];
public:
	T_Matrix();
	T_Matrix(int org, int des, DH_Param dh);
	void calcMatrix(DH_Param dh);
	double get(int r, int c) const;
	int getOrg() const;
	int getDes() const;
	int set(double val, int r, int c);
	void T_Mat_multiply(const T_Matrix A, const T_Matrix B);
	void printMatrix();
};

class f_kin_solver_3dof {
private:
	DH_Param DH_Parameters[3];
	T_Matrix FK_Transform;
	T_Matrix T01, T12, T23;
public:
	f_kin_solver_3dof();
	f_kin_solver_3dof(DH_Param L1, DH_Param L2,DH_Param L3);
	void printSolver();
	void updateTMatrices();
	int updateTheta(int link, double newTheta);
	int updateBaseSlider(double newSliderPos);
};

