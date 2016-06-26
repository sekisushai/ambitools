declare name        "MemsBedev HOA Encoder";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

// Description: This tool does the Discrete Spherical Fourier Transform (DSFT) [1] of signal from a rigid spherical microphone with 50-node Lebedev grid geometry [1].
// CAUTION: This tool does only the DSFT, to obtain the Ambisonics components you should filter the signals with corresponding radial filters [1].

// Inputs: 26
// Outputs: (M+1)^2

// References: 
// [1] P. Lecomte, P.-A. Gauthier, C. Langrenne, A. Garcia, and A. Berry, “On the use of a Lebedev grid for Ambisonics,” in Audio Engineering Society Convention 139, 2015.

import("math.lib");
import("music.lib");
import("lib/lebedev.lib");
import("lib/ymn.lib");
import("lib/gui.lib");
import("filter.lib");

M	=	3; // Maximum order M=3 for 26-node Lebedev grid [1].

ins	=	26;
outs	=	(M+1)^2;

vol	=	hslider("[1]Gain[unit:dB][style:knob]", 0, -10, 50, 0.1) : db2linear : smooth(0.999);

// Vector of weighted spherical harmonics : spherical harmonics times the speaker weight for weighed quadrature rules [1].
row(i)	=	par(j,ins,yacn(i,azimuth(j),elevation(j))*(weight3(j)));

process	=	hgroup("[0]Inputs",par(i,ins,id(i,0)))<:par(i,outs,buswg(row(i)):>_):hgroup("[1]Outputs",par(i,outs,*(vol)):par(i,M+1,metermute(i)));