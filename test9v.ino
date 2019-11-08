
#define motorA1 3
#define motorA2 4
#define motorB1 5
#define motorB2 6
#define motorBPWM 7
#define motorAPWM 2
#define motorCPWM 8

#define motorC1 9
#define motorC2 10

#define TIMP_ROTATIE_STANGA 250
#define TIMP_ROTATIE_DREAPTA 250
#define TIMP_MERS_FATA 250
#define TIMP_MERS_SPATE 250


// ir-urile citesc 1 daca nu exista obiect (daca e negru)
// citesc 0 daca e obiect (daca e alb)

// fotorezistorii citesc valori intre 0 si 1023
// trebuie calibrati pe loc

const int fotoPin1 = A9; //   |-foto2-----------foto3-|
const int fotoPin2 = A8; // foto1                   foto4
const int fotoPin3 = A15; //   |                       |
const int fotoPin4 = A14; //   |                       |
const int fotoPin5 = A13; //   |                       |
const int fotoPin6 = A12; //   |                       |
const int fotoPin7 = A11; // foto8                   foto5
const int fotoPin8 = A10; //   |-foto7-----------foto6-|
// calibrarile hardcodate
const int fotoMax1 = 0;
const int fotoMax2 = 0;
const int fotoMax3 = 0;
const int fotoMax4 = 0;
const int fotoMax5 = 0;
const int fotoMax6 = 0;
const int fotoMax7 = 0;
const int fotoMax8 = 0;
// ir-urile de pe fund        |-----------------------|
const int irPinB1 = 52;  //    |           *           |
const int irPinB2 = 30;  //    |  irB1    ***    irB2  |
const int irPinB3 = 53;  //    |           *           |
const int irPinB4 = 31;  //    |                       |
const int irPinB5 = 50;  //    |                       |
const int irPinB6 = 32;  //    |  irB3           irB4  |
// ir-urile de pe fata        |                       |
const int irPinT1 = 51;  //    |   irB5         irB6   |
const int irPinT2 = 33;  //    --------         --------
const int irPinT3 = 48;  //     \ irT1 \       / irT2 /
const int irPinT4 = 34; //      --------     --------
const int irPinT5 = 49; //      | irT3 |     | irT4 |
const int irPinT6 = 35; //      --------     --------
                        //       \ irT5 \   / irT6 /
int fotoSum[9];         //        -------- --------;

int fotoFirstMax = 0;
int fotoDirection;

int irValueB1;
int irValueB2;
int irValueB3;
int irValueB4;
int irValueB5;
int irValueB6;
int irValueT1;
int irValueT2;
int irValueT3;
int irValueT4;
int irValueT5;
int irValueT6;

int sum;
int medie;
int initial;


// variabile globales

char initialTeren = 0;
char floorIRSensors[6];

char timerCorrection = 0;
long time0 = 0;

const unsigned long minut = 60000;
const unsigned long secunda = 1000;
unsigned long startMillis;
unsigned long startGlobalTime;
unsigned long currentMillis;
unsigned long startIRMillis;

#define DURATA_RUNDA 150000


boolean startFirstDance = 0;
boolean shit = 0;

boolean bStartDanceShit = 1;

boolean irOutput[6] = {0};

boolean terenInamic = 0;

// intoarce terenul initial
// 1 - alb, 0 - negru
//char readInitialTeren() {
//  char teren = 0;
//  for(char i = 0; i < 6; i++) {
//    teren += digitalRead(floorIRSensors[i]);
//  }
//  return (teren > 3) ? 1 : 0;
//}


void teren() {
  // citim informatii despre teren
  irValueB1 = digitalRead(irPinB1);
  irValueB2 = digitalRead(irPinB2);
  irValueB3 = digitalRead(irPinB3);
  irValueB4 = digitalRead(irPinB4);
  irValueB5 = digitalRead(irPinB5);
  irValueB6 = digitalRead(irPinB6);

  sum = irValueB1 + irValueB2 + irValueB3 + irValueB4 + irValueB5 + irValueB6;

  if (sum > 3) {
    medie = 1;
  } else {
    medie = 0;
  }

  if (medie != initial) {
    terenInamic = 1;
    Serial.println("bv boss, suteaza");
  } else {
    terenInamic = 0;
    Serial.println("go drunk, you're home");
  }
}

