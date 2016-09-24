# flowmonitor

This is a coolant-flow software interlock.  It can be used to shut down machinery if the coolant flow fails, 
e.g. due to pump failure or blockage.

It receives pulses at a few Hz from a hall-effect flow sensor.  The relay will be turned on after a few pulses,
and will turn off if no pulses are received for a second.

The SPST NO output relay should be connected in series with the coil on the machinery's main contactor, in series
with any emergency-stop button.
