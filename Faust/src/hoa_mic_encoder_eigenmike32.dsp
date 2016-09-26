declare name        "eigenmike32 HOA Encoder";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

// Description: This tool does the Discrete Spherical Fourier Transform (DSFT) [1] of signal from a rigid spherical microphone with Pentakis dodecahedron geometry (mh acoustics eigenmike32 [1]).
// CAUTION: This tool does only the DSFT, to obtain the Ambisonics components you should filter the signals with corresponding radial filters [2].

// Inputs: 32
// Outputs: (M+1)^2

// References: 
// [1] G. Elko, R. A. Kubli, and J. Meyer, “Audio system based on at least second-order eigenbeams,” 2009.
// [2] S. Moreau, J. Daniel, and S. Bertet, “3d sound field recording with higher order ambisonics-objective measurements and validation of spherical microphone,” in Audio Engineering Society Convention 120, 2006, pp. 1–24.
// [3] P. Lecomte, P.-A. Gauthier, C. Langrenne, A. Garcia, and A. Berry, “On the use of a Lebedev grid for Ambisonics,” in Audio Engineering Society Convention 139, 2015.

import("stdfaust.lib");
import("lib/eigenmike32.lib");
import("lib/ymn.lib");
import("lib/gui.lib");

M	=	4; // Maximum order M=4 for eigenmike32 [2].

ins	=	32;
outs	=	(M+1)^2;

vol	=	hslider("[1]Gain[unit:dB][style:knob]", 0, -10, 50, 0.1) : ba.db2linear : si.smooth(0.999);

// Vector of weighted spherical harmonics : spherical harmonics times the speaker weight for weighed quadrature rules [3].
row(i)	=	par(j,ins,yacn(i,azimuth(j),elevation(j))*weight(j));

process	=	hgroup("[0]Inputs",par(i,ins,id(i,0)))<:par(i,outs,buswg(row(i)):>_):hgroup("[1]Outputs",par(i,outs,*(vol)):par(i,M+1,metermute(i)));