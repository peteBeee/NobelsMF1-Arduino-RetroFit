# History / Back Story 

I purchased an "untested" Nobels MF-1 midi foot controller on ebay, it had obviously been underwater at some point and as a result the internal control board was no longer functional.

Rather than attempt to repair it (there is a custom EPRom on the board that could be damaged), I instead decided to reuse what I could of the case and internal circuitry and team it up with an Arduino. Luckily there is already a MIDI library for the Arduino so that is one hurdle passed off the bat.

Here I will document my findings along with circuit diagrams, arduino sketch code and 3D STL files for parts where needeed. I'm not sure it will be of use to anyone but I'm quite proud of it so far.

***NOTE:*** I am building this to work with the BOSS RC505 Loopstation, it turns out this particular device has its own "quirky" requirements when receiving MIDI control (CC) instructions. It requires a CC signal of level 127 to be sent when the pedal is pressed and a second signal, level 0, to be sent when it is released.

See the Wiki for more detail:
https://github.com/peteBeee/NobelsMF1-Arduino-RetroFit/wiki
