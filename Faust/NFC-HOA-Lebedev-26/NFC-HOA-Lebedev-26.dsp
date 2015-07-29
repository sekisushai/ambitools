declare name        "NFC-HOA with 26 nodes Lebedev grid up to order 3";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("../lib/lebedev.lib");
import("../lib/ymn.lib");
import("../lib/nfc.lib");

import("music.lib");

smooth(c)	= *(1-c) : +~*(c);
gain01		= vgroup("Gains",vslider("Source 1 Gain", 0, -20, +20, 0.1) : db2linear : smooth(0.999)*(1-checkbox("Mute 1")));
gain02		= vgroup("Gains",vslider("Source 2 Gain", 0, -20, +20, 0.1) : db2linear : smooth(0.999)*(1-checkbox("Mute 2")));

rhp = nentry("Speaker Radius", 1, 0.5, 10, 0.01);
r1 = vslider("Source 1 Radius", 2, 0.5, 10, 0.01);
r2 = vslider("Source 2 Radius", 2, 0.5, 10, 0.01);

speakers = 26;

mute = par(i,4,_*vgroup("Mute Order",1-checkbox("%i")));

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

vmeter(x)		= attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop			= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);

theta1=vslider("Source 1 Theta", 0, 0, 360, 0.1)*PI/180;
delta1=vslider("Source 1 Delta", 0, -90, 90, 0.1)*PI/180;

theta2=vslider("Source 2 Theta", 0, 0, 360, 0.1)*PI/180;
delta2=vslider("Source 2 Delta", 0, -90, 90, 0.1)*PI/180;

source1 = _*(rhp*gain01/r1)<:par(i,speakers,_*weight3(i)<:mute:(_,ORDRE1(r1,rhp)*LegendreP1(angle(theta1,delta1,azimuth(i),elevation(i))),ORDRE2(r1,rhp)*LegendreP2(angle(theta1,delta1,azimuth(i),elevation(i))),ORDRE3(r1,rhp)*LegendreP3(angle(theta1,delta1,azimuth(i),elevation(i)))):>_);

source2 = _*(rhp*gain02/r2)<:par(i,speakers,_*weight3(i)<:mute:(_,ORDRE1(r2,rhp)*LegendreP1(angle(theta2,delta2,azimuth(i),elevation(i))),ORDRE2(r2,rhp)*LegendreP2(angle(theta2,delta2,azimuth(i),elevation(i))),ORDRE3(r2,rhp)*LegendreP3(angle(theta2,delta2,azimuth(i),elevation(i)))):>_);

process = _,_:hgroup("Parameters",(source1,source2)):>par(i,speakers,_):par(i,speakers,hgroup("Outputs",id(i,0)));
//process = _;

