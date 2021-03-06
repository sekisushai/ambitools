declare name        "NF-HOA encoder";
declare version     "1.1";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014-2018";

// Description: This tool creates an HOA scene from N inputs. Each input is encoded as a source in space. Source types are plane or spherical waves.

// References: 
//[1] Lecomte, P., & Gauthier, P.-A. (2015). Real-Time 3D Ambisonics using Faust, Processing, Pure Data, And OSC. In 15th International Conference on Digital Audio Effects (DAFx-15). Trondheim, Norway.

// Inputs: N
// Outputs: (M+1)^2

import("stdfaust.lib");
import("nfc.lib");
import("ymn.lib");
import("gui.lib");

// maximum order for Ambisonics components
M	=	5;
// number of inputs (number of sources to encoder)
N	=	2;

ins     =       N;
outs    =       (M+1)^2;

outsvumeter =   vumeter(0,outs);

g(i)	=	hslider("[%i+1][osc:/gain_%i -20 20][style:knob]Gain %2i",0,-20,20,0.1): ba.db2linear; // gain
r(i)	=	hslider("[%i+2][osc:/radius_%i 0.5 50][style:knob]Radius %2i", 2, 1, 50, 0.01);// radius
t(i)	=	hslider("[%i+3][osc:/azimuth_%i 0 360][style:knob]Azimuth %2i", 0, 0, 360, 0.1)*ma.PI/180; // azimuth
d(i)	=	hslider("[%i+4][osc:/elevation_%i -90 90][style:knob]Elevation %2i", 0, -90, 90, 0.1)*ma.PI/180; // elevation

spherical(i)	=	hgroup("[%i+5]Spherical Wave",checkbox("Yes"));

// Spherical restitution speaker layout radius r2 is needeed to stabilize near-field filters, see [1]
r2(i)	=	nentry("[~]Speaker Radius %2i", 1.07, 0.5, 10, 0.01); // louspeaker radius

r22(i) = r2(i) + (1-spherical(i))*(r(i)-r2(i)); // r2 is transformed to r when shperical(i) = 0 ==> spherical/plane wave selection;

source(i) = hgroup("Source %2i",_*g(i)*r22(i)/r(i)<:par(m,M+1,nf(m,r(i),r22(i))<:par(i,2*m+1,_)):>yvec((M+1)^2,t(i),d(i)));

//process	=	hgroup("",vgroup("Parameters",par(i,N,_<:selecteur(i))):>si.bus((M+1)^2):vgroup("[~]Outputs",outsvumeter));
process = hgroup("",vgroup("Parameters",par(i,N,_<:source(i))):>si.bus((M+1)^2):vgroup("[~]Outputs",outsvumeter));
