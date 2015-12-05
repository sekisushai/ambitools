declare name        "MemsBedev HOA Encoder";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("math.lib");
import("music.lib");
import("lib/lebedev.lib");
import("lib/ymn.lib");

// bus with gains
gain(c) = R(c) with {
  R((c,cl)) = R(c),R(cl);
  R(1)      = _;
  R(0)      = !;
 //R(0)      = !:0; // if you need to preserve the number of outputs
  R(float(0)) = R(0);
  R(float(1)) = R(1)*vol;
  R(c)      = *(c)*vol;
};

smooth(c)       = *(1-c) : +~*(c);
vol             = hslider("Gain[unit:dB]", 0, -10, 50, 0.1) : db2linear : smooth(0.999);
vmeter(x)		= attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop			= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);


// n = number of inputs
// m = number of output
//matrix(n,m) = par(i,n,_) <: par(i,m,gain(a(i)):>_);

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",id(i,m*m-1)));

process= vgroup("",hgroup("Inputs 1-25",par(i,25,id(i,0))),hgroup("Inputs 26-50",par(i,25,id(i,25))))<:par(i,36,buswg(row(i)):>_):par(i,36,*(vol)):vgroup("B-Format",bus(36):(hgroup("0-3",meterm(0),meterm(1),meterm(2),meterm(3)),hgroup("4-5",meterm(4),meterm(5))));

row(0)=par(i,50,y0(azimuth(i),elevation(i))*(weight5(i)));
row(1)=par(i,50,y1(azimuth(i),elevation(i))*(weight5(i)));
row(2)=par(i,50,y2(azimuth(i),elevation(i))*(weight5(i)));
row(3)=par(i,50,y3(azimuth(i),elevation(i))*(weight5(i)));
row(4)=par(i,50,y4(azimuth(i),elevation(i))*(weight5(i)));
row(5)=par(i,50,y5(azimuth(i),elevation(i))*(weight5(i)));
row(6)=par(i,50,y6(azimuth(i),elevation(i))*(weight5(i)));
row(7)=par(i,50,y7(azimuth(i),elevation(i))*(weight5(i)));
row(8)=par(i,50,y8(azimuth(i),elevation(i))*(weight5(i)));
row(9)=par(i,50,y9(azimuth(i),elevation(i))*(weight5(i)));
row(10)=par(i,50,y10(azimuth(i),elevation(i))*(weight5(i)));
row(11)=par(i,50,y11(azimuth(i),elevation(i))*(weight5(i)));
row(12)=par(i,50,y12(azimuth(i),elevation(i))*(weight5(i)));
row(13)=par(i,50,y13(azimuth(i),elevation(i))*(weight5(i)));
row(14)=par(i,50,y14(azimuth(i),elevation(i))*(weight5(i)));
row(15)=par(i,50,y15(azimuth(i),elevation(i))*(weight5(i)));
row(16)=par(i,50,y16(azimuth(i),elevation(i))*(weight5(i)));
row(17)=par(i,50,y17(azimuth(i),elevation(i))*(weight5(i)));
row(18)=par(i,50,y18(azimuth(i),elevation(i))*(weight5(i)));
row(19)=par(i,50,y19(azimuth(i),elevation(i))*(weight5(i)));
row(20)=par(i,50,y20(azimuth(i),elevation(i))*(weight5(i)));
row(21)=par(i,50,y21(azimuth(i),elevation(i))*(weight5(i)));
row(22)=par(i,50,y22(azimuth(i),elevation(i))*(weight5(i)));
row(23)=par(i,50,y23(azimuth(i),elevation(i))*(weight5(i)));
row(24)=par(i,50,y24(azimuth(i),elevation(i))*(weight5(i)));
row(25)=par(i,50,y25(azimuth(i),elevation(i))*(weight5(i)));
row(26)=par(i,50,y26(azimuth(i),elevation(i))*(weight5(i)));
row(27)=par(i,50,y27(azimuth(i),elevation(i))*(weight5(i)));
row(28)=par(i,50,y28(azimuth(i),elevation(i))*(weight5(i)));
row(29)=par(i,50,y29(azimuth(i),elevation(i))*(weight5(i)));
row(30)=par(i,50,y30(azimuth(i),elevation(i))*(weight5(i)));
row(31)=par(i,50,y31(azimuth(i),elevation(i))*(weight5(i)));
row(32)=par(i,50,y32(azimuth(i),elevation(i))*(weight5(i)));
row(33)=par(i,50,y33(azimuth(i),elevation(i))*(weight5(i)));
row(34)=par(i,50,y34(azimuth(i),elevation(i))*(weight5(i)));
row(35)=par(i,50,y35(azimuth(i),elevation(i))*(weight5(i)));

