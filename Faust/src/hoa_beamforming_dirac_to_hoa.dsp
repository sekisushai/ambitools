declare name        "Beamformin Dirac on HOA";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

import("lib/lebedev.lib");
import("lib/ymn.lib");
import("lib/nfc.lib");

//HOA soundscape spatially filtered by a Beampattern.

vmeter(x)		= 	attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop			= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",_:id(i,m*m-1)));

//Beampattern steering direction
theta0=vslider("[5]Azimuth", 0, 0, 360, 0.1)*PI/180;
delta0=vslider("[6]Elevation", 0, -90, 90, 0.1)*PI/180;

//Beamparten gain
smooth(c)       = *(1-c) : +~*(c);
vol             = vslider("[4]Gain[unit:dB]", 0, -20, 20, 0.1) : db2linear : smooth(0.999);

//Mute order
mute=rint(vslider("[7]Order",5,0,5,1));

//Bypass with crossfader
trig=vgroup("[2]On/Off",checkbox("On"));
duree=nentry("[3]Crossfade[unit:s]",1,0.1,10,0.1);
compteurhaut1=countup(duree*SR,1-trig)/(duree*SR);
compteurbas1=countdown(duree*SR,trig)/(duree*SR);
compteurhaut2=countup(duree*SR,trig)/(duree*SR);
compteurbas2=countdown(duree*SR,1-trig)/(duree*SR);

// ALTERNATIVE VERSION to duplicate first component (pressure), i.e. for demo without ambisonics decoding playback
// updownsig1=par(i,36,_<:select2(trig,*(vol*compteurbas1),*(vol*compteurhaut1))):ytot(theta0,delta0):>_<:ytot(theta0,delta0):((_<:(_,_)),bus(35));
// updownsig2=par(i,36,_<:select2(trig,*(compteurhaut2),*(compteurbas2))):((_<:(_,_)),bus(35));
// selecteur=hgroup("Parameters",vgroup("[1]Inputs",bus(36):(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))))<:((updownsig2),(updownsig1)):>bus(37));
// process=selecteur:(_,vgroup("Outputs",(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5)))));

updownsig1=par(i,36,_<:select2(trig,*(vol*compteurbas1),*(vol*compteurhaut1))):ytot(theta0,delta0):>_*(1/(4*PI))<:ytot(theta0,delta0):bus(36);
updownsig2=par(i,36,_<:select2(trig,*(compteurhaut2),*(compteurbas2))):bus(36);
selecteur=hgroup("Parameters",vgroup("[1]Inputs",bus(36):(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))))<:((updownsig2),(updownsig1)):>bus(36));
process=selecteur:vgroup("Outputs",(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))));

//BACKUP CROSSFADER 1 WAY trig=(1-checkbox("On"));
//BACKUP CROSSFADER 1 WAY compteurhaut=countup(SR,trig)/SR;
//BACKUP CROSSFADER 1 WAY compteurbas=1-compteurhaut;


matrix(n,m) =par(i,m,buswg(row(i)):>_);

//BACKUP CROSSFADER 1 WAY selecteur=hgroup("Parameters",vgroup("Inputs",bus(36):(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))))<:(par(i,36,*(compteurbas)),(par(i,36,*(vol*(compteurhaut)))<:matrix(36,36))):>bus(36));

//BACKUP NO CROSSFADER on=checkbox("On");
//BACKUP NO CROSSFADER selecteur=hgroup("Parameters",vgroup("Inputs",bus(36):(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))))<:(par(i,36,*(1-on)),(par(i,36,*(vol*on))<:matrix(36,36))):>bus(36));

