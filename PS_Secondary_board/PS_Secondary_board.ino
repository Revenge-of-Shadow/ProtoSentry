#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>

//      Receiving digital pins.
#define     DPin_Justice  2
#define     DPin_finish   3
#define     DPin_move     4
#define     DPin_scan     5
#define     DPin_shoot    6
#define     DPin_spot     7
//      Receiving digital pins end.

#define     DPin_CS       8
#define     DPin_Speaker  9

#define     length        12        //  Fixed amount of file entries, including NONE.

enum tracks{                        //  Useless here, but pretty.
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

TMRpcm tmrpcm;

void playAudio(const char* filename, int duration){
  Serial.print(filename);
  Serial.print(" : playing for ");
  Serial.print(duration);
  Serial.println(" milliseconds. ");
  if(!duration) return;

  tmrpcm.stopPlayback();  //  Paranoia.
  tmrpcm.play(filename);
  delay(duration);
  tmrpcm.stopPlayback();
  digitalWrite(DPin_Speaker, HIGH);
  Serial.println("Done.");
}

void setup() {  
  Serial.begin(9600);
  digitalWrite(DPin_Speaker, HIGH);
  while (!SD.begin(DPin_CS)) { 
    Serial.println("SD fail");  
    delay(2000);
  }

  pinMode(DPin_Justice, INPUT);
  pinMode(DPin_finish,  INPUT);
  pinMode(DPin_move,    INPUT);
  pinMode(DPin_scan,    INPUT);
  pinMode(DPin_shoot,   INPUT);
  pinMode(DPin_spot,    INPUT);

  tmrpcm.speakerPin = DPin_Speaker;
  tmrpcm.setVolume(6);
}

void loop() {
  
  if(digitalRead(DPin_Justice)){
    tmrpcm.play(filenames[Justice]);
    tmrpcm.loop(true);
    while(digitalRead(DPin_Justice))
    {
      delay(10);
    }
    tmrpcm.stopPlayback();
    tmrpcm.loop(false);
  }
  else
  if(digitalRead(DPin_finish)){
    playAudio(filenames[finish], delays[finish]);
  }
  else
  if(digitalRead(DPin_move)){
    playAudio(filenames[move], delays[move]);
  }
  else
  if(digitalRead(DPin_scan)){
    int chosen = random(scan, scan3+1);
    playAudio(filenames[chosen], delays[chosen]);
  }
  else
  if(digitalRead(DPin_shoot)){
    int chosen = random(shoot, shoot3+1); 
    playAudio(filenames[chosen], delays[chosen]);
  }
  else
  if(digitalRead(DPin_spot)){
    playAudio(filenames[spot], delays[spot]);
  }

  digitalWrite(DPin_Speaker, HIGH);
  delay(10);
}
