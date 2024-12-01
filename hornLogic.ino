int horn = 9;
int btnpress = 7; 

void setup() {
  Serial.begin(9600);
  pinMode(horn, OUTPUT);
  pinMode(btnpress, INPUT);
}

int btnValue = 0;
int i = 0; //incrementer variable
enum hornStates {INIT, Off , On } currState;
void HornTicks() {
  //State Transitions
  switch(currState) {
    case INIT:
      currState = Off;
      break;
    case Off:
      if (btnValue == 1 && i < 5) {
        i++;
        currState = Off;
      }
      else if (btnValue == 0 && i < 5) {
        i = 0;
        currState = Off;
      }
      else {
        currState = On;
        i = 0;
      }
      break;
    case On:
      if (btnValue == 0 && i < 5) {
        i++;
        currState = On;
      }
      else if (btnValue == 1) {
        currState = On;
        i = 0;
      }
      else {
        i = 0;
        currState = Off;
      }
      break;
    default:
      currState = INIT;
      i = 0;
      break;
  }

  //State Actions
  switch(currState) {
    case INIT:
      digitalWrite(horn,0);
      break;
    case Off:
      digitalWrite(horn, 0);
      break;
    case On:
      digitalWrite(horn, 1);
      break;
    default:
      digitalWrite(horn, 0);
      break;
  }

}

//horn = 0 off, 1 on
//btn = 1 when pressed
void loop() {
  // put your main code here, to run repeatedly:
   //digitalWrite(horn,1);
   btnValue = digitalRead(btnpress);
   //Serial.println(btnValue);
   Serial.println(currState);

    HornTicks();

//     delay(1000);

//     digitalWrite(horn,0);

//     delay(1000);
}
