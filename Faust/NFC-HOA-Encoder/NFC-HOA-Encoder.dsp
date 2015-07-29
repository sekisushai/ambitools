declare name        "NFC-HOA Encoder up to order 5";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";


import("../lib/nfc.lib");
import("../lib/ymn.lib");

// Spherical restitution speaker layout radius r2 is needeed to stabilize near-field filters
r2 = nentry("Rhp", 1.07, 0.5, 10, 0.01);
r1 = vslider("Source 1 Radius", 2, 0.5, 10, 0.01);

vmeter(x)		= attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop			= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",id(i,0)));

theta=vslider("Source 1 Theta", 0, 0, 360, 0.1)*PI/180;
delta=vslider("Source 1 Delta", 0, -90, 90, 0.1)*PI/180;

// This gain is introduced here to have the correct gain of the virtual source, defined at its position, not at origin
gain=r2/r1;

process = _<:hgroup("Parameters",(ORDRE0*gain<:par(i,1,_)),(ORDRE1(r1,r2)*gain<:par(i,3,_)),(ORDRE2(r1,r2)*gain<:par(i,5,_)),(ORDRE3(r1,r2)*gain<:par(i,7,_)),(ORDRE4(r1,r2)*gain<:par(i,9,_)),(ORDRE5(r1,r2)*gain<:par(i,11,_)):y001(theta,delta),
y111(theta,delta),y11m1(theta,delta),y101(theta,delta),
y221(theta,delta),y22m1(theta,delta),y211(theta,delta),y21m1(theta,delta),y201(theta,delta),
y331(theta,delta),y33m1(theta,delta),y321(theta,delta),y32m1(theta,delta),y311(theta,delta),y31m1(theta,delta),y301(theta,delta),
y441(theta,delta),y44m1(theta,delta),y431(theta,delta),y43m1(theta,delta),y421(theta,delta),y42m1(theta,delta),y411(theta,delta),y41m1(theta,delta),y401(theta,delta),
y551(theta,delta),y55m1(theta,delta),y541(theta,delta),y54m1(theta,delta),y531(theta,delta),y53m1(theta,delta),y521(theta,delta),y52m1(theta,delta),y511(theta,delta),y51m1(theta,delta),y501(theta,delta)):vgroup("B-Format",hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5)));
