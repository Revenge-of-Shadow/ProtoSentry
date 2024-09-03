 //      Sending digital pins.
#define     DPin_Justice  8
#define     DPin_finish   9
#define     DPin_move     10
#define     DPin_scan     11
#define     DPin_shoot    12
#define     DPin_spot     13
//      Sending digital pins end.

#define DPin_Light        7
#define DPin_Echo         6
#define DPin_Trigger      5
#define DPin_Switch       4
#define DPin_SRotation    3
#define DPin_STilt        2

#define servo_pause       20

#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <Servo.h>


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

const char* filenames [12] = {      //  All of the filenames. 
  "",
  "Justice.wav",
  "finish.wav",
  "move.wav",
  "scan.wav",
  "scan2.wav",
  "scan3.wav",
  "shoot.wav",
  "shoot2.wav",
  "shoot3.wav",
  "spot.wav",
  "spot2.wav"
};

const int delays [12] = {             //  All of the corresponding delays.
  0,      //  none
  120000, //  Justice
  500,    //  finish
  500,    //  move
  400,    //  scan
  400,    //  scan2
  400,    //  scan3
  400,    //  shoot
  400,    //  shoot2
  400,    //  shoot3
  200,    //  spot
  200     //  spot2

};

void rotateServoR(int degree){
  sRotation.write(degree);
  delay(servo_pause);
}
void rotateServoT(int degree){
  sTilt.write(degree);
  delay(servo_pause);
}

void  signalSend(int pin, int duration){
  Serial.print(pin);
  Serial.print(" On...");
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
  Serial.println(" Off.");
}


void setup(){
  //        Pure initialization.

  pinMode(DPin_Justice, OUTPUT);
  pinMode(DPin_finish,  OUTPUT);
  pinMode(DPin_move,    OUTPUT);
  pinMode(DPin_scan,    OUTPUT);
  pinMode(DPin_shoot,   OUTPUT);
  pinMode(DPin_spot,    OUTPUT);

  digitalWrite(DPin_Justice, LOW);
  digitalWrite(DPin_finish,  LOW);
  digitalWrite(DPin_move,    LOW);
  digitalWrite(DPin_scan,    LOW);
  digitalWrite(DPin_shoot,   LOW);
  digitalWrite(DPin_spot,    LOW);
  

  pinMode(DPin_Echo, INPUT);
  pinMode(DPin_Trigger, OUTPUT);
  sRotation.attach(DPin_SRotation);
  sTilt.attach(DPin_STilt);
  digitalWrite(DPin_Light, HIGH);

  Serial.begin(9600);

  //        Action.
  signalSend(DPin_move, delays[move]);
}


int hor_degrees       = 90;
bool  subtract_degrees  = true;
bool  alarmed           = false;
float distance;
void loop(){ 
  //    Disabled state.
  if(!digitalRead(DPin_Switch))
  {
    rotateServoR(90);
    rotateServoT(60);
    while(!digitalRead(DPin_Switch)){
      delay(500);
      digitalWrite(DPin_Light, LOW);
      delay(500);
      digitalWrite(DPin_Light, HIGH);
    }

    
    rotateServoT(90);
    rotateServoR(90);
    
    signalSend(DPin_finish, delays[finish]);
  }
  //    Disabled state end.
  

  else{
    rotateServoR(hor_degrees);
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
        signalSend(DPin_spot, delays[spot]);
      }
      alarmed = true;
    }
    else if(distance){
      alarmed = false;
    }
    
    if(alarmed){  //  Attack as long as alarmed.
      rotateServoT(100);
      signalSend(DPin_shoot, delays[shoot]);
      rotateServoT(90);
    }
    else{         //  Not alarmed, proceed with sweeping.

        if(subtract_degrees){       //  Going left.
          if(hor_degrees == 0){     //  Can not go left anymoe. Reversing.
            subtract_degrees = false;
            ++hor_degrees;          //  Save a tick, exclude a bug.
          }
          else{                     //  Turning left as planned.
            --hor_degrees;
          }
        }
        else{                       //  Going right.
          if(hor_degrees == 180){   //  Can not go right anymoe. Reversing.
            subtract_degrees = true;
            --hor_degrees;          //  Save a tick, exclude a bug.
          }
          else{                     //  Turning right as planned.
            ++hor_degrees;
          }
        }

        if(hor_degrees%90 == 0){
          signalSend(DPin_scan, delays[scan]);
        }

    }
  }


}
