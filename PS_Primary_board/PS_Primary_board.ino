 
#define DPin_Signal       9 
#define DPin_Light        7
#define DPin_Echo         6
#define DPin_Trigger      5
#define DPin_Switch       4
#define DPin_SRotation    3
#define DPin_STilt        2

#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <Servo.h>
   // create an object for use in this sketch


Servo sRotation;
Servo sTilt;

enum tracks{
  NONE,
  Justice,
  finish,
  move,
  scan,
  scan2,
  scan3,
  shoot,
  shoot2,
  shoot3,
  spot,
  spot2
};

void rotateServoR(int degree, int pause){
  sRotation.write(degree);
  delay(pause);
}
void rotateServoT(int degree, int pause){
  sTilt.write(degree);
  delay(pause);
}



void setup(){
  //        Pure initialization.
  pinMode(DPin_Echo, INPUT);
  pinMode(DPin_Trigger, OUTPUT);
  pinMode(DPin_Signal, OUTPUT);
  sRotation.attach(DPin_SRotation);
  sTilt.attach(DPin_STilt);
  digitalWrite(DPin_Light, HIGH);

  Serial.begin(9600);

  //        Action.
  analogWrite(DPin_Signal, move);


}


int hor_degrees       = 90;
bool  subtract_degrees  = true;
bool  alarmed           = false;
float distance;
void loop(){ 
  rotateServoR(hor_degrees, 50);
  
  //    Disabled state.
  if(!digitalRead(DPin_Switch))
  {
    rotateServoR(90, 100);
    rotateServoT(60, 100);
    while(!digitalRead(DPin_Switch)){
      delay(500);
      digitalWrite(DPin_Light, LOW);
      delay(500);
      digitalWrite(DPin_Light, HIGH);
    }

    
    rotateServoT(90, 100);
    rotateServoR(90, 100);

    
    
    Serial.println("Setup finished.");
  }


  //  Main loop.
  
  //  Check for the distance and set alarmed.
    digitalWrite(DPin_Trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(DPin_Trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(DPin_Trigger, LOW);

    long duration = pulseIn(DPin_Echo, HIGH, 5000L);
    // Convert the time into a distance
    distance = duration*0.034/2;


  if(distance < 100.0 && distance > 40){  //  Distance below normal, become alarmed.
    if(!alarmed){ //  The sound is played only on the moment of becoming alarmed.
      analogWrite(DPin_Signal, spot);
    }
    alarmed = true;
  }
  else if(distance){
    alarmed = false;
  }
  
  if(alarmed){  //  Attack as long as alarmed.

    analogWrite(DPin_Signal, shoot);
    rotateServoT(100, 100);
    rotateServoT(90, 100);
  }
  else{         //  Not alarmed, proceed with sweeping.

      if(subtract_degrees){       //  Going left.
        if(hor_degrees == 0){     //  Can not go left anymoe. Reversing.
          subtract_degrees = false;
        }
        else{                     //  Turning left as planned.
          --hor_degrees;
        }
      }
      else{                       //  Going right.
        if(hor_degrees == 180){   //  Can not go right anymoe. Reversing.
          subtract_degrees = true;
        }
        else{                     //  Turning right as planned.
          ++hor_degrees;
        }
      }

      if(hor_degrees%30 == 0){
        analogWrite(DPin_Signal, scan);
      }

  }


  delay(100);

}
