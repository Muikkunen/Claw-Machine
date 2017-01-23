#define delay_first 600
#define delay_second 600
#define delay_first_both 1000
#define delay_second_both 1000
#define max_z_location 2088

 

#include <Servo.h>

Servo myservo1;

int x_axis = 0;
int y_axis = 0;
const int buttonPin = 2;
int buttonState = 0;

// seuraavat kolme määrittävät stepperien pyörimisen pinnin
const int stepPin1 = 11;
const int stepPin2 = 3;
const int stepPin3 = 6;

// seuraavat kolme määrittävät steppereiden pyörimissuunnan
const int dirPin1 = 12;
const int dirPin2 = 4;
const int dirPin3 = 7;

const int stepsInFullRound = 400;
boolean stepper_direction_x;
boolean stepper_direction_y;

// xy-sijainti
int x_location = 0;
int y_location = 0;
int z_location = 0;

 

// Set pins
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  myservo1.attach(9);

  // x-suunnan stepperi
  pinMode(stepPin1, OUTPUT);    
  pinMode(dirPin1, OUTPUT);
  digitalWrite(stepPin1, LOW);
  digitalWrite(dirPin1, LOW);

 

 

 // y-suunnan stepperi
  pinMode(stepPin2, OUTPUT);    
  pinMode(dirPin2, OUTPUT);
  digitalWrite(stepPin2, LOW);
  digitalWrite(dirPin2, LOW);

  // z-suunnan stepperi
  pinMode(stepPin3, OUTPUT);    
  pinMode(dirPin3, OUTPUT);
  digitalWrite(stepPin3, LOW);
  digitalWrite(dirPin3, LOW);

}

// mikäli nappulaa painettu, siirrytään tähän funktioon

void grip() {
   Serial.print("x-sijanti: ");
   Serial.print(x_location);
   Serial.print(" y-sijainti: ");
   Serial.print(y_location);
   Serial.print(" \n");

   //avataan koura ensimmäisen kerran
   myservo1.write(0);

   // lasketaan kouraa
   digitalWrite(dirPin3, 14);
   for (int i = 0; i < max_z_location; i++) { // 20 cm laskulle
     digitalWrite(stepPin3, HIGH);
     delayMicroseconds(delay_first);
     digitalWrite(stepPin3, LOW);
     delayMicroseconds(delay_second);
   }

   // suljetaan koura
   myservo1.write(100);
   delay(100);

   // nostetaan koura
   digitalWrite(dirPin3, LOW);
   for (int i = 0; i < max_z_location; i++) { // 20 cm nostolle
     digitalWrite(stepPin3, HIGH);
     delayMicroseconds(delay_first);
     digitalWrite(stepPin3, LOW);
     delayMicroseconds(delay_second);
   }

 

   // liikutaan x-tasossa (0,y) kohtaan
   for (x_location; x_location >= 0; x_location = x_location - 10) {
     digitalWrite(dirPin1, 0);
      for (int i = 0; i < 10; i++) {
        digitalWrite(stepPin1, HIGH);
        delayMicroseconds(delay_first);
        digitalWrite(stepPin1, LOW);
        delayMicroseconds(delay_second);
      }
      Serial.println(x_location);
   }

  // liikutaan y-tasossa (x,0) kohtaan, eli tässä vaiheessa (0,0) kohtaan
    for (y_location; y_location >= 0; y_location = y_location - 10) {
     digitalWrite(dirPin2, 0);
      for (int i = 0; i < 10; i++) {
        digitalWrite(stepPin2, HIGH);
        delayMicroseconds(delay_first);
        digitalWrite(stepPin2, LOW);
        delayMicroseconds(delay_second);
      }
      Serial.println(y_location);
   }

   // avataan koura
   myservo1.write(0);
   delay(2000);
   myservo1.write(100);
}

void loop() {
  buttonState = digitalRead(buttonPin); // luetaan nappulan asento
  x_axis = analogRead(A0);              // luetaan joystickin x-asento
  y_axis = analogRead(A1);              // luetaan joystickin y-asento

  if (!buttonState) {
    grip();
  }
  else {

    // määritellään x-suunta
    if (x_axis < 400) {
      stepper_direction_x = LOW; // stepperin suunta vasemmalle
    }

    else {
      stepper_direction_x = HIGH; // stepperin suunta oikealle
    }

    // määritellään y-suunta
    if (y_axis < 400) {
      stepper_direction_y = LOW; // stepperin suunta ylös
    }
     else {
      stepper_direction_y = HIGH; // stepperin suunta alas
    }

    // vinoon liikkuminen
    if ((((0 <= x_axis) && (x_axis < 400)) || ((600 <= x_axis))) && (((0 <= y_axis) && (y_axis < 400)) || ((600 <= y_axis)))) {
      digitalWrite(dirPin1, stepper_direction_x);
      digitalWrite(dirPin2, stepper_direction_y);

      for (int i = 0; i < 10; i++) {
        if (!((stepper_direction_x == 0) && (x_location <= 0))) {
          digitalWrite(stepPin1, HIGH);
        }

        if (!((stepper_direction_y == 0) && (y_location <= 0))) {
          digitalWrite(stepPin2, HIGH);
        }
        delayMicroseconds(delay_first_both);

        if (!((stepper_direction_x == 0) && (x_location <= 0))) {
          digitalWrite(stepPin1, LOW);
          if (stepper_direction_x) {
              x_location++;
            }
            else {
              x_location--;
            }
        }

        if (!((stepper_direction_y == 0) && (y_location <= 0))) {
          digitalWrite(stepPin2, LOW);
          if (stepper_direction_y) {
              y_location++;
            }
            else {
              y_location--;
            }
        }

        delayMicroseconds(delay_second_both);
      }
    }
    else {
      if (((0 <= x_axis) && (x_axis < 400)) || ((600 <= x_axis))) {
        if (!((stepper_direction_x == 0) && (x_location <= 0))) { // estä liikkuminen x-akselin negatiiviselle puolelle
          digitalWrite(dirPin1, stepper_direction_x);

          for (int i = 0; i < 10; i++) {
            digitalWrite(stepPin1, HIGH);
            delayMicroseconds(delay_first);
            digitalWrite(stepPin1, LOW);
            delayMicroseconds(delay_second);
            if (stepper_direction_x) {
              x_location++;
            }
            else {
              x_location--;
            }
          }
        }
      }
      else if (((0 <= y_axis) && (y_axis < 400)) || ((600 <= y_axis))) {
        if (!((stepper_direction_y == 0) && (y_location <= 0))) { // estä liikkuminen y-akselin negatiiviselle puolelle
          digitalWrite(dirPin2, stepper_direction_y);
          for (int i = 0; i < 10; i++) {
            digitalWrite(stepPin2, HIGH);
            delayMicroseconds(delay_first);
            digitalWrite(stepPin2, LOW);
            delayMicroseconds(delay_second);

            if (stepper_direction_y) {
              y_location++;
            }
            else {
               y_location--;
            }
          }
        }
      }
    }
  }
  buttonState = 0;  // alusta nappula nollaksi
}