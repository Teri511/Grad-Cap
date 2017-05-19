/*
    SmartMatrix Features Demo - Louis Beaudoin (Pixelmatix)
    This example code is released into the public domain
*/

#include <SmartMatrix3.h>
#include <FastLED.h>

#include "imgs/gimpbitmap.h"
#include "imgs/kris.c"
#include "imgs/ub.c"
#include "imgs/pacframe0.c"
#include "imgs/pacframe1.c"
#include "imgs/pacframe2.c"
#include "imgs/digframe0.c"
#include "imgs/digframe1.c"
#include "imgs/pkaframe0.c"
#include "imgs/pkaframe1.c"
#include "imgs/fygframe0.c"
#include "imgs/fygframe1.c"
#include "imgs/linkframe0.c"
#include "imgs/linkframe1.c"
#include "imgs/octoframe0.c"
#include "imgs/octoframe1.c"
#include "imgs/mobframe0.c"
#include "imgs/mobframe1.c"
#include "imgs/marframe0.c"
#include "imgs/marframe1.c"
#include "imgs/gmbframe0.c"
#include "imgs/gmbframe1.c"
#include "imgs/rghostframe0.c"
#include "imgs/rghostframe1.c"
#include "imgs/pghostframe0.c"
#include "imgs/pghostframe1.c"
#include "imgs/bghostframe0.c"
#include "imgs/bghostframe1.c"
#include "imgs/oghostframe0.c"
#include "imgs/oghostframe1.c"
#include "imgs/foodbeer.c"
#include "imgs/slackbot.c"
#include "imgs/pikachu0.c"
#include "imgs/pikachu1.c"
#include "imgs/ash0.c"
#include "imgs/ash1.c"
#include "imgs/pokemonbg.c"
#include "imgs/CSE50.c"
#include "imgs/jesse0.c"
#include "imgs/jesse1.c"
#include "imgs/james0.c"
#include "imgs/james1.c"

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
const uint16_t scale = 20;
uint8_t noise[kMatrixWidth][kMatrixHeight];
static uint16_t x;
static uint16_t y;
static uint16_t z;
const uint16_t speed = 2;

const int dirtWidth = 15;
const int dirtHeight = 15;
uint8_t dirtBitmap[] = {
            __X_____, _____X__,
            _____X__, ________,
            ________, X__X___X,
            ___X____, _____X__,
            _____X__, ________,
            X_______, ________,
            ________, __X_____,
            _X______, ________,
            ____X___, ________,
            X_______, ________,
            ________, ________,
            ________, ________,
            _X______, ________,
            ______X_, ____X___,
            ________, ________,
        };


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
void fillnoise8() {
  for(int i = 0; i < kMatrixWidth; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < kMatrixHeight; j++) {
      int joffset = scale * j;
      noise[i][j] = inoise8(x + ioffset,y + joffset,z);
    }
  }
  z += speed;
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
  scrollingLayer.setColor({0xff, 0xff, 0xff});
  scrollingLayer.setMode(wrapForward);
  scrollingLayer.setOffsetFromTop(11);
  scrollingLayer.setSpeed(40);
  scrollingLayer.setFont(font6x10);
  scrollingLayer.start("Hullo???", 1); 
  delay(3000);
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

void pokemon(){
  drawBitmap(0,0,&pokemonbg);
  for(int i=0;i<128-30;i++){
    switch(i%4){
      case 0:
      drawBitmap(32-i,8,&ash0);
      drawBitmap(32+16-i,8,&pikachu0);
      drawBitmap(32+36-i,8,&jesse0);
      drawBitmap(32+52-i,8,&james1);
      break;
      case 1:
      drawBitmap(32-i,8,&ash1);
      drawBitmap(32+16-i,8,&pikachu1);
      drawBitmap(32+36-i,8,&jesse1);
      drawBitmap(32+52-i,8,&james0);
      break;
      case 2:
      drawBitmap(32-i,8,&ash0);
      drawBitmap(32+16-i,8,&pikachu0);
      drawBitmap(32+36-i,8,&jesse0);
      drawBitmap(32+52-i,8,&james1);
      break;
      case 3:
      drawBitmap(32-i,8,&ash1);
      drawBitmap(32+16-i,8,&pikachu1);
      drawBitmap(32+36-i,8,&jesse1);
      drawBitmap(32+52-i,8,&james0);
      break;
    } 
    backgroundLayer.swapBuffers();
    delay(100);
  }
}

