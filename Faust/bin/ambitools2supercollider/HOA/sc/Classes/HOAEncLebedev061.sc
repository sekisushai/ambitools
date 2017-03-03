FaustHOAEncLebedev061 : MultiOutUGen
{
  *ar { | in1, in2, in3, in4, in5, in6, gain(0.0) |
      ^this.multiNew('audio', in1, in2, in3, in4, in5, in6, gain)
  }

  *kr { | in1, in2, in3, in4, in5, in6, gain(0.0) |
      ^this.multiNew('control', in1, in2, in3, in4, in5, in6, gain)
  } 

  checkInputs {
    if (rate == 'audio', {
      6.do({|i|
        if (inputs.at(i).rate != 'audio', {
          ^(" input at index " + i + "(" + inputs.at(i) + 
            ") is not audio rate");
        });
      });
    });
    ^this.checkValidInputs
  }

  init { | ... theInputs |
      inputs = theInputs
      ^this.initOutputs(4, rate)
  }

  name { ^"FaustHOAEncLebedev061" }
}

