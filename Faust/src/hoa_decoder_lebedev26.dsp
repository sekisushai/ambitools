declare name        "HOA Decoder up to order 3 for 26 nodes Lebedev grid";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

// Description: Basic decoder (mode-matching) for Lebedev grid with 26 nodes, working up to order 3 [1]. 
// Possibility to choose with or without near-field compensation [2].

// References: 
//[1] Lecomte, P., Gauthier, P.-A., Langrenne, C., Garcia, A., & Berry, A. (2015). On the use of a Lebedev grid for Ambisonics. In Audio Engineering Society Convention outs/29. New York.
//[2] Lecomte, P., & Gauthier, P.-A. (2015). Real-Time 3D Ambisonics using Faust, Processing, Pure Data, And OSC. In 15th International Conference on Digital Audio Effects (DAFx-15). Trondheim, Norway.

// Inputs: (M+1)^2
// Outputs: 26

import("stdfaust.lib");
import("lib/ymn.lib");
import("lib/nfc.lib");
import("lib/lebedev.lib");
import("lib/gui.lib");

// Maximum order 3 to have no aliasing in the sweet spot.
M	=	3;

ins	=	(M+1)^2;
outs	=	26;

near	=	vgroup("[3]NFC",checkbox("Yes"));

// Spherical restitution speaker layout radius r2 is needeed to stabilize near-field filters, see [2]
r2	=	nentry("[4]Speakers Radius", 1.07, 0.5, 10, 0.01);

// Gains: CAUTION with maximal value (60 dB!) it's to compensate the attenuation of the microphone radial filters.
volin	=	vslider("[1]Inputs Gain[unit:dB][osc:/levelin -10 60]", 0, -10, 60, 0.1) : ba.db2linear : si.smooth(0.999);
volout	=	vslider("[2]Outputs Gain[unit:dB][osc:/levelout -10 60]", 0, -10, 60, 0.1) : ba.db2linear : si.smooth(0.999);

matrix(n,m)	=	hgroup("B-Format",si.bus(ins):par(i,M+1,metermute(i)))<:par(i,m,buswg(row(i)):>_*(volout));

// When near-field compensation is activated, multiplication by 4*PI*r2 to have the correct gain, see [2]
selecteur	=	si.bus(ins)<:((par(i,ins,*(near*volin*r2)):par(m,M+1,par(i,2*m+1,nfc(m,r2)))),par(i,ins,*((1-near)*volin))):>si.bus(ins);

// Analytic decoder matrix Wlebedev.YLebedev [1]
// Vector of weighted spherical harmonics : spherical harmonics times the speaker weight for weighet quadrature rules [1]
row(i)	=	par(j,ins,yacn(j,azimuth(i),elevation(i))*weight3(i));

process	=	hgroup("Inputs",selecteur:matrix(ins,outs)):(hgroup("Outputs 1-outs/2",par(i,outs/2,id2(i,0))),hgroup("Outputs 14-26",par(i,outs/2,id2(i,outs/2))));