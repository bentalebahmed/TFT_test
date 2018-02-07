/*
  By Ahmed Bentaleb 
  you will need :
 * TFT with SD card in it
 * DHT11
 * Arduino nano
 * 2 Buttons
 * 
 * you will find an application in ma GitHub that will convert photos so you can show them
 * in the TFT that will save photos with names like :0, 1, 2 ...
*/


// include the necessary libraries
#include <SPI.h>
#include <SD.h>
#include <TFT.h>  // Arduino LCD library
#include <DHT11.h> // temp library

// pin definition for the Uno
#define sd_cs   7
#define lcd_cs  10
#define dc      9
#define rst     8
#define menuB   6 // Menu button pin
#define scrollB 5 // Scrolling Button pin (changing Time / photos)
#define DHTPin  4 // DHT11 pin


TFT TFTscreen = TFT(lcd_cs, dc, rst);
DHT11 dht11(DHTPin);


char Tem[3];
char Hum[3];


// nbr of image
int imageN = -1;

// temp humi
float temp, humi;

// time: hour and minits
int h = 0;
int m = 0;

// t0, t1 to clalculate sec
long int t0;
long int t1;

// menu button state and last state
int menuState = 0;
int menuLastState = 0;

// scroll button state and last state
int scrollState = 0;
int scrollLastState = 0;

// menu counter
int menuCount = 0;

// scroll counter
boolean addOne = false;

// storing time in string array
char TH[3];
char TM[3];

// this variable represents the image to be drawn on screen
PImage image[3];


void setup() {


  //initialize the buttons
  pinMode(menuB, INPUT_PULLUP);
  pinMode(scrollB, INPUT_PULLUP);

  // initialize the GLCD and show a message
  // asking the user to open the serial line
  TFTscreen.begin();
  TFTscreen.background(200, 200, 200);
  TFTscreen.textSize(2);
  TFTscreen.stroke(0, 0, 0);

  if (!SD.begin(sd_cs)) {
    TFTscreen.text("failed!", 0, 0);
    return;
  }
  TFTscreen.text("OK!", 0, 20);

  // initialize and clear the GLCD screen
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);

  // now that the SD card can be access, try to load the
  // image file.
 
}

void loop() {


  menuCheck();
  scrollCheck();
  calculateTime();

  switch (menuCount) {
    case 0:
      TFTscreen.textSize(3);
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text(TH, 10, 90);
      TFTscreen.text(":", 25, 90);
      TFTscreen.text(TM, 40, 90);
      TFTscreen.textSize(2);
      TFTscreen.text("Smart Glass", 10, 20);

      break;
    case 1:
      printTime();
      break;
    case 2:
      editHoures();
      break;
    case 3:
      editMinits();
      break;
    case 4:
      printImage();
      break;
    case 5:
       printTempurature();
      break;
  }
  
 
     dht11.read(humi, temp);  
     delay(DHT11_RETRY_DELAY); 
  
}

void menuCheck() {

  menuState = digitalRead(menuB);
  if ( menuState != menuLastState) {
    if (menuState == 0) {
      if (menuCount >= 0 && menuCount < 5 ) {
        menuCount++;
      } else {
        menuCount = 0;
      }
      addOne = false;
      TFTscreen.background(200, 200, 200);
    }
    menuLastState = menuState;
  }
}


void scrollCheck() {
  scrollState = digitalRead(scrollB);
  if ( scrollLastState != scrollState) {
    if (scrollState == 0) {
      addOne = true;
      TFTscreen.background(200, 200, 200);
    }
  }
  scrollLastState = scrollState;
}


void loadImage() {

  if (addOne) {
    imageN++;
    addOne = false;
  }
  if (!image[imageN].isValid()) {
    imageN = 0;
    menuCount = 0;
    addOne = false;
  }

}

void printImage() {
  loadImage();
  TFTscreen.image(image[imageN], 0, 0);
}

void printTime() {
  TFTscreen.textSize(5);
  TFTscreen.stroke(0, 0, 255);
  TFTscreen.text(" : ", 40, 40);
  String H = String(h);
  String M = String(m);
  char TH[3];
  char TM[3];
  H.toCharArray(TH, 3);
  M.toCharArray(TM, 3);
  if (menuCount == 2) TFTscreen.stroke(255, 0, 0);
  else  TFTscreen.stroke(0, 0, 255);
  TFTscreen.text(TH, 20, 40);
  if (menuCount == 3)TFTscreen.stroke(255, 0, 0);
  else TFTscreen.stroke(0, 0, 255);
  TFTscreen.text(TM, 100, 40);
}

void editHoures() {
  if (h > 0 && h <= 12) {
    if (addOne) {
      h++;
      addOne = false;
    }
  }
  else h = 1;
  printTime();
}

void editMinits() {
  if (m >= 0 && m < 59) {
    if (addOne) {
      m++;
      addOne = false;
    }
  }
  else m = 0;
  printTime();
}

void calculateTime() {
  t0 = millis();
  if (t0 - t1 > 60000) {
    t1 = t0;
    TFTscreen.background(200, 200, 200);
    m++;
    if (m > 59) {
      h++;
      m = 0;
    }
  }
}

void printTempurature() {

   String ST = String(temp);
   String SH = String(humi);
   ST.toCharArray(Tem, 3);
   SH.toCharArray(Hum, 3);
    TFTscreen.background(200, 200, 200);
    TFTscreen.textSize(4);
    TFTscreen.stroke(0, 190, 0);
    TFTscreen.text(Tem, 0, 70);
     TFTscreen.text(Hum, 50, 70);
 
}
