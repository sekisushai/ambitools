declare name        "HOA-Beamformer to Mono";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

import("math.lib");
import("lib/ymn.lib");
import("music.lib");

// Beamforming on HOA signals : 36 inputs corresponding to 5th order 3D HOA scene are combined to produce a monophonic output as if the capture was done with a directionnal microphone. According to [1]
// [1] Meyer, J., & Elko, G. (2002). A highly scalable spherical microphone array based on an orthonormal decomposition of the soundfield. 2002 IEEE International 
// Conference on Acoustics, Speech, and Signal Processing, 2, II–1781–II–1784. doi:10.1109/ICASSP.2002.5744968

smooth(c)    = *(1-c) : +~*(c);
vmeter(x)	= attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop	= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);
vol                 = vslider("[1]Gain", 0, -10, 50, 0.1) : db2linear : smooth(0.999);

t=vslider("[3]Azimuth", 0, 0, 360, 0.1)*PI/180;
d=vslider("[4]Elevation", 0, -90, 90, 0.1)*PI/180;
order=rint(vslider("[2]Order", 1, 1, 5, 1));

C0=(0.8862*(order==1)+0.3939*(order==2)+0.2216*(order==3)+0.333985*(order==4)+0.254469*(order==5))*vol*(1)^0.5;
C1=(1.535*(order==1)+0.6822*(order==2)+0.3837*(order==3)+0.333985*(order==4)+0.254469*(order==5))*vol*(1/3)^0.5;
C2=(0*(order==1)+0.8807*(order==2)+0.4954*(order==3)+0.333985*(order==4)+0.254469*(order==5))*vol*(1/5)^0.5;
C3=(0*(order==1)+0*(order==2)+0.5862*(order==3)+0.333985*(order==4)+0.254469*(order==5))*vol*(1/7)^0.5;
C4=(0*(order==1)+0*(order==2)+0*(order==3)+0.333985*(order==4)+0.254469*(order==5))*vol*(1/9)^0.5;
C5=(0*(order==1)+0*(order==2)+0*(order==3)+0*(order==4)+0.254469*(order==5))*vol*(1/11)^0.5;

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",id(i,0)));

process=par(i,36,_)<:hgroup("Beampattern Parameters",
(*(y0(t,d)*C0),
*(y1(t,d)*C1),*(y2(t,d)*C1),*(y3(t,d)*C1),
*(y4(t,d)*C2),*(y5(t,d)*C2),*(y6(t,d)*C2),*(y7(t,d)*C2),*(y8(t,d)*C2),
*(y9(t,d)*C3),*(y10(t,d)*C3),*(y11(t,d)*C3),*(y12(t,d)*C2),*(y13(t,d)*C3),*(y14(t,d)*C3),*(y15(t,d)*C3),
*(y16(t,d)*C4),*(y17(t,d)*C4),*(y18(t,d)*C4),*(y19(t,d)*C4),*(y20(t,d)*C4),*(y21(t,d)*C4),*(y22(t,d)*C4),*(y23(t,d)*C4),*(y24(t,d)*C4),
*(y25(t,d)*C5),*(y26(t,d)*C5),*(y27(t,d)*C5),*(y28(t,d)*C5),*(y29(t,d)*C5),*(y30(t,d)*C5),*(y31(t,d)*C5),*(y32(t,d)*C5),*(y33(t,d)*C5),*(y34(t,d)*C5),*(y35(t,d)*C5)
)):vgroup("B-Format",hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))):>_<:(_,_);
