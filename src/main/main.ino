/*
    SmartMatrix Features Demo - Louis Beaudoin (Pixelmatix)
    This example code is released into the public domain
*/

#include <SmartMatrix3.h>

#include "gimpbitmap.h"
#include "kris.c"
#include "ub.c"
#include "pacframe0.c"
#include "pacframe1.c"
#include "pacframe2.c"
#include "linkframe0.c"
#include "linkframe1.c"
#include "octoframe0.c"
#include "octoframe1.c"
#include "mobframe0.c"
#include "mobframe1.c"
#include "rghostframe0.c"
#include "rghostframe1.c"
#include "pghostframe0.c"
#include "pghostframe1.c"
#include "bghostframe0.c"
#include "bghostframe1.c"
#include "oghostframe0.c"
#include "oghostframe1.c"
#include "foodbeer.c"
#include "slackbot.c"

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 32;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

const int defaultBrightness = 100*(255/100);    // full brightness
//const int defaultBrightness = 15*(255/100);    // dim: 15% brightness
const int defaultScrollOffset = 6;
const rgb24 defaultBackgroundColor = {0x40, 0, 0};

// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;

void drawBitmap(int16_t x, int16_t y, const gimp32x32bitmap* bitmap) {
  for(unsigned int i=0; i < bitmap->height; i++) {
    for(unsigned int j=0; j < bitmap->width; j++) {
      rgb24 pixel = { bitmap->pixel_data[(i*bitmap->width + j)*3 + 0],
                      bitmap->pixel_data[(i*bitmap->width + j)*3 + 1],
                      bitmap->pixel_data[(i*bitmap->width + j)*3 + 2] };

      backgroundLayer.drawPixel(x + j, y + i, pixel);
    }
  }
}

// the setup() method runs once, when the sketch starts
void setup() {
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);

  Serial.begin(38400);

  matrix.addLayer(&backgroundLayer); 
  matrix.addLayer(&scrollingLayer); 
  matrix.addLayer(&indexedLayer); 
  matrix.begin();

  matrix.setBrightness(defaultBrightness/4);

  scrollingLayer.setOffsetFromTop(defaultScrollOffset);

  backgroundLayer.enableColorCorrection(true);
  backgroundLayer.fillScreen({0,0,0});
  
}

void krisani(){
  drawBitmap(0,0,&kris);
  backgroundLayer.swapBuffers();
  scrollingLayer.setColor({0x00, 0x00, 0x00});
  scrollingLayer.setMode(wrapForward);
  scrollingLayer.setOffsetFromTop(4);
  scrollingLayer.setSpeed(40);
  scrollingLayer.setFont(font6x10);
  scrollingLayer.start("Read the Documentation", 1); 
}

void spaghettani(){
  drawBitmap(0,0,&spaghett);
  backgroundLayer.swapBuffers();
  scrollingLayer.setColor({0xff, 0xff, 0xff});
  scrollingLayer.setMode(wrapForward);
  scrollingLayer.setOffsetFromTop(11);
  scrollingLayer.setSpeed(40);
  scrollingLayer.setFont(font6x10);
  scrollingLayer.start("SPAGHETT, Spooked ya!", 1); 
}

void ubani(){
  drawBitmap(0,0,&ub);
  backgroundLayer.swapBuffers();
  scrollingLayer.setColor({0x00, 0x00, 0xff});
  scrollingLayer.setOffsetFromTop(17);
  scrollingLayer.setMode(wrapForward);
  scrollingLayer.setSpeed(40);
  scrollingLayer.setFont(font6x10);
  scrollingLayer.start("Stop Taking My Money!!!", 1); 
}

void foodbeerani(){
  drawBitmap(0,0,&foodbeer);
  backgroundLayer.swapBuffers();
  scrollingLayer.setColor({0xff, 0xff, 0xff});
  scrollingLayer.setMode(wrapForward);
  scrollingLayer.setOffsetFromTop(4);
  scrollingLayer.setSpeed(40);
  scrollingLayer.setFont(font6x10);
  scrollingLayer.start("Stronger CSE at UB", 1); 
}

void pacani(int ypos){

  for(int i=0;i<113;i++){
    switch(i%4){
      case 0:
      drawBitmap(32-i,ypos,&pacframe0);
      drawBitmap(32+16-i,ypos,&rghostframe0);
      drawBitmap(32+32-i,ypos,&pghostframe0);
      drawBitmap(32+48-i,ypos,&bghostframe0);
      drawBitmap(32+64-i,ypos,&oghostframe0);
      break;
      case 1:
      drawBitmap(32-i,ypos,&pacframe1);
      drawBitmap(32+16-i,ypos,&rghostframe0);
      drawBitmap(32+32-i,ypos,&pghostframe0);
      drawBitmap(32+48-i,ypos,&bghostframe0);
      drawBitmap(32+64-i,ypos,&oghostframe0);
      break;
      case 2:
      drawBitmap(32-i,ypos,&pacframe2);
      drawBitmap(32+16-i,ypos,&rghostframe1);
      drawBitmap(32+32-i,ypos,&pghostframe1);
      drawBitmap(32+48-i,ypos,&bghostframe1);
      drawBitmap(32+64-i,ypos,&oghostframe1);
      break;
      case 3:
      drawBitmap(32-i,ypos,&pacframe1);
      drawBitmap(32+16-i,ypos,&rghostframe1);
      drawBitmap(32+32-i,ypos,&pghostframe1);
      drawBitmap(32+48-i,ypos,&bghostframe1);
      drawBitmap(32+64-i,ypos,&oghostframe1);
      break;
    } 
    backgroundLayer.swapBuffers();
    delay(50);
  }
}

void linkani(int ypos){
  backgroundLayer.fillScreen({128,224,16});
  rgb24 color;
  color.red=128;
  color.green=224;
  color.blue=16;
  for(int i=0;i<113;i++){
    switch(i%4){
      case 0:
      drawBitmap(i-16,ypos,&linkframe0);
      drawBitmap(i-32,ypos,&octoframe0);
      drawBitmap(i-57,ypos,&mobframe0);
      break;
      case 1:
      drawBitmap(i-16,ypos,&linkframe0);
      drawBitmap(i-32,ypos,&octoframe0);
      drawBitmap(i-57,ypos,&mobframe0);
      break;
      case 2:
      drawBitmap(i-16,ypos,&linkframe1);
      drawBitmap(i-32,ypos,&octoframe1);
      drawBitmap(i-57,ypos,&mobframe1);
      break;
      case 3:
      drawBitmap(i-16,ypos,&linkframe1);
      drawBitmap(i-32,ypos,&octoframe1);
      drawBitmap(i-57,ypos,&mobframe1);
      break;
    }
    //x,y1,y2,color
    backgroundLayer.drawFastVLine(i-58,ypos,ypos+16, color); 
    backgroundLayer.swapBuffers();
    delay(50);
  }
}

void loop() {

    // clear screen
    //backgroundLayer.fillScreen(defaultBackgroundColor);
    //backgroundLayer.swapBuffers();

    //ubani();
    //delay(6000);

    //foodbeerani();
    //delay(6000);

    //krisani();
    //delay(6000);

    //spaghettani();
    //delay(6000);

    //pacani(7);
    //delay(500);

    linkani(8);
    delay(500);
}
