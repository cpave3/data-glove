#include <Adafruit_NeoPixel.h>

int LED_RED = 6;
int LED_YELLOW = 7;
int LED_BLUE = 8;
int LED_GREEN = 9;

int LED_NEO = 5;

int BUTTON = 2;

#define PIXELS 1

int ch = 0; // 0, 1, 2, 3

Adafruit_NeoPixel pixels(PIXELS, LED_NEO, NEO_GRB + NEO_KHZ800);

String data = "";

void setup() {
  // Declare the LED as an output
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON, INPUT);

  pixels.begin();

  Serial.begin(9600);
}

void updatePixels(int r, int g, int b) {
  for (int i = 0; i < PIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(g / 4, r / 4, b / 4));

    pixels.show();   // Send the updated pixel colors to the hardware.
  }
}


void loop() {

  // Reset all the LEDs
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  pixels.clear();

  // get the pot value
  int potVal = analogRead(A0);
  int buttonState = digitalRead(BUTTON);

  // use the pot to set the channel
  if (potVal < 256) {
    ch = 3;
  } else if (potVal > 256 && potVal < 512) {
    ch = 2;
  } else if (potVal > 512 && potVal < 768) {
    ch = 1;
  } else if (potVal > 768) {
    ch = 0;
  }

  // when the button is pressed, gather data. when released, send it via serial
  if (buttonState == 1) {
    data = "ch:" + String(ch) + ",event:press";
  } else if (data != "") {
    Serial.println(data);
    data = "";
  }

  // show an LED based on which channel is active
  // todo: this will be controlled by the hall effect sensors eventually
  switch (ch) {
    case 0:
      digitalWrite(LED_GREEN, HIGH); // Turn the LED on
      updatePixels(0, 255, 0);
      break;
    case 1:
      digitalWrite(LED_BLUE, HIGH); // Turn the LED on
      updatePixels(0, 0, 255);
      break;
    case 2:
      digitalWrite(LED_RED, HIGH); // Turn the LED on
      updatePixels(255, 0, 0);
      break;
    case 3:
      digitalWrite(LED_YELLOW, HIGH); // Turn the LED on
      updatePixels(255, 255, 0);
      break;
  }


  // todo: Next, we need to hook up the neopixel, and change it to the color matching the channel when the button is pushed


  delay(50);
}