void IR() {
  // vedem daca e (bila)
  // da' doar daca e

  irValueT1 = digitalRead(irPinT1);
  irValueT2 = digitalRead(irPinT2);
  irValueT3 = digitalRead(irPinT3);
  irValueT4 = digitalRead(irPinT4);
  irValueT5 = digitalRead(irPinT5);
  irValueT6 = digitalRead(irPinT6);

  for(int i = 0; i < 6; i++) {
    irOutput[i] = 0;
  }

  if (irValueT3 == 0 && irValueT1 == 1) {
    Serial.println("Ai bila boss, pe stanga");
    irOutput[0] = 1;
  }
  if (irValueT3 == 0 && irValueT1 == 0) {
    Serial.println("Stai parcat ca dai cu stanga in perete");
    irOutput[1] = 1;
  }
  if (irValueT5 == 0) {
    Serial.println("Scoate fierastraul ca ai robot in stanga");
    irOutput[2] = 1;
  }
  if (irValueT4 == 0 && irValueT2 == 1) {
    Serial.println("Ai bila boss, pe dreapta");
    irOutput[3] = 1;
  }
    if (irValueT4 == 0 && irValueT2 == 0) {
      Serial.println("Stai parcat ca dai cu dreapta in perete");
      irOutput[4] = 1;
    }
  if (irValueT6 == 0) {
    Serial.println("Scoate fierastraul ca ai robot in dreapta");
    irOutput[5] = 1;
  }
}

boolean citireIRinterpretare = 0;

void interpretareIR() {
  if (irOutput[0] == 1) {
    bStartDanceShit = 0;
    if(citireIRinterpretare == 0){
      startIRMillis = millis();
      citireIRinterpretare = 1;
    }
    if(currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA) {
      setLeftMotion();
    } else if (currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA + TIMP_MERS_FATA){
      setForwardMotion();
    } else {
      bStartDanceShit = 1;
    }
  } else if (irOutput[1] == 1) {
    bStartDanceShit = 0;
    if(citireIRinterpretare == 0){
      startIRMillis = millis();
      citireIRinterpretare = 1;
    }
    if(currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA) {
      setRightMotion();
    } else {
      bStartDanceShit = 1;
    }
  } else if (irOutput[2] == 1) {
    bStartDanceShit = 0;
    if(citireIRinterpretare == 0){
      startIRMillis = millis();
      citireIRinterpretare = 1;
    }
    if(currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA) {
      setRightMotion();
    } else {
      bStartDanceShit = 1;
    }
  } else if (irOutput[3] == 1) {
    bStartDanceShit = 0;
    if(citireIRinterpretare == 0){
      startIRMillis = millis();
      citireIRinterpretare = 1;
    }
    if(currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA) {
      setRightMotion();
    } else if (currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA + TIMP_MERS_FATA){
      setForwardMotion();
    } else {
      bStartDanceShit = 1;
    }
  } else if (irOutput[4] == 1) {
    bStartDanceShit = 0;
    if(citireIRinterpretare == 0){
      startIRMillis = millis();
      citireIRinterpretare = 1;
    }
    if(currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA) {
      setLeftMotion();
    } else {
      bStartDanceShit = 1;
    }
  } else if (irOutput[5] == 1) {
    bStartDanceShit = 0;
    if(citireIRinterpretare == 0){
      startIRMillis = millis();
      citireIRinterpretare = 1;
    }
    if(currentMillis - startIRMillis <= TIMP_ROTATIE_STANGA) {
      setLeftMotion();
    } else {
      bStartDanceShit = 1;
    }
  }  else {
     citireIRinterpretare = 0;
  }
  
}

