declare name "HOA Converter : ACN N3D to FuMa";
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
M	=	5;

// Number of inputs
ins     =       (M+1)^2;
outs     =       ins;

// ACN_N3D Input
conversion(1,3) =   par(i,M+1,ACNFuMa(i)); // ACN_N3D to FuMa

ACNFuMa(0) = _*(1/sqrt(2));
ACNFuMa(1) = ro.cross(3):(_,ro.cross(2)):
            (_*(1/sqrt(3)),_*(1/sqrt(3)),_*(1/sqrt(3)));
ACNFuMa(2) = (ro.cross(3),_,_):(_,ro.cross(3),_):(_,_,ro.cross(2),_):(_,_,_,ro.cross(2)):
            (_*(1/sqrt(5)),_*(2/sqrt(15)),_*(2/sqrt(15)),_*(2/sqrt(15)),_*(2/sqrt(15)));
ACNFuMa(3) = (ro.cross(4),_,_,_):(_,ro.cross(4),_,_):(_,_,ro.cross(3),_,_):(_,_,_,ro.cross(3),_):(_,_,_,_,ro.cross(2),_):(_,_,_,_,_,ro.cross(2)):
            (_*(1/sqrt(7)),_*sqrt(45/224),_*sqrt(45/224),_*(3/sqrt(35)),_*(3/sqrt(35)),_*sqrt(8/35),_*sqrt(8/35));
ACNFuMa(m) = par(i,2*m+1,!:0);

process = si.bus(ins):hgroup("[1]ACN N3D",par(i,M+1,meterm(i))):conversion(1,3):hgroup("[2]FuMa",par(i,M+1,meterm(i)));