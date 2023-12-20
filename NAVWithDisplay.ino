  #include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <Adafruit_HMC5883_U.h>

//Motor Pins
int motorRightPin1 = 8;
int motorRightPin2 = 9;

int motorLeftPin1 = 10;
int motorLeftPin2 = 11;

//GPS Cardinal Bountries
long double northLat = 33.377391;
long double eastLong = -111.700973;
long double southLat = 33.376762;
long double westLong = -111.701263;

//GPS Grid Checkpoints
long double latZero = southLat;
long double latOne = ((northLat - southLat)/4)+southLat;
long double latTwo = (((northLat - southLat)/4) * 2)+ southLat;
long double latThree = (((northLat - southLat)/4) * 3)+ southLat;
long double latFour = northLat;

long double longZero = eastLong;
long double longOne = ((westLong - eastLong)/4)+eastLong;
long double longTwo = (((westLong - eastLong)/4) * 2) + eastLong;
long double longThree = (((westLong - eastLong)/4) * 3) + eastLong;
long double longFour = westLong;

// OLED display dimensions
const int OLED_WIDTH = 128;
const int OLED_HEIGHT = 64;

// OLED display object
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// GPS object
TinyGPSPlus gps;

// Magnetometer object
Adafruit_HMC5883_Unified magnetometer = Adafruit_HMC5883_Unified(12345);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Initialize magnetometer
  if(!magnetometer.begin()) {
    Serial.println("Error initializing magnetometer");
  }

  // Initialize Motors
  pinMode(motorRightPin1, OUTPUT);
  pinMode(motorRightPin2, OUTPUT);
  pinMode(motorLeftPin1, OUTPUT);
  pinMode(motorLeftPin2, OUTPUT);
}

void loop() {
  // Read data from GPS module
  while (Serial.available()) {
    gps.encode(Serial.read());
  }

  // Read data from magnetometer
  sensors_event_t event;
  magnetometer.getEvent(&event);

  // Calculate compass direction
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  if (heading < 0) {
    heading += 2 * PI;
  }
  int direction = heading * 180 / M_PI;
//  int direction = heading;

  // Display GPS coordinates and compass direction on OLED display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Lat: ");
  display.println(gps.location.lat(), 6);
  display.print("Long: ");
  display.println(gps.location.lng(), 6);
  display.print("Direction: ");
  display.println(direction);
  display.display();
}

void goForward() {
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);

  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
}

void goBackWard() {
  digitalWrite(motorRightPin2, HIGH);
  digitalWrite(motorRightPin1, LOW);

  digitalWrite(motorLeftPin2, HIGH);
  digitalWrite(motorLeftPin1, LOW);
}

void turnRight() {
  digitalWrite(motorRightPin1, HIGH);
  digitalWrite(motorRightPin2, LOW);

  digitalWrite(motorLeftPin2, HIGH);
  digitalWrite(motorLeftPin1, LOW);
}

void turnLeft() {
  digitalWrite(motorRightPin2, HIGH);
  digitalWrite(motorRightPin1, LOW);

  digitalWrite(motorLeftPin1, HIGH);
  digitalWrite(motorLeftPin2, LOW);
}
