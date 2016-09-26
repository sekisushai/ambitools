FaustSoundcheck : MultiOutUGen
{
  *ar { | in1, volume(-96.0), freq(1000.0), destination(1.0), sine_wave(0.0), white_noise(0.0), pink_noise(0.0), input(0.0) |
      ^this.multiNew('audio', in1, volume, freq, destination, sine_wave, white_noise, pink_noise, input)
  }

  *kr { | in1, volume(-96.0), freq(1000.0), destination(1.0), sine_wave(0.0), white_noise(0.0), pink_noise(0.0), input(0.0) |
      ^this.multiNew('control', in1, volume, freq, destination, sine_wave, white_noise, pink_noise, input)
  } 

  checkInputs {
    if (rate == 'audio', {
      1.do({|i|
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
      ^this.initOutputs(50, rate)
  }

  name { ^"FaustSoundcheck" }
}

