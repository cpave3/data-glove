#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

int PIN = 14; // NeoPixel

// Finger Sensors
int FINGER_1 = 16;
int FINGER_2 = 17;
int FINGER_3 = 18;
int FINGER_4 = 19;

int SEND_BUTTON = 3; // Button to send signal to host

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

byte x = 0b00000000;  // the 0b prefix indicates a binary constant
String data = "";

void setup() {

  pixels.begin();

  pinMode(FINGER_1, INPUT);
  pinMode(FINGER_2, INPUT);
  pinMode(FINGER_3, INPUT);
  pinMode(FINGER_4, INPUT);
  pinMode(SEND_BUTTON, INPUT_PULLUP);

  Serial.begin(57600);
}

void loop() {

  // Reset byte to 0 each loop to reflect only held buttons
  x = 0b00000000;

  // read all the fingers
  int f1State = !digitalRead(FINGER_1);
  int f2State = !digitalRead(FINGER_2);
  int f3State = !digitalRead(FINGER_3);
  int f4State = !digitalRead(FINGER_4);

  // Read the send button
  int sendState = !digitalRead(SEND_BUTTON);

  // todo: remove this maybe
  int intensity = 100;
  if (f4State) {
    intensity = 250;
  }

  // Set pixel on
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, pixels.Color(f1State * intensity, f2State * intensity, f3State * intensity));
  pixels.show();   // Send the updated pixel colors to the hardware.

  // Write data back to serial
  if (sendState == 1) {
    bitWrite(x, 0, f1State);
    bitWrite(x, 1, f2State);
    bitWrite(x, 2, f3State);
    bitWrite(x, 3, f4State);
    data = "ch:" + String(x) + ",event:press";
  } else if (data != "") {
    Serial.println(data);
    data = "";
  }

  // Avoid spamming
  delay(100);

}