//b(0)=(0.0126984,0.0126984,0.0126984,0.0126984,0.0126984,0.0126984,0.022575,0.022575,0.022575,0.022575,0.022575,0.022575,0.022575,0.022575,0.022575,0.022575,0.022575,0.022575,0.0210938,0.0210938,0.0210938,0.0210938,0.0210938,0.0210938,0.0210938,0.0210938,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733,0.0201733);
//b(1)=(0.,0.0219943,0.,-0.0219943,0.,0.,0.0276486,0.,-0.0276486,0.,0.0276486,-0.0276486,-0.0276486,0.0276486,0.0276486,0.,-0.0276486,0.,0.0210937,-0.0210937,-0.0210937,0.0210937,0.0210937,-0.0210937,-0.0210937,0.0210937,0.0105352,-0.0105352,-0.0105352,0.0105352,0.0316055,0.0105352,-0.0105352,-0.0316055,-0.0316055,-0.0105352,0.0105352,0.0316055,0.0316055,0.0105352,-0.0105352,-0.0316055,-0.0316055,-0.0105352,0.0105352,0.0316055,0.0105352,-0.0105352,-0.0105352,0.0105352);
//b(2)=(0.,0.,0.0219943,0.,-0.0219943,0.,0.,0.0276486,0.,-0.0276486,0.0276486,0.0276486,-0.0276486,-0.0276486,0.,0.0276486,0.,-0.0276486,0.0210937,0.0210937,-0.0210937,-0.0210937,0.0210937,0.0210937,-0.0210937,-0.0210937,0.0105352,0.0105352,-0.0105352,-0.0105352,0.0105352,0.0316055,0.0316055,0.0105352,-0.0105352,-0.0316055,-0.0316055,-0.0105352,0.0105352,0.0316055,0.0316055,0.0105352,-0.0105352,-0.0316055,-0.0316055,-0.0105352,0.0105352,0.0105352,-0.0105352,-0.0105352);
//b(3)=(0.0219943,0.,0.,0.,0.,-0.0219943,0.0276486,0.0276486,0.0276486,0.0276486,0.,0.,0.,0.,-0.0276486,-0.0276486,-0.0276486,-0.0276486,0.0210938,0.0210938,0.0210938,0.0210938,-0.0210938,-0.0210938,-0.0210938,-0.0210938,0.0316055,0.0316055,0.0316055,0.0316055,0.0105352,0.0105352,0.0105352,0.0105352,0.0105352,0.0105352,0.0105352,0.0105352,-0.0105352,-0.0105352,-0.0105352,-0.0105352,-0.0105352,-0.0105352,-0.0105352,-0.0105352,-0.0316055,-0.0316055,-0.0316055,-0.0316055);
//b(4)=(0.,0.0245904,-0.0245904,0.0245904,-0.0245904,0.,0.0218581,-0.0218581,0.0218581,-0.0218581,0.,0.,0.,0.,0.0218581,-0.0218581,0.0218581,-0.0218581,0.,0.,0.,0.,0.,0.,0.,0.,4.34922*10^-19,-1.30477*10^-18,2.17461*10^-18,-3.04445*10^-18,0.0284113,-0.0284113,-0.0284113,0.0284113,0.0284113,-0.0284113,-0.0284113,0.0284113,0.0284113,-0.0284113,-0.0284113,0.0284113,0.0284113,-0.0284113,-0.0284113,0.0284113,4.34922*10^-19,-1.30477*10^-18,2.17461*10^-18,-3.04445*10^-18);
//b(5)=(0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.0437162,-0.0437162,0.0437162,-0.0437162,0.,0.,0.,0.,0.0272319,-0.0272319,0.0272319,-0.0272319,0.0272319,-0.0272319,0.0272319,-0.0272319,0.00710282,-0.00710282,0.00710282,-0.00710282,0.0213085,0.0213085,-0.0213085,-0.0213085,0.0213085,0.0213085,-0.0213085,-0.0213085,0.0213085,0.0213085,-0.0213085,-0.0213085,0.0213085,0.0213085,-0.0213085,-0.0213085,0.00710282,-0.00710282,0.00710282,-0.00710282);
//b(6)=(0.,0.,0.,0.,0.,0.,0.0437162,0.,-0.0437162,0.,0.,0.,0.,0.,-0.0437162,0.,0.0437162,0.,0.0272319,-0.0272319,-0.0272319,0.0272319,-0.0272319,0.0272319,0.0272319,-0.0272319,0.0213085,-0.0213085,-0.0213085,0.0213085,0.0213085,0.00710282,-0.00710282,-0.0213085,-0.0213085,-0.00710282,0.00710282,0.0213085,-0.0213085,-0.00710282,0.00710282,0.0213085,0.0213085,0.00710282,-0.00710282,-0.0213085,-0.0213085,0.0213085,0.0213085,-0.0213085);
//b(7)=(0.,0.,0.,0.,0.,0.,0.,0.0437162,0.,-0.0437162,0.,0.,0.,0.,0.,-0.0437162,0.,0.0437162,0.0272319,0.0272319,-0.0272319,-0.0272319,-0.0272319,-0.0272319,0.0272319,0.0272319,0.0213085,0.0213085,-0.0213085,-0.0213085,0.00710282,0.0213085,0.0213085,0.00710282,-0.00710282,-0.0213085,-0.0213085,-0.00710282,-0.00710282,-0.0213085,-0.0213085,-0.00710282,0.00710282,0.0213085,0.0213085,0.00710282,-0.0213085,-0.0213085,0.0213085,0.0213085);
//b(8)=(0.0283945,-0.0141973,-0.0141973,-0.0141973,-0.0141973,0.0283945,0.0126198,0.0126198,0.0126198,0.0126198,-0.0252396,-0.0252396,-0.0252396,-0.0252396,0.0126198,0.0126198,0.0126198,0.0126198,0.,0.,0.,0.,0.,0.,0.,0.,0.0328065,0.0328065,0.0328065,0.0328065,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,-0.0164033,0.0328065,0.0328065,0.0328065,0.0328065);
//b(9)=(0.,0.0265606,0.,-0.0265606,0.,0.,0.0166944,0.,-0.0166944,0.,-0.0333888,0.0333888,0.0333888,-0.0333888,0.0166944,0.,-0.0166944,0.,-0.0169821,0.0169821,0.0169821,-0.0169821,-0.0169821,0.0169821,0.0169821,-0.0169821,-0.00231317,0.00231317,0.00231317,-0.00231317,0.0208185,-0.0300712,0.0300712,-0.0208185,-0.0208185,0.0300712,-0.0300712,0.0208185,0.0208185,-0.0300712,0.0300712,-0.0208185,-0.0208185,0.0300712,-0.0300712,0.0208185,-0.00231317,0.00231317,0.00231317,-0.00231317);
//b(10)=(0.,0.,-0.0265606,0.,0.0265606,0.,0.,-0.0166944,0.,0.0166944,0.0333888,0.0333888,-0.0333888,-0.0333888,0.,-0.0166944,0.,0.0166944,0.0169821,0.0169821,-0.0169821,-0.0169821,0.0169821,0.0169821,-0.0169821,-0.0169821,0.00231317,0.00231317,-0.00231317,-0.00231317,0.0300712,-0.0208185,-0.0208185,0.0300712,-0.0300712,0.0208185,0.0208185,-0.0300712,0.0300712,-0.0208185,-0.0208185,0.0300712,-0.0300712,0.0208185,0.0208185,-0.0300712,0.00231317,0.00231317,-0.00231317,-0.00231317);
//b(11)=(0.,0.,0.,0.,0.,0.,0.0408928,-0.0408928,0.0408928,-0.0408928,0.,0.,0.,0.,-0.0408928,0.0408928,-0.0408928,0.0408928,0.,0.,0.,0.,0.,0.,0.,0.,1.04084*10^-18,-3.12253*10^-18,5.20422*10^-18,-7.2859*10^-18,0.0226644,-0.0226644,-0.0226644,0.0226644,0.0226644,-0.0226644,-0.0226644,0.0226644,-0.0226644,0.0226644,0.0226644,-0.0226644,-0.0226644,0.0226644,0.0226644,-0.0226644,-1.04084*10^-18,3.12253*10^-18,-5.20422*10^-18,7.2859*10^-18);
//b(12)=(0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.0415974,-0.0415974,0.0415974,-0.0415974,-0.0415974,0.0415974,-0.0415974,0.0415974,0.0169983,-0.0169983,0.0169983,-0.0169983,0.0169983,0.0169983,-0.0169983,-0.0169983,0.0169983,0.0169983,-0.0169983,-0.0169983,-0.0169983,-0.0169983,0.0169983,0.0169983,-0.0169983,-0.0169983,0.0169983,0.0169983,-0.0169983,0.0169983,-0.0169983,0.0169983);
//b(13)=(0.,-0.0205738,0.,0.0205738,0.,0.,0.0387943,0.,-0.0387943,0.,-0.0258629,0.0258629,0.0258629,-0.0258629,0.0387943,0.,-0.0387943,0.,0.0131543,-0.0131543,-0.0131543,0.0131543,0.0131543,-0.0131543,-0.0131543,0.0131543,0.0304602,-0.0304602,-0.0304602,0.0304602,-0.016126,-0.00537532,0.00537532,0.016126,0.016126,0.00537532,-0.00537532,-0.016126,-0.016126,-0.00537532,0.00537532,0.016126,0.016126,0.00537532,-0.00537532,-0.016126,0.0304602,-0.0304602,-0.0304602,0.0304602);
//b(14)=(0.,0.,-0.0205738,0.,0.0205738,0.,0.,0.0387943,0.,-0.0387943,-0.0258629,-0.0258629,0.0258629,0.0258629,0.,0.0387943,0.,-0.0387943,0.0131543,0.0131543,-0.0131543,-0.0131543,0.0131543,0.0131543,-0.0131543,-0.0131543,0.0304602,0.0304602,-0.0304602,-0.0304602,-0.00537532,-0.016126,-0.016126,-0.00537532,0.00537532,0.016126,0.016126,0.00537532,-0.00537532,-0.016126,-0.016126,-0.00537532,0.00537532,0.016126,0.016126,0.00537532,0.0304602,0.0304602,-0.0304602,-0.0304602);
//b(15)=(0.0335968,0.,0.,0.,0.,-0.0335968,-0.0105585,-0.0105585,-0.0105585,-0.0105585,0.,0.,0.,0.,0.0105585,0.0105585,0.0105585,0.0105585,-0.0214808,-0.0214808,-0.0214808,-0.0214808,0.0214808,0.0214808,0.0214808,0.0214808,0.0263336,0.0263336,0.0263336,0.0263336,-0.0204817,-0.0204817,-0.0204817,-0.0204817,-0.0204817,-0.0204817,-0.0204817,-0.0204817,0.0204817,0.0204817,0.0204817,0.0204817,0.0204817,0.0204817,0.0204817,0.0204817,-0.0263336,-0.0263336,-0.0263336,-0.0263336);
//b(16)=(0.,0.0281718,0.0281718,0.0281718,0.0281718,0.,0.0125208,0.0125208,0.0125208,0.0125208,-0.0500832,-0.0500832,-0.0500832,-0.0500832,0.0125208,0.0125208,0.0125208,0.0125208,-0.0207987,-0.0207987,-0.0207987,-0.0207987,-0.0207987,-0.0207987,-0.0207987,-0.0207987,-0.00147951,-0.00147951,-0.00147951,-0.00147951,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,-0.00147951,-0.00147951,-0.00147951,-0.00147951);
//b(17)=(0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,1.81188*10^-19,5.43563*10^-19,9.05938*10^-19,1.26831*10^-18,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,1.81188*10^-19,5.43563*10^-19,9.05938*10^-19,1.26831*10^-18);
//b(18)=(0.,0.,0.,0.,0.,0.,0.0354142,0.,-0.0354142,0.,0.,0.,0.,0.,-0.0354142,0.,0.0354142,0.,-0.0294138,0.0294138,0.0294138,-0.0294138,0.0294138,-0.0294138,-0.0294138,0.0294138,-0.00627703,0.00627703,0.00627703,-0.00627703,0.0188311,-0.0272004,0.0272004,-0.0188311,-0.0188311,0.0272004,-0.0272004,0.0188311,-0.0188311,0.0272004,-0.0272004,0.0188311,0.0188311,-0.0272004,0.0272004,-0.0188311,0.00627703,-0.00627703,-0.00627703,0.00627703);
//b(19)=(0.,0.,0.,0.,0.,0.,0.,-0.0354142,0.,0.0354142,0.,0.,0.,0.,0.,0.0354142,0.,-0.0354142,0.0294138,0.0294138,-0.0294138,-0.0294138,-0.0294138,-0.0294138,0.0294138,0.0294138,0.00627703,0.00627703,-0.00627703,-0.00627703,0.0272004,-0.0188311,-0.0188311,0.0272004,-0.0272004,0.0188311,0.0188311,-0.0272004,-0.0272004,0.0188311,0.0188311,-0.0272004,0.0272004,-0.0188311,-0.0188311,0.0272004,-0.00627703,-0.00627703,0.00627703,0.00627703);
//b(20)=(0.,-0.0212959,0.0212959,-0.0212959,0.0212959,0.,0.0473242,-0.0473242,0.0473242,-0.0473242,0.,0.,0.,0.,0.0473242,-0.0473242,0.0473242,-0.0473242,0.,0.,0.,0.,0.,0.,0.,0.,1.78054*10^-18,-5.34163*10^-18,8.90272*10^-18,-1.24638*10^-17,-0.00894723,0.00894723,0.00894723,-0.00894723,-0.00894723,0.00894723,0.00894723,-0.00894723,-0.00894723,0.00894723,0.00894723,-0.00894723,-0.00894723,0.00894723,0.00894723,-0.00894723,1.78054*10^-18,-5.34163*10^-18,8.90272*10^-18,-1.24638*10^-17);
//b(21)=(0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,-0.0378594,0.0378594,-0.0378594,0.0378594,0.,0.,0.,0.,0.0314447,-0.0314447,0.0314447,-0.0314447,0.0314447,-0.0314447,0.0314447,-0.0314447,0.0290785,-0.0290785,0.0290785,-0.0290785,-0.00671042,-0.00671042,0.00671042,0.00671042,-0.00671042,-0.00671042,0.00671042,0.00671042,-0.00671042,-0.00671042,0.00671042,0.00671042,-0.00671042,-0.00671042,0.00671042,0.00671042,0.0290785,-0.0290785,0.0290785,-0.0290785);
//b(22)=(0.,0.,0.,0.,0.,0.,0.0133853,0.,-0.0133853,0.,0.,0.,0.,0.,-0.0133853,0.,0.0133853,0.,-0.0111174,0.0111174,0.0111174,-0.0111174,0.0111174,-0.0111174,-0.0111174,0.0111174,0.0355874,-0.0355874,-0.0355874,0.0355874,-0.0308424,-0.0102808,0.0102808,0.0308424,0.0308424,0.0102808,-0.0102808,-0.0308424,0.0308424,0.0102808,-0.0102808,-0.0308424,-0.0308424,-0.0102808,0.0102808,0.0308424,-0.0355874,0.0355874,0.0355874,-0.0355874);
//b(23)=(0.,0.,0.,0.,0.,0.,0.,0.0133853,0.,-0.0133853,0.,0.,0.,0.,0.,-0.0133853,0.,0.0133853,-0.0111174,-0.0111174,0.0111174,0.0111174,0.0111174,0.0111174,-0.0111174,-0.0111174,0.0355874,0.0355874,-0.0355874,-0.0355874,-0.0102808,-0.0308424,-0.0308424,-0.0102808,0.0102808,0.0308424,0.0308424,0.0102808,0.0102808,0.0308424,0.0308424,0.0102808,-0.0102808,-0.0308424,-0.0308424,-0.0102808,-0.0355874,-0.0355874,0.0355874,0.0355874);
//b(24)=(0.0380952,0.0142857,0.0142857,0.0142857,0.0142857,0.0380952,-0.0275132,-0.0275132,-0.0275132,-0.0275132,0.0253968,0.0253968,0.0253968,0.0253968,-0.0275132,-0.0275132,-0.0275132,-0.0275132,-0.0246094,-0.0246094,-0.0246094,-0.0246094,-0.0246094,-0.0246094,-0.0246094,-0.0246094,0.0142547,0.0142547,0.0142547,0.0142547,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.00425141,0.0142547,0.0142547,0.0142547,0.0142547);
//b(25)=(0.,0.0295468,0.,-0.0295468,0.,0.,0.00928568,0.,-0.00928568,0.,-0.0371427,0.0371427,0.0371427,-0.0371427,0.00928568,0.,-0.00928568,0.,-0.0125942,0.0125942,0.0125942,-0.0125942,-0.0125942,0.0125942,0.0125942,-0.0125942,-0.000467862,0.000467862,0.000467862,-0.000467862,-0.00140359,0.0369611,-0.0369611,0.00140359,0.00140359,-0.0369611,0.0369611,-0.00140359,-0.00140359,0.0369611,-0.0369611,0.00140359,0.00140359,-0.0369611,0.0369611,-0.00140359,-0.000467862,0.000467862,0.000467862,-0.000467862);
//b(26)=(0.,0.,0.0295468,0.,-0.0295468,0.,0.,0.00928568,0.,-0.00928568,-0.0371427,-0.0371427,0.0371427,0.0371427,0.,0.00928568,0.,-0.00928568,-0.0125942,-0.0125942,0.0125942,0.0125942,-0.0125942,-0.0125942,0.0125942,0.0125942,-0.000467862,-0.000467862,0.000467862,0.000467862,0.0369611,-0.00140359,-0.00140359,0.0369611,-0.0369611,0.00140359,0.00140359,-0.0369611,0.0369611,-0.00140359,-0.00140359,0.0369611,-0.0369611,0.00140359,0.00140359,-0.0369611,-0.000467862,-0.000467862,0.000467862,0.000467862);
//b(27)=(0.,0.,0.,0.,0.,0.,0.0293639,0.0293639,0.0293639,0.0293639,0.,0.,0.,0.,-0.0293639,-0.0293639,-0.0293639,-0.0293639,-0.0398265,-0.0398265,-0.0398265,-0.0398265,0.0398265,0.0398265,0.0398265,0.0398265,-0.00443853,-0.00443853,-0.00443853,-0.00443853,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,0.0103566,-0.0103566,-0.0103566,-0.0103566,-0.0103566,-0.0103566,-0.0103566,-0.0103566,-0.0103566,0.00443853,0.00443853,0.00443853,0.00443853);
//b(28)=(0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,5.43563*10^-19,1.63069*10^-18,2.71781*10^-18,3.80494*10^-18,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-0.0355082,0.0355082,-5.43563*10^-19,-1.63069*10^-18,-2.71781*10^-18,-3.80494*10^-18);
//b(29)=(0.,-0.0220229,0.,0.0220229,0.,0.,0.0484479,0.,-0.0484479,0.,0.0276845,-0.0276845,-0.0276845,0.0276845,0.0484479,0.,-0.0484479,0.,-0.0281616,0.0281616,0.0281616,-0.0281616,-0.0281616,0.0281616,0.0281616,-0.0281616,-0.0122053,0.0122053,0.0122053,-0.0122053,-0.00313851,0.00453341,-0.00453341,0.00313851,0.00313851,-0.00453341,0.00453341,-0.00313851,-0.00313851,0.00453341,-0.00453341,0.00313851,0.00313851,-0.00453341,0.00453341,-0.00313851,-0.0122053,0.0122053,0.0122053,-0.0122053);
//b(30)=(0.,0.,0.0220229,0.,-0.0220229,0.,0.,-0.0484479,0.,0.0484479,-0.0276845,-0.0276845,0.0276845,0.0276845,0.,-0.0484479,0.,0.0484479,0.0281616,0.0281616,-0.0281616,-0.0281616,0.0281616,0.0281616,-0.0281616,-0.0281616,0.0122053,0.0122053,-0.0122053,-0.0122053,-0.00453341,0.00313851,0.00313851,-0.00453341,0.00453341,-0.00313851,-0.00313851,0.00453341,-0.00453341,0.00313851,0.00313851,-0.00453341,0.00453341,-0.00313851,-0.00313851,0.00453341,0.0122053,0.0122053,-0.0122053,-0.0122053);
//b(31)=(0.,0.,0.,0.,0.,0.,0.0339065,-0.0339065,0.0339065,-0.0339065,0.,0.,0.,0.,-0.0339065,0.0339065,-0.0339065,0.0339065,0.,0.,0.,0.,0.,0.,0.,0.,2.51061*10^-18,-7.53183*10^-18,1.2553*10^-17,-1.75743*10^-17,-0.0273342,0.0273342,0.0273342,-0.0273342,-0.0273342,0.0273342,0.0273342,-0.0273342,0.0273342,-0.0273342,-0.0273342,0.0273342,0.0273342,-0.0273342,-0.0273342,0.0273342,-2.51061*10^-18,7.53183*10^-18,-1.2553*10^-17,1.75743*10^-17);
//b(32)=(0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.0410014,-0.0410014,0.0410014,-0.0410014,-0.0205007,-0.0205007,0.0205007,0.0205007,-0.0205007,-0.0205007,0.0205007,0.0205007,0.0205007,0.0205007,-0.0205007,-0.0205007,0.0205007,0.0205007,-0.0205007,-0.0205007,-0.0410014,0.0410014,-0.0410014,0.0410014);
//b(33)=(0.,0.0203893,0.,-0.0203893,0.,0.,-0.0192232,0.,0.0192232,0.,0.0256309,-0.0256309,-0.0256309,0.0256309,-0.0192232,0.,0.0192232,0.,-0.0260726,0.0260726,0.0260726,-0.0260726,-0.0260726,0.0260726,0.0260726,-0.0260726,0.0351912,-0.0351912,-0.0351912,0.0351912,-0.0029057,-0.000968566,0.000968566,0.0029057,0.0029057,0.000968566,-0.000968566,-0.0029057,-0.0029057,-0.000968566,0.000968566,0.0029057,0.0029057,0.000968566,-0.000968566,-0.0029057,0.0351912,-0.0351912,-0.0351912,0.0351912);
//b(34)=(0.,0.,0.0203893,0.,-0.0203893,0.,0.,-0.0192232,0.,0.0192232,0.0256309,0.0256309,-0.0256309,-0.0256309,0.,-0.0192232,0.,0.0192232,-0.0260726,-0.0260726,0.0260726,0.0260726,-0.0260726,-0.0260726,0.0260726,0.0260726,0.0351912,0.0351912,-0.0351912,-0.0351912,-0.000968566,-0.0029057,-0.0029057,-0.000968566,0.000968566,0.0029057,0.0029057,0.000968566,-0.000968566,-0.0029057,-0.0029057,-0.000968566,0.000968566,0.0029057,0.0029057,0.000968566,0.0351912,0.0351912,-0.0351912,-0.0351912);
//b(35)=(0.0421159,0.,0.,0.,0.,-0.0421159,-0.0281259,-0.0281259,-0.0281259,-0.0281259,0.,0.,0.,0.,0.0281259,0.0281259,0.0281259,0.0281259,-0.00673191,-0.00673191,-0.00673191,-0.00673191,0.00673191,0.00673191,0.00673191,0.00673191,-0.000750248,-0.000750248,-0.000750248,-0.000750248,0.023091,0.023091,0.023091,0.023091,0.023091,0.023091,0.023091,0.023091,-0.023091,-0.023091,-0.023091,-0.023091,-0.023091,-0.023091,-0.023091,-0.023091,0.000750248,0.000750248,0.000750248,0.000750248);


