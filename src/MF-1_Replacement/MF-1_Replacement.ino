#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, SERIAL_PORT_HARDWARE1, MIDI)

const int switchPins[2][6] = {
 {26,28,30,32,34,36},
 {42,44,46,48,50,52}
 };
const int characters[17][7] = {
 {1,1,1,1,1,1,0},//0
 {0,1,1,0,0,0,0},//1
 {1,1,0,1,1,0,1},//2
 {1,1,1,1,0,0,1},//3
 {0,1,1,0,0,1,1},//4
 {1,0,1,1,0,1,1},//5
 {1,0,1,1,1,1,1},//6
 {1,1,1,0,0,0,0},//7
 {1,1,1,1,1,1,1},//8
 {1,1,1,1,0,1,1},//9
 {1,0,0,1,1,1,0},//10 - C
 {0,0,0,0,0,0,1},//11 - '-'
 {0,0,0,0,0,0,0},//12 - blank
 {0,0,0,0,0,1,1},//13 - top left
 {0,1,0,0,0,0,1},//14 - top right
 {0,0,0,0,1,0,1},//15 - bottom left
 {0,0,1,0,0,0,1},//16 - bottom right

 };
const int segmentPins[7] = {47,49,41,53,43,45,51};
const int digitPins[3] = {31,33,35};
const int dotPin = 37;

volatile int lastCCSent = 0;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);
  Serial.begin(9600);

  initSwitchPins(1);
  initSwitchPins(2);
  initDisplayPins();
  digitalWrite(dotPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int switchNo;
  int ccNo = 0;

  if(lastCCSent > 0){
    display(10, (lastCCSent / 10), (lastCCSent % 10), false);
  }
  else{
    display(12,12,12,false);
  }

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

void initDisplayPins(){
  for(int pin = 0; pin < 8; pin++){
    pinMode(segmentPins[pin], OUTPUT);
  }
  for(int pin = 0; pin < 3; pin++){
    pinMode(digitPins[pin], OUTPUT);
  }
  pinMode(dotPin, OUTPUT);
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

void display(int h, int t, int u, bool showLeadingZeros){
if(h == 0 && !showLeadingZeros) h = 12;
if(h == 0 && t == 0 && !showLeadingZeros) t = 12;

int digits[] = {h,t,u};
for(int digit = 0; digit < 3; digit++){
  digitalWrite(digitPins[0], LOW);
  digitalWrite(digitPins[1], LOW);
  digitalWrite(digitPins[2], LOW);


  for(int segment = 0; segment < 8; segment++){
    digitalWrite(segmentPins[segment], characters[digits[digit]][segment] == 0 ? LOW : HIGH);
  }

  digitalWrite(digitPins[0], digit == 0 ? HIGH : LOW);
  digitalWrite(digitPins[1], digit == 1 ? HIGH : LOW);
  digitalWrite(digitPins[2], digit == 2 ? HIGH : LOW);

  delay(5);
}

}

