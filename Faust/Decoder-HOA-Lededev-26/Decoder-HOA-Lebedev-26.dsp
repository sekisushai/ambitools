declare name        "HOA Decoder up to order 3 for 26 nodes Lebedev grid";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("math.lib");
import("music.lib");

// bus with gains
gain(c) = R(c) with {
  R((c,cl)) = R(c),R(cl);
  R(1)      = _;
  R(0)      = !;
  //R(0)      = !:0; // if you need to preserve the number of outputs
 R(float(0)) = R(0);
  R(float(1)) = R(1);
  R(c)      = *(c);
};


vmeter(x)		= attach(x, envelop(x) : vbargraph("[unit:dB]", -100, 10));
envelop			= abs : max(db2linear(-100)) : linear2db : min(10)  : max ~ -(80.0/SR);

id(x,delta) =  vgroup("%2a",vmeter) with{
a = x+1+delta;};

idmute(x,delta) =  vgroup("%2a",_*(1-checkbox("Mute")):vmeter) with{
a = x+1+delta;};

meterm(m) = par(i,2*m+1,hgroup("%m",_*(1-checkbox("Mute")):idmute(i,0)));

matrix(n,m) = hgroup("B-Format",meterm(0),meterm(1),meterm(2),meterm(3))<:par(i,m,gain(a(i)):>hgroup("Outputs",id(i,0)));

//matrix(n,m) = hgroup("Inputs",par(i,n,vgroup("%2i", vmeter*(1-checkbox("Mute")))))<:par(i,m,gain(a(i)):>hgroup("Outputs",id(i,0)));

a(0)=(0.04761904761904762, 0, 0, 0.08247860988423225, 0, 0, 0, 0, 0.10647942749999, 0, 0, 0, 0, 0, 0, 0.12598815766974242);

a(1)=(0.04761904761904762, 0.08247860988423225, 0, 0,0.0922138891954147, 0, 0, 0, -0.053239713749995, 0.09960238411119948, 0, 0, 0, -0.07715167498104597, 0, 0);

a(2)=(0.04761904761904762, 0, 0.08247860988423225, 0, -0.0922138891954147, 0, 0, 0, -0.053239713749995, 0, -0.09960238411119948, 0, 0, 0, -0.07715167498104597, 0); 

a(3)=(0.04761904761904762, -0.08247860988423225, 0, 0, 0.0922138891954147, 0, 0, 0, -0.053239713749995, -0.09960238411119948, 0, 0, 0, 0.07715167498104597, 0, 0);

a(4)=(0.04761904761904762, 0, -0.08247860988423225, 0, -0.0922138891954147, 0, 0, 0, -0.053239713749995, 0, 0.09960238411119948, 0, 0, 0, 0.07715167498104597, 0);

a(5)=(0.04761904761904762, 0, 0, -0.08247860988423225, 0, 0, 0, 0, 0.10647942749999, 0, 0, 0, 0, 0, 0, -0.12598815766974242); 

a(6)=(0.0380952380952381,0.04665694748158436, 0, 0.046656947481584346, 0.036885555678165885, 0, 0.07377111135633177, 0, 0.021295885499998, 0.028171808490950564, 0, 0.06900655593423544, 0, 0.06546536707079773, 0, 
-0.01781741612749496); 

a(7)=(0.0380952380952381, 0, 0.04665694748158436, 0.046656947481584346, -0.036885555678165885, 0, 0, 0.07377111135633177, 0.021295885499998, 0, -0.028171808490950564,-0.06900655593423544, 0, 0, 0.06546536707079773, -0.01781741612749496); 

a(8)=(0.0380952380952381, -0.04665694748158436, 0, 0.046656947481584346, 0.036885555678165885, 0, -0.07377111135633177, 0,0.021295885499998, -0.028171808490950564, 0, 0.06900655593423544, 0, -0.06546536707079773, 0, -0.01781741612749496); 

a(9)=(0.0380952380952381, 0, -0.04665694748158436, 0.046656947481584346,-0.036885555678165885, 0, 0, -0.07377111135633177, 0.021295885499998, 0, 0.028171808490950564, -0.06900655593423544, 0, 0, -0.06546536707079773, -0.01781741612749496); 

a(10)=(0.0380952380952381,0.046656947481584346, 0.046656947481584346, 0, 0, 0.07377111135633176, 0, 0, -0.042591770999996, -0.05634361698190111, 0.05634361698190111, 0, 0, -0.04364357804719848, -0.04364357804719848, 
0); 

a(11)=(0.0380952380952381, -0.046656947481584346, 0.046656947481584346, 0, 0, -0.07377111135633176, 0, 0, -0.042591770999996, 0.05634361698190111, 0.05634361698190111, 0, 0,0.04364357804719848, -0.04364357804719848, 0); 

