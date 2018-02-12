declare name "HOA Converter : FuMa to ACN SN3D";
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
M	=	3;

// Number of inputs
ins     =       (M+1)^2;
outs     =       ins;

// FuMa Input
conversion(3,2) =   par(i,M+1,FuMaACN(i)):par(m,M+1,par(n,2*m+1,_*(1/sqrt(2*m+1)))); // FuMa to ACN_SN3D : FuMa to ACN_N3D to ACN_SN3D

FuMaACN(0) = _*sqrt(2);
FuMaACN(1) = (ro.cross(2),_):(_,ro.cross(2)):
            (_*sqrt(3),_*sqrt(3),_*sqrt(3));
FuMaACN(2) = ro.cross(5):(_,ro.cross(2),_,_):(_,_,ro.cross(3)):
            (_*(sqrt(15)/2),_*(sqrt(15)/2),_*sqrt(5),_*(sqrt(15)/2),_*(sqrt(15)/2));
FuMaACN(3) = ro.cross(7):(_,ro.cross(2),ro.cross(2),_,_):(_,_,ro.cross(2),_,_,_):(_,_,_,ro.cross(4)):
            (_*sqrt(35/8),_*(sqrt(35)/3),_*sqrt(224/45),_*sqrt(7),_*sqrt(224/45),_*(sqrt(35)/3),_*sqrt(35/8));
FuMaACN(m) = par(i,2*m+1,!:0); // normally they shouldn't be FuMa components for M>3


process = si.bus(ins):hgroup("[1]FuMa",par(i,M+1,meterm(i))):conversion(3,2):hgroup("[2]ACN SN3D",par(i,M+1,meterm(i)));