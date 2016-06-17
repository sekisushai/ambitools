declare name        "HOA Beamforming Hypercardioid To HOA";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2016";

// Description: This tool applies a hypercardioid beampattern to the HOA scene to enhances some directions according to the chosen beampattern. See [1] for more details. The proposed beampattern are regular hypercardioid as described in [2]. The more high the order of the beampattern the more selective is the directionnal filtering.

// Inputs: (M+1)^2
// Outputs: (M+1)^2 + (M1+1)^2 where M1 is the hypercardioid order.

// References: 
// [1] P. Lecomte, P.-A. Gauthier, C. Langrenne, A. Berry, and A. Garcia, “Filtrage directionnel dans un scène sonore 3D par une utilisation conjointe de Beamforming et d’Ambisonie d’ordre elevés,” in CFA / VISHNO 2016, 2016, pp. 169–175. 
// [2] J. Meyer and G. Elko, “A highly scalable spherical microphone array based on an orthonormal decomposition of the soundfield,” in IEEE International Conference on Acoustics, Speech, and Signal Processing, 2002, vol. 2, pp. 1781–1784.

// CAUTION : this code could take long time to compile for higher order M. It is normal in regards of the size of the matrix involved ((M+1)^2*(M+1)^2*(M1+1)^2).

import("lib/ymn.lib");
import("lib/cijk.lib");
import("lib/gui.lib");

M	=	3; // Maximum order of original HOA scene
M1	=	2; // Maximum order of hyppercardioid beampattern (implemented up to order M1=3, but very CPU consuming when M1=3 is chosen)

ins	=	(M+1)^2;
outs	=	(M+M1+1)^2; //ins // Should be (M+M1+1)^2 to avoid loosing some information after filtering.

t	=	hslider("Azimuth[style:knob]", 0, 0, 360, 0.1)*PI/180;
d	=	hslider("Elevation[style:knob]", 0, -90, 90, 0.1)*PI/180;

order	=	int(hslider("Order[style:knob]",0,0,M1,1)); // Order of the beampattern used for filtering, order=0 is a bypass.

norm(m)	=	1/sqrt(2*m+1);

// ORDER 0
hypercoeff(0,0)	=	1;

// ORDER 1
hypercoeff(1,0)	=	0.24993;
hypercoeff(1,1)	=	0.433017;

// ORDER 2
hypercoeff(2,0)	=	0.11112;
hypercoeff(2,1)	=	0.19245;
hypercoeff(2,2)	=	0.248448;

// ORDER 3
hypercoeff(3,0)	=	0.0625128;
hypercoeff(3,1)	=	0.108241;
hypercoeff(3,2)	=	0.139751;
hypercoeff(3,3)	=	0.165365;

hypercoeff(x1,x2)	=	0;

g(beam,m)	=	hypercoeff(beam,m)*norm(m);

gvec(beam,M)	=	par(m,M+1,par(n,2*m+1,g(beam,m)));

// TERM i,j OF THE FILTER MATRIX
mat(beam,i,j)	=	gvec(beam,beam):par(k,(beam+1)^2,_*Cijk(i,j,k)):yvec((beam+1)^2,t,d):>_;

// A ROW OF THE MATRIX
row(beam,i)	=	par(j,ins,mat(beam,i,j));

// in = number of inputs
// out = number of output
matrix(beam,in,out)	=	par(i,in,_)<: par(i,out,buswg(row(beam,i)):>_);

//process=bus(ins)<:par(i,M1+1,par(j,ins,_*(i==order))):bus(ins),par(i,M1,matrix(i+1,ins,outs)):>bus(outs);
process	=	bus(ins):hgroup("[1]Inputs",par(i,M+1,meterm(i)))
		<:hgroup("[3]Parameters",par(i,M1+1,matrix(i,ins,outs)):interleave(int(outs),int(M1+1)):par(i,outs,selectn(M1+1,order)))
		:hgroup("[2]Outputs",par(i,int(sqrt(outs)),meterm(i)));

