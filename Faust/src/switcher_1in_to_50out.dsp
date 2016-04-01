declare name        "Switcher 1 input to 50 outputs";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2016";


dest            = rint(hslider("Destination", 1, 1, 50, 1));

process         = _ <: par(i,50, _*((i+1)==dest));
