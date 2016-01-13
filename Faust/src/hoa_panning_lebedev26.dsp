declare name        "NFC-HOA with 26 nodes Lebedev grid up to order 3";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("lib/lebedev.lib");
import("lib/ymn.lib");
import("lib/nfc.lib");
import("music.lib");

//HOA equivalent panning law for Lebedev grid with 26 speakers, up to order 3 [1]
//Lecomte, P., Gauthier, P.-A., Langrenne, C., Garcia, A., & Berry, A. (2015). On the use of a Lebedev grid for Ambisonics. In Audio Engineering Society Convention 139. New York.

smooth(c)	= *(1-c) : +~*(c);
gain01		= vgroup("[1]Gains",vslider("Source 1 Gain", 0, -20, +20, 0.1) : db2linear : smooth(0.999)*(1-checkbox("Mute 1")));
gain02		= vgroup("[1]Gains",vslider("Source 2 Gain", 0, -20, +20, 0.1) : db2linear : smooth(0.999)*(1-checkbox("Mute 2")));

mute = par(i,4,_*vgroup("[2]Mute Order",1-checkbox("%i")));

rhp = nentry("[9]Speaker Radius", 1, 0.5, 10, 0.01);
r1 = vslider("[3]Source 1 Radius", 2, 0.5, 10, 0.01);
r2 = vslider("[6]Source 2 Radius", 2, 0.5, 10, 0.01);

speakers = 26;

// vmeter2 produces an osc alias and send the value of the bargraph on this alias when -xmit 2 is used at execution time
vmeter2(x,i) 		= x<:attach(x, envelop(x) : vbargraph("[unit:dB][osc:/output%i]", -70, 6));
envelop			= abs : max(db2linear(-70)) : linear2db : min(6)  : max ~ -(80.0/SR);

id(x,delta) =  vgroup("%2a",vmeter2(_,a)) with{
a = x+1+delta;};

theta1=vslider("[4]Source 1 Azimuth", 0, 0, 360, 0.1)*PI/180;
delta1=vslider("[5]Source 1 Elevation", 0, -90, 90, 0.1)*PI/180;

theta2=vslider("[7]Source 2 Azimuth", 0, 0, 360, 0.1)*PI/180;
delta2=vslider("[8]Source 2 Elevation", 0, -90, 90, 0.1)*PI/180;

source1 = _*(rhp*gain01/r1)<:par(i,speakers,_*weight3(i)<:mute:(_,nf1(r1,rhp)*LegendreP1(angle(theta1,delta1,azimuth(i),elevation(i))),nf2(r1,rhp)*LegendreP2(angle(theta1,delta1,azimuth(i),elevation(i))),nf3(r1,rhp)*LegendreP3(angle(theta1,delta1,azimuth(i),elevation(i)))):>_);

source2 = _*(rhp*gain02/r2)<:par(i,speakers,_*weight3(i)<:mute:(_,nf1(r2,rhp)*LegendreP1(angle(theta2,delta2,azimuth(i),elevation(i))),nf2(r2,rhp)*LegendreP2(angle(theta2,delta2,azimuth(i),elevation(i))),nf3(r2,rhp)*LegendreP3(angle(theta2,delta2,azimuth(i),elevation(i)))):>_);

process = _,_:hgroup("Parameters",(source1,source2)):>par(i,speakers,_):par(i,speakers,hgroup("Outputs",id(i,0)));
