#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>

#define     DPin_CS       8
#define     DPin_Speaker  9
#define     APin_Signal   0

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
  200,    //  finish
  200,    //  move
  200,    //  scan
  200,    //  scan2
  200,    //  scan3
  200,    //  shoot
  200,    //  shoot2
  200,    //  shoot3
  200,    //  spot
  200     //  spot2

};

TMRpcm tmrpcm;

void playAudio(const char* filename, int duration){
  if(!duration) return;

  tmrpcm.stopPlayback();  //  Paranoia.
  tmrpcm.play(filename);
  delay(duration);
  tmrpcm.stopPlayback();
}

void setup() {  
  if (!SD.begin(DPin_CS)) { 
    Serial.println("SD fail");  
    delay(2000);
  }

  tmrpcm.speakerPin = DPin_Speaker;
  tmrpcm.setVolume(6);
}

void loop() {

  int val = analogRead(APin_Signal);

  if(val < length && val > 0){
    playAudio(filenames[val], delays[val]);
  }

  digitalWrite(DPin_Speaker, HIGH);
}
