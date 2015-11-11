declare name        "HOA-Beamformer";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("math.lib");
import("../lib/ymn.lib");
import("music.lib");

// Meyer, J., & Elko, G. (2002). A highly scalable spherical microphone array based on an orthonormal decomposition of the soundfield. 2002 IEEE International 
// Conference on Acoustics, Speech, and Signal Processing, 2, II–1781–II–1784. doi:10.1109/ICASSP.2002.5744968

smooth(c)       = *(1-c) : +~*(c);
vol             = hslider("Volume Amplifier (dB)", 0, -10, 60, 0.1) : db2linear : smooth(0.999);

t=hslider("theta", 0, 0, 360, 0.1)*PI/180;
d=hslider("delta", 0, -90, 90, 0.1)*PI/180;
on=checkbox("On");

C0=vol*0.2216*(1)^0.5*on;
C1=vol*0.3837*(1/3)^0.5*on;
C2=vol*0.4954*(1/5)^0.5*on;
C3=vol*0.5862*(1/7)^0.5*on;

process=par(i,16,_)<:(par(i,16,(1-on)*_),
(
y001(t,d)*C0,
y111(t,d)*C1,y11m1(t,d)*C1,y101(t,d)*C1,
y221(t,d)*C2,y22m1(t,d)*C2,y211(t,d)*C2,y21m1(t,d)*C2,y201(t,d)*C2,
y331(t,d)*C3,y33m1(t,d)*C3,y321(t,d)*C3,y32m1(t,d)*C2,y311(t,d)*C3,y31m1(t,d)*C3,y301(t,d)*C3
)):>par(i,16,_);