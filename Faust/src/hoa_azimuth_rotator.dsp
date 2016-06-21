declare name        "HOA Azimuth Rotator";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

// Description: This tool rotates the HOA scene around the z-axis. Driven with OSC from head-tracking, (for example with andOSC application for Android with andOSC.pd patch provided with ambitools), this tool can compensate the head rotation around z-axis when the rendering is made over headphones. See [2] for the matrix definition.

// References: 
// [1] M. Kronlachner, “Spatial Transformations for the Alteration of Ambisonic Recordings,” Graz University Of Technology, Austria, 2014.

// Inputs: (M+1)^2
// Outputs: (M+1)^2

import("math.lib");
import("music.lib");
import("lib/ymn.lib");

M=5; // Maximum order of original HOA scene

ins=(M+1)^2; // Number of inputs (= number of outputs).

t=hslider("Azimuth[osc:/azimuth 0 360]", 0, 0, 360, 0.01)*PI/180; // Slider with azimuth rotation angle

// SUB-MATRIX TERM, AT EACH ORDER
// diagonal terms, anti-diagonal terms, extra diagonal terms
rot(m,i,j)	=	case{
			(1,1) => 1; // (i,j) is on the extra-diagonal AND on the diagonal.
			(0,1) => cos((m-i)*t); // (i,j) is NOT on the extra-diaognal AND on the diagonal.
			(1,0) => sin((m-i)*t); // (i,j) is on the extra diagonal AND not on the diagonal. sinus is anti-symmetric which correspond to the anti-symetric matrix
			(0,0) => 0; // (i,j) is NOT on the extra-diagonal AND NOT on the diagonal
			}(i+1+j+1==2*m+1+1,i==j); //test: ((i,j) is on the extra diagonal, (i,j) is on the diagonal)
			
// MAIN-MATRIX ROW			
row(M,i)	=	par(m,M+1,
			  par(j,2*m+1,term 
			    with{term = if((i >= m^2) & (i< (m+1)^2),rot(m,int(i-m^2),j),0);}
			    )
			    );

// Matrix multiplication
// n = number of inputs
// m = number of outputs
matrix(n,m) = par(i,n,_) <: par(i,m,buswg(row(M,i)):>_);

process = matrix(ins,ins);

// EXAMPLE OF A MATRIX AT ORDER 5
// a(0)=(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// 
// a(1)=(0, cos(t), 0, sin(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(2)=(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(3)=(0, -1*sin(t), 0, cos(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// 
// a(4)=(0, 0, 0, 0, cos(2*t), 0, 0, 0, sin(2*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(5)=(0, 0, 0, 0, 0, cos(t), 0, sin(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(6)=(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(7)=(0, 0, 0, 0, 0, -1*sin(t), 0, cos(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(8)=(0, 0, 0, 0, sin(2*t), 0, 0, 0, cos(2*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// 
// a(9)=(0, 0, 0, 0, 0, 0, 0, 0, 0, cos(3*t), 0, 0, 0, 0, 0, sin(3*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(10)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(2*t), 0, 0, 0, sin(2*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(11)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(t), 0, sin(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(12)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(13)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(t), 0, cos(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(14)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(2*t), 0, 0, 0, cos(2*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(15)=(0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(3*t), 0, 0, 0, 0, 0, cos(3*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// 
// a(16)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(4*t), 0, 0, 0, 0, 0, 0, 0, sin(4*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(17)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(3*t), 0, 0, 0, 0, 0, sin(3*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(18)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(2*t), 0, 0, 0, sin(2*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(19)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(t), 0, sin(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// a(20)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(21)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(t), 0, cos(t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(22)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(2*t), 0, 0, 0, cos(2*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(23)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(3*t), 0, 0, 0, 0, 0, cos(3*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
// a(24)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(4*t), 0, 0, 0, 0, 0, 0, 0, cos(4*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); 
// 
// a(25)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(5*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, sin(5*t));
// a(26)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(4*t), 0, 0, 0, 0, 0, 0, 0, sin(4*t), 0);
// a(27)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(3*t), 0, 0, 0, 0, 0, sin(3*t), 0, 0);
// a(28)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(2*t), 0, 0, 0, sin(2*t), 0, 0, 0);
// a(29)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(t), 0, sin(t), 0, 0, 0, 0);
// a(30)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
// a(31)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(t), 0, cos(t), 0, 0, 0, 0);
// a(32)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(2*t), 0, 0, 0, cos(2*t), 0, 0, 0);
// a(33)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(3*t), 0, 0, 0, 0, 0, cos(3*t), 0, 0);
// a(34)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(4*t), 0, 0, 0, 0, 0, 0, 0, cos(4*t), 0);
// a(35)=(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1*sin(5*t), 0, 0, 0, 0, 0, 0, 0, 0, 0, cos(5*t));
