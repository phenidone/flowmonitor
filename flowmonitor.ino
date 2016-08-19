/* -*- C++ -*-
 * 
 * (C) 2016 W. Brodie-Tyrrell
 * 
 * Simple monitor code for a pulsing flow-meter.  Enables its output when the input is pulsing.
 * 
 * Used to shutdown liquid-cooled systems when the coolant flow ceases.
 *
 * Designed for ATtiny85/arduino.
 */

#include <avr/interrupt.h>

#define TIMEOUT 1000      ///< 1s of no pulses means we turn off
#define POLL_PERIOD 100   ///< 10Hz
#define MINPULSES 10      ///< need this many toggles before starting up
#define DEBOUNCE 3        ///< need 3ms between edges before recognising a state change

#define INPIN 1           ///< pin-change monitoring on PC1 = pin 6
#define OUTPIN 4          ///< output-enable is PC4 = pin3

volatile unsigned long lastChange=0;
volatile int pulses=0;
volatile boolean running=false;

void setup()
{
  pinMode(OUTPIN, OUTPUT);
  digitalWrite(OUTPUT, 0);
  cli();
  GIMSK=(1 << PCIE);   // enable pin-change interrupt
  PCMSK=(1 << INPIN);    // on the correct pin
  sei();
}

// pin-change interrupt
ISR(PCINT0_vect)
{
  unsigned long now=millis();
  unsigned long dt=now-lastChange;

  if(dt < DEBOUNCE){
    return;
  }
  
  if(pulses < MINPULSES){
    ++pulses;
  }
  else{
    running=true;
  }
  lastChange=now;
}

// polling loop at about 10Hz
void loop()
{
  unsigned long now=millis();
  unsigned long dt=now-lastChange;    // should be overflow-safe
  if(dt > TIMEOUT){
    pulses=0;
    running=false;
  }

  digitalWrite(OUTPIN, running);
  delay(POLL_PERIOD);
}
