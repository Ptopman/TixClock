#include <Wire.h>
#include <MD_DS3231.h>
int latchPin = 2;
int clockPin = 3;
int dataPin = 4;

int leds[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//int leds[24] = {1, 1, 1 , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int hour1 = 0;
int hour2 = 0;
int min1 = 0;
int min2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  writereg();
  for (int i = 0; i < 24; i++) {
    leds[i] = 1;
    writereg();
    delay(100);
  }
  for (int i = 0; i < 24; i++) {
    leds[i] = 0;
  }
  writereg();
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  delay(500);
}

void loop() {
  RTC.readTime();
  Serial.print(RTC.h); Serial.print(":"); Serial.print(RTC.m); Serial.print("."); Serial.println(RTC.s);
  for (int i = 0; i < 24; i++) {
    Serial.print(leds[i]);
  }
  Serial.println("");
  int hour = 0;
  //converting time to 12hr
  if (RTC.h >= 12) {
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  } else {
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }
  if (RTC.h > 12) {
    hour = RTC.h - 12;
  }
  else if (RTC.h == 0) {
    hour = 12;
  }
  else {
    hour = RTC.h;
  }
  //parse hour
  if (hour < 10) {
    hour1 = 0;
    hour2 = hour;
  } else {
    hour1 = String(hour).charAt(0) - 48;
    hour2 = String(hour).charAt(1) - 48;
  }
  if (hour1 == 1) {
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    Serial.print(hour1);
    Serial.println(" : 1");
  }
  else if (hour1 == 2) {
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    Serial.println(" : 2");

  } else {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    Serial.println(" : 0");
  }
  //parse min
  if (RTC.m < 10) {
    min1 = 0;
    min2 = String(RTC.m).charAt(0) - 48;
  } else {
    min1 = String(RTC.m).charAt(0) - 48;
    min2 = String(RTC.m).charAt(1) - 48;
  }
  //writereg code;
  int counter = 0;
  for (int i = 0; i < 9; i++) {
    if (counter < hour2) {
      leds[i] = 1;
    } else {
      leds[i] = 0;
    }
    counter++;
  }
  counter = 0;
  for (int i = 9; i < 15; i++) {
    if (counter < min1) {
      leds[i] = 1;
    } else {
      leds[i] = 0;
    }
    counter++;
  }
  counter = 0;
  for (int i = 15; i < 24; i++) {
    if (counter < min2) {
      leds[i] = 1;
    } else {
      leds[i] = 0;
    }
    counter++;
  }
  writereg();
  delay(1000);
}

void writereg()
{
  digitalWrite(latchPin, LOW);
  for (int i = (24 - 1); i >= 0; i--)
  {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, leds[i]);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
  //delay(500);
}

void printleds() {
  for (int i = 0; i < 24; i++) {
    Serial.println(leds[i]);
  }
}

