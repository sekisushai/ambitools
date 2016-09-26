declare name        "Soundcheck";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "BSD";
declare copyright   "(c) GRAME 2006";

// Description: Multichannel audio tester. The test signal are among: sinus tone, white noise, pink noise, audio input.

// Input: 1
// Outputs: N

import("stdfaust.lib");

N=50;

pink    = f : (+ ~ g) with {
    f(x)    = 0.04957526213389*x - 0.06305581334498*x' +
          0.01483220320740*x'';
    g(x)    = 1.80116083982126*x - 0.80257737639225*x';
};


// User interface
//----------------
smooth(c)       = *(1-c) : +~*(c);
vol             = hslider("[1]Volume", -96, -96, 0, 0.1): ba.db2linear : si.smooth(0.999);
freq            = hslider("[2]Freq", 1000, 0, 24000, 0.1);
dest            = int(hslider("[3]Destination", 1, 1, N, 1));

testsignal      = _*checkbox("[7]Input")
		+ os.osci(freq)*checkbox("[4]Sine Wave")
                + no.noise * checkbox("[5]White Noise")
                + pink(no.noise) * ba.db2linear(20)  * checkbox("[6]Pink Noise");

process         = vgroup( "Multichannel Audio Tester", testsignal*vol <: par(i,N, _*((i+1)==dest)) );
