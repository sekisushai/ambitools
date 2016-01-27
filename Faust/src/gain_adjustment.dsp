declare name        "50-channels gain equalizer";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2014";

import("math.lib");
import("music.lib");
import("filter.lib");

gain(i,delta)= nentry("%2a[unit:dB]", 0, -20, 20, 0.1) : db2linear : smooth(0.999) with{a = i+1+delta;};

process=hgroup("Gain equalizer",
vgroup("1-10",par(i,10,_*gain(i,0))),
vgroup("11-20",par(i,10,_*gain(i,10))),
vgroup("21-30",par(i,10,_*gain(i,20))),
vgroup("31-40",par(i,10,_*gain(i,30))),
vgroup("41-50",par(i,10,_*gain(i,40))));