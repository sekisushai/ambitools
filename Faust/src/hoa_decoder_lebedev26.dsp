declare name        "HOA Decoder up to order 3 for 26 nodes Lebedev grid";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("math.lib");
import("music.lib");
import("lib/ymn.lib");
import("lib/nfc.lib");
import("lib/lebedev.lib");

//Basic decoder (mode-matching) for Lebedev grid with 50 nodes, working up to order 5 [1]. 
//Possibility to choose with or without near-field compensation.
//[1] Lecomte, P., Gauthier, P.-A., Langrenne, C., Garcia, A., & Berry, A. (2015). On the use of a Lebedev grid for Ambisonics. In Audio Engineering Society Convention 139. New York.
//[2] Lecomte, P., & Gauthier, P.-A. (2015). Real-Time 3D Ambisonics using Faust, Processing, Pure Data, And OSC. In 15th International Conference on Digital Audio Effects (DAFx-15). Trondheim, Norway.

nf=vgroup("NFC",checkbox("NEAR FIELD COMPENSATION"));
// Spherical restitution speaker layout radius r2 is needeed to stabilize near-field filters, see [1]
r2 = nentry("Speakers Radius", 1.07, 0.5, 10, 0.01);

vmeter(x)		= attach(x, envelop(x) : vbargraph("[unit:dB]", -70, 6));
// vmeter2 produces an osc alias and send the value of the bargraph on this alias when -xmit 2 is used at execution time
vmeter2(x,i) 		= x<:attach(x, envelop(x) : vbargraph("[unit:dB][osc:/output%i]", -70, 6));
envelop			= abs : max(db2linear(-70)) : linear2db : min(6)  : max ~ -(80.0/SR);

// Volume controler : CAUTION with maximal value (60 dB!) it's to compensate the attenuation of the microphone radial filters.
smooth(c)       = *(1-c) : +~*(c);
vol             = hslider("Gain[style:knob][unit:dB]", 0, -10, 60, 0.1) : db2linear : smooth(0.999);

id(x,delta) =  vgroup("%2a",vmeter2(_,a)) with{
a = x+1+delta;};

idmute(x,delta) =  vgroup("%2a",_*(1-checkbox("Mute")):vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",_*(1-checkbox("Mute")):idmute(i,m*m-1)));

matrix(n,m) = vgroup("B-Format",bus(16):hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)))<:par(i,m,buswg(row(i)):>_);

// When near-field compensation is activated, multiplication by 4*PI*r2 to have the correct gain, see [2]
selecteur=vgroup("Parameters",bus(16)<:((par(i,16,*(nf*vol*4*PI*r2)):(_,par(i,3,nfc1(r2)),par(i,5,nfc2(r2)),par(i,7,nfc3(r2)))),par(i,16,*((1-nf)*vol))):>bus(16));

process=hgroup("",selecteur:matrix(16,26)):(hgroup("Outputs 1-26",par(i,13,id(i,0))),hgroup("Outputs 26-50",par(i,13,id(i,13))));

//Analytic decoder matrix Wlebedev.YLebedev [1]
//Vector of weighted spherical harmonics : spherical harmonics times the speaker weight for weighet quadrature rules [1]
wy3(t,d,g)=(y0(t,d)*g,y1(t,d)*g,y2(t,d)*g,y3(t,d)*g,y4(t,d)*g,y5(t,d)*g,y6(t,d)*g,y7(t,d)*g,y8(t,d)*g,y9(t,d)*g,y10(t,d)*g,y11(t,d)*g,y12(t,d)*g,y13(t,d)*g,y14(t,d)*g,y15(t,d)*g);

row(i)=wy3(azimuth(i),elevation(i),weight5(i));
