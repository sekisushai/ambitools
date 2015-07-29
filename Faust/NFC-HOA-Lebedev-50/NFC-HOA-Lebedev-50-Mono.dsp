declare name        "NFC-HOA up to order 5 on a 50 nodes Lebedev (Mono Version)";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("../lib/lebedev.lib");
import("../lib/ymn.lib");
import("../lib/nfc.lib");


smooth(c)	= *(1-c) : +~*(c);
gain01		= vslider("Source 1 Gain", 0, -20, +20, 0.1) : db2linear : smooth(0.999);

rhp = nentry("Speaker Radius", 1, 0.5, 10, 0.01);
r1 = vslider("Source 1 Radius", 2, 0.5, 50, 0.01);
speakers = 50;

mute = par(i,6,_*vgroup("Mute Order",1-checkbox("%i")));

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

vmeter(x)		= attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop			= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);

theta1=vslider("Source 1 Theta", 0, 0, 360, 0.1)*PI/180;
delta1=vslider("Source 1 Delta", 0, -90, 90, 0.1)*PI/180;

source1 = _*(rhp*gain01/r1)<:par(i,speakers,_*weight5(i)<:mute:(_,ORDRE1(r1,rhp)*LegendreP1(angle(theta1,delta1,azimuth(i),elevation(i))),ORDRE2(r1,rhp)*LegendreP2(angle(theta1,delta1,azimuth(i),elevation(i))),ORDRE3(r1,rhp)*LegendreP3(angle(theta1,delta1,azimuth(i),elevation(i))),ORDRE4(r1,rhp)*LegendreP4(angle(theta1,delta1,azimuth(i),elevation(i))),ORDRE5(r1,rhp)*LegendreP5(angle(theta1,delta1,azimuth(i),elevation(i)))):>_);

process = _<:hgroup("Parameters",source1):>par(i,speakers,_):vgroup("Outputs",par(i,speakers/2,hgroup("1-25",id(i,0))),par(i,speakers/2,hgroup("26-50",id(i,speakers/2))));

