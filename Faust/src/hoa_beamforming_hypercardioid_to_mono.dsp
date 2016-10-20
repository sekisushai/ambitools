declare name        "HOA-Beamformer to Mono";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

// Description: This tool helps to extract a monophonic signal from the HOA scene with a beampatern. The Ambisonics components inputs are combined to produce a monophonic output as if the capture was done with a directionnal microphone. The beampatern provided are regular hypercardioid up to order 5 [1].

// References: 
// [1] Meyer, J., & Elko, G. (2002). A highly scalable spherical microphone array based on an orthonormal decomposition of the soundfield. 2002 IEEE International Conference on Acoustics, Speech, and Signal Processing, 2, II–1781–II–1784.

// Inputs: (M+1)^2
// Outputs: 1

import("stdfaust.lib");
import("lib/ymn.lib");
import("lib/gui.lib");

M	=	3; // Maximum order of original HOA scene.

// Output gain
vol	=	vslider("[2]Output Gain", 0, -10, 10, 0.1) : ba.db2linear : si.smooth(0.999);

t	=	vslider("[3]Azimuth", 0, 0, 360, 0.1)*ma.PI/180;
d	=	vslider("[4]Elevation", 0, -90, 90, 0.1)*ma.PI/180;

order	=	int(vslider("[1]Order",1,1,M,1));

norm(m)	=	1/sqrt(2*m+1);

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

gvec(beam,M)	=	par(m,M+1,par(n,2*m+1,_*g(beam,m)));

process		=	hgroup("Inputs",par(i,M+1,metermute(i)):yvec((M+1)^2,t,d)<:par(m,M,gvec(m+1,M):>_*vol):ba.selectn(M,order-1)):hgroup("Output",hmeter);