declare name        "HOA Beamforming Hypercardioid To HOA";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2016";

// Description : This tool applies a hypercardioid beampattern to the HOA scene to enhances some directions according to the chosen beampattern. See [1] for more details. The proposed beampattern are regular hypercardioid as described in [2]

// Inputs : (M+1)^2
// Outputs : (M+1)^2 + (M1+1)^2 where M1 is the hypercardioid order.

// References : 
// [1] P. Lecomte, P.-A. Gauthier, C. Langrenne, A. Berry, and A. Garcia, “Filtrage directionnel dans un scène sonore 3D par une utilisation conjointe de Beamforming et d’Ambisonie d’ordre elevés,” in CFA / VISHNO 2016, 2016, pp. 169–175. 
// [2] J. Meyer and G. Elko, “A highly scalable spherical microphone array based on an orthonormal decomposition of the soundfield,” in IEEE International Conference on Acoustics, Speech, and Signal Processing, 2002, vol. 2, pp. 1781–1784.

// IMPORTANT : this code could take long time to compile.. it's normal in regards of the size of the matrix involved (36*36*36)..

import("lib/ymn.lib");
import("lib/cijk.lib");

M=5; // Maximum order of original HOA scene
M1=3; // Maximum order of hyppercardioid beampattern

ins=(M+1)^2;
outs=(M+1)^2; // Should be (M1max+Mmax)^2 to avoid loosing some information after filtering.

t=hslider("[2]Azimuth", 0, 0, 360, 0.1)*PI/180;
d=hslider("[3]Elevation", 0, -90, 90, 0.1)*PI/180;

order=int(hslider("[1]Order",1,1,3,1)-1);

norm(m)=1/sqrt(2*m+1);

// ORDER 1
// c10=0.249993;
// c11=0.433017;

// g10=c10*norm(0)*y0(t,d);

// g11=c11*norm(1)*y1(t,d);
// g12=c11*norm(1)*y2(t,d);
// g13=c11*norm(1)*y3(t,d);

// ORDER 2
// c20=0.11112;
// c21=0.19245;
// c22=0.248448;

// g20=c20*norm(0)*y0(t,d);

// g21=c21*norm(1)*y1(t,d);
// g22=c21*norm(1)*y2(t,d);
// g23=c21*norm(1)*y3(t,d);

// g24=c22*norm(2)*y4(t,d);
// g25=c22*norm(2)*y5(t,d);
// g26=c22*norm(2)*y6(t,d);
// g27=c22*norm(2)*y7(t,d);
// g27=c22*norm(2)*y8(t,d);

// ORDER 3
// c30=0.0625128;
// c31=0.108241;
// c32=0.139751;
// c33=0.165365;

// g20=c20*norm(0)*y0(t,d);

// g21=c21*norm(1)*y1(t,d);
// g22=c21*norm(1)*y2(t,d);
// g23=c21*norm(1)*y3(t,d);

// g24=c21*norm(2)*y4(t,d);
// g25=c21*norm(2)*y5(t,d);
// g26=c21*norm(2)*y6(t,d);
// g27=c21*norm(2)*y7(t,d);
// g28=c21*norm(2)*y8(t,d);

// g29=c21*norm(3)*y9(t,d);
// g30=c21*norm(3)*y10(t,d);
// g31=c21*norm(3)*y11(t,d);
// g32=c21*norm(3)*y12(t,d);
// g33=c21*norm(3)*y13(t,d);
// g34=c21*norm(3)*y14(t,d);
// g35=c21*norm(3)*y15(t,d);

// COEFFS IN MATRIX FORM
coeffs = waveform {
0.249993,	0.433017,	0,		0,
0.11112,	0.19245,	0.248448,	0,
0.0625128,	0.108241,	0.139751,	0.165365
};
mat2(row,col,content)    = \(i,j).(content, i*col+j : rdtable);
hypercoeff=mat2(3,4,coeffs);

g(M1,0)=hypercoeff(M1,0)*norm(0)*y0(t,d);

g(M1,1)=hypercoeff(M1,1)*norm(1)*y1(t,d);
g(M1,2)=hypercoeff(M1,1)*norm(1)*y2(t,d);
g(M1,3)=hypercoeff(M1,1)*norm(1)*y3(t,d);

g(M1,4)=hypercoeff(M1,2)*norm(2)*y4(t,d);
g(M1,5)=hypercoeff(M1,2)*norm(2)*y5(t,d);
g(M1,6)=hypercoeff(M1,2)*norm(2)*y6(t,d);
g(M1,7)=hypercoeff(M1,2)*norm(2)*y7(t,d);
g(M1,8)=hypercoeff(M1,2)*norm(2)*y8(t,d);

g(M1,9)=hypercoeff(M1,3)*norm(3)*y9(t,d);
g(M1,10)=hypercoeff(M1,3)*norm(3)*y10(t,d);
g(M1,11)=hypercoeff(M1,3)*norm(3)*y11(t,d);
g(M1,12)=hypercoeff(M1,3)*norm(3)*y12(t,d);
g(M1,13)=hypercoeff(M1,3)*norm(3)*y13(t,d);
g(M1,14)=hypercoeff(M1,3)*norm(3)*y14(t,d);
g(M1,15)=hypercoeff(M1,3)*norm(3)*y15(t,d);

// TERM i,j OF THE FILTER MATRIX
m(i,j)=par(k,(M1+1)^2,g(order,k)*Cijk(i,j,k)):>+; 

// A ROW OF THE MATRIX
a(i)=par(j,outs,m(i,j));

// n = number of inputs
// m = number of output
matrix(n,m) = par(i,n,_) <: par(i,m,buswg(a(i)):>_);

process=matrix(ins,outs);




