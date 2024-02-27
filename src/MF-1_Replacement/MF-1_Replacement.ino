#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, SERIAL_PORT_HARDWARE1, MIDI)

const int switchPins[2][6] = {
 {26,28,30,32,34,36},
 {42,44,46,48,50,52}
};

volatile int lastCCSent;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);
  Serial.begin(9600);

  initSwitchPins(1);
  initSwitchPins(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  int switchNo;
  int ccNo = 0;

  switchNo = readFootSwitch(1);
  if(switchNo == 0){
    switchNo = readFootSwitch(2);
    if(switchNo > 0){
      ccNo = CCFromSwitch(switchNo, 2);
    }
  }else{
    ccNo = CCFromSwitch(switchNo, 1);
  }

  if(ccNo == 0 && lastCCSent != 0){
    //pedal released, send 0 to last CC
    MIDI.sendControlChange(lastCCSent, 0, 1);
    lastCCSent = 0;
  }
  else{
    if(ccNo != lastCCSent){
      //Send 127 to CC, initiating the pedal press
      MIDI.sendControlChange(ccNo, 127, 1);
      lastCCSent = ccNo;
    }
  }
}

void initSwitchPins(int row){
   pinMode(switchPins[row - 1][0], INPUT);
   pinMode(switchPins[row - 1][1], INPUT);
   for(int i = 2; i<=6; i++){
     pinMode(switchPins[row - 1][i], OUTPUT);
   }
}

int readFootSwitch(int row){
  int result = 0;

  //Send pins 3-6 high
  //test pin 1 after each
  //if pin1 high, set result and exit
  for(int i = 2; i <= 5; i++){
    digitalWrite(switchPins[row-1][i], HIGH);
    if(digitalRead(switchPins[row-1][0]) == HIGH) result = i - 1;
    digitalWrite(switchPins[row-1][i], LOW);
    if(result > 0) return result + 10;
  }

  //Send pins 3-4 high
  //test pin 2 after each
  //if pin2 high, set result and exit
  for(int i = 2; i <= 5; i++){
    digitalWrite(switchPins[row-1][i], HIGH);
    if(digitalRead(switchPins[row-1][1]) == HIGH) result = i - 1;
    digitalWrite(switchPins[row-1][i], LOW);
    if(result > 0) return result + 20;
  }

  return result;
}

int CCFromSwitch(int switchNo, int row){
  switch(switchNo){
    case 21: //Wiredly the two boards (upper and lower are wired slightly differently - assume its related to how the original firmware worked - so button 1 can be on one of two pins)
    case 23:
      return (10 * row) + 1;
    case 14:
      return (10 * row) + 2;
    case 22: //Wiredly the two boards (upper and lower are wired slightly differently - assume its related to how the original firmware worked - so button 3 can be on one of two pins)
    case 24:
      return (10 * row) + 3;
    case 13:
      return (10 * row) + 4;
    case 12:
      return (10 * row) + 5;
    case 11:
      return (10 * row) + 6;
    default:
      return 0;
  }
}

