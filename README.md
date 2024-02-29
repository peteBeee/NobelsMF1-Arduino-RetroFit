# History / Back Story 

I purchased an "untested" Nobels MF-1 midi foot controller on ebay, it had obviously been underwater at some point and as a result the internal control board was no longer functional.

Rather than attempt to repair it (there is a custom EPRom on the board that could be damaged), I instead decided to reuse what I could of the case and internal circuitry and team it up with an Arduino. Luckily there is already a MIDI library for the Arduino so that is one hurdle passed off the bat.

Here I will document my findings along with circuit diagrams, arduino sketch code and 3D STL files for parts where needeed. I'm not sure it will be of use to anyone but I'm quite proud of it so far.

***NOTE:*** I am building this to work with the BOSS RC505 Loopstation, it turns out this particular device has its own "quirky" requirements when receiving MIDI control (CC) instructions. It requires a CC signal of level 127 to be sent when the pedal is pressed and a second signal, level 0, to be sent when it is released.

# Road Map
1. Reuse existing foot switch circuitry with Arduino - **Committed - 26/02/2024**
2. Wire up MIDI port and send signals to PC - **Committed - 26/02/2024**
3. Have MIDI signals, *reliably*, interpreted by BOSS RC505 - **Committed - 27/02/2024**
4. 3D Model and print parts to install Aduino and ports in the existing case *with minimal case modification*
5. Reuse original LED segment display board to show which switch has been pressed / CC sent
7. Add software switch to switch between RC505 style signals and more standard signals
8. Add hardware switch to replace soft switch for 505/Standard signals
9. Add Ethernet shield and a web configuration UI

# Hardware Choice

I went with the Arduino Mega 2560 R3 (well actually a clone but its all compatible) as the connection of the each switch board requires 6 digital pins (12 in total) and a Arduino UNO R3 only has 14, 2 of which are needed for the MIDI serial communication. Although "technically" this would be enough it leaves no room for additional outputs (e.g. LED indicators). The Mega 2560 has 54 digital pins for only a few Euro more and there is plenty of space inside the pedal casing for the additional board size. In addition to this the Mega has multiple serial connections so the MIDI can be connected to the serial 2 pins and debug statements can still be sent to serial 1 (default) and viewed in the IDE - this makes/made development so much easier.

https://store.arduino.cc/products/arduino-mega-2560-rev3
