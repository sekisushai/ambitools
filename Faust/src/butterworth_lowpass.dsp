declare name        "Butterworth Lowpass";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

//Butterworth 2nd order lowpass from filter.lib

import("filter.lib");

freq=hslider("Cutoff Frequency[unit:Hz]",50,10,1000,1);
//order = hslider("Filter order",2,1,5,1);

// Volume controller
vol             = hslider("Gain[style:knob][unit:dB]", 0, -20, 20, 0.1) : db2linear : smooth(0.999);

process=_*(vol):lowpass(2,freq):_;