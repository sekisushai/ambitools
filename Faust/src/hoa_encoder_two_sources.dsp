declare name        "NF-HOA encoder for two sources to order 5";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

// HOA encoder up to order 5 for spherical wave or plane wave
//[1] Lecomte, P., & Gauthier, P.-A. (2015). Real-Time 3D Ambisonics using Faust, Processing, Pure Data, And OSC. In 15th International Conference on Digital Audio Effects (DAFx-15). Trondheim, Norway.

import("lib/nfc.lib");
import("lib/ymn.lib");

vmeter(x)		= attach(x, envelop(x) : vbargraph("[unit:dB]",-70, 6));
envelop			= abs : max(db2linear(-70)) : linear2db : min(6)  : max ~ -(80.0/SR);

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",id(i,m*m-1)));

smooth(c)	= *(1-c) : +~*(c);
gain01		= vslider("[01][osc:/source1_gain -20 20]Source 1 Gain", 0, -20, 20, 0.1) : db2linear : smooth(0.999);
gain02		= vslider("[06][osc:/source2_gain -20 20]Source 2 Gain", 0, -20, 20, 0.1) : db2linear : smooth(0.999);

// Spherical restitution speaker layout radius r2 is needeed to stabilize near-field filters, see [1]
rhp = nentry("[11]Speaker Radius", 1, 0.5, 10, 0.01);
r1 = vslider("[03][osc:/source1_radius 0.5 50]Source 1 Radius", 2, 0.5, 50, 0.01);
r2 = vslider("[08][osc:/source2_radius 0.5 50]Source 2 Radius", 2, 0.5, 50, 0.01);

spherical1=vgroup("[02]Spherical Wave",checkbox("Yes"));
spherical2=vgroup("[07]Spherical Wave",checkbox("Yes"));

theta1=vslider("[04][osc:/source1_theta 0 360]Source 1 Azimuth", 0, 0, 360, 0.1)*PI/180;
delta1=vslider("[05][osc:/source1_delta -90 90]Source 1 Elevation", 0, -90, 90, 0.1)*PI/180;

theta2=vslider("[09][osc:/source2_theta 0 360]Source 2 Azimuth", 0, 0, 360, 0.1)*PI/180;
delta2=vslider("[10][osc:/source2_delta -90 90]Source 2 Elevation", 0, -90, 90, 0.1)*PI/180;

// Plane wave
plane(t,d)=_<:ytot(t,d);

// Spherical wave : near-field filters included
spherical(r,t,d)=_<:((nf0<:bus(1)),(nf1(r,rhp)<:bus(3)),(nf2(r,rhp)<:bus(5)),(nf3(r,rhp)<:bus(7)),(nf4(r,rhp)<:bus(9)),(nf5(r,rhp)<:bus(11))):ytot(t,d);

process=hgroup("Parameters",(_,_):(*(gain01),*(gain02))<:(*(spherical1),*(spherical2),*(1-spherical1),*(1-spherical2)):(spherical(r1,theta1,delta1),spherical(r2,theta2,delta2),plane(theta1,delta1),plane(theta2,delta2))):>bus(36):vgroup("B-Format",hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5)));