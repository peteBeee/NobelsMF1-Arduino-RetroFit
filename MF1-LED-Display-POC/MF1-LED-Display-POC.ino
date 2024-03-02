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
const int digitPins[3] = {33,35,37};
const int dotPin = 39;

void setup() {
  setupDisplayPins();
}

int i = 0;

void loop() {
    //displayNumber(i / 100,(i / 10) % 10,i % 10, false);
    //displayNumber(10,2,1, false);
    //displayNumber(11,11,11,false);

    switch(i+1){
      case 1:  displayNumber(13,11,11,false); break;
      case 2:  displayNumber(14,11,11,false); break;
      case 3:  displayNumber(11,13,11,false); break;
      case 4:  displayNumber(11,14,11,false); break;
      case 5:  displayNumber(11,11,13,false); break;
      case 6:  displayNumber(11,11,14,false); break;
      case 7:  displayNumber(15,11,11,false); break;
      case 8:  displayNumber(16,11,11,false); break;
      case 9:  displayNumber(11,15,11,false); break;
      case 10: displayNumber(11,16,11,false); break;
      case 11: displayNumber(11,11,15,false); break;
      case 12: displayNumber(11,11,16,false); break;
      default: displayNumber(11,11,11,false); break;
    }

    // delay (10);
    i++;
    if(i == 12) i = 0;
}
void displayNumber(int h, int t, int u, bool showLeadingZeros){
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
void setupDisplayPins(){
  for(int pin = 0; pin < 8; pin++){
    pinMode(segmentPins[pin], OUTPUT);
  }
  for(int pin = 0; pin < 3; pin++){
    pinMode(digitPins[pin], OUTPUT);
  }
  pinMode(dotPin, OUTPUT);
}