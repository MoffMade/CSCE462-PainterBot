#include <cmath>
#include <vector>
#include <iostream>
#include <stdlib.h>
#define PI 3.141592653589793238463

using namespace std;
#ifndef FORWARD_KINEMATICS_H
#define FORWARD_KINEMATICS_H
double degree2rad(double degrees);
double rad2degree(double rads);

class DH_Param {
	//Angles are sotred in radians, entered and displayed in degrees
public:
	double alpha;
	double a;
	double d;
	double theta;
	DH_Param(): alpha(0), a(0), d(0), theta(0){};
	DH_Param(double al, double ai, double di, double th): alpha(degree2rad(al)), a(ai), d(di), theta(degree2rad(th)) {};
	void printDH(){
		cout<<"("<<rad2degree(alpha)<<", "<<a<<", "<<d<<", "<<rad2degree(theta)<<") "<<endl;
	};
};

class armLink {
	//Stores the position of the link in the original reference frame.
private:
	vector<double> start_position;	//Start position in frame {0}
	vector<double> end_position;	//End position in frame {0}
public:
	armLink();
	armLink(vector<double> start, vector<double> end);
	vector<double> getStart(){return start_position;};
	vector<double> getEnd(){return end_position;};
	void setStart(vector<double> spos){start_position=spos;};
	void setEnd(vector<double> epos){end_position=epos;};

};
class T_Matrix {
	//Allows for creation of a transformation matrix from a set of DH parameters
	//Contains helper functions for multiplying by another T Matrix and a column vector 
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
	void setOrg(int nOrg){origin_frame=nOrg;};
	void setDes(int nDes){destination_frame=nDes;};
	int set(double val, int r, int c);
	void T_Mat_multiply(const T_Matrix A, const T_Matrix B);
	vector<double> pointTransform(vector<double> org_pos);
	void printMatrix();
};

class f_kin_solver {
	/*Stores T Matrix for each link to the next as well as each link in reference to frame {0}
	Contains functions to change the prismatic joint's d variable and the revolute joint's theta variable
	The T Matrices are updated and the armLink positions are recalculated after each update
	updateTheta and updateBaseSlider functions return a vector of (X,Y,Z) positions for the link's end points.
		P0-> Base-Link 1
		P1-> Link 1-Link 2
		P2-> Link 2-Link 3
		P3-> Link 3 End Effector/Paint Brush
	*see project design doc for diagram of the arm

	For DH Parameters for the PaintBot Project: 
		L0 is prismatic. d is variable, other three are fixed.
		L1 is fixed, describes transformation from XZ axis to XY reference frames of L1-3
		L2 and L3 are revolute. theta is variable, other three are fixed.
	*/
private:
	DH_Param DH_Parameters[4];
	T_Matrix  T12, T23, T34, T01, T02, T03, T04;
	armLink Link0, Link1, Link2, Link3;
public:
	f_kin_solver();
	f_kin_solver(DH_Param L0, DH_Param L1, DH_Param L2,DH_Param L3);
	void initLinkPoints();
	void printSolver();
	void printLinkPoints();
	void updateTMatrices();
	vector<vector<double> > updateArmLinks();
	vector<vector<double> > updateTheta(double newTheta, int sub); //Returns a 2D vector with a X,Y pair as each row.
	vector<vector<double> > updateBaseSlider(double newSliderPos);
};


#endif