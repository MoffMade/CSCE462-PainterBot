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

vector<vector<double> > kin_solver::inverseSolver(double end_x, double end_y){
    //Change from graphics XY plane to ZX reference frame of robot
    vector<vector<double> > linkPositions, XYpositions;
    cout<<"Got "<<end_x<<" and "<<end_y<<endl;
    //Check range:  (x-300)^2+(y-150)^2<=175^2 || x^2+(y-150)^2<=175^2 || (x>=0&&x<=300&&y>=-25&&y<=325))
    if( pow((end_x-300),2)+pow((end_y-150),2)<=pow(175,2) || pow(end_x,2)+pow((end_y-150),2)<=pow(175,2) || (end_x>=0&&end_x<=300&&end_y>=-25&&end_y<=325)) {
        double theta2, theta3, d_length;
        //requested (z,x) position for end effector
        double z_end=end_x;
        double x_end=end_y;
        //arm link lengths, used in calculations
        double l1, l2, l3;
        l1=150;
        l2=100;
        l3=75;
        double curr_d=DH_Parameters[0].d;

        //Find new d_length
        if(z_end<curr_d){
            d_length=curr_d-10;
        }
        else if(z_end>curr_d){
            d_length=curr_d+10;
        }
        else{
            d_length=curr_d;
        }
        if(d_length<0){ d_length=0;}
        if(d_length>300){ d_length=300;}

        //Find theta3 using law of cosines between the end of the prismatic joint and the desired end effector position
        double theta3_arg=(pow(l2,2)+pow(l3,2)-pow(z_end-d_length,2)-pow(x_end-l1,2))/(2*l2*l3);
        if(theta3_arg>=-1&&theta3_arg<=1){
            theta3=PI-acos(theta3_arg);
        }
        else{
            if(theta3_arg<-1) theta3=PI-PI;
            else theta3=PI-0;
        }

        //Find theta2 by finding alpha across from the vector (d_length,0) to (z_end, x_end) in the triangle formed with L1 and the vector
        // (d_length, L1) to (z_end, x_end) and beta across from L3 in the triangle formed with L2 and the vector (d_length, L1) to (z_end, x_end)
        //vec is the magnitude of vector (d_length, L1) to (z_end, x_end)
        double vec= sqrt(pow(z_end-d_length,2)+pow(x_end-l1,2));
        double alpha, beta;
        double alpha_arg=(pow(x_end-l1,2)+pow(l1,2)-pow(x_end,2))/(2*l1*vec);
        if(alpha_arg>=-1&&alpha_arg<=1){
            alpha=acos(alpha_arg);
        }
        else{
            if(alpha_arg<-1) alpha=PI;
            else alpha=0;
        }
        double beta_arg=(pow(vec,2)+pow(l2,2)-pow(l3,2))/(2*l2*vec);
        if(beta_arg>=-1&&beta_arg<=1){
            beta=acos(beta_arg);
        }
        else{
            if(beta_arg<-1) beta=PI;
            else beta=0;
        }
        theta2=PI+beta-alpha;
cout<<"Theta3 Arg: "<<theta3_arg<<" Alpha Arg: "<<alpha_arg<<" Beta Arg: "<<beta_arg<<endl;

        //Shift into 0<=theta<2*PI range

        if(theta2<0)
            theta2=2*PI-fmod(theta2, 2*PI);
        if(theta3<0)
            theta3=2*PI-fmod(theta3, 2*PI);

        cout<<"Inverse solution- "<<"D: "<<d_length<<" theta2: "<<rad2degree(theta2)<<" theta3: "<<rad2degree(theta3)<<endl;

        updateBaseSlider(d_length);
        XYpositions=updateThetas(rad2degree(theta2),rad2degree(theta3));
        /*
        updateTheta(fmod(theta2,360),2);
        XYpositions=updateTheta(fmod(theta3,360),3);
        */
    }
    else{
        cout<<"Invalid end effector position, must be within reachable workspace; no change in position."<<endl;
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
