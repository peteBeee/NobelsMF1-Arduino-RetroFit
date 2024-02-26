#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, SERIAL_PORT_HARDWARE1, MIDI)

const int switchPins[2][6] = {
 {26,28,30,32,34,36},
 {42,44,46,48,50,52}
};

volatile int lastButtonPressed;

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

  if(ccNo == 0){
    lastButtonPressed = 0;
  }
  else{
    if(ccNo != lastButtonPressed){
      sendCCForFootControl(ccNo, 1, 2);
      lastButtonPressed = ccNo;
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

void sendCCForFootControl(int ccNo, int channel, int delayBetweenOnOff){
  MIDI.sendControlChange(ccNo, 127, channel);
  delay(delayBetweenOnOff);
  MIDI.sendControlChange(ccNo, 127, channel);
  delay(delayBetweenOnOff);
  MIDI.sendControlChange(ccNo, 127, channel);
  delay(delayBetweenOnOff);
  MIDI.sendControlChange(ccNo, 0, channel);
  delay(delayBetweenOnOff);

  MIDI.sendControlChange(ccNo, 127, channel);
  delay(delayBetweenOnOff);
  MIDI.sendControlChange(ccNo, 127, channel);
  delay(delayBetweenOnOff);
  MIDI.sendControlChange(ccNo, 127, channel);
  delay(delayBetweenOnOff);
  MIDI.sendControlChange(ccNo, 0, channel);
  delay(delayBetweenOnOff);

  Serial.print("Sent CC");
  Serial.println(ccNo);
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
    case 21:
    case 23:
      return (10 * row) + 1;
    case 14:
      return (10 * row) + 2;
    case 22:
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