void pacani(int ypos){
  backgroundLayer.fillScreen({0,0,0});
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

void digdugani(int ypos){
  rgb24 color;
  color.red=0xff;
  color.green=0xb8;
  color.blue=0x00;
  backgroundLayer.fillScreen({0xf8,0x70,0x00});
  backgroundLayer.fillRectangle(0, 0, 32, 15, color);
  backgroundLayer.drawMonoBitmap(0, 16, dirtWidth, dirtHeight, color, dirtBitmap);
  backgroundLayer.drawMonoBitmap(16, 16, dirtWidth, dirtHeight, color, dirtBitmap);
  color.red=0xff;
  color.green=0xff;
  color.blue=0x00;
  backgroundLayer.drawMonoBitmap(0, 0, dirtWidth, dirtHeight, color, dirtBitmap);
  backgroundLayer.drawMonoBitmap(16, 0, dirtWidth, dirtHeight, color, dirtBitmap);
  color.red=0;
  color.green=0;
  color.blue=0;
  for(int i=-3;i<96;i++){
    switch(i%4){
      case 0:
      drawBitmap(i-16,ypos,&digframe0);
      drawBitmap(i-32,ypos,&pkaframe0);
      drawBitmap(i-48,ypos,&fygframe0);
      break;
      case 1:
      drawBitmap(i-16,ypos,&digframe1);
      drawBitmap(i-32,ypos,&pkaframe0);
      drawBitmap(i-48,ypos,&fygframe0);
      break;
      case 2:
      drawBitmap(i-16,ypos,&digframe0);
      drawBitmap(i-32,ypos,&pkaframe1);
      drawBitmap(i-48,ypos,&fygframe1);
      break;
      case 3:
      drawBitmap(i-16,ypos,&digframe1);
      drawBitmap(i-32,ypos,&pkaframe1);
      drawBitmap(i-48,ypos,&fygframe1);
      break;
    }
    //x,y1,y2,color
    //uncomment for moblin ani cleanup
    //backgroundLayer.drawFastVLine(i-58,ypos,ypos+16, color);
    backgroundLayer.drawFastVLine(i,ypos+1,ypos+14, color); 
    backgroundLayer.drawFastVLine(i+1,ypos+3,ypos+12, color); 
    //backgroundLayer.drawFastVLine(i-33,ypos,ypos+16,color);
    backgroundLayer.swapBuffers();
    delay(100);
  }
}

void linkani(int ypos){
  backgroundLayer.fillScreen({128,224,16});
  rgb24 color;
  color.red=128;
  color.green=224;
  color.blue=16;
  for(int i=0;i<96;i++){
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
    //uncomment for moblin ani cleanup
    backgroundLayer.drawFastVLine(i-58,ypos,ypos+16, color); 
    //backgroundLayer.drawFastVLine(i-33,ypos,ypos+16,color);
    backgroundLayer.swapBuffers();
    delay(50);
  }
}

void marioani(int ypos){
  backgroundLayer.fillScreen({0,255,255});
  rgb24 color;
  color.red=0;
  color.green=255;
  color.blue=255;
  for(int i=0;i<70;i++){
    switch(i%4){
      case 0:
      drawBitmap(i-16,ypos,&marframe0);
      drawBitmap(i-36,ypos,&gmbframe0);
      break;
      case 1:
      drawBitmap(i-16,ypos,&marframe0);
      drawBitmap(i-36,ypos,&gmbframe0);
      break;
      case 2:
      drawBitmap(i-16,ypos,&marframe1);
      drawBitmap(i-36,ypos,&gmbframe1);
      break;
      case 3:
      drawBitmap(i-16,ypos,&marframe1);
      drawBitmap(i-36,ypos,&gmbframe1);
      break;
    }
    //x,y1,y2,color
    backgroundLayer.drawFastVLine(i-37,ypos,ypos+16,color);
    backgroundLayer.drawFastVLine(i-17,ypos,ypos+16,color);
    backgroundLayer.swapBuffers();
    delay(50);
  }
}

void lavalampani(int total_runs){

      for(int runs=0; runs < total_runs; runs++){
    rgb24 *buffer = backgroundLayer.backBuffer();

    static uint8_t ihue=0;
    fillnoise8();
    for(int i = 0; i < kMatrixWidth; i++) {
      for(int j = 0; j < kMatrixHeight; j++) {
        // We use the value at the (i,j) coordinate in the noise
        // array for our brightness, and the flipped value from (j,i)
        // for our pixel's hue.
        buffer[kMatrixWidth*j + i] = CRGB(CHSV(noise[j][i],255,noise[i][j]));

          // You can also explore other ways to constrain the hue used, like below
         //buffer[kMatrixHeight*j + i] = CRGB(CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]));
      }
    }
    ihue+=1;
    backgroundLayer.swapBuffers(false);
  }
  backgroundLayer.fillScreen({0,0,0}); 
  backgroundLayer.swapBuffers();
}

void loop() {

    // clear screen
    //backgroundLayer.fillScreen(defaultBackgroundColor);
    //backgroundLayer.swapBuffers();

    //ubani();
    //delay(6000);

    //foodbeerani();
    //delay(6000);

    //lavalampani(1000);
    
    drawBitmap(0,0,&cseatub);
    backgroundLayer.swapBuffers();
    delay(3000);

    //krisani();
    //delay(6000);

    drawBitmap(0,0,&cseatub);
    backgroundLayer.swapBuffers();
    delay(3000);

    spaghettani();
    delay(6000);

    drawBitmap(0,0,&cseatub);
    backgroundLayer.swapBuffers();
    delay(3000);
    
    pacani(7);
    delay(500);

    drawBitmap(0,0,&cseatub);
    backgroundLayer.swapBuffers();
    delay(3000);

    linkani(8);
    //linkani(8);
    //delay(500);

    //marioani(8);
    //delay(500);

    digdugani(8);
    delay(500);
    
    drawBitmap(0,0,&cseatub);
    backgroundLayer.swapBuffers();
    delay(3000);
    
    pokemon();
    delay(500);
}
