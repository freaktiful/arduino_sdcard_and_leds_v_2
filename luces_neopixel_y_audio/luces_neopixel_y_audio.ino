// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

#define SD_ChipSelectPin 4 //Chip select is pin number 4

TMRpcm music; //Lib object is named "music"

#define PIN 6
#define PIN2 3


#define NUMPIXELS 7
#define NUMPIXELS2 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);

int boton = 2;
long tiempo_anterior;
long diferencia;
bool encendido = false;

bool flagLuces = false;

void setup() {
  Serial.begin(9600);
  pinMode(boton, INPUT);

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // initialize all pixels to "off"

  pixels2.begin(); // This initializes the NeoPixel library.
  pixels2.show(); // initialize all pixels to "off"

  attachInterrupt(digitalPinToInterrupt(boton), botonLuces, FALLING);

  music.speakerPin = 9; //Auido out on pin 9

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }

  music.setVolume(5);    //   0 to 7. Set volume level
  music.quality(1);        //  Set 1 for 2x oversampling Set 0 for normal

}

void loop() {
  if(flagLuces){
      flagLuces = false;
      cambiaEstadoLED();
    }
}

void cambiaEstadoLED(){
  music.play("ALARMA.wav");
  if(!encendido){
    encender();
    Serial.println("encendido");
    music.disable();
  }
  else{
    apagar();
    Serial.println("apagado");
    music.disable();
  }

  encendido = !encendido;
}

void botonLuces(){
  diferencia = millis() - tiempo_anterior;
  tiempo_anterior = millis();
  //Serial.println(diferencia);

  if(diferencia > 500){
    flagLuces = true;
  }
}

void encender(){
  uint16_t i, j;

  for (j = 0; j < 255; j++) {
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(23,161,165));
    }
    pixels.setBrightness(j);
    pixels.show();

    for(int i=0;i<NUMPIXELS2;i++){
      pixels2.setPixelColor(i, pixels2.Color(23,161,165));
    }
    pixels2.setBrightness(j);
    pixels2.show();
    
    delay(20);
  }
}

void apagar(){
  uint16_t i, j;

  for (j = 255; j > 0; j--) {
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(23,161,165));
    }
    pixels.setBrightness(j);
    pixels.show();
    
    for(int i=0;i<NUMPIXELS2;i++){
      pixels2.setPixelColor(i, pixels2.Color(23,161,165));
    }
    pixels2.setBrightness(j);
    pixels2.show();
    
    delay(10);
  }
}
// For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
/*brighten(){
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(23,161,165)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}*/