a(12)=(0.0380952380952381, -0.046656947481584346, -0.046656947481584346, 0, 0, 0.07377111135633176, 0, 0, -0.042591770999996, 0.05634361698190111, -0.05634361698190111, 0, 0, 0.04364357804719848, 0.04364357804719848, 0); 

a(13)=(0.0380952380952381, 0.046656947481584346, -0.046656947481584346, 0, 0, -0.07377111135633176, 0, 0, -0.042591770999996, -0.05634361698190111, -0.05634361698190111, 0, 0, -0.04364357804719848, 0.04364357804719848, 0); 

a(14)=(0.0380952380952381, 0.04665694748158435, 0, -0.046656947481584346, 0.036885555678165885, 0, -0.07377111135633177, 0, 0.021295885499998, 0.028171808490950564, 0, -0.06900655593423544, 0, 0.06546536707079773, 0, 0.01781741612749496); 

a(15)=(0.0380952380952381, 0, 0.04665694748158435, -0.046656947481584346, -0.036885555678165885, 0, 0, -0.07377111135633177, 0.021295885499998, 0, -0.028171808490950564, 0.06900655593423544, 0, 0, 0.06546536707079773, 
0.01781741612749496); 

a(16)=(0.0380952380952381, -0.04665694748158435, 0, -0.046656947481584346, 0.036885555678165885, 0, 0.07377111135633177, 0, 0.021295885499998, -0.028171808490950564, 0, -0.06900655593423544, 0, -0.06546536707079773, 0, 0.01781741612749496); 

a(17)=(0.0380952380952381, 0, -0.04665694748158435, -0.046656947481584346, -0.036885555678165885, 0, 0, 0.07377111135633177, 0.021295885499998, 0, 0.028171808490950564, 0.06900655593423544, 0, 0, -0.06546536707079773, 0.01781741612749496); 

a(18)=(0.03214285714285714, 0.03214285714285713, 0.03214285714285713, 0.03214285714285714, 0, 0.0414962501379366, 0.041496250137936605, 0.041496250137936605, 0, -0.02587745847533827, 0.02587745847533827, 0, 0.06338656910463872, 0.020044593143431825, 0.020044593143431825, -0.032732683535398856); 

a(19)=(0.03214285714285714, -0.03214285714285713, 0.03214285714285713, 0.03214285714285714, 0, -0.0414962501379366, -0.041496250137936605, 0.041496250137936605, 0, 0.02587745847533827, 0.02587745847533827, 0, -0.06338656910463872, -0.020044593143431825, 0.020044593143431825, -0.032732683535398856); 

a(20)=(0.03214285714285714, -0.03214285714285713, -0.03214285714285713, 0.03214285714285714, 0, 0.0414962501379366, -0.041496250137936605, -0.041496250137936605, 0, 0.02587745847533827, -0.02587745847533827, 0, 0.06338656910463872, -0.020044593143431825, -0.020044593143431825, -0.032732683535398856); 

a(21)=(0.03214285714285714, 0.03214285714285713, -0.03214285714285713, 0.03214285714285714, 0, -0.0414962501379366, 0.041496250137936605, -0.041496250137936605, 0, -0.02587745847533827, -0.02587745847533827, 0, -0.06338656910463872, 0.020044593143431825, -0.020044593143431825, -0.032732683535398856); 

a(22)=(0.03214285714285714, 0.03214285714285713, 0.03214285714285713, -0.03214285714285714, 0, 0.0414962501379366, -0.041496250137936605, -0.041496250137936605, 0, -0.02587745847533827, 0.02587745847533827, 0, -0.06338656910463872, 0.020044593143431825, 0.020044593143431825, 0.032732683535398856); 

a(23)=(0.03214285714285714, -0.03214285714285713, 0.03214285714285713, -0.03214285714285714, 0, -0.0414962501379366, 0.041496250137936605, -0.041496250137936605, 0, 0.02587745847533827, 0.02587745847533827, 0, 0.06338656910463872, -0.020044593143431825, 0.020044593143431825, 0.032732683535398856); 

a(24)=(0.03214285714285714, -0.03214285714285713, -0.03214285714285713, -0.03214285714285714, 0, 0.0414962501379366, 0.041496250137936605, 0.041496250137936605, 0, 0.02587745847533827, -0.02587745847533827, 0, -0.06338656910463872, -0.020044593143431825, -0.020044593143431825, 0.032732683535398856); 

a(25)=(0.03214285714285714, 0.03214285714285713, -0.03214285714285713, -0.03214285714285714, 0, -0.0414962501379366, -0.041496250137936605, 0.041496250137936605, 0, -0.02587745847533827, -0.02587745847533827, 0, 0.06338656910463872, 0.020044593143431825, -0.020044593143431825, 0.032732683535398856);

process = matrix(16,26);