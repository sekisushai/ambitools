declare name "HOA Rotator";
declare version "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2016";

import("stdfaust.lib");
import("lib/ymn.lib");

// Description: This tool rotates the HOA scene around the x-axis (roll angle), y-axis (pitch angle), and z-axis (yaw angle). Driven with OSC from head-tracking, (for example with andOSC application for Android with andOSC.pd patch provided with ambitools), this tool can compensate the head rotations. See [2] for the matrix definition. Implentation according to [1] with corrections.

// References: 
//[1] J. Ivanic and K. Ruedenberg, “Rotation matrices for real spherical harmonics. Direct determination by recursion,” J. Phys. Chem., vol. 100, no. 15, pp. 6342–6347, 1996.
// In TABLE 2 of this paper, from the corrections, the formulaes for the term V(l,m,m') should be taken from the original paper...

// Inputs: (M+1)^2
// Outputs: (M+1)^2

// Sliders
yaw     =   hslider("Yaw[osc:/yaw 0 360]", 0, 0, 360, 0.01)*ma.PI/180; // Slider with yaw rotation angle
pitch   =   hslider("Pitch[osc:/picth 0 360]", 0, 0, 360, 0.01)*ma.PI/180; // Slider with pitch rotation angle
roll    =   hslider("Roll[osc:/roll 0 360]", 0, 0, 360, 0.01)*ma.PI/180; // Slider with roll rotation angle

// Maximum required order
M	=	4;

ins =   (M+1)^2;

// Zero-th order
rot(0,m,n) = 1;

// First order rotation matrix (n1, n2)
rot(1,-1,-1)    =   cos(roll)*cos(yaw) - sin(pitch)*sin(roll)*sin(yaw);
rot(1,-1,0)     =   -1*cos(pitch)*sin(roll);
rot(1,-1,1)     =   cos(yaw)*sin(pitch)*sin(roll) + cos(roll)*sin(yaw);
rot(1,0,-1)     =   cos(yaw)*sin(roll) + cos(roll)*sin(pitch)*sin(yaw);
rot(1,0,0)      =   cos(pitch)*cos(roll);
rot(1,0,1)      =   -1*cos(roll)*cos(yaw)*sin(pitch) + sin(roll)*sin(yaw);
rot(1,1,-1)     =   -1*cos(pitch)*sin(yaw);
rot(1,1,0)      =   sin(pitch);
rot(1,1,1)      =   cos(pitch)*cos(yaw);
rot(1,m,n)      =   0; // other cases for 1st order.


// Recurrence computation for higher-orders
//denom(m,n2)     =   ba.if(abs(n2)<m,(m+n2)*(m-n2),2*m*(2*m-1));
denom(m,n2)     =   case{
                    (1)  => (m+n2)*(m-n2);
                    (0)  => 2*m*(2*m-1);
                    }(abs(n2)<m);



u(m,n1,n2)      =   sqrt((m+n1)*(m-n1)/denom(m,n2));
v(m,n1,n2)      =   1/2*sqrt((1+(n1==0))*(m+abs(n1)-1)*(m+abs(n1))/denom(m,n2))*(1-2*(n1==0));
w(m,n1,n2)      =   -1/2*sqrt((m-abs(n1)-1)*(m-abs(n1))/denom(m,n2))*(1-(n1==0));

//U(m,n1,n2)      =   ba.if(n1==0,P(0,m,0,n2),P(0,m,n1,n2));
U(m,0,n2)       =   P(0,m,0,n2);
U(m,n1,n2)      =   P(0,m,n1,n2);

V(m,n1,n2)      =   case{
                    (1,0,0)     =>P(1,m,1,n2)+P(-1,m,-1,n2);
                    (0,1,0)     =>P(1,m,n1-1,n2)*sqrt(1+(n1==1))-P(-1,m,-n1+1,n2)*(1-(n1==1));
                    (0,0,1)     =>P(1,m,n1+1,n2)*(1-(n1==-1))+P(-1,m,-n1-1,n2)*sqrt(1+(n1==-1)); // sqrt(1+(n1==1)) is right, in the correction of the paper it's sqrt(1-(n1==1))
                    }(n1==0,n1>0,n1<0);
                    
W(m,n1,n2)      =   case{
                    (1,0,0)     => 0; // Shouldn't be defined but covers some pattern matching cases.
                    (0,1,0)     => P(1,m,n1+1,n2)+P(-1,m,-n1-1,n2);
                    (0,0,1)     => P(1,m,n1-1,n2)-P(-1,m,-n1+1,n2);
                    }(n1==0,n1>0,n1<0);

P(i,m,mu,n2)    =   case{
                    (1,0,0)     => rot(1,i,0)*rot(m-1,mu,n2);
                    (0,1,0)     => rot(1,i,1)*rot(m-1,mu,m-1)-rot(1,i,-1)*rot(m-1,mu,-m+1);
                    (0,0,1)     => rot(1,i,1)*rot(m-1,mu,-m+1)+rot(1,i,-1)*rot(m-1,mu,m-1);
                    }(abs(n2)<m,n2==m,n2==-m);

// Other cases
rot(m,n1,n2)    =   u(m,n1,n2)*U(m,n1,n2)+v(m,n1,n2)*V(m,n1,n2)+w(m,n1,n2)*W(m,n1,n2);

// Main-matrix row			
//row(M,i)	=	par(m,M+1,
//			  par(j,2*m+1,term 
//			    with{term = ba.if((i >= m^2) & (i< (m+1)^2),rot(m,int(i-m^2)-m,j-m),0);}
//			    )
//			    );
row(M,i)	=	par(m,M+1,
			  par(j,2*m+1,term 
			    with{
                                term = 
                                case{
                                    (0) => 0;
                                    (1) => rot(m,int(i-m^2)-m,j-m);
                                    }((i >= m^2) & (i< (m+1)^2));
                                }
                             )
			    );
			    
			    
// Matrix multiplication
// n = number of inputs
// m = number of outputs
matrix(n,m) = par(i,n,_) <: par(i,m,buswg(row(M,i)):>_);

process = matrix(ins,ins);


