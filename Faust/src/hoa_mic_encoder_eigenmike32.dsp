declare name        "Pentakis dodecahedron HOA Encoder";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

import("math.lib");
import("music.lib");
import("lib/eigenmike32.lib");
import("lib/ymn.lib");

smooth(c)       = *(1-c) : +~*(c);
vol             = hslider("[2]Gain[unit:dB]", 0, -10, 50, 0.1) : db2linear : smooth(0.999);
vmeter(x)	= attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop		= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",id(i,m*m-1)));

process= hgroup("[0]Inputs 1-32",par(i,32,id(i,0)))<:par(i,25,buswg(row(i)):>_):par(i,25,*(vol)):hgroup("[1]B-Format",meterm(0),meterm(1),meterm(2),meterm(3),meterm(4));

row(0)=par(i,32,y0(azimuth(i),elevation(i))*(weight(i)));
row(1)=par(i,32,y1(azimuth(i),elevation(i))*(weight(i)));
row(2)=par(i,32,y2(azimuth(i),elevation(i))*(weight(i)));
row(3)=par(i,32,y3(azimuth(i),elevation(i))*(weight(i)));
row(4)=par(i,32,y4(azimuth(i),elevation(i))*(weight(i)));
row(5)=par(i,32,y5(azimuth(i),elevation(i))*(weight(i)));
row(6)=par(i,32,y6(azimuth(i),elevation(i))*(weight(i)));
row(7)=par(i,32,y7(azimuth(i),elevation(i))*(weight(i)));
row(8)=par(i,32,y8(azimuth(i),elevation(i))*(weight(i)));
row(9)=par(i,32,y9(azimuth(i),elevation(i))*(weight(i)));
row(10)=par(i,32,y10(azimuth(i),elevation(i))*(weight(i)));
row(11)=par(i,32,y11(azimuth(i),elevation(i))*(weight(i)));
row(12)=par(i,32,y12(azimuth(i),elevation(i))*(weight(i)));
row(13)=par(i,32,y13(azimuth(i),elevation(i))*(weight(i)));
row(14)=par(i,32,y14(azimuth(i),elevation(i))*(weight(i)));
row(15)=par(i,32,y15(azimuth(i),elevation(i))*(weight(i)));
row(16)=par(i,32,y16(azimuth(i),elevation(i))*(weight(i)));
row(17)=par(i,32,y17(azimuth(i),elevation(i))*(weight(i)));
row(18)=par(i,32,y18(azimuth(i),elevation(i))*(weight(i)));
row(19)=par(i,32,y19(azimuth(i),elevation(i))*(weight(i)));
row(20)=par(i,32,y20(azimuth(i),elevation(i))*(weight(i)));
row(21)=par(i,32,y21(azimuth(i),elevation(i))*(weight(i)));
row(22)=par(i,32,y22(azimuth(i),elevation(i))*(weight(i)));
row(23)=par(i,32,y23(azimuth(i),elevation(i))*(weight(i)));
row(24)=par(i,32,y24(azimuth(i),elevation(i))*(weight(i)));