void foto() {
  fotoSum[0] = analogRead(fotoPin1) + analogRead(fotoPin2);
  fotoSum[1] = analogRead(fotoPin2) + analogRead(fotoPin3);
  fotoSum[2] = analogRead(fotoPin3) + analogRead(fotoPin4);
  fotoSum[3] = analogRead(fotoPin4) + analogRead(fotoPin5);
  fotoSum[4] = analogRead(fotoPin5) + analogRead(fotoPin6);
  fotoSum[5] = analogRead(fotoPin6) + analogRead(fotoPin7);
  fotoSum[6] = analogRead(fotoPin7) + analogRead(fotoPin8);
  fotoSum[7] = analogRead(fotoPin8) + analogRead(fotoPin1);

  for (int i = 0; i < 8; i++) {
    if (fotoSum[i] > fotoFirstMax) {
      fotoDirection = i;
      fotoFirstMax = fotoSum[i];
    }
  }

  switch (fotoDirection) {
    case 0:
      Serial.println("fata stg");
      break;
    case 1:
      Serial.println("fata");
      break;
    case 2:
      Serial.println("fata dreapta");
      break;
    case 3:
      Serial.println("dreapta");
      break;
    case 4:
      Serial.println("spate dreapta");
      break;
    case 5:
      Serial.println("spate");
      break;
    case 6:
      Serial.println("spate stanga");
      break;
    case 7:
      Serial.println("stanga");
      break;
    default:
      Serial.println("n-ar trb sa vezi asta");
  }
}

void citire() {
  teren();
  foto();
  IR();
}
void setMotorPerieIn() {
  digitalWrite(motorC1, LOW);
  digitalWrite(motorC2, HIGH);
}

void setMotorPerieOut() {
  digitalWrite(motorC1, HIGH);
  digitalWrite(motorC2, LOW);
}

void stopMotorPerie() {
  digitalWrite(motorC1, LOW);
  digitalWrite(motorC2, LOW);
}

void setForwardMotion() {
  setMovingSpeed();
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}


