declare name "HOA Converter : ACN N3D to ACN SN3D";
declare version "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2016"; 

import("stdfaust.lib");
import("gui.lib");

//Description : this tool converts HOA signals defined with a convention 1 to HOA signals defined with convention 2. Proposed conventions are ACN N3D, ACN SN3D, FuMa. For ACN to FuMa, the ordering change is as in [1]
//[1] https://en.wikipedia.org/wiki/Ambisonic_data_exchange_formats

// Input ACN:       0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
// Output FuMa:     0   3   1   2   6   7   5   8   4   12  13  11  14  10  15  9 : W   XYZ RSTUV   KLMNOPQ

// Input FuMa: 0    1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 : W  XYZ RSTUV   KLMNOPQ
// Output ACN: 0    2   3   1   6   8   4   5   7   15  13  11  9   10  12  14

// Maximum required order (M = 3 for FuMa).
M	=	10;

// Number of inputs
ins     =       (M+1)^2;
outs     =       ins;

// ACN_N3D Input
conversion(1,2) =   par(m,M+1,par(n,2*m+1,_*(1/sqrt(2*m+1)))); // ACN_N3D to ACN_SN3D

process = si.bus(ins):hgroup("[1]ACN N3D",par(i,M+1,meterm(i))):conversion(1,2):hgroup("[2]ACN SN3D",par(i,M+1,meterm(i)));