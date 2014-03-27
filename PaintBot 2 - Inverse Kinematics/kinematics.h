#include <cmath>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "../forwardKinematics.h"
#define PI 3.141592653589793238463

using namespace std;
#ifndef KINEMATICS_H
#define KINEMATICS_H
class kin_solver {
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

    Provides for Forward Kinematic Solving, as well as Inverse Kinematic Solving
    */
private:
    DH_Param DH_Parameters[4];
    T_Matrix  T12, T23, T34, T01, T02, T03, T04;
    armLink Link0, Link1, Link2, Link3;
public:
    kin_solver();
    kin_solver(DH_Param L0, DH_Param L1, DH_Param L2,DH_Param L3);
    void initLinkPoints();
    void printSolver();
    void printLinkPoints();
    void updateTMatrices();
    double getD(){return DH_Parameters[0].d;}
    double getTheta2(){return DH_Parameters[2].theta;}
    double getTheta3(){return DH_Parameters[3].theta;}
    vector<vector<double> > updateArmLinks();
    vector<vector<double> > updateTheta(double newTheta, int sub); //Returns a 2D vector with a X,Y pair as each row.
    vector<vector<double> > updateThetas(double newTheta2, double newTheta3);
    vector<vector<double> > updateBaseSlider(double newSliderPos);
    vector<vector<double> > forwardSolver(double newSliderPos, double newTheta2, double newTheta3);
    vector<vector<double> > inverseSolver(double end_x, double end_y); //Takes X and Y coord of requested end effector position and upadtes the DH parameters
};


#endif