void setBackwardMotion() {
  setMovingSpeed();
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void setRightMotion() {
  setTurningSpeed();
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void setLeftMotion() {
  setTurningSpeed();
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void stopMotion() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void setTurningSpeed() {
  analogWrite(motorAPWM, 50);
  analogWrite(motorBPWM, 50);
}

void setMovingSpeed() {
  analogWrite(motorAPWM, 200);
  analogWrite(motorBPWM, 200);
}

unsigned long getSecFromStart() {
  long time12 = millis();
//  if(timerCorrection == 2) {
//    timerCorrection = 0;
//    time12--;
//  } else {
//    timerCorrection++;
//  }
  if( (((time12 / 1000) * 1000 - time12) < (-900)) && (((time12 / 1000) * 1000 - time12) > (-1000)) ) {
    return time12 / 1000 + 1;
  }
  return time12 / 1000;
}

// direction 0 left
// direction 1 right
void rotateLeftRightDegrees(bool direction, short degrees1) {
  if(direction == 0) {
    setLeftMotion();
  } else {
    setRightMotion();
  }
//  delay(TIMP_ROTATIE_STANGA * degrees1);
//  stopMotion();
}

// direction 0 forward
// direction 1 backward
void goFrontBackCm(bool direction, short distance) {
  if(direction == 0) {
    setForwardMotion();
  } else {
    setBackwardMotion();
  }
//  delay(TIMP_MERS_FATA * distance);
//  stopMotion();
}


void startDanceShit(unsigned long millis, boolean activate) {
  if(activate == 0) {
    return;
  }
  if(millis - startMillis <= TIMP_ROTATIE_STANGA) {
    rotateLeftRightDegrees(0, 90);
  } else if (millis - startMillis <= 2 * TIMP_ROTATIE_STANGA) {
    goFrontBackCm(0, 40);
    shit = 0;
  } else if (millis - startMillis <= 3 * TIMP_ROTATIE_STANGA) {
    rotateLeftRightDegrees(1, 90);
    shit = 0;
  } else if (millis - startMillis <= 4 * TIMP_ROTATIE_STANGA) {
    goFrontBackCm(0, 40);
    shit = 0;
  } else if (millis - startMillis <= 5 * TIMP_ROTATIE_STANGA) {
    rotateLeftRightDegrees(1, 90);
    shit = 0;
  } else if (shit == 0){
    startMillis = millis + 1000;
    shit = 1;
  }
}

unsigned long startOrientareMillis;
boolean citireStartOrientare = 1;

void orientarePoarta() {
  if(citireStartOrientare == 1) {
    startOrientareMillis = millis();
    citireStartOrientare = 0;
  }
  if(terenInamic == 1) {
    if(fotoDirection == 2 || fotoDirection == 3 || fotoDirection == 4 || fotoDirection == 5) {
      if(currentMillis - startOrientareMillis <= TIMP_ROTATIE_STANGA) {
        rotateLeftRightDegrees(0, 90);
      } else {
        citireStartOrientare = 1;
      }
    } else if (fotoDirection == 0 || fotoDirection == 6 || fotoDirection == 7) { 
      if(currentMillis - startOrientareMillis <= TIMP_ROTATIE_STANGA) {
        rotateLeftRightDegrees(1, 90);
      } else {
        citireStartOrientare = 1;
      }
    } else {
      goFrontBackCm(0, 40); 
      setMotorPerieOut();
      // plm?
    }
  } else {
    if(fotoDirection == 2 || fotoDirection == 3 || fotoDirection == 4 || fotoDirection == 1) {
      if(currentMillis - startOrientareMillis <= TIMP_ROTATIE_STANGA) {
        rotateLeftRightDegrees(0, 90);
      } else {
        citireStartOrientare = 1;
      }
    } else if(fotoDirection == 0 || fotoDirection == 7 || fotoDirection == 5) {
      if(currentMillis - startOrientareMillis <= TIMP_ROTATIE_STANGA) {
        rotateLeftRightDegrees(1, 90);
      } else {
        citireStartOrientare = 1;
      }
    } else {
      goFrontBackCm(0, 40);
      citireStartOrientare = 1;
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorC1, OUTPUT);
  pinMode(motorC2, OUTPUT);
  
  pinMode(motorBPWM, OUTPUT);
  pinMode(motorAPWM, OUTPUT);
  
  pinMode(irPinB1, INPUT);
  pinMode(irPinB2, INPUT);
  pinMode(irPinB3, INPUT);
  pinMode(irPinB4, INPUT);
  pinMode(irPinB5, INPUT);
  pinMode(irPinB6, INPUT);

  pinMode(irPinT1, INPUT);
  pinMode(irPinT2, INPUT);
  pinMode(irPinT3, INPUT);
  pinMode(irPinT4, INPUT);
  pinMode(irPinT5, INPUT);
  pinMode(irPinT6, INPUT);

  
  // startDanceShit() ;
  startGlobalTime = millis();

  irValueB1 = digitalRead(irPinB1);
  irValueB2 = digitalRead(irPinB2);
  irValueB3 = digitalRead(irPinB3);
  irValueB4 = digitalRead(irPinB4);
  irValueB5 = digitalRead(irPinB5);
  irValueB6 = digitalRead(irPinB6);


  sum = irValueB1 + irValueB2 + irValueB3 + irValueB4 + irValueB5 + irValueB6;

  if (sum >= 3) {
    initial = 1; // terenul pe care incepem e negru
  } else {
    initial = 0; // terenul pe care incepem e alb
  }

  delay(5000);
  startMillis = millis();
  bStartDanceShit = 1;
}



void loop() {
  
  currentMillis = millis();
  
  
  citire();
  
  interpretareIR();

  // if()

  if(currentMillis - startGlobalTime <= 100000)  {
    startDanceShit(currentMillis, bStartDanceShit);
  } else if(currentMillis - startGlobalTime <= 140000) {
    orientarePoarta();
  } else {
    while(1);
  }
   
  Serial.println(currentMillis);
  
}



 
  
