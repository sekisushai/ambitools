declare name        "NFC-HOA with 06 nodes Lebedev grid up to order 1";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

// Description: This tool computes the driving signal of loudspeakers arranged on a 26-node Lebedev grid with equivalent panning law for an HOA scene with N sources [1]. Source types are plane or spherical waves.

// References: 
//[1] Lecomte, P., & Gauthier, P.-A. (2015). Real-Time 3D Ambisonics using Faust, Processing, Pure Data, And OSC. In 15th International Conference on Digital Audio Effects (DAFx-15). Trondheim, Norway.
//[2] Lecomte, P., & Gauthier, P.-A. (2015). Real-Time 3D Ambisonics using Faust, Processing, Pure Data, And OSC. In 15th International Conference on Digital Audio Effects (DAFx-15). Trondheim, Norway.

// Inputs: N
// Outputs: 26

import("stdfaust.lib");
import("lib/nfc.lib");
import("lib/ymn.lib");
import("lib/lebedev.lib");
import("lib/gui.lib");

M	=	1; // maximum order for Ambisonics components
N	=	2; // number of inputs (number of sources to encode)

ins	=	N;
outs	=	6;

g(i)	=	hslider("[%i+1][osc:/gain_%i -20 20][style:knob]Gain %2i",0,-30,20,0.1): ba.db2linear : si.smooth(0.999); // gain
r(i)	=	hslider("[%i+2][osc:/radius_%i 0.5 50][style:knob]Radius %2i", 2, 0.5, 50, 0.01); // radius
t(i)	=	hslider("[%i+3][osc:/azimuth_%i 0 360][style:knob]Azimuth %2i", 0, 0, 360, 0.1)*ma.PI/180; // azimuth
d(i)	=	hslider("[%i+4][osc:/elevation_%i -90 90][style:knob]Elevation %2i", 0, -90, 90, 0.1)*ma.PI/180; // elevation

mute	=	par(i,M+1,_*vgroup("[2]Mute Order",1-checkbox("%i")));

spherical(i)	=	hgroup("[%i+5]Spherical Wave",checkbox("Yes"));
// Spherical restitution speaker layout radius r2 is needeed to stabilize near-field filters, see [1]
r2	=	nentry("[~]Speaker Radius", 1.07, 0.5, 10, 0.01); // louspeaker radius

// For plane wave, gain multiplication by 4*PI*r2; for spherical wave, gain multiplication by (4*PI*r2)/(4*PI*r(i)) [2].
selecteur(i)	=	_*(g(i))<:(*(spherical(i)),*(1-spherical(i)))<:(*(r2/r(i))<:par(m,M+1,nf(m,r(i),r2))),(*(r2)<:par(m,M+1,nfc(m,r2))):>par(m,M+1,*(2*m+1)):mute;

signal(source,speaker)	=	hgroup("",selecteur(source):par(m,M+1,_*(LegendreP(m,gamma))):>_*(weight1(speaker)))
			with {
			gamma=angle(t(source),d(source),azimuth(speaker),elevation(speaker));
			};

process=si.bus(N)<:par(speaker,outs,par(source,N,signal(source,speaker)):>_):hgroup("[~]Outputs",par(i,outs,id2(i,0)));

