// SMART PORT PROTOTYPE – DATE SCREEN
// Displays the event date, real-time clock, and a long 
// countdown timer (26 hours and 10 minutes total).

// import libraries
#include <Arduino.h>    // Core Arduino functions
#include <U8g2lib.h>    // OLED display control
#include <I2C_RTC.h>    // Real-Time Clock module
#include <Wire.h>       // I2C communication

/*
  Overview:
  This version includes a static event date displayed on top 
  and a dynamic countdown for the next day’s flight.
*/


// RTC Setup 
static DS1307 RTC;

// OLED Setup 
U8G2_SH1106_128X64_WINSTAR_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/10, /* dc=*/9, /* reset=*/8);

// Countdown Configuration
// 26 hours and 10 minutes converted to seconds
const unsigned long COUNTDOWN_SECONDS = 26UL * 3600UL + 10UL * 60UL; 
unsigned long startTime;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  RTC.begin();
  u8g2.begin();

  // Check if RTC is connected
  if (!RTC.isConnected()) {
    Serial.println("RTC Not Connected!");
    while (true);
  }

  Serial.println("*** SmartPort Prototype ***");

  startTime = millis(); // start countdown timer
}

void loop() {
  // Get current time from RTC 
  uint8_t hour   = RTC.getHours();
  uint8_t minute = RTC.getMinutes();

  // Format real-time clock HH:MM
  char timeString[6];
  sprintf(timeString, "%02d:%02d", hour, minute);

  // Calculate countdown time
  unsigned long elapsed = (millis() - startTime) / 1000; // seconds elapsed
  unsigned long remaining;
  if (elapsed >= COUNTDOWN_SECONDS) {
    remaining = 0;
  } else {
    remaining = COUNTDOWN_SECONDS - elapsed;
  }

  // Convert to hours, minutes, seconds
  unsigned long remHours   = remaining / 3600;
  unsigned long remMinutes = (remaining % 3600) / 60;
  unsigned long remSeconds = remaining % 60;

  char countdownString[9];
  sprintf(countdownString, "%02lu:%02lu:%02lu", remHours, remMinutes, remSeconds);

  // update OLED Display
  u8g2.clearBuffer();

  // Top line: static message of event date
  u8g2.setFont(u8g2_font_DigitalDisco_tu);
  u8g2.drawStr(1, 20, "BOARDING 22/11/25!");

  // Middle line: real-time clock
  u8g2.setFont(u8g2_font_8bitclassic_tf);
  u8g2.setCursor(40, 40);
  u8g2.print(timeString);

  // Bottom line: countdown timer
  u8g2.setCursor(25, 60);
  u8g2.print(countdownString);

  u8g2.sendBuffer();

  delay(1000); // refresh display every second
}
