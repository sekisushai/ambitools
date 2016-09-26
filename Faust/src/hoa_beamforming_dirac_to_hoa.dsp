declare name        "Beamforming Dirac on HOA";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2016";

// Description: This tool helps to explore the HOA scene with a "flashlight". It applies a directionnal filter (Dirac function) to the HOA scene to just leave the sound from the chosen direction [1].

// References: 
// [1] P. Lecomte, P.-A. Gauthier, C. Langrenne, A. Berry, and A. Garcia, “Filtrage directionnel dans un scène sonore 3D par une utilisation conjointe de Beamforming et d’Ambisonie d’ordre elevés,” in CFA / VISHNO 2016, 2016, pp. 169–175.

// Inputs: (M+1)^2
// Outputs: (M1+1)^2

import("stdfaust.lib");
import("lib/lebedev.lib");
import("lib/ymn.lib");
import("lib/nfc.lib");
import("lib/gui.lib");

M	=	5; // Order of the input HOA scene.
M1	=	M; // Order of the ouptut HOA scene. For the moment, as to be equal to M because of the Bypass..

ins	=	(M+1)^2;
outs	=	ins;

// Steering angles
theta0	=	vslider("[5]Azimuth", 0, 0, 360, 0.1)*ma.PI/180;
delta0	=	vslider("[6]Elevation", 0, -90, 90, 0.1)*ma.PI/180;

// Output gain
vol	=	vslider("[4]Gain[unit:dB]", 0, -20, 20, 0.1) : ba.db2linear : si.smooth(0.999);

// Bypass with crossfader
trig	=	vgroup("[2]On/Off",checkbox("On"));
duree	=	nentry("[3]Crossfade[unit:s]",1,0.1,10,0.1);
compteurhaut1	=	ba.countup(duree*ma.SR,1-trig)/(duree*ma.SR);
compteurbas1	=	ba.countdown(duree*ma.SR,trig)/(duree*ma.SR);
compteurhaut2	=	ba.countup(duree*ma.SR,trig)/(duree*ma.SR);
compteurbas2	=	ba.countdown(duree*ma.SR,1-trig)/(duree*ma.SR);

// ALTERNATIVE VERSION to duplicate first component (pressure), i.e. for demo without ambisonics decoding playback
// updownsig1=par(i,36,_<:select2(trig,*(vol*compteurbas1),*(vol*compteurhaut1))):ytot(theta0,delta0):>_<:ytot(theta0,delta0):((_<:(_,_)),bus(35));
// updownsig2=par(i,36,_<:select2(trig,*(compteurhaut2),*(compteurbas2))):((_<:(_,_)),bus(35));
// selecteur=hgroup("Parameters",vgroup("[1]Inputs",bus(36):(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))))<:((updownsig2),(updownsig1)):>bus(37));
// process=selecteur:(_,vgroup("Outputs",(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5)))));

updownsig1	=	par(i,ins,_<:select2(trig,*(vol*compteurbas1),*(vol*compteurhaut1))):yvec(ins,theta0,delta0):>_*(1/(4*ma.PI))<:yvec(outs,theta0,delta0):si.bus(outs);
updownsig2	=	par(i,ins,_<:select2(trig,*(compteurhaut2),*(compteurbas2))):si.bus(outs);
selecteur	=	hgroup("Parameters",hgroup("[1]Inputs",par(i,M+1,meterm(i)))<:((updownsig2),(updownsig1)):>si.bus(outs));

matrix(n,m)	=	par(i,m,buswg(row(i)):>_);

process		=	selecteur:hgroup("Outputs",par(i,M1+1,meterm(i)));