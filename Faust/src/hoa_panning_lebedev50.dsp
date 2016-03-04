declare name        "NFC-HOA with Lebedev grid up to order 5";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("lib/lebedev.lib");
import("lib/ymn.lib");
import("lib/nfc.lib");

//HOA equivalent panning law for Lebedev grid with 50 speakers, up to order 5 [1]
//Lecomte, P., Gauthier, P.-A., Langrenne, C., Garcia, A., & Berry, A. (2015). On the use of a Lebedev grid for Ambisonics. In Audio Engineering Society Convention 139. New York.

smooth(c)	= *(1-c) : +~*(c);
gain01		= vslider("[1][osc:/source1_gain]Source 1 Gain", 0, -20, 20, 0.1) : db2linear : smooth(0.999);
gain02		= vslider("[5][osc:/source2_gain]Source 2 Gain", 0, -20, 20, 0.1) : db2linear : smooth(0.999);


rhp = nentry("[9]Speaker Radius", 1, 0.5, 10, 0.01);
r1 = vslider("[2][osc:source1_radius]Source 1 Radius", 2, 0.5, 50, 0.01);
r2 = vslider("[6][osc:source2_radius]Source 2 Radius", 2, 0.5, 50, 0.01);

speakers = 50;

mute = par(i,6,_*vgroup("[0]Mute Order",1-checkbox("%i")));

// vmeter2 produces an osc alias and send the value of the bargraph on this alias when -xmit 2 is used at execution time
vmeter2(x,i) 		= x<:attach(x, envelop(x) : vbargraph("[unit:dB][osc:/output%i]", -70, 6));
envelop			= abs : max(db2linear(-70)) : linear2db : min(6)  : max ~ -(80.0/SR);

id(x,delta) =  vgroup("%2a",vmeter2(_,a)) with{
a = x+1+delta;};

theta1=vslider("[3][osc:source1_theta]Source 1 Theta", 0, 0, 360, 0.1)*PI/180;
delta1=vslider("[4][osc:source1_delta]Source 1 Delta", 0, -90, 90, 0.1)*PI/180;

theta2=vslider("[7][osc:source2_theta]Source 2 Theta", 0, 0, 360, 0.1)*PI/180;
delta2=vslider("[8][osc:source2_delat]Source 2 Delta", 0, -90, 90, 0.1)*PI/180;

source1 = _*(rhp*gain01/r1)<:par(i,speakers,_*weight5(i)<:mute:(_,nf1(r1,rhp)*LegendreP1(angle(theta1,delta1,azimuth(i),elevation(i))),nf2(r1,rhp)*LegendreP2(angle(theta1,delta1,azimuth(i),elevation(i))),nf3(r1,rhp)*LegendreP3(angle(theta1,delta1,azimuth(i),elevation(i))),nf4(r1,rhp)*LegendreP4(angle(theta1,delta1,azimuth(i),elevation(i))),nf5(r1,rhp)*LegendreP5(angle(theta1,delta1,azimuth(i),elevation(i)))):>_);

source2 = _*(rhp*gain02/r2)<:par(i,speakers,_*weight5(i)<:mute:(_,nf1(r2,rhp)*LegendreP1(angle(theta2,delta2,azimuth(i),elevation(i))),nf2(r2,rhp)*LegendreP2(angle(theta2,delta2,azimuth(i),elevation(i))),nf3(r2,rhp)*LegendreP3(angle(theta2,delta2,azimuth(i),elevation(i))),nf4(r2,rhp)*LegendreP4(angle(theta2,delta2,azimuth(i),elevation(i))),nf5(r2,rhp)*LegendreP5(angle(theta2,delta2,azimuth(i),elevation(i)))):>_);

process = _,_:hgroup("Parameters",(source1,source2)):>par(i,speakers,_):vgroup("Outputs",par(i,speakers/2,hgroup("1-25",id(i,0))),par(i,speakers/2,hgroup("26-50",id(i,speakers/2))));

