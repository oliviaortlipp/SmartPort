// SMART PORT PROTOTYPE – BOARDING SOON SCREEN
// Displays a 1 hour and 14 minute countdown with live 
// clock and boarding status message.

// Import Libraries
#include <Arduino.h>    // Core Arduino functions
#include <U8g2lib.h>    // OLED display control
#include <I2C_RTC.h>    // Real-Time Clock module
#include <Wire.h>       // I2C communication

/*
  Overview:
  This version provides a mid-range countdown display,
  suitable for showing time remaining before boarding gates open.
*/



// RTC Setup 
static DS1307 RTC;

// OLED Setup 
U8G2_SH1106_128X64_WINSTAR_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/10, /* dc=*/9, /* reset=*/8);

// Countdown Configuration
const long COUNTDOWN_SECONDS = 1 * 3600 + 14 * 60; // Total duration: 1 hour and 14 minutes (converted to seconds)
unsigned long startTime;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  RTC.begin();
  u8g2.begin();

  // Verify RTC connection
  if (!RTC.isConnected()) {
    Serial.println("RTC Not Connected!");
    while (true);
  }

  Serial.println("*** SmartPort Prototype ***");

  startTime = millis(); // start countdown
}

void loop() {
  // Read current time from RTC 
  uint8_t hour   = RTC.getHours();
  uint8_t minute = RTC.getMinutes();

  // Format real-time clock as HH:MM
  char timeString[9];
  sprintf(timeString, "%02d:%02d", hour, minute);

  // Calculate countdown time
  unsigned long elapsed = (millis() - startTime) / 1000; // seconds elapsed
  long remaining = COUNTDOWN_SECONDS - elapsed;
  if (remaining < 0) remaining = 0;

  // Convert to hours, minutes, seconds
  long remHours   = remaining / 3600;
  long remMinutes = (remaining % 3600) / 60;
  long remSeconds = remaining % 60;

  char countdownString[9];
  sprintf(countdownString, "%02ld:%02ld:%02ld", remHours, remMinutes, remSeconds);

  // Update OLED Display
  u8g2.clearBuffer();

  // Top line: status/boarding message
  u8g2.setFont(u8g2_font_DigitalDisco_tu);
  u8g2.drawStr(8, 20, "BOARDING SOON!");

  // Middle line: real-time clock
  u8g2.setFont(u8g2_font_8bitclassic_tf);
  u8g2.setCursor(40, 40);
  u8g2.print(timeString);

  // Bottom line: countdown timer
  u8g2.setCursor(29, 60);
  u8g2.print(countdownString);

  u8g2.sendBuffer();

  delay(1000); // refresh display every second
}
