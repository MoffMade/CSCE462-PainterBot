#include "kinematics.h"

kin_solver::kin_solver(DH_Param l0, DH_Param l1, DH_Param l2, DH_Param l3){
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

void kin_solver::initLinkPoints(){
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
void kin_solver::printSolver(){
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

void kin_solver::updateTMatrices(){
    //Recalculates each matrix for the solver using the DH parameters, then matrix multiplication.
    T01.calcMatrix(DH_Parameters[0]);
    T12.calcMatrix(DH_Parameters[1]);
    T23.calcMatrix(DH_Parameters[2]);
    T34.calcMatrix(DH_Parameters[3]);
    T02.T_Mat_multiply(T01,T12);
    T03.T_Mat_multiply(T02,T23);
    T04.T_Mat_multiply(T03,T34);
};
vector<vector<double> > kin_solver::updateArmLinks(){
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
vector<vector<double> > kin_solver::updateTheta(double newTheta, int sub){
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

vector<vector<double> >  kin_solver::updateBaseSlider(double newSliderPos){
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

vector<vector<double> > kin_solver::updateThetas(double newTheta2, double newTheta3){
    vector<vector<double> > linkPositions, XYpositions;
    DH_Parameters[2].theta=degree2rad(newTheta2);
    DH_Parameters[3].theta=degree2rad(newTheta3);
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
    return XYpositions;
};

vector<vector<double> > kin_solver::forwardSolver(double newSliderPos, double newTheta2, double newTheta3){
    vector<vector<double> > linkPositions, XYpositions;
    //Uses functions from forward kinematic solver to update the links based on forward kinematic model
    updateBaseSlider(newSliderPos);
    XYpositions=updateThetas(newTheta2,newTheta3);
    return XYpositions;
};

vector<vector<double> > kin_solver::inverseSolver(double end_x, double end_y){
    vector<vector<double> > linkPositions, XYpositions;
    //Check range:  (x-300)^2+(y-150)^2<=175^2 || x^2+(y-150)^2<=175^2 || (x>=0&&x<=300&&y>=-25&&y<=325))
    if( pow((end_x-300),2)+pow((end_y-150),2)<=pow(175,2) || pow(end_x,2)+pow((end_y-150),2)<=pow(175,2) || (end_x>=0&&end_x<=300&&end_y>=-25&&end_y<=325)) {
        double theta2, theta3, d_length, phi, psi;
        //arm link lengths, used in calculations
        double l1, l2, l3;
        l1=150;
        l2=100;
        l3=75;
        double curr_d=DH_Parameters[0].d;
        //(x-d)^2+(y-l1)^2<=(l2+l3)^2 - outer reachable circle equation
        //(x-d)^2+(y-l1)^2>=(l2-l3)^2 - inner reachable circle equation
        //(end_x, end_y) must be within outer circle and outside inner circle


        //(l2-l3)^2<=(x-d)^2+(y-l1)^2<=(l2+l3)^2
        //Find new d_length
        //Check to make sure within reachable space without moving D
        if(((pow(end_x-curr_d,2)+pow(end_y-l1,2))>pow(l2+l3,2))||((pow(end_x-curr_d,2)+pow(end_y-l1,2))<pow(l2-l3,2))){
            //change d_length to include (end_x, end_y) in reachable circle
            d_length=curr_d;
            while(((pow(end_x-d_length,2)+pow(end_y-l1,2))>pow(l2+l3,2))||((pow(end_x-d_length,2)+pow(end_y-l1,2))<pow(l2-l3,2))){
                if(end_x<curr_d){
                    d_length-=1;
                }
                else
                    d_length+=1;
            }
        }
        else{
            //(end_x, end_y) is already reachable
            d_length=curr_d;
        }
        if(d_length<0){ d_length=0;}
        if(d_length>300){ d_length=300;}

        //Magnitudes of vectors from (d,0) to (x,y) and (d, L1) to (x,y)
        //Used in theta3 and phi calculations
        double r=sqrt(pow(end_x-d_length,2)+pow(end_y-l1,2));
        double b=sqrt(pow(end_x-d_length,2)+pow(end_y,2));

        double theta3_arg=(pow(r,2)-pow(l2,2)-pow(l3,2))/(2*l2*l3);
        if(theta3_arg<-1){theta3_arg=-1;}
        else if(theta3_arg>1){theta3_arg=1;}
        theta3= acos(theta3_arg);

        //determine unique theta2 based on theta3
        double phi_arg=(pow(l1,2)+pow(r,2)-pow(b,2))/(2*l1*r);
        //ensure acos range is met (small rounding errors in calc can cause values +-1 +- 1e-14
        if(phi_arg<-1){phi_arg=-1;}
        else if(phi_arg>1){phi_arg=1;}

        phi=acos(phi_arg);
        psi=atan2((l3*sin(theta3)),(l2+l3*cos(theta3)));

        if(end_x>=d_length){
            theta2=PI-(psi+phi);
        }
        else
            theta2=PI-(psi-phi);

        //Update Links with new joint values
        XYpositions=forwardSolver(d_length, fmod(rad2degree(theta2),360), fmod(rad2degree(theta3),360));
    }
    else{
        cout<<"Invalid position, must be within reachable workspace; no change in position."<<endl;
        linkPositions.push_back(Link0.getStart());
        linkPositions.push_back(Link1.getStart());
        linkPositions.push_back(Link2.getStart());
        linkPositions.push_back(Link3.getStart());
        XYpositions=linkPositions;
    }
    return XYpositions;
};

void kin_solver::printLinkPoints(){
    cout<<"Link Points"<<endl;
    cout<<"Point 0: "<<Link0.getStart()[0]<<", "<<Link0.getStart()[1]<<", "<<Link0.getStart()[2]<<endl;
    cout<<"Point 1: "<<Link1.getStart()[0]<<", "<<Link1.getStart()[1]<<", "<<Link1.getStart()[2]<<endl;
    cout<<"Point 2: "<<Link2.getStart()[0]<<", "<<Link2.getStart()[1]<<", "<<Link2.getStart()[2]<<endl;
    cout<<"Point 3: "<<Link3.getStart()[0]<<", "<<Link3.getStart()[1]<<", "<<Link3.getStart()[2]<<endl;
}
